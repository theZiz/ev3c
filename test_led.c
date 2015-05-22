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
	ev3_init_led();

	ev3_clear_lcd();

	ev3_text_lcd_normal(0,EV3_Y_LCD-9,"Press < to continue");

	ev3_text_lcd_small(0,0,"Left LED green");
	ev3_set_led(LEFT_LED,GREEN_LED,255);

	while (!ev3_button_pressed(BUTTON_LEFT));
	while (ev3_button_pressed(BUTTON_LEFT));
	
	ev3_text_lcd_small(0,17,"Right LED red");
	ev3_set_led(RIGHT_LED,RED_LED,255);
	
	while (!ev3_button_pressed(BUTTON_LEFT));
	while (ev3_button_pressed(BUTTON_LEFT));
	
	ev3_text_lcd_small(0,34,"All LEDs half power");
	ev3_set_led(LEFT_LED,GREEN_LED,127);
	ev3_set_led(RIGHT_LED,RED_LED,127);
	
	while (!ev3_button_pressed(BUTTON_LEFT));
	while (ev3_button_pressed(BUTTON_LEFT));
	
	ev3_text_lcd_small(0,51,"All LEDs swap");
	ev3_set_led(LEFT_LED,GREEN_LED,0);
	ev3_set_led(RIGHT_LED,RED_LED,0);
	ev3_set_led(LEFT_LED,RED_LED,127);
	ev3_set_led(RIGHT_LED,GREEN_LED,127);
	
	while (!ev3_button_pressed(BUTTON_LEFT));
	while (ev3_button_pressed(BUTTON_LEFT));
	
	ev3_text_lcd_small(0,68,"All LEDs off");
	ev3_set_led(LEFT_LED,RED_LED,0);
	ev3_set_led(RIGHT_LED,GREEN_LED,0);
	
	while (!ev3_button_pressed(BUTTON_LEFT));
	while (ev3_button_pressed(BUTTON_LEFT));
	
	ev3_text_lcd_small(0,85,"All LEDs amber");
	ev3_set_led(LEFT_LED,GREEN_LED,255);
	ev3_set_led(RIGHT_LED,RED_LED,255);
	ev3_set_led(LEFT_LED,RED_LED,255);
	ev3_set_led(RIGHT_LED,GREEN_LED,255);
	
	while (!ev3_button_pressed(BUTTON_LEFT));
	while (ev3_button_pressed(BUTTON_LEFT));
	
	ev3_text_lcd_small(0,102,"All LEDs yellow");
	ev3_set_led(LEFT_LED,GREEN_LED,255);
	ev3_set_led(RIGHT_LED,RED_LED,25);
	ev3_set_led(LEFT_LED,RED_LED,25);
	ev3_set_led(RIGHT_LED,GREEN_LED,255);
	
	while (!ev3_button_pressed(BUTTON_LEFT));
	while (ev3_button_pressed(BUTTON_LEFT));
	
	ev3_quit_button();
	ev3_quit_led();
	ev3_quit_lcd();
	return 0;
}
