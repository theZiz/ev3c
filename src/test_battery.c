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
	int c = ev3c_current();
	printf("EV3 battery current: %i.%i mA\n",c/1000,c%1000);
	int v = ev3c_voltage();
	printf("EV3 battery voltage: %i.%i mV\n",v/1000,v%1000);
	return 0;
}
