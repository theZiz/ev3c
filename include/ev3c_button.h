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

#ifndef __EV3C_BUTTON
#define __EV3C_BUTTON

#include "ev3c_core.h"

/* Title: ev3c_button
 * 
 * A simple method to get the state of the buttons. */

/*enum: ev3_button_identifier
 * 
 * The different buttons types
 * 
 * Values:
 * BUTTON_LEFT - left button
 * BUTTON_RIGHT - right button
 * BUTTON_UP - up button
 * BUTTON_DOWN - down button
 * BUTTON_CENTER - center button
 * BUTTON_BACK - back button*/
enum ev3_button_identifier
{
	BUTTON_LEFT,
	BUTTON_UP,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_CENTER,
	BUTTON_BACK
};

/* Function: ev3_init_buttion
 * 
 * Initializes the button functions. Need to be called before using
 * <ev3_button_pressed>.*/
void ev3_init_button();

/* Function: ev3_button_pressed
 * 
 * Tells you, whether the named button is pressed or not.
 * 
 * Parameter:
 * button - (<ev3_button_identifier>) name of the button to check it's
 * state
 * 
 * Returns:
 * int32_t - 1 if pressed, 0 if not pressed*/
int32_t ev3_button_pressed(enum ev3_button_identifier button);

/* Function: ev3_quit_button
 * 
 * Finishes the work with the buttons. Has to be called at the very
 * end of the program */
void ev3_quit_button();

#endif
