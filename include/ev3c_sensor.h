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

#ifndef __EV3C_SENSOR
#define __EV3C_SENSOR

#include "ev3c_core.h"

/* Title: ev3c_sensor
 * 
 * These functions are for reading sensor values of the EV3 brick. The
 * flow is to search first for all connected sensors with
 * <ev3_load_sensors>. This list can then be searched with
 * <ev3_search_sensor_by_identifier> and <ev3_search_sensor_by_port>
 * (or by hand). To use a sensor, it has to be opened with
 * <ev3_open_sensor>. The sensor values are stored in the <ev3_sensor>
 * struct (variables bin_data and val_data). However they need to be
 * updated by hand calling <ev3_update_sensor_bin> or
 * <ev3_update_sensor_val>. In the very end the sensors should be closed
 * using <ev3_close_sensor> or using <ev3_delete_sensors>, which cleans
 * the list and closes all sensors if open.
 * 
 * Keep in mind, that some sensors are not identified automaticly (like
 * the NXT sound sensor). These device need to be set to the right
 * driver using <ev3_driver_sensor>.*/

/* enum: ev3_sensor_identifier
 * 
 * a number for every (yet) supported sensor in ev3dev. For more
 * information have a look at the
 * <ev3dev wiki at http://www.ev3dev.org/docs/sensors>.
 * 
 * Values:
 * EV3_ANALOG_XX - EV3_ANALOG_XX
 * NXT_ANALOG - NXT_ANALOG
 * HT_NXT_COLOR - HT_NXT_COLOR
 * HT_NXT_ANGLE - HT_NXT_ANGLE
 * HT_NXT_ACCEL - HT_NXT_ACCEL
 * HT_NXT_BAROMETRIC - HT_NXT_BAROMETRIC
 * HT_NXT_COLOR_V2 - HT_NXT_COLOR_V2
 * HT_NXT_EOPD - HT_NXT_EOPD
 * HT_NXT_FORCE - HT_NXT_FORCE
 * HT_NXT_GYRO - HT_NXT_GYRO
 * HT_NXT_IR_LINK - HT_NXT_IR_LINK
 * HT_NXT_IR_RECEIVER - HT_NXT_IR_RECEIVER
 * HT_NXT_PIR - HT_NXT_PIR
 * HT_NXT_COMPASS - HT_NXT_COMPASS
 * HT_NXT_MAG - HT_NXT_MAG
 * HT_NXT_IR_SEEK_V2 - HT_NXT_IR_SEEK_V2
 * HT_NXT_SMUX - HT_NXT_SMUX
 * HT_SUPER_PRO - HT_SUPER_PRO
 * LEGO_EV3_UART_30 - LEGO_EV3_UART_30
 * LEGO_EV3_UART_32 - LEGO_EV3_UART_32
 * LEGO_EV3_UART_29 - LEGO_EV3_UART_29
 * LEGO_EV3_TOUCH - LEGO_EV3_TOUCH
 * LEGO_EV3_UART_33 - LEGO_EV3_UART_33
 * WEDO_HUB - WEDO_HUB
 * WEDO_MOTION - WEDO_MOTION
 * WEDO_TILT - WEDO_TILT
 * LEGO_POWER_STORAGE - LEGO_POWER_STORAGE
 * TMP275 - TMP275
 * LEGO_NXT_TOUCH - LEGO_NXT_TOUCH
 * LEGO_NXT_LIGHT - LEGO_NXT_LIGHT
 * LEGO_NXT_SOUND - LEGO_NXT_SOUND
 * LEGO_NXT_US - LEGO_NXT_US
 * MI_XG1300L - MI_XG1300L
 * MS_ABSOLUTE_IMU - MS_ABSOLUTE_IMU
 * MS_ANGLE - MS_ANGLE
 * MS_EV3_SMUX - MS_EV3_SMUX
 * MS_LIGHT_ARRAY - MS_LIGHT_ARRAY
 * MS_LINE_LEADER - MS_LINE_LEADER
 * MS_8CH_SERVO - MS_8CH_SERVO
 * PCF8574 - PCF8574
 * PCF8591 - PCF8591
 * DS1307 - DS1307
 * MS_NXT_TOUCH_MUX - MS_NXT_TOUCH_MUX
 * UNKNOWN_SENSOR- UNKNOWN_SENSOR*/
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
	UNKNOWN_SENSOR
};

/* enum: ev3_bin_data_format_enum
 * 
 * the format of the binary raw data of the sensors.
 * 
 * Values:
 * U8 - unsigned, one byte length
 * S8 - signed, one byte length
 * U16 - unsigned, two bytes length
 * S16 - signed, two bytes length
 * S16_BE - signed, two beytes length, Big Endian
 * S32 - signed, four bytes length
 * FLOAT - float*/
