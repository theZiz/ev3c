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
  
#include "ev3c_battery.h"

uint32_t ev3c_current()
{
	char buffer[32];
	ev3_read_file("/sys/class/power_supply/legoev3-battery/current_now",buffer,32);
	return atoi(buffer);
}

uint32_t ev3c_voltage()
{
	char buffer[32];
	ev3_read_file("/sys/class/power_supply/legoev3-battery/voltage_now",buffer,32);
	return atoi(buffer);
}
