#include "ev3c.h"

#include <dirent.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

enum ev3_data_format_enum get_data_format(char* buffer)
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

enum ev3_sensor_identifier get_identifier(char* buffer)
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
	return UNKNOWN;
}

int read_file(char* file,char* buffer,int size)
{
	int fd = open(file,O_RDONLY);
	if (fd < 0)
		return fd;
	int c = read(fd,buffer,size);
	printf("%i\n",c);
	if (c > 0)
		c--;
	buffer[c] = 0;
	close(fd);
	return 0;
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
			ev3_string buffer;
			char file[1024];
			//loading the struct with some initial values
			sprintf(file,"/sys/class/lego-sensor/%s/driver_name",dir->d_name);
			read_file(file,sensor->driver_name,EV3_STRING_LENGTH);
			sensor->driver_identifier = get_identifier(sensor->driver_name);
			sprintf(file,"/sys/class/lego-sensor/%s/port_name",dir->d_name);
			read_file(file,buffer,EV3_STRING_LENGTH);
			sensor->port_nr = atoi(&buffer[2]);
			sensor->sensor_nr = atoi(&(dir->d_name[6]));
			sensor->fd = -1;
			memset(sensor->data,0,32);
			sprintf(file,"/sys/class/lego-sensor/%s/num_values",dir->d_name);
			read_file(file,buffer,EV3_STRING_LENGTH);
			sensor->data_count = atoi(buffer);
			sprintf(file,"/sys/class/lego-sensor/%s/bin_data_format",dir->d_name);
			read_file(file,buffer,EV3_STRING_LENGTH);
			sensor->data_format = get_data_format(buffer);
			sprintf(file,"/sys/class/lego-sensor/%s/units",dir->d_name);
			read_file(file,sensor->units,EV3_STRING_LENGTH);
			sprintf(file,"/sys/class/lego-sensor/%s/decimals",dir->d_name);
			read_file(file,buffer,EV3_STRING_LENGTH);
			sensor->decimals = atoi(buffer);
			sprintf(file,"/sys/class/lego-sensor/%s/poll_ms",dir->d_name);
			if (read_file(file,buffer,EV3_STRING_LENGTH))
				sensor->poll_ms = atoi(buffer);
			else
				sensor->poll_ms = -1;
			sprintf(file,"/sys/class/lego-sensor/%s/modes",dir->d_name);
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
			sensor->next = NULL;
			sprintf(file,"/sys/class/lego-sensor/%s/mode",dir->d_name);
			read_file(file,buffer,EV3_STRING_LENGTH);
			for (sensor->mode = 0; sensor->mode < sensor->mode_count; sensor->mode++)
				if (strcmp(sensor->modes[sensor->mode],buffer) == 0)
					break;
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

int ev3_open_sensor( ev3_sensor_ptr sensor )
{
	if (sensor->fd < 0)
	{
		char file[1024];
		sprintf(file,"/sys/class/lego-sensor/sensor%i/bin_data",sensor->sensor_nr);
		sensor->fd = open(file,O_RDONLY);
	}
	return sensor->fd;
}

ev3_sensor_ptr ev3_search_sensor_by_identifier( ev3_sensor_ptr sensors, enum ev3_sensor_identifier identifier, int not_ready)
{
	ev3_sensor_ptr sensor = sensors;
	while (sensor)
	{
		if (sensor->driver_identifier == identifier)
		{
			if (not_ready == 0)
				return sensor;
			if (sensor->fd < 0)
				return sensor;
		}
		sensor = sensor->next;
	}
	return NULL;
}

void ev3_close_sensor( ev3_sensor_ptr sensor )
{
	if (sensor->fd >= 0)
	{
		close(sensor->fd);
		sensor->fd = -1;
	}
}

void ev3_update_sensor_bin( ev3_sensor_ptr sensor)
{
	lseek(sensor->fd,0,SEEK_SET);
	int i;
	switch (sensor->data_format)
	{
		case U8: case S8:
			for (i = 0; i < sensor->data_count; i++)
				read(sensor->fd,&(sensor->data[i]),1);
			break;
		case U16: case S16:
			for (i = 0; i < sensor->data_count; i++)
				read(sensor->fd,&(sensor->data[i]),2);
			break;
		case S16_BE:
			for (i = 0; i < sensor->data_count; i++)
			{
				uint8_t* data = (uint8_t*)&(sensor->data[i]);
				read(sensor->fd,&(data[3]),1);
				read(sensor->fd,&(data[2]),1);
			}
			break;
		default:
			read(sensor->fd,sensor->data,sensor->data_count * 4);
			break;
	}
}

void ev3_update_sensor_value( ev3_sensor_ptr sensor);

void ev3_mode_sensor( ev3_sensor_ptr sensor, const char* mode);
