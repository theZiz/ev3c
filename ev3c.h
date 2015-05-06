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

/* Section: Sensors
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
 * <ev3dev wiki at http://www.ev3dev.org/docs/sensors>.*/
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
 * the format of the binary raw data of the sensors.*/
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

/* Section: Motors
 * 
 * These functions are for reading motor values of the EV3 brick. At the
 * moment only tacho-motors are supported from ev3c. The
 * flow is to search first for all connected motors with
 * <ev3_load_motors>. This list can then be searched with
 * <ev3_search_motor_by_identifier> and <ev3_search_motor_by_port>
 * (or by hand). To use a motor, it has to be opened with
 * <ev3_open_motor>.
 * 
 * TODO
 * 
 * In the very end the motors should be closed
 * using <ev3_close_motor> or using <ev3_delete_motors>, which cleans
 * the list and closes all motors if open. */

/* enum: ev3_motor_identifier
 * 
 * a number for supported tacho motor in ev3dev. For more
 * information have a look at the
 * <ev3dev wiki at http://www.ev3dev.org/docs/motors>.*/
enum ev3_motor_identifier
{
	FI_L12_EV3,
	LEGO_EV3_L_MOTOR,
	LEGO_EV3_M_MOTOR,
	UNKNOWN_MOTOR
};

/* enum: ev3_motor_identifier
 * 
 * status returned from <ev3_motor_state>*/
enum ev3_motor_status
{
	MOTOR_RUNNING = 1,
	MOTOR_RAMPING = 2,
	MOTOR_HOLDING = 4,
	MOTOR_STALLED = 8
};

/* type: ev3_motor
 * 
 * A struct for every loaded motor.
 * 
 * Variables:
 * driver_name - (<ev3_string>) Name of the driver
 * driver_identifier - (<ev3_motor_identifier>) Identifier of the
 * driver
 * port - (char) Port ('A'..'D') of the EV3 brick
 * motor_nr - (int32) internal, incrementing counter of the motors in
 * ev3dev
 * commands - (array of <ev3_string>) the supported commands of the motor.
 * command_count - (int32) the number of supported commands of the motor.
 * command - (int32) the command mode.
 * next - (pointer of <ev3_motor>) next element in the linked list.
 * NULL for the last element.*/

typedef struct ev3_motor_struct *ev3_motor_ptr;
typedef struct ev3_motor_struct
{
	ev3_string driver_name;
	enum ev3_motor_identifier driver_identifier;
	char port;
	int32_t motor_nr;
	int32_t duty_cycle_fd;
	int32_t duty_cycle_sp_fd;
	int32_t position_fd;
	int32_t position_sp_fd;
	int32_t speed_fd;
	int32_t speed_sp_fd;
	int32_t ramp_up_sp_fd;
	int32_t ramp_down_sp_fd;
	int32_t time_sp_fd;
	ev3_string commands[16];
	int32_t command_count;
	ev3_string stop_commands[16];
	int32_t stop_command_count;
	int32_t stop_command;
	int32_t speed_regulation;
	ev3_motor_ptr next;
} ev3_motor;

/* Function: ev3_load_motors
 * 
 * Returns:
 * *ev3_motor - returns a linked list of all available motors.*/
ev3_motor_ptr ev3_load_motors( void );

/* Function: ev3_delete_motors
 * 
 * Deletes the linked list of available motors.
 * 
 * Parameters:
 * motors - (pointer of <ev3_motor>) list to delete*/
void ev3_delete_motors( ev3_motor_ptr motors );

/* Function: ev3_open_motor
 * 
 * Opens the motor to be used. If called twice nothing happens.
 * 
 * Parameters:
 * motor - (pointer of <ev3_motor>) the motor to be opened. You can
 * just iterate of all motors returns by <ev3_load_motors> or you
 * search for a specific motor with <ev3_search_motor_by_identifier>
 * or <ev3_search_motor_by_port> and open just this one.
 * 
 * Returns:
 * *ev3_motor - returns the parameter. Useful if you opened the motor
 * with ev3_open_motor(ev3_search_motor_by_identifier(...)).*/
ev3_motor_ptr ev3_open_motor( ev3_motor_ptr motor );

