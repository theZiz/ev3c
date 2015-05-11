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


/* Title: ev3c_core
 * 
 * Some basic defines.*/
#ifndef __EV3C_CORE
#define __EV3C_CORE

#include <stdint.h>
#include <dirent.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

/* Define: EV3_STRING_LENGTH
 * 
 * length (magic number) for all strings of modes, commands and similar */
#define EV3_STRING_LENGTH 64

/* type: ev3_string
 * 
 * type used for all strings used from the driver lile mode names,
 * commands and similar. The max length is <EV3_STRING_LENGTH>.*/
typedef char ev3_string[EV3_STRING_LENGTH];

/* Function ev3_read_file
 * 
 * Reads the whole file if enough space is available in the buffer
 * 
 * Parameter:
 * file - (char*) file to read
 * buffer - (char*) buffer to write to
 * size - (int32_t) size of the buffer
 * 
 * Returns:
 * int32_t - 0 on sucess, else the error fd*/
int32_t ev3_read_file(char* file,char* buffer,int32_t size);

/* Function ev3_write_file
 * 
 * Writes the whole buffer to the file.
 * 
 * Parameter:
 * file - (char*) file to write to
 * buffer - (char*) buffer to read from
 * size - (int32_t) size to write
 * 
 * Returns:
 * int32_t - 0 on sucess, else the error fd*/
int32_t ev3_write_file(char* file,char* buffer,int32_t size);

#endif
