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
  
#include "ev3c_button.h"

#include <linux/input.h>

int32_t __ev3_button_fd = -1;

void ev3_init_button()
{
	__ev3_button_fd = open("/dev/input/by-path/platform-gpio-keys.0-event", O_RDONLY);
}

int32_t ev3_button_pressed(enum ev3_button_identifier button)
{
	uint8_t keys[96];
	lseek(__ev3_button_fd,0,SEEK_SET);
	ioctl (__ev3_button_fd, EVIOCGKEY(sizeof keys), &keys);
	switch (button)
	{
		case BUTTON_LEFT:
			return keys[KEY_LEFT >> 3] & (1 << (KEY_LEFT & 7))?0:1;
		case BUTTON_UP:
			return keys[KEY_UP >> 3] & (1 << (KEY_UP & 7))?0:1;
		case BUTTON_RIGHT:
			return keys[KEY_RIGHT >> 3] & (1 << (KEY_RIGHT & 7))?0:1;
		case BUTTON_DOWN:
			return keys[KEY_DOWN >> 3] & (1 << (KEY_DOWN & 7))?0:1;
		case BUTTON_CENTER:
			return keys[KEY_ENTER >> 3] & (1 << (KEY_ENTER & 7))?0:1;
		case BUTTON_BACK:
			return keys[KEY_BACKSPACE >> 3] & (1 << (KEY_BACKSPACE & 7))?0:1;
	}
	return 0;
}

void ev3_quit_button()
{
	close(__ev3_button_fd);
}