/* Function: ev3_search_motor_by_identifier
 * 
 * Searches for a motor in the linked list identified with the
 * identifier. It may be, that you connected two motors with the same
 * identifier. In that case you can specify to search only for not yet
 * opened motors.
 * 
 * Parameters:
 * motors - (pointer of <ev3_motor>) linked list of motors created by
 * <ev3_load_motors>.
 * identifier - (ev3_motor_identifier) Identifier to search for.
 * not_ready - (int32) specifies, whether an already opened motor
 * should be returned or not. If not_ready is 1 only a not yet opened
 * motor will be returned.
 * 
 * Returns:
 * *ev3_motor - the found motor. May be NULL if nothing is found.*/
ev3_motor_ptr ev3_search_motor_by_identifier( ev3_motor_ptr motors, enum ev3_motor_identifier identifier, int32_t not_ready);

/* Function: ev3_search_motor_by_port
 * 
 * Searches for a motor in the linked list identified with the
 * port. The ports are labeled like on the EV3 brick starting with 'A'.
 * 
 * Parameters:
 * motors - (pointer of <ev3_motor>) linked list of motors created by
 * <ev3_load_motors>.
 * port - (char) Port to search for.
 * 
 * Returns:
 * *ev3_motor - the found motor. May be NULL if nothing is found.*/
ev3_motor_ptr ev3_search_motor_by_port( ev3_motor_ptr motors, char port);

/* Function: ev3_close_motor
 * 
 * Closes the motor. The pointer is not freed. The values are not
 * reseted or invalid.
 * 
 * Parameters:
 * motor - (point of <ev3_motor>) motor to close.*/
void ev3_close_motor( ev3_motor_ptr motor );

/* Function: ev3_command_motor
 * 
 * Sets the command of the motor, e.g. run-forever.
 * 
 * Parameters:
 * motor - (pointer of <ev3_motor>) motor to change it's command
 * command - (int32) command to be set to. It refers to the number in the
 * array commands in motor. E.g. run-forever would be 0.
 * 
 * Returns:
 * *ev3_motor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_command_motor( motor, 1 ) );*/
ev3_motor_ptr ev3_command_motor( ev3_motor_ptr motor, int32_t command);

/* Function: ev3_command_motor_by_name
 * 
 * Sets the command of the motor, e.g. "run-forever". Slower,
 * but more easy to use than <ev3_command_motor>, which makes sense
 * espcecially if you change the command of a motor quite often.
 * If such a command doesn't exist, nothing happens and no command is
 * changed.
 * 
 * Parameters:
 * motor - (pointer of <ev3_motor>) motor to change it's command
 * command - (char*) command to be set to. It refers to the name from in the
 * array commands in motor, e.g. "run-forever".
 * 
 * Returns:
 * *ev3_motor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_command_motor( motor, 1 ) );*/
ev3_motor_ptr ev3_command_motor_by_name( ev3_motor_ptr motor, char* command);

/* Function: ev3_stop_command_motor
 * 
 * Sets the stop_command of the motor, e.g. hold.
 * 
 * Parameters:
 * motor - (pointer of <ev3_motor>) motor to change it's stop_command
 * stop_command - (int32) stop_command to be set to. It refers to the number in the
 * array stop_commands in motor. E.g. hold would be 2.
 * 
 * Returns:
 * *ev3_motor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_stop_command_motor( motor, 1 ) );*/
ev3_motor_ptr ev3_stop_command_motor( ev3_motor_ptr motor, int32_t stop_command);

/* Function: ev3_stop_command_motor_by_name
 * 
 * Sets the stop_command of the motor, e.g. "hold". Slower,
 * but more easy to use than <ev3_stop_command_motor>, which makes sense
 * espcecially if you change the stop_command of a motor quite often.
 * If such a stop_command doesn't exist, nothing happens and no stop_command is
 * changed.
 * 
 * Parameters:
 * motor - (pointer of <ev3_motor>) motor to change it's stop_command
 * stop_command - (char*) stop_command to be set to. It refers to the name from in the
 * array stop_commands in motor, e.g. "hold".
 * 
 * Returns:
 * *ev3_motor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_stop_command_motor( motor, 1 ) );*/
ev3_motor_ptr ev3_stop_command_motor_by_name( ev3_motor_ptr motor, char* stop_command);

/* Function: ev3_reset_motor
 * 
 * Reset the motor to a default state. It may be a good idea to call
 * this right after loading but before opening the motor.
 * 
 * Parameters:
 * motor - (pointer of <ev3_motor>) motor to reset
 * 
 * Returns:
 * ev3_motor - the same pointer as passed as argument.*/