enum ev3_bin_data_format_enum
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
 * port - (int32) Port (1..4) of the EV3 brick
 * sensor_nr - (int32) internal, incrementing counter of the sensors in
 * ev3dev
 * bin_fd - (int32) file descriptor for reading the raw data for the
 * sensor. -1 if not set or if an error occured.
 * val_fd - (array of int32) 8 file descriptors for reading the formated
 * data of the sensor.
 * bin_data - (array of union) the read raw data of the sensor.
 * data_count values are stored. Use <ev3_update_sensor_bin> to update
 * the data before using it. The union consists of u8 (uint8), s8 (int8),
 * u16 (uint16), s16 (int16), s16_be (int16 big endian), s32 (int32) and
 * f (float). Which you should use depends on bin_data_format. However
 * s32 should always give you useful results (except for f and s16_be).
 * val_data - (array of union) the read formated data of the sensor.
 * data_count values are stored. Use <ev3_update_sensor_val> to update
 * the data before using it. The union consits only of s32 (int32) and
 * f (float). f makes only sense to use, if bin_data_format is FLOAT.
 * units - (<ev3_string>) the units used by the values, like V (Volt),
 * pct (Percent) or u/m² (unicors per square meter).
 * decimals - (int32) the decimal shift you should consider while interpretating
 * the formated values, especially for the units to make sense. E.g.
 * if the formated value is 667, units "pct" and decimals 1, the value
 * would be 66.7% .
 * poll_ms - (int32) the time in ms, when the sensor is reread for
 * I2C devices.
 * modes - (array of <ev3_string>) the supported modes of the sensor.
 * mode_count - (int32) the number of supported modes of the sensor.
 * However some modes may not be useable anyway. Have a look at the
 * documentation of the sensor type for more information.
 * mode - (int32) the choosen mode.
 * next - (pointer of <ev3_sensor>) next element in the linked list.
 * NULL for the last element.*/

typedef struct ev3_sensor_struct *ev3_sensor_ptr;
typedef struct ev3_sensor_struct
{
	ev3_string driver_name;
	enum ev3_sensor_identifier driver_identifier;
	int32_t port;
	int32_t sensor_nr;
	int32_t bin_fd;
	int32_t val_fd[8];
	union
	{
		uint8_t u8;
		int8_t s8;
		uint16_t u16;
		int16_t s16;
		int16_t s16_be;
		int32_t s32;
		float f;
	} bin_data[8];
	union
	{
		int32_t s32;
		float f;
	} val_data[8];
	int32_t data_count;
	enum ev3_bin_data_format_enum bin_data_format;
	ev3_string units;
	int32_t decimals;
	int32_t poll_ms;
	ev3_string modes[16];
	int32_t mode_count;
	int32_t mode;
	ev3_sensor_ptr next;
} ev3_sensor;

/* Function: ev3_load_sensors
 * 
 * Returns:
 * *ev3_sensor - returns a linked list of all available sensors.*/
ev3_sensor_ptr ev3_load_sensors( void );

/* Function: ev3_delete_sensors
 * 
 * Deletes the linked list of available sensors.
 * 
 * Parameters:
 * sensors - (pointer of <ev3_sensor>) list to delete*/
void ev3_delete_sensors( ev3_sensor_ptr sensors );

/* Function: ev3_open_sensor
 * 
 * Opens the sensor to be used. If called twice nothing happens.
 * 
 * Parameters:
 * sensor - (pointer of <ev3_sensor>) the sensor to be opened. You can
 * just iterate of all sensors returns by <ev3_load_sensors> or you
 * search for a specific sensor with <ev3_search_sensor_by_identifier>
 * or <ev3_search_sensor_by_port> and open just this one.
 * 
 * Returns:
 * *ev3_sensor - returns the parameter. Useful if you opened the sensor
 * with ev3_open_sensor(ev3_search_sensor_by_identifier(...)).*/
ev3_sensor_ptr ev3_open_sensor( ev3_sensor_ptr sensor );

/* Function: ev3_search_sensor_by_identifier
 * 
 * Searches for a sensor in the linked list identified with the
 * identifier. It may be, that you connected two sensors with the same
 * identifier (e.g. two ev3-touch-sensors). In that case you can
 * specify to search only for not yet opened sensors.
 * 
 * Parameters:
 * sensors - (pointer of <ev3_sensor>) linked list of sensors created by
 * <ev3_load_sensors>.
 * identifier - (ev3_sensor_identifier) Identifier to search for.
 * not_ready - (int32) specifies, whether an already opened sensor
 * should be returned or not. If not_ready is 1 only a not yet opened
 * sensor will be returned.
 * 
 * Returns:
 * *ev3_sensor - the found sensor. May be NULL if nothing is found.*/
