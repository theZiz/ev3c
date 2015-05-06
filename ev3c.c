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

#include "ev3c.h"

#include <dirent.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

////////////
// Sensor //
////////////


enum ev3_bin_data_format_enum get_data_format(char* buffer)
{
	if (strcmp(buffer,"u8") == 0)
		return U8;
	if (strcmp(buffer,"s8") == 0)
		return S8;
	if (strcmp(buffer,"u16") == 0)
		return U16;
	if (strcmp(buffer,"s16") == 0)
		return S16;
	if (strcmp(buffer,"s16_be") == 0)
		return S16_BE;
	if (strcmp(buffer,"s32") == 0)
		return S32;
	if (strcmp(buffer,"float") == 0)
		return FLOAT;
	return S32;
}

enum ev3_sensor_identifier get_sensor_identifier(char* buffer)
{
	if (strcmp(buffer,"ev3-analog-xx") == 0)
		return EV3_ANALOG_XX;
	if (strcmp(buffer,"nxt-analog") == 0)
		return NXT_ANALOG;
	if (strcmp(buffer,"ht-nxt-color") == 0)
		return HT_NXT_COLOR;
	if (strcmp(buffer,"ht-nxt-angle") == 0)
		return HT_NXT_ANGLE;
	if (strcmp(buffer,"ht-nxt-accel") == 0)
		return HT_NXT_ACCEL;
	if (strcmp(buffer,"ht-nxt-barometric") == 0)
		return HT_NXT_BAROMETRIC;
	if (strcmp(buffer,"ht-nxt-color-v2") == 0)
		return HT_NXT_COLOR_V2;
	if (strcmp(buffer,"ht-nxt-eopd") == 0)
		return HT_NXT_EOPD;
	if (strcmp(buffer,"ht-nxt-force") == 0)
		return HT_NXT_FORCE;
	if (strcmp(buffer,"ht-nxt-gyro") == 0)
		return HT_NXT_GYRO;
	if (strcmp(buffer,"ht-nxt-ir-link") == 0)
		return HT_NXT_IR_LINK;
	if (strcmp(buffer,"ht-nxt-ir-receiver") == 0)
		return HT_NXT_IR_RECEIVER;
	if (strcmp(buffer,"ht-nxt-pir") == 0)
		return HT_NXT_PIR;
	if (strcmp(buffer,"ht-nxt-compass") == 0)
		return HT_NXT_COMPASS;
	if (strcmp(buffer,"ht-nxt-mag") == 0)
		return HT_NXT_MAG;
	if (strcmp(buffer,"ht-nxt-seek-v2") == 0)
		return HT_NXT_IR_SEEK_V2;
	if (strcmp(buffer,"ht-nxt-smux") == 0)
		return HT_NXT_SMUX;
	if (strcmp(buffer,"ht-super-pro") == 0)
		return HT_SUPER_PRO;
	if (strcmp(buffer,"lego-ev3-uart-30") == 0)
		return LEGO_EV3_UART_30;
	if (strcmp(buffer,"lego-ev3-uart-32") == 0)
		return LEGO_EV3_UART_32;
	if (strcmp(buffer,"lego-ev3-uart-29") == 0)
		return LEGO_EV3_UART_29;
	if (strcmp(buffer,"lego-ev3-touch") == 0)
		return LEGO_EV3_TOUCH;
	if (strcmp(buffer,"lego-ev3-uart-33") == 0)
		return LEGO_EV3_UART_33;
	if (strcmp(buffer,"wedo-hub") == 0)
		return WEDO_HUB;
	if (strcmp(buffer,"wedo-motion") == 0)
		return WEDO_MOTION;
	if (strcmp(buffer,"wedo-tilt") == 0)
		return WEDO_TILT;
	if (strcmp(buffer,"lego-power-storage") == 0)
		return LEGO_POWER_STORAGE;
	if (strcmp(buffer,"tmp275") == 0)
		return TMP275;
	if (strcmp(buffer,"lego-nxt-touch") == 0)
		return LEGO_NXT_TOUCH;
	if (strcmp(buffer,"lego-nxt-light") == 0)
		return LEGO_NXT_LIGHT;
	if (strcmp(buffer,"lego-nxt-sound") == 0)
		return LEGO_NXT_SOUND;
	if (strcmp(buffer,"lego-next-us") == 0)
		return LEGO_NXT_US;
	if (strcmp(buffer,"mi-xg1300l") == 0)
		return MI_XG1300L;
	if (strcmp(buffer,"ms-absolute-imu") == 0)
		return MS_ABSOLUTE_IMU;
	if (strcmp(buffer,"ms-angle") == 0)
		return MS_ANGLE;
	if (strcmp(buffer,"ms-ev3-smux") == 0)
		return MS_EV3_SMUX;
	if (strcmp(buffer,"ms-light-array") == 0)
		return MS_LIGHT_ARRAY;
	if (strcmp(buffer,"ms-line-leader") == 0)
		return MS_LINE_LEADER;
	if (strcmp(buffer,"ms-8ch-servo") == 0)
		return MS_8CH_SERVO;
	if (strcmp(buffer,"pcf8574") == 0)
		return PCF8574;
	if (strcmp(buffer,"pcf8591") == 0)
		return PCF8591;
	if (strcmp(buffer,"ds1307") == 0)
		return DS1307;
	if (strcmp(buffer,"ms-nxt-touch-mux") == 0)
		return MS_NXT_TOUCH_MUX;
	return UNKNOWN_SENSOR;
}

