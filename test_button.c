/*
 The contents of this file are subject to the "do whatever you like"-license.
 That means: Do, whatver you want, this file is under public domain. It is an
 example for ev3c. Copy it and learn from it for your project and release
 it under every license you want. ;-)
 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , ziz_at_mailbox.org, http://github.com/theZiz
*/

#include "ev3c.h"

#include <stdio.h>

int main(int argc,char** argv)
{
	ev3_init_lcd();
	ev3_init_button();
	while (!ev3_button_pressed(BUTTON_BACK))
	{
		ev3_clear_lcd();
		ev3_text_lcd_large(0,0,"Button Test");
		if (ev3_button_pressed(BUTTON_LEFT))
			ev3_text_lcd_normal(0,20,"Left");
		if (ev3_button_pressed(BUTTON_RIGHT))
			ev3_text_lcd_normal(0,30,"Right");
		if (ev3_button_pressed(BUTTON_UP))
			ev3_text_lcd_normal(0,40,"Up");
		if (ev3_button_pressed(BUTTON_DOWN))
			ev3_text_lcd_normal(0,50,"down");
		if (ev3_button_pressed(BUTTON_CENTER))
			ev3_text_lcd_normal(0,60,"Center");
		ev3_text_lcd_normal(0,120,"Back: Exit");
		usleep(1000);
	}
	ev3_quit_button();
	ev3_quit_lcd();
	return 0;
}
