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
  * Alexander Matthes (Ziz) , ziz_at_mailbox.com */


/* Title: ev3c
 * 
 * ev3c is a C wrapper library for interacting with the
 * - sensors
 * - motors
 * - leds
 * - buttons
 * - lcd and
 * - battery
 * of the Mindstorms EV3 brick using ev3dev.*/
#ifndef __EV3C
#define __EV3C

#include <stdint.h>

/* Define: EV3_STRING_LENGTH
 * 
 * length (magic number) for all strings of modes, commands and similar */
#define EV3_STRING_LENGTH 64

/* type: ev3_string
 * 
 * type used for all strings used from the driver lile mode names,
 * commands and similar. The max length is <EV3_STRING_LENGTH>.*/
typedef char ev3_string[EV3_STRING_LENGTH];

/* enum: ev3_sensor_identifier
 * 
 * a number for every (yet) supported sensor in ev3dev*/
enum ev3_sensor_identifier
{
	EV3_ANALOG_XX,
	NXT_ANALOG,
	HT_NXT_COLOR,
	HT_NXT_ANGLE,
	HT_NXT_ACCEL,
	HT_NXT_BAROMETRIC,
	HT_NXT_COLOR_V2,
	HT_NXT_EOPD,
	HT_NXT_FORCE,
	HT_NXT_GYRO,
	HT_NXT_IR_LINK,
	HT_NXT_IR_RECEIVER,
	HT_NXT_PIR,
	HT_NXT_COMPASS,
	HT_NXT_MAG,
	HT_NXT_IR_SEEK_V2,
	HT_NXT_SMUX,
	HT_SUPER_PRO,
	LEGO_EV3_UART_30,
	LEGO_EV3_UART_32,
	LEGO_EV3_UART_29,
	LEGO_EV3_TOUCH,
	LEGO_EV3_UART_33,
	WEDO_HUB,
	WEDO_MOTION,
	WEDO_TILT,
	LEGO_POWER_STORAGE,
	TMP275,
	LEGO_NXT_TOUCH,
	LEGO_NXT_LIGHT,
	LEGO_NXT_SOUND,
	LEGO_NXT_US,
	MI_XG1300L,
	MS_ABSOLUTE_IMU,
	MS_ANGLE,
	MS_EV3_SMUX,
	MS_LIGHT_ARRAY,
	MS_LINE_LEADER,
	MS_8CH_SERVO,
	PCF8574,
	PCF8591,
	DS1307,
	MS_NXT_TOUCH_MUX,
	UNKNOWN
};

enum ev3_data_format_enum
{
	U8,
	S8,
	U16,
	S16,
	S16_BE,
	S32,
	FLOAT
};

/* type: ev3_sensor
 * 
 * A struct for every loaded sensor.
 * 
 * Variables:
 * driver_name - (<ev3_string>) Name of the driver
 * driver_identifier - (<ev3_sensor_identifier>) Identifier of the
 * driver
 * port_nr - (int32) Port (1..4) of the EV3 brick
 * sensor_nr - (int32) internal, incrementing counter of the sensors in
 * ev3dev */

typedef struct ev3_sensor_struct *ev3_sensor_ptr;
typedef struct ev3_sensor_struct
{
	ev3_string driver_name;
	enum ev3_sensor_identifier driver_identifier;
	int32_t port_nr;
	int32_t sensor_nr;
	int32_t fd;
	union
	{
		uint8_t u8;
		int8_t s8;
		uint16_t u16;
		int16_t s16;
		int16_t s16_be;
		int32_t s32;
		float f;
	} data[8];
	int data_count;
	enum ev3_data_format_enum data_format;
	ev3_string units;
	int decimals;
	int poll_ms;
	ev3_string modes[16];
	int mode_count;
	int mode;
	ev3_sensor_ptr next;
} ev3_sensor;

ev3_sensor_ptr ev3_load_sensors( void );

void ev3_delete_sensors( ev3_sensor_ptr sensors );

int ev3_open_sensor( ev3_sensor_ptr sensor );

ev3_sensor_ptr ev3_search_sensor_by_identifier( ev3_sensor_ptr sensors, enum ev3_sensor_identifier identifier, int not_ready);

void ev3_close_sensor( ev3_sensor_ptr sensor );

void ev3_update_sensor_bin( ev3_sensor_ptr sensor );

void ev3_update_sensor_value( ev3_sensor_ptr sensor );

void ev3_mode_sensor( ev3_sensor_ptr sensor, const char* mode);

#endif