int32_t read_file(char* file,char* buffer,int32_t size)
{
	int32_t fd = open(file,O_RDONLY);
	if (fd < 0)
		return fd;
	int32_t c = read(fd,buffer,size);
	if (c > 0)
		c--;
	buffer[c] = 0;
	close(fd);
	return 0;
}

void load_sensor( ev3_sensor_ptr sensor, int32_t nr)
{
	ev3_string buffer;
	char file[1024];
	//loading the struct with some initial values
	sprintf(file,"/sys/class/lego-sensor/sensor%i/driver_name",nr);
	read_file(file,sensor->driver_name,EV3_STRING_LENGTH);
	sensor->driver_identifier = get_sensor_identifier(sensor->driver_name);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/port_name",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->port = atoi(&buffer[2]);
	sensor->sensor_nr = nr;
	sensor->bin_fd = -1;
	int32_t i;
	for (i = 0; i < 8; i++)
		sensor->val_fd[i] = -1;
	memset(sensor->bin_data,0,32);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/num_values",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->data_count = atoi(buffer);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/bin_data_format",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->bin_data_format = get_data_format(buffer);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/units",nr);
	read_file(file,sensor->units,EV3_STRING_LENGTH);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/decimals",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->decimals = atoi(buffer);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/poll_ms",nr);
	if (read_file(file,buffer,EV3_STRING_LENGTH))
		sensor->poll_ms = atoi(buffer);
	else
		sensor->poll_ms = -1;
	sprintf(file,"/sys/class/lego-sensor/sensor%i/modes",nr);
	read_file(file,file,1024);
	sensor->mode_count = 0;
	char* mom = file;
	char* end;
	while (end = strchr(mom,' '))
	{
		end[0] = 0;
		sprintf(sensor->modes[sensor->mode_count],"%s",mom);
		sensor->mode_count++;
		mom = end;
		mom++;
	}
	sprintf(sensor->modes[sensor->mode_count],"%s",mom);
	sensor->mode_count++;
	sprintf(file,"/sys/class/lego-sensor/sensor%i/mode",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	for (sensor->mode = 0; sensor->mode < sensor->mode_count; sensor->mode++)
		if (strcmp(sensor->modes[sensor->mode],buffer) == 0)
			break;	
}

ev3_sensor_ptr ev3_load_sensors( void )
{
	DIR *d;
	struct dirent *dir;
	ev3_sensor_ptr first_sensor = NULL;
	ev3_sensor_ptr last_sensor = NULL;
	d = opendir("/sys/class/lego-sensor");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name,".") == 0 ||
				strcmp(dir->d_name,"..") == 0)
				continue;
			ev3_sensor_ptr sensor = (ev3_sensor_ptr)malloc(sizeof(ev3_sensor));
			load_sensor(sensor,atoi(&(dir->d_name[6])));
			sensor->next = NULL;
			if (last_sensor)
				last_sensor->next = sensor;
			else
				first_sensor = sensor;
			last_sensor = sensor;
		}
		closedir(d);
	}	
	return first_sensor;
}

