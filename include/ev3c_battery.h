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

#ifndef __EV3C_BATTERY
#define __EV3C_BATTERY

#include "ev3c_core.h"

/* Title: ev3c_battery
 * 
 * Some functions for getting the state of the battery. It is not
 * possible to give a percentage of the remaining energy, so only
 * voltage and current are returned. However a voltage above 7 is good.*/

/* Function: ev3c_current
 * 
 * Returns the current of the battery.
 * 
 * Returns:
 * uint32_t - the current in µA*/
uint32_t ev3c_current();

/* Function: ev3c_voltage
 * 
 * Returns the voltage of the battery.
 * 
 * Returns:
 * uint32_t - the voltage in µV*/
uint32_t ev3c_voltage();

#endif
