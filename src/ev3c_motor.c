 /* This file is part of ev3c.
  * Ev3c is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * Ev3c is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with ev3c.  If not, see <http://www.gnu.org/licenses/>
  *
  * For feedback and questions about my Files and Projects please mail me,
  * Alexander Matthes (Ziz) , ziz_at_mailbox.org, http://github.com/theZiz */

#include "ev3c_motor.h"

enum ev3_motor_identifier get_motor_identifier(char* buffer)
{
	if (strcmp(buffer,"fi-l12-ev3") == 0)
		return FI_L12_EV3;
	if (strcmp(buffer,"lego-ev3-l-motor") == 0)
		return LEGO_EV3_L_MOTOR;
	if (strcmp(buffer,"lego-ev3-m-motor") == 0)
		return LEGO_EV3_M_MOTOR;
	return UNKNOWN_MOTOR;
}

void load_motor( ev3_motor_ptr motor, int32_t nr)
{
	if (motor == NULL)
		return;
	ev3_string buffer;
	char file[1024];
	//loading the struct with some initial values
	sprintf(file,"/sys/class/tacho-motor/motor%i/driver_name",nr);
	ev3_read_file(file,motor->driver_name,EV3_STRING_LENGTH);
	motor->driver_identifier = get_motor_identifier(motor->driver_name);
	sprintf(file,"/sys/class/tacho-motor/motor%i/address",nr);
	ev3_read_file(file,buffer,EV3_STRING_LENGTH);
	motor->port = buffer[13]; //ev3-ports:outX
	motor->motor_nr = nr;
	motor->duty_cycle_fd = -1;
	motor->duty_cycle_sp_fd = -1;
	motor->position_fd = -1;
	motor->position_sp_fd = -1;
	motor->speed_fd = -1;
	motor->speed_sp_fd = -1;
	motor->ramp_up_sp_fd = -1;
	motor->ramp_down_sp_fd = -1;
	motor->time_sp_fd = -1;
	motor->command_stream = NULL;
	sprintf(file,"/sys/class/tacho-motor/motor%i/commands",nr);
	ev3_read_file(file,file,1024);
	motor->command_count = 0;
	char* mom = file;
	char* end;
	while (end = strchr(mom,' '))
	{
		end[0] = 0;
		sprintf(motor->commands[motor->command_count],"%s",mom);
		motor->command_count++;
		mom = end;
		mom++;
	}
	sprintf(motor->commands[motor->command_count],"%s",mom);
	motor->command_count++;
	sprintf(file,"/sys/class/tacho-motor/motor%i/stop_actions",nr);
	ev3_read_file(file,file,1024);
	motor->stop_action_count = 0;
	mom = file;
	while (end = strchr(mom,' '))
	{
		end[0] = 0;
		sprintf(motor->stop_actions[motor->stop_action_count],"%s",mom);
		motor->stop_action_count++;
		mom = end;
		mom++;
	}
	sprintf(motor->stop_actions[motor->stop_action_count],"%s",mom);
	motor->stop_action_count++;
	sprintf(file,"/sys/class/tacho-motor/motor%i/stop_action",nr);
	ev3_read_file(file,buffer,EV3_STRING_LENGTH);
	for (motor->stop_action = 0; motor->stop_action < motor->stop_action_count; motor->stop_action++)
		if (strcmp(motor->stop_actions[motor->stop_action],buffer) == 0)
			break;
	sprintf(file,"/sys/class/tacho-motor/motor%i/max_speed",nr);
	ev3_read_file(file,file,EV3_STRING_LENGTH);
	motor->max_speed = atoi(file);
}

ev3_motor_ptr ev3_load_motors( void )
{
	DIR *d;
	struct dirent *dir;
	ev3_motor_ptr first_motor = NULL;
	ev3_motor_ptr last_motor = NULL;
	d = opendir("/sys/class/tacho-motor");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name,".") == 0 ||
				strcmp(dir->d_name,"..") == 0)
				continue;
			ev3_motor_ptr motor = (ev3_motor_ptr)malloc(sizeof(ev3_motor));
			load_motor(motor,atoi(&(dir->d_name[5])));
			motor->next = NULL;
			if (last_motor)
				last_motor->next = motor;
			else
				first_motor = motor;
			last_motor = motor;
		}
		closedir(d);
	}
	return first_motor;
}

void ev3_delete_motors( ev3_motor_ptr motors )
{
	while (motors)
	{
		ev3_motor_ptr next = motors->next;
		ev3_close_motor(motors);
		ev3_reset_motor(motors);
		free(motors);
		motors = next;
	}
}