void ev3_delete_sensors( ev3_sensor_ptr sensors )
{
	while (sensors)
	{
		ev3_sensor_ptr next = sensors->next;
		ev3_close_sensor(sensors);
		free(sensors);
		sensors = next;
	}
}

ev3_sensor_ptr ev3_open_sensor( ev3_sensor_ptr sensor )
{
	if (sensor == NULL)
		return NULL;
	if (sensor->bin_fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/lego-sensor/sensor%i/bin_data",sensor->sensor_nr);
		sensor->bin_fd = open(file,O_RDONLY);
	}
	int32_t i;
	for (i = 0; i < sensor->data_count; i++)
		if (sensor->val_fd[i] < 0)
		{
			char file[1024];
			sprintf(file,"/sys/class/lego-sensor/sensor%i/value%i",sensor->sensor_nr,i);
			sensor->val_fd[i] = open(file,O_RDONLY);
		}
	return sensor;
}

ev3_sensor_ptr ev3_search_sensor_by_identifier( ev3_sensor_ptr sensors, enum ev3_sensor_identifier identifier, int32_t not_ready)
{
	ev3_sensor_ptr sensor = sensors;
	while (sensor)
	{
		if (sensor->driver_identifier == identifier)
		{
			if (not_ready == 0)
				return sensor;
			if (sensor->bin_fd < 0)
				return sensor;
		}
		sensor = sensor->next;
	}
	return NULL;
}

ev3_sensor_ptr ev3_search_sensor_by_port( ev3_sensor_ptr sensors, int32_t port)
{
	ev3_sensor_ptr sensor = sensors;
	while (sensor)
	{
		if (sensor->port == port)
			return sensor;
		sensor = sensor->next;
	}
	return NULL;
}

void ev3_close_sensor( ev3_sensor_ptr sensor )
{
	if (sensor == NULL)
		return;
	if (sensor->bin_fd >= 0)
	{
		close(sensor->bin_fd);
		sensor->bin_fd = -1;
	}
	int32_t i;
	for (i = 0; i < sensor->data_count; i++)
		if (sensor->val_fd[i] >= 0)
		{
			close(sensor->val_fd[i]);
			sensor->val_fd[i] = -1;
		}
}

void ev3_update_sensor_bin( ev3_sensor_ptr sensor)
{
	if (sensor == NULL)
		return;
	lseek(sensor->bin_fd,0,SEEK_SET);
	int32_t i,r;
	switch (sensor->bin_data_format)
	{
		case U8: case S8:
			for (i = 0; i < sensor->data_count; i++)
				r = read(sensor->bin_fd,&(sensor->bin_data[i]),1);
			break;
		case U16: case S16:
			for (i = 0; i < sensor->data_count; i++)
				r = read(sensor->bin_fd,&(sensor->bin_data[i]),2);
			break;
		case S16_BE:
			for (i = 0; i < sensor->data_count; i++)
			{
				uint8_t* data = (uint8_t*)&(sensor->bin_data[i]);
				r = read(sensor->bin_fd,&(data[3]),1);
				r = read(sensor->bin_fd,&(data[2]),1);
			}
			break;
		default:
			r = read(sensor->bin_fd,sensor->bin_data,sensor->data_count * 4);
			break;
	}
}

void ev3_update_sensor_val( ev3_sensor_ptr sensor)
{
	if (sensor == NULL)
		return;
	int32_t i;
	ev3_string buffer;
	for (i = 0; i < sensor->data_count; i++)
	{
		lseek(sensor->val_fd[i],0,SEEK_SET);
		int32_t c = read(sensor->val_fd[i],buffer,EV3_STRING_LENGTH);
		if (c > 0)
		{
			c--;
			buffer[c] = 0;
			if (sensor->bin_data_format == FLOAT)
				sensor->val_data[i].f = atof(buffer);
			else
				sensor->val_data[i].s32 = atoi(buffer);
		}
	}
}

