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
  
#include "ev3c_lcd.h"

#include "large_font.xbm"
#define LARGE_FONT_PIXEL(x,y) ((large_font_bits[((x) >> 3) + (y) * 32 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define LARGE_LETTER_WIDTH 16
#define LARGE_LETTER_HEIGHT 16
#include "normal_font.xbm"
#define NORMAL_FONT_PIXEL(x,y) ((normal_font_bits[((x) >> 3) + (y) * 16 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define NORMAL_LETTER_WIDTH 8
#define NORMAL_LETTER_HEIGHT 9
#include "small_font.xbm"
#define SMALL_FONT_PIXEL(x,y) ((small_font_bits[((x) >> 3) + (y) * 16 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define SMALL_LETTER_WIDTH 8
#define SMALL_LETTER_HEIGHT 8
#include "tiny_font.xbm"
#define TINY_FONT_PIXEL(x,y) ((tiny_font_bits[((x) >> 3) + (y) * 16 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define TINY_LETTER_WIDTH 5
#define TINY_FONT_WIDTH 8
#define TINY_LETTER_HEIGHT 7

int32_t __fbfd;
unsigned char* __fbp = NULL;
    
void ev3_init_lcd()
{
	__fbfd = open("/dev/fb0", O_RDWR);
	__fbp = (char*)mmap(0, EV3_SY_LCD, PROT_READ | PROT_WRITE, MAP_SHARED, __fbfd, 0);
}

void ev3_clear_lcd()
{
	memset(__fbp,0,EV3_SY_LCD);
}

#define EV3_PIXEL_SET(x,y) __fbp[((x) >> 3) + (y) * EV3_S_LCD] |= 1 << ((x) & 7)
#define EV3_PIXEL_UNSET(x,y) __fbp[((x) >> 3) + (y) * EV3_S_LCD] &= 0xFF ^ (1 << ((x) & 7))

#define EV3_PIXEL(x,y,bit) {if (bit) EV3_PIXEL_SET(x,y); else EV3_PIXEL_UNSET(x,y);}

void ev3_pixel_lcd(int32_t x,int32_t y,int32_t bit)
{
	EV3_PIXEL(x,y,bit);
}

void ev3_text_lcd_large(int32_t x,int32_t y,const char* text)
{
	int32_t i;
	for (i = 0; text[i]; i++)
	{
		if (text[i] < ' ' || text[i] > 127)
			continue;
		int32_t c_x = ((text[i] - ' ') % 16) * LARGE_LETTER_WIDTH;
		int32_t c_y = ((text[i] - ' ') / 16) * LARGE_LETTER_HEIGHT;
		int32_t a;
		for (a = 0; a < LARGE_LETTER_WIDTH; a++)
		{
			int32_t X = x + i * LARGE_LETTER_WIDTH + a;
			if (X < 0)
				continue;
			if (X >= (int32_t)EV3_X_LCD)
				return;
			int32_t b;
			for (b = 0; b < LARGE_LETTER_HEIGHT; b++)
			{
				int32_t Y = y + b;
				if (Y < 0)
					continue;
				if (Y >= (int32_t)EV3_Y_LCD)
					break;
				if (LARGE_FONT_PIXEL(c_x + a,c_y + b))
					EV3_PIXEL_SET(X,Y);
			}
		}
	}
}

void ev3_text_lcd_normal(int32_t x,int32_t y,const char* text)
{
	int32_t i;
	for (i = 0; text[i]; i++)
	{
		if (text[i] < ' ' || text[i] > 127)
			continue;
		int32_t c_x = ((text[i] - ' ') % 16) * NORMAL_LETTER_WIDTH;
		int32_t c_y = ((text[i] - ' ') / 16) * NORMAL_LETTER_HEIGHT;
		int32_t a;
		for (a = 0; a < NORMAL_LETTER_WIDTH; a++)
		{
			int32_t X = x + i * NORMAL_LETTER_WIDTH + a;
			if (X < 0)
				continue;
			if (X >= (int32_t)EV3_X_LCD)
				return;
			int32_t b;
			for (b = 0; b < NORMAL_LETTER_HEIGHT; b++)
			{
				int32_t Y = y + b;
				if (Y < 0)
					continue;
				if (Y >= (int32_t)EV3_Y_LCD)
					break;
				if (NORMAL_FONT_PIXEL(c_x + a,c_y + b))
					EV3_PIXEL_SET(X,Y);
			}
		}
	}
}

