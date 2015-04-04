#include "ev3c.h"

#include <stdio.h>

int main(int argc,char** argv)
{
	ev3_sensor_ptr sensors = ev3_load_sensors();
	ev3_sensor_ptr sensor = sensors;
	int i;
	while (sensor)
	{
		ev3_open_sensor(sensor);
		ev3_update_sensor_bin(sensor);
		printf("==== %s ====\n",sensor->driver_name);
		printf("ident: %i\n",sensor->driver_identifier);
		printf("sensor: %i\n",sensor->sensor_nr);
		printf("port: %i\n",sensor->port_nr);
		printf("fd: %i\n",sensor->fd);
		printf("data count: %i\n",sensor->data_count);
		for (i = 0; i < sensor->data_count; i++)
		{
			printf("data[%i].s32: %i\n",i,sensor->data[i].s32);
			printf("data[%i].f: %f\n",i,sensor->data[i].f);
		}
		printf("data format: %i\n",sensor->data_format);
		printf("units: %s\n",sensor->units);
		printf("decimals: %s\n",sensor->decimals);
		printf("poll ms: %i\n",sensor->poll_ms);
		printf("mode count: %i\n",sensor->mode_count);
		for (i = 0; i < sensor->mode_count; i++)
		{
			printf("mode[%i]: %s\n",i,sensor->modes[i]);
		}
		printf("mode: %i\n",sensor->mode);
		sensor = sensor->next;
	}
	ev3_sensor_ptr touch_ev3 = ev3_search_sensor_by_identifier( sensors, LEGO_EV3_TOUCH, 0);
	ev3_open_sensor(touch_ev3);
	for (i = 0; i < 10; i++)
	{
		ev3_update_sensor_bin(touch_ev3);
		printf("%i: %i\n",i,touch_ev3->data[0].s32);
		sleep(1);
	}
	ev3_delete_sensors(sensors);
	return 0;
}
