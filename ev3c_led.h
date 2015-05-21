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

#ifndef __EV3C_LED
#define __EV3C_LED

#include "ev3c_core.h"

/* Title: ev3c_led
 * 
 * Some functions for using the leds*/

/* enum: ev3_led_name
 * 
 * name of an led
 * 
 * Values:
 * LEFT_LED - left led
 * RIGHT_LED - right led */
enum ev3_led_name
{
	LEFT_LED = 0,
	RIGHT_LED = 2
};

/* enum: ev3_led_color
 * 
 * color of an led
 * 
 * Values:
 * GREEN_LED - green led
 * RED_LED - red led */
enum ev3_led_color
{
	GREEN_LED = 0,
	RED_LED = 1
};

/* Function: ev3_init_led
 * 
 * Initializes the led functions. Need to be called before using the leds
 * */
void ev3_init_led();

/* Function: ev3_set_led
 * 
 * Sets the brightness of an led.
 * 
 * Parameters:
 * led - (<ev3_led_name>) which led (LEFT_LED or RIGHT_LED)
 * color - (<ev3_led_color>) which color (GREEN_LED or RED_LED)
 * value - (int32_t) brightness, possible values are between 0 and 255 */
void ev3_set_led(enum ev3_led_name led,enum ev3_led_color color,int32_t value);

/* Function: ev3_get_led
 * 
 * Returns the brightness of an led.
 * 
 * Parameters:
 * led - (<ev3_led_name>) which led (LEFT_LED or RIGHT_LED)
 * color - (<ev3_led_color>) which color (GREEN_LED or RED_LED)
 * 
 * Returns:
 * int32_t - brightness, possible values are between 0 and 255*/
int32_t ev3_get_led(enum ev3_led_name led,enum ev3_led_color color);

/* Function: ev3_quit_led
 * 
 * Finishes the work with the leds. Has to be called at the very
 * end of the program */
void ev3_quit_led();

#endif