void ev3_text_lcd_small(int32_t x,int32_t y,const char* text)
{
	int32_t i;
	for (i = 0; text[i]; i++)
	{
		if (text[i] < ' ' || text[i] > 127)
			continue;
		int32_t c_x = ((text[i] - ' ') % 16) * SMALL_LETTER_WIDTH;
		int32_t c_y = ((text[i] - ' ') / 16) * SMALL_LETTER_HEIGHT;
		int32_t a;
		for (a = 0; a < SMALL_LETTER_WIDTH; a++)
		{
			int32_t X = x + i * SMALL_LETTER_WIDTH + a;
			if (X < 0)
				continue;
			if (X >= (int32_t)EV3_X_LCD)
				return;
			int32_t b;
			for (b = 0; b < SMALL_LETTER_HEIGHT; b++)
			{
				int32_t Y = y + b;
				if (Y < 0)
					continue;
				if (Y >= (int32_t)EV3_Y_LCD)
					break;
				if (SMALL_FONT_PIXEL(c_x + a,c_y + b))
					EV3_PIXEL_SET(X,Y);
			}
		}
	}
}

void ev3_text_lcd_tiny(int32_t x,int32_t y,const char* text)
{
	int32_t i;
	for (i = 0; text[i]; i++)
	{
		if (text[i] < ' ' || text[i] > 127)
			continue;
		int32_t c_x = ((text[i] - ' ') % 16) * TINY_FONT_WIDTH;
		int32_t c_y = ((text[i] - ' ') / 16) * TINY_LETTER_HEIGHT;
		int32_t a;
		for (a = 0; a < TINY_LETTER_WIDTH; a++)
		{
			int32_t X = x + i * TINY_LETTER_WIDTH + a;
			if (X < 0)
				continue;
			if (X >= (int32_t)EV3_X_LCD)
				return;
			int32_t b;
			for (b = 0; b < TINY_LETTER_HEIGHT; b++)
			{
				int32_t Y = y + b;
				if (Y < 0)
					continue;
				if (Y >= (int32_t)EV3_Y_LCD)
					break;
				if (TINY_FONT_PIXEL(c_x + a,c_y + b))
					EV3_PIXEL_SET(X,Y);
			}
		}
	}
}

void ev3_rectangle_lcd(int32_t x,int32_t y,int32_t w,int32_t h,int32_t bit)
{
	int32_t a,b;
	int32_t minx = x;
	int32_t miny = y;
	int32_t maxx = x+w;
	int32_t maxy = y+h;
	if (minx >= (int32_t)EV3_X_LCD)
		return;
	if (miny >= (int32_t)EV3_Y_LCD)
		return;
	if (maxx < 0)
		return;
	if (maxy < 0)
		return;
	if (minx < 0)
		minx = 0;
	if (miny < 0)
		miny = 0;
	if (maxx >= EV3_X_LCD)
		maxx = EV3_X_LCD-1;
	if (maxy >= EV3_Y_LCD)
		maxy = EV3_Y_LCD-1;

	if (bit)
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
				EV3_PIXEL_SET(a,b);
	else
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
				EV3_PIXEL_UNSET(a,b);
}

