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

	ev3_clear_lcd();

	ev3_circle_lcd( 50, 30, 20, 1);

	ev3_circle_lcd( 70, 30, 20, 0);

	ev3_circle_lcd_out( 50, 30, 25, 1);

	ev3_rectangle_lcd( 90, 30, 20, 10, 1);

	ev3_rectangle_lcd( 100, 35, 5, 5, 0);

	ev3_rectangle_lcd_out( 100, 35, 15, 15, 1);

	ev3_ellipse_lcd( 140, 20, 30, 15, 1);

	ev3_ellipse_lcd_out( 140, 25, 30, 25, 1);

	ev3_line_lcd( 40, 10, 70, 50, 1);

	ev3_line_lcd( 5, 5, 5, EV3_Y_LCD -5, 1);

	ev3_text_lcd_large( 5, 60, "Large text!");

	ev3_text_lcd_normal( 5, 80, "Normal text!");

	ev3_text_lcd_small( 5, 90, "Small text!");

	ev3_text_lcd_tiny( 5, 100, "Tiny text!");

	ev3_quit_lcd();
	return 0;
}
