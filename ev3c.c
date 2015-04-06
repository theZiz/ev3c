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
	sensor->driver_identifier = get_identifier(sensor->driver_name);
	sprintf(file,"/sys/class/lego-sensor/sensor%i/port_name",nr);
	read_file(file,buffer,EV3_STRING_LENGTH);
	sensor->port_nr = atoi(&buffer[2]);
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
		if (sensor->port_nr == port)
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

ev3_sensor_ptr ev3_driver_sensor( ev3_sensor_ptr sensor, const char* driver)
{
	if (sensor == NULL)
		return NULL;
	int32_t was_open = (sensor->bin_fd >= 0);
	if (was_open)
		ev3_close_sensor(sensor);
	char file[1024];
	sprintf(file,"/sys/class/lego-port/port%i/set_device",sensor->port_nr-1);
	int32_t fd = open(file,O_WRONLY);
	int32_t l = strlen(driver);
	int32_t r = write(fd,driver,l);
	close(fd);
	if (l == r)
	{
		sprintf(file,"/sys/class/lego-port/port%i/in%i:%s/lego-sensor",sensor->port_nr-1,sensor->port_nr,driver);
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