void ev3_rectangle_lcd_out(int32_t x,int32_t y,int32_t w,int32_t h,int32_t bit)
{
	int32_t a;
	int32_t minx = x;
	int32_t miny = y;
	int32_t maxx = x+w;
	int32_t maxy = y+h;
	if (minx >= (int32_t)EV3_X_LCD)
		return;
	if (miny >= (int32_t)EV3_Y_LCD)
		return;
	if (maxx < 0)
		return;
	if (maxy < 0)
		return;
	if (minx < 0)
		minx = 0;
	if (miny < 0)
		miny = 0;
	if (maxx >= EV3_X_LCD)
		maxx = EV3_X_LCD-1;
	if (maxy >= EV3_Y_LCD)
		maxy = EV3_Y_LCD-1;

	if (bit)
	{
		if (y == miny)
			for (a = minx; a <= maxx; a++)
				EV3_PIXEL_SET(a,miny);
		if (y+w == maxy)
			for (a = minx; a <= maxx; a++)
				EV3_PIXEL_SET(a,maxy);
		if (x == minx)
			for (a = miny+1; a <= maxy-1; a++)
				EV3_PIXEL_SET(minx,a);
		if (x+h == maxx)
			for (a = miny+1; a <= maxy-1; a++)
				EV3_PIXEL_SET(maxx,a);
	}
	else
	{
		if (y == miny)
			for (a = minx; a <= maxx; a++)
				EV3_PIXEL_UNSET(a,miny);
		if (y+w == maxy)
			for (a = minx; a <= maxx; a++)
				EV3_PIXEL_UNSET(a,maxy);
		if (x == minx)
			for (a = miny+1; a <= maxy-1; a++)
				EV3_PIXEL_UNSET(minx,a);
		if (x+h == maxx)
			for (a = miny+1; a <= maxy-1; a++)
				EV3_PIXEL_UNSET(maxx,a);
	}
}

void ev3_circle_lcd(int32_t x,int32_t y,int32_t r,int32_t bit)
{
	int32_t a,b;
	int32_t minx = x-r;
	int32_t miny = y-r;
	int32_t maxx = x+r;
	int32_t maxy = y+r;
	if (minx >= (int32_t)EV3_X_LCD)
		return;
	if (miny >= (int32_t)EV3_Y_LCD)
		return;
	if (maxx < 0)
		return;
	if (maxy < 0)
		return;
	if (minx < 0)
		minx = 0;
	if (miny < 0)
		miny = 0;
	if (maxx >= EV3_X_LCD)
		maxx = EV3_X_LCD-1;
	if (maxy >= EV3_Y_LCD)
		maxy = EV3_Y_LCD-1;

	if (bit)
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
			{
				if ((a-x)*(a-x) + (b-y)*(b-y) >= r*r)
					continue;
				EV3_PIXEL_SET(a,b);
			}
	else
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
			{
				if ((a-x)*(a-x) + (b-y)*(b-y) >= r*r)
					continue;
				EV3_PIXEL_UNSET(a,b);
			}
}

#define MIRROR_PIXEL_SET(a,b) \
{ \
	if (y+b >= 0 && y+b < EV3_Y_LCD) \
	{ \
		if (x+a >= 0 && x+a < EV3_X_LCD) \
			EV3_PIXEL_SET(x+a,y+b); \
		if (x-a >= 0 && x-a < EV3_X_LCD) \
			EV3_PIXEL_SET(x-a,y+b); \
	} \
	if (y-b >= 0 && y-b < EV3_Y_LCD) \
	{ \
		if (x+a >= 0 && x+a < EV3_X_LCD) \
			EV3_PIXEL_SET(x+a,y-b); \
		if (x-a >= 0 && x-a < EV3_X_LCD) \
			EV3_PIXEL_SET(x-a,y-b); \
	} \
}
#define MIRROR_PIXEL_UNSET(a,b) \
{ \
	if (y+b >= 0 && y+b < EV3_Y_LCD) \
	{ \
		if (x+a >= 0 && x+a < EV3_X_LCD) \
			EV3_PIXEL_UNSET(x+a,y+b); \
		if (x-a >= 0 && x-a < EV3_X_LCD) \
			EV3_PIXEL_UNSET(x-a,y+b); \
	} \
	if (y-b >= 0 && y-b < EV3_Y_LCD) \
	{ \
		if (x+a >= 0 && x+a < EV3_X_LCD) \
			EV3_PIXEL_UNSET(x+a,y-b); \
		if (x-a >= 0 && x-a < EV3_X_LCD) \
			EV3_PIXEL_UNSET(x-a,y-b); \
	} \
}