ev3_sensor_ptr ev3_mode_sensor( ev3_sensor_ptr sensor, int32_t mode)
{
	if (sensor == NULL)
		return NULL;
	if (mode < 0)
		return sensor;
	if (mode >= sensor->mode_count)
		return sensor;
	char file[1024];
	sprintf(file,"/sys/class/lego-sensor/sensor%i/mode",sensor->sensor_nr);
	int32_t fd = open(file,O_WRONLY);
	int32_t l = strlen(sensor->modes[mode]);
	int32_t r = write(fd,sensor->modes[mode],l);
	if (l == r)
	{
		sensor->mode = mode;
		ev3_string buffer;
		sprintf(file,"/sys/class/lego-sensor/sensor%i/num_values",sensor->sensor_nr);
		read_file(file,buffer,EV3_STRING_LENGTH);
		sensor->data_count = atoi(buffer);
		sprintf(file,"/sys/class/lego-sensor/sensor%i/units",sensor->sensor_nr);
		read_file(file,sensor->units,EV3_STRING_LENGTH);
		if (sensor->bin_fd >= 0)
		{
			ev3_close_sensor(sensor);
			ev3_open_sensor(sensor);
		}
	}
	close(fd);
	return sensor;
}

ev3_sensor_ptr ev3_mode_sensor_by_name( ev3_sensor_ptr sensor, char* mode)
{
	if (sensor == NULL)
		return NULL;
	if (mode == NULL)
		return sensor;
	int32_t i;
	for (i = 0; i < sensor->mode_count; i++)
		if (strcmp(sensor->modes[i],mode) == 0)
			return ev3_mode_sensor( sensor, i );
	return sensor;
}

ev3_sensor_ptr ev3_driver_sensor( ev3_sensor_ptr sensor, const char* driver)
{
	if (sensor == NULL)
		return NULL;
	int32_t was_open = (sensor->bin_fd >= 0);
	if (was_open)
		ev3_close_sensor(sensor);
	char file[1024];
	sprintf(file,"/sys/class/lego-port/port%i/set_device",sensor->port-1);
	int32_t fd = open(file,O_WRONLY);
	int32_t l = strlen(driver);
	int32_t r = write(fd,driver,l);
	close(fd);
	if (l == r)
	{
		sprintf(file,"/sys/class/lego-port/port%i/in%i:%s/lego-sensor",sensor->port-1,sensor->port,driver);
		DIR *d;
		struct dirent *dir;
		d = opendir(file);
		int32_t nr = sensor->sensor_nr;
		if (d)
		{
			while ((dir = readdir(d)) != NULL)
			{
				if (strcmp(dir->d_name,".") == 0 ||
					strcmp(dir->d_name,"..") == 0)
					continue;
				break;
			}
			nr = atoi(&(dir->d_name[6]));
			closedir(d);
		}			
		load_sensor( sensor, nr);
	}
	if (was_open)
		ev3_open_sensor(sensor);
	return sensor;
}

