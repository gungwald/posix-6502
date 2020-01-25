/*
 * more - Implemented for ProDOS on 6502 CPU
 * Copyright (c) 2015 Bill Chatfield
 * All rights reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* 

TODO
1. Back up a page.
2. Find ways to make it faster.
3. Fix problem with bottom line when it is longer than a screen line.
4. Pull the disk from the Internet.
5. Integrate standard file selector.
6. Implement search forward.
7. Implement search backward.
8. Goto beginning, end.
9. Display file percentage in control line. 

*/

#include <stdio.h>
#include <string.h>     /* strlen */
#include <stdint.h>     /* uint8_t, UINT8_MAX */
#include <stdlib.h>     /* EXIT_SUCCESS */
#include <stdbool.h>    /* true, false */
#include <ctype.h>      /* tolower */
#include <conio.h>      /* screensize, revers */

#include "characters.h"
#include "consoleio.h"

#define STRING_SIZE 128

char *input_file_name(char fileName[], size_t fileNameSize);
void more(char *file_name);
uint8_t chomp(char *line);
char *trunc(char *s, size_t max_len);
uint8_t display_line(const char *s);

/* Have to use global variables because local variable capacity
   is limited. */
unsigned char screen_width;
unsigned char screen_height;

int main(int argc, char *argv[])
{
    static char fileName[STRING_SIZE];
    uint8_t i;

    screensize(&screen_width, &screen_height);
    /*
    printf("width=%d height=%d\n", screen_width, screen_height);
    */

    if (argc <= 1) {
        /* ProDOS doesn't support piping input/output so, if there are no 
           arguments, ask for a file name. */
        input_file_name(fileName, sizeof(fileName));
        if (fileName != NULL && strcmp(fileName,"")!=0) {
            more(fileName);
        }
    }
    else {
        /* If there are arguments, process them. */
        for (i = 1; i < argc; ++i) {
            more(argv[i]);
        }
    }
    return EXIT_SUCCESS;
}

char *input_file_name(char fileName[], size_t fileNameSize)
{
    printf("FILE:");
    cioReadLine(fileName, sizeof(line));
    return fileName;
}

uint8_t display_line(const char *line)
{
    size_t len;
    uint8_t line_count = 0;

    puts(line);
    len = strlen(line);
    line_count = len / screen_width;
    if ((len==0) || (len % screen_width > 0)) {
        line_count++;
    }
    return line_count;
}

uint8_t display_control_line(uint8_t line_count)
{
    char c;

    revers(true);
    cputs("--More--");
    revers(false);
    c = tolower(cgetc());
    cioEraseBackwards(8);
    switch (c) {
        case 'b':
            /* Go back one page.*/
            break;
        case ' ':
            /* Print the next page.*/
            line_count = 0;
            break;
        case 'q':
            /* Quit.*/
            exit(EXIT_SUCCESS);
        case CARRIAGE_RETURN:
            /* Display one more line. */
            line_count -= 1;
            break;
    }
    return line_count;
}

void more(char *file_name) 
{
    FILE *f;
    uint8_t line_count = 0;

    f = fopen(file_name, "r");
    if (f != NULL) {
        while (fgets(line, sizeof(line), f) != NULL) {
            chomp(line);
            if (line_count >= screen_height - 1) {
                line_count = display_control_line(line_count);
            }
            line_count += display_line(line);
        }
        if (fclose(f)==EOF) {
            perror(file_name);
        }
    }
    else {
        perror(file_name);
    }
}

/**
 * Removes any line terminating characters at the end of 'line'.
 *
 * @param   line    The line from which terminators should be removed.
 *
 * @return  The number of line terminating characters removed.
 *          <p>
 *          On ProDOS the
 *          line terminator sequence is a single carriage
 *          return (ASCII code 13).
 *          <p>
 *          On Windows the sequence is a 2-character pair,
 *          a carriage return and a line feed. So, where Perl
 *          chomp would return 1 for the pair, this function would
 *          return 2 for the number of characters. This
 *          difference would only occur on Windows.
 */
uint8_t chomp(char *line)
{
    size_t i;
    size_t count = 0;

    i = strlen(line);
    /* i cannot be allowed to go negative because it is unsigned.
       This is the only loop that guarantees i stays positive.
       If an "i >= 0" condition is used, i must go negative to
       exit the loop. We don't want that. */
    while (i > 0) {
        --i;
        if (line[i] == '\r' || line[i] == '\n') {
            line[i] = '\0';
            ++count;
        }
        else {
            break;
        }
    }
    return count;
}

