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

#ifndef __EV3C_LCD
#define __EV3C_LCD

#include "ev3c_core.h"
  
/* Title: ev3c_lcd
 * 
 * Some simple functions for drawing text and basic primitives to the
 * lcd. */

/* Define: EV3_X_LCD
 * 
 * x size of the EV3 screen (178)*/
#define EV3_X_LCD 178

/* Define: EV3_Y_LCD
 * 
 * y size of the EV3 screen (128)*/
#define EV3_Y_LCD 128

/* Define: EV3_S_LCD
 * 
 * length of the scanline of the EV3 screen in bytes (24)*/
#define EV3_S_LCD 24

/* Define: EV3_SY_LCD
 * 
 * product of the scanline length and the screen height as whole size
 * of the screenbuffer array (3072).*/
#define EV3_SY_LCD (EV3_Y_LCD * EV3_S_LCD)

/* Function: ev3_init_lcd
 * 
 * Initializes the lcd. You need to call this first! */
void ev3_init_lcd();

/* Function: ev3_text_lcd_large
 * 
 * Draws text in a large font (each letter 16x16)
 * 
 * Parameter:
 * x - (int32_t) x position on the screen
 * y - (int32_t) y position on the screen
 * text - (const char*) text to draw*/
void ev3_text_lcd_large(int32_t x,int32_t y,const char* text);

/* Function: ev3_text_lcd_normal
 * 
 * Draws text in a normal font (each letter 8x9)
 * 
 * Parameter:
 * x - (int32_t) x position on the screen
 * y - (int32_t) y position on the screen
 * text - (const char*) text to draw*/
void ev3_text_lcd_normal(int32_t x,int32_t y,const char* text);

/* Function: ev3_text_lcd_small
 * 
 * Draws text in a small font (each letter 8x8)
 * 
 * Parameter:
 * x - (int32_t) x position on the screen
 * y - (int32_t) y position on the screen
 * text - (const char*) text to draw*/
void ev3_text_lcd_small(int32_t x,int32_t y,const char* text);

/* Function: ev3_text_lcd_tiny
 * 
 * Draws text in a tiny font (each letter 5x7)
 * 
 * Parameter:
 * x - (int32_t) x position on the screen
 * y - (int32_t) y position on the screen
 * text - (const char*) text to draw*/
void ev3_text_lcd_tiny(int32_t x,int32_t y,const char* text);

/* Function: ev3_clear_lcd
 * 
 * Blanks the screen*/
void ev3_clear_lcd();

/* Function: ev3_rectangle_lcd
 * 
 * Draws a filled rectangle
 * 
 * Parameter:
 * x,y - (int32_t,int32_t) position of the left upper corner of the
 * rectangle
 * w - (int32_t) width of the rectangle
 * h - (int32_t) height of the rectangle
 * bit - color of the rectangle (1 black, 0 white) */
void ev3_rectangle_lcd(int32_t x,int32_t y,int32_t w,int32_t h,int32_t bit);

/* Function: ev3_rectangle_lcd_out
 * 
 * Draws a rectangle frame
 * 
 * Parameter:
 * x,y - (int32_t,int32_t) position of the left upper corner of the
 * rectangle
 * w - (int32_t) width of the rectangle
 * h - (int32_t) height of the rectangle
 * bit - color of the rectangle (1 black, 0 white) */
void ev3_rectangle_lcd_out(int32_t x,int32_t y,int32_t w,int32_t h,int32_t bit);

/* Function: ev3_circle_lcd
 * 
 * Draws a filled circle
 * 
 * Parameter:
 * x,y - (int32_t,int32_t) position of the center of the circle
 * r - (int32_t) radius of the circle
 * bit - color of the circle (1 black, 0 white) */
void ev3_circle_lcd(int32_t x,int32_t y,int32_t r,int32_t bit);

/* Function: ev3_circle_lcd_out
 * 
 * Draws a circle frame
 * 
 * Parameter:
 * x,y - (int32_t,int32_t) position of the center of the circle
 * r - (int32_t) radius of the circle
 * bit - color of the circle (1 black, 0 white) */
void ev3_circle_lcd_out(int32_t x,int32_t y,int32_t r,int32_t bit);

/* Function: ev3_ellipse_lcd
 * 
 * Draws a filled ellipse
 * 
 * Parameter:
 * x,y - (int32_t,int32_t) position of the center of the ellipse
 * rx,ry - (int32_t,int32_t) x and y radius of the ellipse
 * bit - color of the circle (1 black, 0 white) */
void ev3_ellipse_lcd(int32_t x,int32_t y,int32_t rx,int32_t ry,int32_t bit);

/* Function: ev3_ellipse_lcd_out
 * 
 * Draws a ellipse frame
 * 
 * Parameter:
 * x,y - (int32_t,int32_t) position of the center of the ellipse
 * rx,ry - (int32_t,int32_t) x and y radius of the ellipse
 * bit - color of the circle (1 black, 0 white) */
void ev3_ellipse_lcd_out(int32_t x,int32_t y,int32_t rx,int32_t ry,int32_t bit);

/* Function: ev3_line_lcd
 * 
 * Draws a line
 * 
 * Parameter:
 * x0,y0 - (int32_t,int32_t) start of the line
 * x1,y1 - (int32_t,int32_t) end of the line
 * bit - color of the line (1 black, 0 white) */
void ev3_line_lcd(int32_t x0, int32_t y0, int32_t x1, int32_t y1,int32_t bit);

/* Function:
 * 
 * Finishes the lcd functionality*/
void ev3_quit_lcd();

#endif