///////////
// Motor //
///////////

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
	ev3_string buffer;
	char file[1024];
	//loading the struct with some initial values
	sprintf(file,"/sys/class/tacho-motor/motor%i/driver_name",nr);
	read_file(file,motor->driver_name,EV3_STRING_LENGTH);
	motor->driver_identifier = get_motor_identifier(motor->driver_name);
	sprintf(file,"/sys/class/tacho-motor/motor%i/port_name",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	motor->port = buffer[3];
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
	sprintf(file,"/sys/class/tacho-motor/motor%i/commands",nr);
	read_file(file,file,1024);
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
	sprintf(file,"/sys/class/tacho-motor/motor%i/stop_commands",nr);
	read_file(file,file,1024);
	motor->stop_command_count = 0;
	mom = file;
	while (end = strchr(mom,' '))
	{
		end[0] = 0;
		sprintf(motor->stop_commands[motor->stop_command_count],"%s",mom);
		motor->stop_command_count++;
		mom = end;
		mom++;
	}
	sprintf(motor->stop_commands[motor->stop_command_count],"%s",mom);
	motor->stop_command_count++;
	sprintf(file,"/sys/class/tacho-motor/motor%i/stop_command",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	for (motor->stop_command = 0; motor->stop_command < motor->stop_command_count; motor->stop_command++)
		if (strcmp(motor->stop_commands[motor->stop_command],buffer) == 0)
			break;
	sprintf(file,"/sys/class/tacho-motor/motor%i/speed_regulation",nr);
	read_file(file,file,EV3_STRING_LENGTH);
	motor->speed_regulation = (strcmp(file,"on") == 0);	
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
}

ev3_motor_ptr ev3_command_motor( ev3_motor_ptr motor, int32_t command)
{
	if (motor == NULL)
		return NULL;
	if (command < 0)
		return motor;
	if (command >= motor->command_count)
		return motor;
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/command",motor->motor_nr);
	int32_t fd = open(file,O_WRONLY);
	int32_t l = strlen(motor->commands[command]);
	int32_t r = write(fd,motor->commands[command],l);
	close(fd);
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

ev3_motor_ptr ev3_stop_command_motor( ev3_motor_ptr motor, int32_t stop_command)
{
	if (motor == NULL)
		return NULL;
	if (stop_command < 0)
		return motor;
	if (stop_command >= motor->stop_command_count)
		return motor;
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/stop_command",motor->motor_nr);
	int32_t fd = open(file,O_WRONLY);
	int32_t l = strlen(motor->stop_commands[stop_command]);
	int32_t r = write(fd,motor->stop_commands[stop_command],l);
	if (l == r)
		motor->stop_command = stop_command;
	close(fd);
	return motor;
}

ev3_motor_ptr ev3_stop_command_motor_by_name( ev3_motor_ptr motor, char* stop_command)
{
	if (motor == NULL)
		return NULL;
	if (stop_command == NULL)
		return motor;
	int32_t i;
	for (i = 0; i < motor->stop_command_count; i++)
		if (strcmp(motor->stop_commands[i],stop_command) == 0)
			return ev3_stop_command_motor( motor, i );
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

void set_speed_regulation( ev3_motor_ptr motor, int32_t on)
{
	if (motor->speed_regulation == on)
		return;
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/speed_regulation",motor->motor_nr);
	int32_t fd = open(file,O_WRONLY);
	if (fd < 0)
		return;
	int32_t l;
	if (on)
		l = write(fd,"on",2);
	else
		l = write(fd,"off",3);
	close(fd);
	motor->speed_regulation = on;
}

int32_t ev3_get_duty_cycle( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->duty_cycle_fd,0,SEEK_SET);
	buffer[read(motor->duty_cycle_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_duty_cycle_sp( ev3_motor_ptr motor, int32_t value)
{
	set_speed_regulation( motor, 0 );
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->duty_cycle_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->duty_cycle_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_duty_cycle_sp( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->duty_cycle_sp_fd,0,SEEK_SET);
	buffer[read(motor->duty_cycle_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

int32_t ev3_get_speed( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->speed_fd,0,SEEK_SET);
	buffer[read(motor->speed_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_speed_sp( ev3_motor_ptr motor, int32_t value)
{
	set_speed_regulation( motor, 1 );
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->speed_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->speed_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_speed_sp( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->speed_sp_fd,0,SEEK_SET);
	buffer[read(motor->speed_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_position( ev3_motor_ptr motor, int32_t value)
{
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->position_fd,0,SEEK_SET);
	int32_t l = write(motor->position_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_position( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->position_fd,0,SEEK_SET);
	buffer[read(motor->position_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_position_sp( ev3_motor_ptr motor, int32_t value)
{
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->position_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->position_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_position_sp( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->position_sp_fd,0,SEEK_SET);
	buffer[read(motor->position_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_ramp_up_sp( ev3_motor_ptr motor, int32_t value)
{
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->ramp_up_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->ramp_up_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_ramp_up_sp( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->ramp_up_sp_fd,0,SEEK_SET);
	buffer[read(motor->ramp_up_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_ramp_down_sp( ev3_motor_ptr motor, int32_t value)
{
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->ramp_down_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->ramp_down_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_ramp_down_sp( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->ramp_down_sp_fd,0,SEEK_SET);
	buffer[read(motor->ramp_down_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

ev3_motor_ptr ev3_set_time_sp( ev3_motor_ptr motor, int32_t value)
{
	char buffer[32];
	sprintf(buffer,"%i",value);
	lseek(motor->time_sp_fd,0,SEEK_SET);
	int32_t l = write(motor->time_sp_fd,buffer,strlen(buffer));
	return motor;
}

int32_t ev3_get_time_sp( ev3_motor_ptr motor)
{
	char buffer[32];
	lseek(motor->time_sp_fd,0,SEEK_SET);
	buffer[read(motor->time_sp_fd,buffer,32)] = 0;
	return atoi(buffer);
}

int32_t ev3_motor_state( ev3_motor_ptr motor )
{
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/state",motor->motor_nr);
	read_file(file,file,1024);
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
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/polarity",motor->motor_nr);
	int32_t fd = open(file,O_WRONLY);
	if (fd < 0)
		return;
	int32_t l;
	if (value == -1)
		l = write(motor->time_sp_fd,"inverted",strlen("inverted"));
	else
	if (value == 1)
		l = write(motor->time_sp_fd,"normal",strlen("normal"));
	close(fd);
	return motor;
}

int32_t ev3_get_polarity( ev3_motor_ptr motor)
{
	char file[1024];
	sprintf(file,"/sys/class/tacho-motor/motor%i/polarity",motor->motor_nr);
	read_file(file,file,1024);
	if (strcmp(file,"inverted") == 0)
		return -1;
	return 1;
}

/////////////
// Buttons //
/////////////

/////////
// LCD //
/////////

int32_t __fbfd;
unsigned char* __fbp = NULL;
    
void ev3_init_lcd()
{
	__fbfd = open("/dev/fb0", O_RDWR);
	__fbp = (char*)mmap(0, EV3_SY_LCD, PROT_READ | PROT_WRITE, MAP_SHARED, __fbfd, 0);
}

void ev3_clear_lcd()
{
	memset(__fbp,0,EV3_SY_LCD);
}

#define EV3_PIXEL(x,y,bit) \
if (bit) \
	__fbp[(x >> 3) + (y) * EV3_S_LCD] |= 1 << (x & 7); \
else \
	__fbp[(x >> 3) + (y) * EV3_S_LCD] ^= 1 << (x & 7);
	
void ev3_pixel_lcd(int32_t x,int32_t y,int32_t bit)
{
	EV3_PIXEL(x,y,bit);
}

void ev3_circle_lcd(int32_t x,int32_t y,int32_t r,int32_t bit)
{
	int32_t a,b;
	int32_t minx = x-r;
	int32_t miny = y-r;
	int32_t maxx = x+r;
	int32_t maxy = y+r;
	if (minx >= (int32_t)EV3_X_LCD)
		return;
	if (miny >= (int32_t)EV3_Y_LCD)
		return;
	if (maxx < 0)
		return;
	if (maxy < 0)
		return;
	if (minx < 0)
		minx = 0;
	if (miny < 0)
		miny = 0;
	if (maxx >= EV3_X_LCD)
		maxx = EV3_X_LCD-1;
	if (maxy >= EV3_Y_LCD)
		maxy = EV3_Y_LCD-1;

	if (bit)
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
			{
				if ((a-x)*(a-x) + (b-y)*(b-y) > r*r)
					continue;
				EV3_PIXEL(a,b,1);
			}
	else
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
			{
				if ((a-x)*(a-x) + (b-y)*(b-y) > r*r)
					continue;
				EV3_PIXEL(a,b,0);
			}

}

void ev3_line_lcd(int32_t x0, int32_t y0, int32_t x1, int32_t y1,int32_t bit)
{
	int32_t dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int32_t dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int32_t err = dx+dy, e2;

	if (bit)
		for(;;)
		{
			if (x0 >= 0 && x0 < (int32_t)EV3_X_LCD && y0 >= 0 && y0 < (int32_t)EV3_Y_LCD)
				EV3_PIXEL(x0,y0,1);
			if (x0==x1 && y0==y1) break;
			e2 = 2*err;
			if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
			if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
		}
	else
		for(;;)
		{
			if (x0 >= 0 && x0 < (int32_t)EV3_X_LCD && y0 >= 0 && y0 < (int32_t)EV3_Y_LCD)
				EV3_PIXEL(x0,y0,0);
			if (x0==x1 && y0==y1) break;
			e2 = 2*err;
			if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
			if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
		}	
}

void ev3_quit_lcd()
{
	munmap(__fbp, EV3_SY_LCD);
    close(__fbfd);
}

/////////
// LED //
/////////