ev3_sensor_ptr ev3_search_sensor_by_identifier( ev3_sensor_ptr sensors, enum ev3_sensor_identifier identifier, int32_t not_ready);

/* Function: ev3_search_sensor_by_port
 * 
 * Searches for a sensor in the linked list identified with the
 * port. The ports are labeled like on the EV3 brick starting with 1.
 * 
 * Parameters:
 * sensors - (pointer of <ev3_sensor>) linked list of sensors created by
 * <ev3_load_sensors>.
 * port - (int32) Port to search for.
 * 
 * Returns:
 * *ev3_sensor - the found sensor. May be NULL if nothing is found.*/
ev3_sensor_ptr ev3_search_sensor_by_port( ev3_sensor_ptr sensors, int32_t port);

/* Function: ev3_close_sensor
 * 
 * Closes the sensor. The pointer is not freed. The values are not
 * reseted or invalid. You "just" can't use <ev3_update_sensor_bin> and
 * <ev3_update_sensor_val> anymore.
 * 
 * Parameters:
 * sensor - (point of <ev3_sensor>) sensor to close.*/
void ev3_close_sensor( ev3_sensor_ptr sensor );

/* Function: ev3_update_sensor_bin
 * 
 * Updates the raw values of the sensor. Faster than
 * <ev3_update_sensor_val>, but the values may not be useful. Use it
 * only if you ran into serious speed problems.
 * The sensor needs to be opened with <ev3_open_sensor> first.
 * 
 * Parameters:
 * sensor - (pointer of <ev3_sensor>) sensor to update it's raw values
 * stored in bin_data.*/
void ev3_update_sensor_bin( ev3_sensor_ptr sensor );

/* Function: ev3_update_sensor_val
 * 
 * Updates the formated values of the sensor. Slower than
 * <ev3_update_sensor_bin>, but the values are much more useful.
 * Furthermore if I say "slow" I mean around 50µs, which is in fact
 * quite fast (but still slower than ev3_update_sensor_bin).
 * The sensor needs to be opened with <ev3_open_sensor> first.
 * 
 * Parameters:
 * sensor - (pointer of <ev3_sensor>) sensor to update it's formated
 * values stored in val_data.*/
void ev3_update_sensor_val( ev3_sensor_ptr sensor );

/* Function: ev3_mode_sensor
 * 
 * Sets the mode of the sensor, e.g. COL-COLOR on the color sensor to
 * get the color of the underground
 * 
 * Parameters:
 * sensor - (pointer of <ev3_sensor>) sensor to change it's mode
 * mode - (int32) mode to be set to. It refers to the number in the
 * array modes in sensor. E.g. COL-COLOUR would be 2.
 * 
 * Returns:
 * *ev3_sensor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_mode_sensor( sensor, 1 ) );*/
ev3_sensor_ptr ev3_mode_sensor( ev3_sensor_ptr sensor, int32_t mode);

/* Function: ev3_mode_sensor_by_name
 * 
 * Sets the mode of the sensor, e.g. "COL-COLOR" on the color sensor to
 * get the color of the underground using the name of the mode. Slower,
 * but more easy to use than <ev3_mode_sensor>, which makes sense
 * espcecially if you change the mode of a sensor quite often (e.g.
 * using the IR sensor for IR control and distance measurment at the
 * same time).
 * If such a mode doesn't exist, nothing happens and no mode is changed.
 * 
 * Parameters:
 * sensor - (pointer of <ev3_sensor>) sensor to change it's mode
 * mode - (char*) mode to be set to. It refers to the name from in the
 * array modes in sensor, e.g. "COL-COLOUR".
 * 
 * Returns:
 * *ev3_sensor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_mode_sensor( sensor, 1 ) );*/
ev3_sensor_ptr ev3_mode_sensor_by_name( ev3_sensor_ptr sensor, char* mode);

/* Functiion: ev3_driver_sensor
 * 
 * Some sensors (like the nxt sound sensor) are just identified as
 * generic analog sensor. It is possible to use it, but the value may
 * not be very handy or interpretable. So this function can be used
 * to "force" a driver for a specific device. If the device is already
 * open, it will be open afterwards. The same sensor struct is still
 * used.
 * 
 * Parameters:
 * sensor - (pointer of <ev3_sensor>) sensor to force to a new driver.
 * driver - (const char*) name of the new driver, look in the
 * documentation in the
 * <ev3dev wiki at http://www.ev3dev.org/docs/sensors> to get the right
 * driver name.
 * 
 * Returns:
 * *ev3_sensor - the same pointer as passed as parameter. Useful for
 * such call chains as
 * ev3_open( ev3_driver_sensor( sound, "lego-nxt-sound" ) );*/
ev3_sensor_ptr ev3_driver_sensor( ev3_sensor_ptr sensor, const char* driver);

#endif