ev3_motor_ptr ev3_open_motor( ev3_motor_ptr motor )
{
	if (motor == NULL)
		return NULL;
	if (motor->duty_cycle_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/duty_cycle",motor->motor_nr);
		motor->duty_cycle_fd = open(file,O_RDONLY);
	}
	if (motor->duty_cycle_sp_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/duty_cycle_sp",motor->motor_nr);
		motor->duty_cycle_sp_fd = open(file,O_RDWR);
	}
	if (motor->position_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/position",motor->motor_nr);
		motor->position_fd = open(file,O_RDWR);
	}
	if (motor->position_sp_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/position_sp",motor->motor_nr);
		motor->position_sp_fd = open(file,O_RDWR);
	}
	if (motor->speed_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/speed",motor->motor_nr);
		motor->speed_fd = open(file,O_RDONLY);
	}
	if (motor->speed_sp_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/speed_sp",motor->motor_nr);
		motor->speed_sp_fd = open(file,O_RDWR);
	}
	if (motor->ramp_up_sp_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/ramp_up_sp",motor->motor_nr);
		motor->ramp_up_sp_fd = open(file,O_RDWR);
	}
	if (motor->ramp_down_sp_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/ramp_down_sp",motor->motor_nr);
		motor->ramp_down_sp_fd = open(file,O_RDWR);
	}
	if (motor->time_sp_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/tacho-motor/motor%i/time_sp",motor->motor_nr);
		motor->time_sp_fd = open(file,O_RDWR);
	}
	if (motor->command_stream == NULL)
	{
		char filename[1024];
		sprintf(filename,"/sys/class/tacho-motor/motor%i/command", motor->motor_nr);
		motor->command_stream = fopen(filename, "w");
	}
	return motor;
}

ev3_motor_ptr ev3_search_motor_by_identifier( ev3_motor_ptr motors, enum ev3_motor_identifier identifier, int32_t not_ready)
{
	ev3_motor_ptr motor = motors;
	while (motor)
	{
		if (motor->driver_identifier == identifier)
		{
			if (not_ready == 0)
				return motor;
			if (motor->duty_cycle_fd < 0)
				return motor;
		}
		motor = motor->next;
	}
	return NULL;
}

ev3_motor_ptr ev3_search_motor_by_port( ev3_motor_ptr motors, char port)
{
	ev3_motor_ptr motor = motors;
	while (motor)
	{
		if (motor->port == port)
			return motor;
		motor = motor->next;
	}
	return NULL;
}

void ev3_close_motor( ev3_motor_ptr motor )
{
	if (motor == NULL)
		return;
	if (motor->duty_cycle_fd >= 0)
	{
		close(motor->duty_cycle_fd);
		motor->duty_cycle_fd = -1;
	}
	if (motor->duty_cycle_sp_fd >= 0)
	{
		close(motor->duty_cycle_sp_fd);
		motor->duty_cycle_sp_fd = -1;
	}
	if (motor->position_fd >= 0)
	{
		close(motor->position_fd);
		motor->position_fd = -1;
	}
	if (motor->position_sp_fd >= 0)
	{
		close(motor->position_sp_fd);
		motor->position_sp_fd = -1;
	}
	if (motor->speed_fd >= 0)
	{
		close(motor->speed_fd);
		motor->speed_fd = -1;
	}
	if (motor->speed_sp_fd >= 0)
	{
		close(motor->speed_sp_fd);
		motor->speed_sp_fd = -1;
	}
	if (motor->ramp_up_sp_fd >= 0)
	{
		close(motor->ramp_up_sp_fd);
		motor->ramp_up_sp_fd = -1;
	}
	if (motor->ramp_down_sp_fd >= 0)
	{
		close(motor->ramp_down_sp_fd);
		motor->ramp_down_sp_fd = -1;
	}
	if (motor->time_sp_fd >= 0)
	{
		close(motor->time_sp_fd);
		motor->time_sp_fd = -1;
	}
	if (motor->command_stream != NULL)
	{
		fclose(motor->command_stream);
		motor->command_stream = NULL;
	}
}

ev3_motor_ptr ev3_command_motor( ev3_motor_ptr motor, int32_t command)
{
	if (motor == NULL)
		return NULL;
	if (command < 0)
		return motor;
	if (command >= motor->command_count)
		return motor;

	fwrite(motor->commands[command],
	       strlen(motor->commands[command]),
	       1,
	       motor->command_stream);

	fflush(motor->command_stream);
	return motor;
}

ev3_motor_ptr ev3_command_motor_by_name( ev3_motor_ptr motor, char* command)
{
	if (motor == NULL)
		return NULL;
	if (command == NULL)
		return motor;
	int32_t i;
	for (i = 0; i < motor->command_count; i++)
		if (strcmp(motor->commands[i],command) == 0)
			return ev3_command_motor( motor, i );
	return motor;
}

