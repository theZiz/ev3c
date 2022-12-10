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

#ifndef __EV3C_MOTOR
#define __EV3C_MOTOR

#include "ev3c_core.h"

/* Title: ev3c_motor
 *
 * These functions are for reading motor values of the EV3 brick. At the
 * moment only tacho-motors are supported from ev3c. The
 * flow is to search first for all connected motors with
 * <ev3_load_motors>. This list can then be searched with
 * <ev3_search_motor_by_identifier> and <ev3_search_motor_by_port>
 * (or by hand). To use a motor, it has to be opened with
 * <ev3_open_motor>.
 * Then some motor settings can be set like the speed with
 * <ev3_set_speed_sp> or the time to run <ev3_set_time_sp> and if
 * everything is setup the motor can be run with <ev3_command_motor>.
 * In the very end the motors should be closed
 * using <ev3_close_motor> or using <ev3_delete_motors>, which cleans
 * the list and closes all motors if open. */

/* enum: ev3_motor_identifier
 *
 * a number for supported tacho motor in ev3dev. For more
 * information have a look at the
 * <ev3dev wiki at http://www.ev3dev.org/docs/motors>.
 *
 * Values:
 * FI_L12_EV3 - Firgelli 12V Motor
 * LEGO_EV3_L_MOTOR - large lego motor, EV3 and NXT
 * LEGO_EV3_M_MOTOR - middle lego motor, atm only the small EV3 motor
 * UNKNOWN_MOTOR - unknown motor*/
enum ev3_motor_identifier
{
	FI_L12_EV3,
	LEGO_EV3_L_MOTOR,
	LEGO_EV3_M_MOTOR,
	UNKNOWN_MOTOR
};

/* enum: ev3_motor_identifier
 *
 * status returned from <ev3_motor_state>
 *
 * Values:
 * MOTOR_RUNNING - motor is running
 * MOTOR_RAMPING - motor speed is in- or decreasing
 * MOTOR_HOLDING - motor position is hold
 * MOTOR_STALLED - motor is stalled*/
enum ev3_motor_status
{
	MOTOR_RUNNING = 1,
	MOTOR_RAMPING = 2,
	MOTOR_HOLDING = 4,
	MOTOR_STALLED = 8,
	MOTOR_OVERLOADED = 16
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
 * duty_cycle_fd - (int32) file descriptor for internal use
 * duty_cycle_sp_fd - (int32) file descriptor for internal use
 * position_fd - (int32) file descriptor for internal use
 * position_sp_fd - (int32) file descriptor for internal use
 * speed_fd - (int32) file descriptor for internal use
 * speed_sp_fd - (int32) file descriptor for internal use
 * ramp_up_sp_fd - (int32) file descriptor for internal use
 * ramp_down_sp_fd - (int32) file descriptor for internal use
 * time_sp_fd - (int32) file descriptor for internal use
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
	FILE* command_stream;
	ev3_string commands[16];
	int32_t command_count;
	ev3_string stop_actions[16];
	int32_t stop_action_count;
	int32_t stop_action;
	int32_t max_speed;
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

/* Function: ev3_stop_action_motor
 *
 * Sets the stop_action of the motor, e.g. hold.
 *
 * Parameters:
 * motor - (pointer of <ev3_motor>) motor to change it's stop_action
 * stop_action - (int32) stop_action to be set to. It refers to the number in the
 * array stop_actions in motor. E.g. hold would be 2.
 *
 * Returns:
 * *ev3_motor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_stop_action_motor( motor, 1 ) );*/
ev3_motor_ptr ev3_stop_action_motor( ev3_motor_ptr motor, int32_t stop_action);

/* Function: ev3_stop_action_motor_by_name
 *
 * Sets the stop_action of the motor, e.g. "hold". Slower,
 * but more easy to use than <ev3_stop_action_motor>, which makes sense
 * espcecially if you change the stop_action of a motor quite often.
 * If such a stop_action doesn't exist, nothing happens and no stop_action is
 * changed.
 *
 * Parameters:
 * motor - (pointer of <ev3_motor>) motor to change it's stop_action
 * stop_action - (char*) stop_action to be set to. It refers to the name from in the
 * array stop_actions in motor, e.g. "hold".
 *
 * Returns:
 * *ev3_motor - the same pointer as passed as argument.  Useful for
 * such call chains as
 * ev3_open( ev3_stop_action_motor( motor, 1 ) );*/
ev3_motor_ptr ev3_stop_action_motor_by_name( ev3_motor_ptr motor, char* stop_action);

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
 * Set the duty cycle set point of a motor.
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
 * Set the speed set point of a motor.
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
 * value - (int32_t) value to set. -1 means inversed, 1 is normal*/
ev3_motor_ptr ev3_set_polarity( ev3_motor_ptr motor, int32_t value);

/* Function: ev3_get_polarity
 *
 * Returns the polarity of a motor
 *
 * Parameter:
 * motor - (pointer of <ev3_motor>) motor to get the value from
 *
 * Returns:
 * int32_t - the polarity, 1 is normal, -1 is inversed*/
int32_t ev3_get_polarity( ev3_motor_ptr motor);

#endif
