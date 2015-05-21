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
  
#include "ev3c_led.h"

int32_t __ev3_led_fd[4] = {-1,-1,-1,-1};

void ev3_init_led()
{
	ev3_write_file("/sys/class/leds/ev3-left1:green:ev3dev/trigger","none",4);
	ev3_write_file("/sys/class/leds/ev3-left0:red:ev3dev/trigger","none",4);
	ev3_write_file("/sys/class/leds/ev3-right1:green:ev3dev/trigger","none",4);
	ev3_write_file("/sys/class/leds/ev3-right0:red:ev3dev/trigger","none",4);
	__ev3_led_fd[0] = open("/sys/class/leds/ev3-left1:green:ev3dev/brightness", O_RDWR);
	__ev3_led_fd[1] = open("/sys/class/leds/ev3-left0:red:ev3dev/brightness", O_RDWR);
	__ev3_led_fd[2] = open("/sys/class/leds/ev3-right1:green:ev3dev/brightness", O_RDWR);
	__ev3_led_fd[3] = open("/sys/class/leds/ev3-right0:red:ev3dev/brightness", O_RDWR);
}

void ev3_set_led(enum ev3_led_name led,enum ev3_led_color color,int32_t value)
{
	if (value < 0)
		value = 0;
	if (value > 255)
		value = 255;
	char buffer[16];
	sprintf(buffer,"%i",value);
	lseek(__ev3_led_fd[led + color],0,SEEK_SET);
	int l = write(__ev3_led_fd[led + color],buffer,strlen(buffer));
}

int32_t ev3_get_led(enum ev3_led_name led,enum ev3_led_color color)
{
	char buffer[16];
	lseek(__ev3_led_fd[led + color],0,SEEK_SET);
	buffer[read(__ev3_led_fd[led + color],buffer,16)] = 0;
	return atoi(buffer);
}

void ev3_quit_led()
{
	int i;
	for (i = 0; i < 4; i++)
		close(__ev3_led_fd[i]);
}
