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

#include "ev3c_sensor.h"

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

void load_sensor( ev3_sensor_ptr sensor, int32_t nr)
{
	ev3_string buffer;
	char file[1024];
	//loading the struct with some initial values
	sprintf(file,"/sys/class/lego-sensor/sensor%i/driver_name",nr);
	ev3_read_file(file,sensor->driver_name,EV3_STRING_LENGTH);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/address",nr);
	ev3_read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->port = atoi(&buffer[12]); //ev3-ports:inX
	sensor->sensor_nr = nr;
	sensor->bin_fd = -1;
	int32_t i;
	for (i = 0; i < 8; i++)
		sensor->val_fd[i] = -1;
	memset(sensor->bin_data,0,32);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/num_values",nr);
	ev3_read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->data_count = atoi(buffer);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/bin_data_format",nr);
	ev3_read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->bin_data_format = get_data_format(buffer);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/units",nr);
	ev3_read_file(file,sensor->units,EV3_STRING_LENGTH);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/decimals",nr);
	ev3_read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->decimals = atoi(buffer);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/poll_ms",nr);
	if (ev3_read_file(file,buffer,EV3_STRING_LENGTH))
		sensor->poll_ms = atoi(buffer);
	else
		sensor->poll_ms = -1;
	sprintf(file,"/sys/class/lego-sensor/sensor%i/modes",nr);
	ev3_read_file(file,file,1024);
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
	ev3_read_file(file,buffer,EV3_STRING_LENGTH);
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

ev3_sensor_ptr ev3_search_sensor_by_name( ev3_sensor_ptr sensors, char* name, int32_t not_ready)
{
	ev3_sensor_ptr sensor = sensors;
	while (sensor)
	{
		if ( strcmp(sensor->driver_name,name) == 0 )
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
		ev3_read_file(file,buffer,EV3_STRING_LENGTH);
		sensor->data_count = atoi(buffer);
		sprintf(file,"/sys/class/lego-sensor/sensor%i/units",sensor->sensor_nr);
		ev3_read_file(file,sensor->units,EV3_STRING_LENGTH);
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
		sprintf(file,"/sys/class/lego-port/port%i/ev3-ports:in%i:%s/lego-sensor",sensor->port-1,sensor->port,driver);
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