ev3_motor_ptr ev3_reset_motor( ev3_motor_ptr motor);

/* Function: ev3_get_duty_cycle
 * 
 * Returns the duty cycle of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the duty cycle*/
int32_t ev3_get_duty_cycle( ev3_motor_ptr motor);

/* Function: ev3_set_duty_cycle_sp
 * 
 * Set the duty cycle set point of a motor. Sets the speed regulation
 * to "off" if needed.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set*/
ev3_motor_ptr ev3_set_duty_cycle_sp( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_duty_cycle_sp
 * 
 * Returns the duty cycle set point of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the duty cycle set point*/
int32_t ev3_get_duty_cycle_sp( ev3_motor_ptr motor);

/* Function: ev3_get_speed
 * 
 * Returns the speed of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the speed*/
int32_t ev3_get_speed( ev3_motor_ptr motor);

/* Function: ev3_set_speed_sp
 * 
 * Set the speed set point of a motor. Sets the speed regulation
 * to "on" if needed.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set*/
ev3_motor_ptr ev3_set_speed_sp( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_speed_sp
 * 
 * Returns the speed set point of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the speed set point*/
int32_t ev3_get_speed_sp( ev3_motor_ptr motor);

/* Function: ev3_set_position
 * 
 * Set the position of a motor.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set*/
ev3_motor_ptr ev3_set_position( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_position
 * 
 * Returns the position of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the position*/
int32_t ev3_get_position( ev3_motor_ptr motor);

/* Function: ev3_set_position_sp
 * 
 * Set the position set point of a motor.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set*/
ev3_motor_ptr ev3_set_position_sp( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_position_sp
 * 
 * Returns the position set point of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the position set point*/
int32_t ev3_get_position_sp( ev3_motor_ptr motor);

/* Function: ev3_set_ramp_up_sp
 * 
 * Set the ramp up set point of a motor.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set*/
ev3_motor_ptr ev3_set_ramp_up_sp( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_ramp_up_sp
 * 
 * Returns the ramp up set point of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the ramp up set point*/
int32_t ev3_get_ramp_up_sp( ev3_motor_ptr motor);

/* Function: ev3_set_ramp_down_sp
 * 
 * Set the ramp down set point of a motor.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set*/
ev3_motor_ptr ev3_set_ramp_down_sp( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_ramp_down_sp
 * 
 * Returns the ramp down set point of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the ramp down set point*/
int32_t ev3_get_ramp_down_sp( ev3_motor_ptr motor);

/* Function: ev3_set_time_sp
 * 
 * Set the time set point of a motor.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set*/
ev3_motor_ptr ev3_set_time_sp( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_time_sp
 * 
 * Returns the time set point of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the time set point*/
int32_t ev3_get_time_sp( ev3_motor_ptr motor);

/* Function: ev3_motor_state
 * 
 * Returns the state of the motor.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the state of the motor. The possibilites are all OR
 * combinations of <ev3_motor_status>.*/
int32_t ev3_motor_state( ev3_motor_ptr motor );

/* Function: ev3_set_polarity
 * 
 * Set the polarity of a motor.
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to set the value
 * value - (int32_t) value to set. -1 means inverted, 1 is normal*/
ev3_motor_ptr ev3_set_polarity( ev3_motor_ptr motor, int32_t value)

/* Function: ev3_get_polarity
 * 
 * Returns the polarity of a motor
 * 
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 * 
 * Returns:
 * int32_t - the polarity, 1 is normal, -1 is inverted*/
int32_t ev3_get_polarity( ev3_motor_ptr motor);

/* Section: LCD
 * 
 * Some simple functions for drawing text and basic primitives to the
 * lcd. */

#define EV3_X_LCD 178
#define EV3_Y_LCD 128
#define EV3_S_LCD 24
#define EV3_SY_LCD (EV3_Y_LCD * EV3_S_LCD)

 
void ev3_init_lcd();

void ev3_clear_lcd();

void ev3_circle_lcd(int32_t x,int32_t y,int32_t r,int32_t bit);

void ev3_line_lcd(int32_t x0, int32_t y0, int32_t x1, int32_t y1,int32_t bit);

void ev3_quit_lcd();

#endif