ev3_motor_ptr ev3_stop_action_motor( ev3_motor_ptr motor, int32_t stop_action)
{
	if (motor == NULL)
		return NULL;
	if (stop_action < 0)
		return motor;
	if (stop_action >= motor->stop_action_count)
		return motor;
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/stop_action",motor->motor_nr);
	int32_t fd = open(file,O_WRONLY);
	int32_t l = strlen(motor->stop_actions[stop_action]);
	int32_t r = write(fd,motor->stop_actions[stop_action],l);
	if (l == r)
		motor->stop_action = stop_action;
	close(fd);
	return motor;
}

ev3_motor_ptr ev3_stop_action_motor_by_name( ev3_motor_ptr motor, char* stop_action)
{
	if (motor == NULL)
		return NULL;
	if (stop_action == NULL)
		return motor;
	int32_t i;
	for (i = 0; i < motor->stop_action_count; i++)
		if (strcmp(motor->stop_actions[i],stop_action) == 0)
			return ev3_stop_action_motor( motor, i );
	return motor;
}

ev3_motor_ptr ev3_reset_motor( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return NULL;
	int32_t was_open = (motor->duty_cycle_fd >= 0);
	if (was_open)
		ev3_close_motor(motor);
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/command",motor->motor_nr);
	int32_t fd = open(file,O_WRONLY);
	int32_t l = strlen("reset");
	int32_t r = write(fd,"reset",l);
	close(fd);
	load_motor( motor, motor->motor_nr );
	if (was_open)
		ev3_open_motor(motor);
	return motor;
}

int32_t ev3_get_duty_cycle( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->duty_cycle_fd,0,SEEK_SET);
	buffer[read(motor->duty_cycle_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_duty_cycle_sp( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->duty_cycle_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->duty_cycle_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_duty_cycle_sp( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->duty_cycle_sp_fd,0,SEEK_SET);
	buffer[read(motor->duty_cycle_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

int32_t ev3_get_speed( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->speed_fd,0,SEEK_SET);
	buffer[read(motor->speed_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_speed_sp( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->speed_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->speed_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_speed_sp( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->speed_sp_fd,0,SEEK_SET);
	buffer[read(motor->speed_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_position( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->position_fd,0,SEEK_SET);
	int32_t l = write(motor->position_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_position( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->position_fd,0,SEEK_SET);
	buffer[read(motor->position_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_position_sp( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->position_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->position_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_position_sp( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->position_sp_fd,0,SEEK_SET);
	buffer[read(motor->position_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_ramp_up_sp( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->ramp_up_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->ramp_up_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_ramp_up_sp( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->ramp_up_sp_fd,0,SEEK_SET);
	buffer[read(motor->ramp_up_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_ramp_down_sp( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->ramp_down_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->ramp_down_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_ramp_down_sp( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->ramp_down_sp_fd,0,SEEK_SET);
	buffer[read(motor->ramp_down_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_time_sp( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->time_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->time_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_time_sp( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char buffer[32];
	lseek(motor->time_sp_fd,0,SEEK_SET);
	buffer[read(motor->time_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

int32_t ev3_motor_state( ev3_motor_ptr motor )
{
	if (motor == NULL)
		return 0;
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/state",motor->motor_nr);
	ev3_read_file(file,file,1024);
	int32_t result = 0;
	char* mom = file;
	char* end;
	while ((end = strchr(mom,' ')) || mom)
	{
		if (end)
			end[0] = 0;
		if (strcmp(mom,"running") == 0)
			result |= MOTOR_RUNNING;
		else
		if (strcmp(mom,"ramping") == 0)
			result |= MOTOR_RAMPING;
		else
		if (strcmp(mom,"holding") == 0)
			result |= MOTOR_HOLDING;
		else
		if (strcmp(mom,"stalled") == 0)
			result |= MOTOR_STALLED;
		else
		if (strcmp(mom,"overloaded") == 0)
			result |= MOTOR_OVERLOADED;
		mom = end;
		if (mom)
			mom++;
		else
			break;
	}
	return result;
}

ev3_motor_ptr ev3_set_polarity( ev3_motor_ptr motor, int32_t value)
{
	if (motor == NULL)
		return NULL;
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/polarity",motor->motor_nr);
	int32_t fd = open(file,O_WRONLY);
	if (fd < 0)
		return NULL;
	int32_t l;
	if (value == -1)
		l = write(motor->time_sp_fd,"inversed",strlen("inversed"));
	else
	if (value == 1)
		l = write(motor->time_sp_fd,"normal",strlen("normal"));
	close(fd);
	return motor;
}

int32_t ev3_get_polarity( ev3_motor_ptr motor)
{
	if (motor == NULL)
		return 0;
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/polarity",motor->motor_nr);
	ev3_read_file(file,file,1024);
	if (strcmp(file,"inversed") == 0)
		return -1;
	return 1;
}