void ev3_circle_lcd_out(int32_t x,int32_t y,int32_t r,int32_t bit)
{
	int32_t a,b;
	if (bit)
	{
		a = r;
		for (b = 0; b <= r; b++)
		{
			int once = 0;
			while (a*a + b*b >= r*r)
			{
				once = 1;
				a--;
				MIRROR_PIXEL_SET(a,b)
				if (a == 0)
					break;
			}
			if (!once)
				MIRROR_PIXEL_SET(a,b)
		}
	}
	else
	{
		a = r;
		for (b = 0; b <= r; b++)
		{
			int once = 0;
			while (a*a + b*b >= r*r)
			{
				once = 1;
				a--;
				MIRROR_PIXEL_UNSET(a,b)
				if (a == 0)
					break;
			}
			if (!once)
				MIRROR_PIXEL_UNSET(a,b)
		}
	}

}

void ev3_ellipse_lcd(int32_t x,int32_t y,int32_t rx,int32_t ry,int32_t bit)
{
	int32_t a,b;
	int32_t minx = x-rx;
	int32_t miny = y-ry;
	int32_t maxx = x+rx;
	int32_t maxy = y+ry;
	if (minx >= (int32_t)EV3_X_LCD)
		return;
	if (miny >= (int32_t)EV3_Y_LCD)
		return;
	if (maxx < 0)
		return;
	if (maxy < 0)
		return;
	if (minx < 0)
		minx = 0;
	if (miny < 0)
		miny = 0;
	if (maxx >= EV3_X_LCD)
		maxx = EV3_X_LCD-1;
	if (maxy >= EV3_Y_LCD)
		maxy = EV3_Y_LCD-1;

	if (bit)
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
			{
				if ((a-x)*(a-x)*ry*ry + (b-y)*(b-y)*rx*rx >= rx*rx*ry*ry)
					continue;
				EV3_PIXEL_SET(a,b);
			}
	else
		for (a = minx; a <= maxx; a++)
			for (b = miny; b <= maxy; b++)
			{
				if ((a-x)*(a-x)*ry*ry + (b-y)*(b-y)*rx*rx >= rx*rx*ry*ry)
					continue;
				EV3_PIXEL_UNSET(a,b);
			}
}

void ev3_ellipse_lcd_out(int32_t x,int32_t y,int32_t rx,int32_t ry,int32_t bit)
{
	int32_t a,b;
	if (bit)
	{
		a = rx;
		for (b = 0; b <= ry; b++)
		{
			int once = 0;
			while (a*a*ry*ry + b*b*rx*rx >= rx*rx*ry*ry)
			{
				once = 1;
				a--;
				MIRROR_PIXEL_SET(a,b)
				if (a == 0)
					break;
			}
			if (!once)
				MIRROR_PIXEL_SET(a,b)
		}
	}
	else
	{
		a = rx;
		for (b = 0; b <= ry; b++)
		{
			int once = 0;
			while (a*a*ry*ry + b*b*rx*rx >= rx*rx*ry*ry)
			{
				once = 1;
				a--;
				MIRROR_PIXEL_UNSET(a,b)
				if (a == 0)
					break;
			}
			if (!once)
				MIRROR_PIXEL_UNSET(a,b)
		}
	}

}

void ev3_line_lcd(int32_t x0, int32_t y0, int32_t x1, int32_t y1,int32_t bit)
{
	int32_t dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int32_t dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int32_t err = dx+dy, e2;

	if (bit)
		for(;;)
		{
			if (x0 >= 0 && x0 < (int32_t)EV3_X_LCD && y0 >= 0 && y0 < (int32_t)EV3_Y_LCD)
				EV3_PIXEL(x0,y0,1);
			if (x0==x1 && y0==y1) break;
			e2 = 2*err;
			if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
			if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
		}
	else
		for(;;)
		{
			if (x0 >= 0 && x0 < (int32_t)EV3_X_LCD && y0 >= 0 && y0 < (int32_t)EV3_Y_LCD)
				EV3_PIXEL(x0,y0,0);
			if (x0==x1 && y0==y1) break;
			e2 = 2*err;
			if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
			if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
		}	
}

void ev3_quit_lcd()
{
	munmap(__fbp, EV3_SY_LCD);
    close(__fbfd);
}
