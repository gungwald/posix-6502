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

/* Maximum partial pathname for ProDOS is 64 bytes without a nul
   terminator but including slashes. 
   ProDOS automatically adds the Prefix, which can also be 64 bytes, for 
   a maximum length of 128 bytes. As you can see, this will be 
   problematic. The only way to get to the full 128 bytes is to set
   the Prefix because you can't specify 128 bytes in a pathname, but
   only 64. Fun? */
#define MAX_PATH 64

/* 65 characters are needed for the MAX_PATH plus a nul byte. */
#define PATH_CAPACITY 65
#define LINE_CAPACITY 128
#define CARRIAGE_RETURN '\r'

/* NULL_CHAR is defined to be the character with ASCII Code zero. This is
   different from the NULL constant defined by C, which is a pointer to 
   address 0. Their values are both zero, but their types are different.
   NULL is type 'void *' while NULL_CHAR is type 'char'. */
const char NULL_CHAR        = '\0'; 
const char LINE_FEED        = '\n';     /* Formal name for a new line character */
const char DELETE           = '\x7f';   /* Decimal 127 */

char *input_file_name(void);
void more(char *file_name);
uint8_t chomp(char *line);
char *trunc(char *s, size_t max_len);
uint8_t display_line(const char *s);
void erase_backward(uint8_t count);

/* Have to use global variables because local variable capacity
   is limited. */
unsigned char screen_width;
unsigned char screen_height;
char line[LINE_CAPACITY];

int main(int argc, char *argv[])
{
    char *file_name;
    uint8_t i;

    screensize(&screen_width, &screen_height);
    /*
    printf("width=%d height=%d\n", screen_width, screen_height);
    */

    if (argc <= 1) {
        /* ProDOS doesn't support piping input/output so, if there are no 
           arguments, ask for a file name. */
        file_name = input_file_name();
        if (file_name != NULL && strcmp(file_name,"")!=0) {
            more(file_name);
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

char interactively_read_char()
{
    unsigned char x;
    char c;

    x = wherex();

    revers(true);
    cputc(' ');
    gotox(x);
    c = cgetc();

    revers(false);
    cputc(' ');
    gotox(x);
    return c;
}

char *interactively_read_line(const char *prompt)
{
    const char DELETE = '\x7f';
    const char BACKSPACE = '\x08';
    bool line_is_complete = false;
    char c;
    size_t count = 0;

    cputs(prompt);

    while (! line_is_complete) {
        c = interactively_read_char();
        if (count > 0 && (c == DELETE || c == BACKSPACE)) {
            erase_backward(1);
            count--;
        }
        else if (c == '\r') {
            line[count] = '\0';
            line_is_complete = true;
        }
        else if (count < LINE_CAPACITY - 1) {
            cputc(c);
            line[count++] = c;
        }
        else {
            line[count] = '\0';
            line_is_complete = true;
        }
    }
    cputs("\r\n");
    return line;
}

char *input_file_name() 
{
    static char *file_name;

    file_name = interactively_read_line("File: ");
    return file_name;
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

void erase_backward(uint8_t count)
{
    cclear(1); /* Erase cursor */
    gotox(wherex() - (count + 1));
    cclear(count);
    gotox(wherex() - count);
}
    
uint8_t display_control_line(uint8_t line_count)
{
    char c;

    revers(true);
    cputs("--More--");
    revers(false);
    c = tolower(cgetc());
    erase_backward(8);
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
    size_t len;
    size_t count = 0;

    len = strlen(line);
    /* i cannot go negative */
    for (i = len; i > 0; ) {
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

