#include <stdbool.h>
#include <conio.h>
#include "consoleio.h"
#include "characters.h" /* NULL_CHAR, LINE_FEED, etc */

void cioEraseBackward(unsigned char count)
{
    cclear(1); /* Erase cursor */
    gotox(wherex() - (count + 1));
    cclear(count);
    gotox(wherex() - count);
}

char cioReadChar()
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

char *cioReadLine(char *line, size_t lineSize)
{
    bool line_is_complete = false;
    char c;
    size_t count = 0;

    while (! line_is_complete) {
        c = cioReadChar();
        if (count > 0 && (c == DELETE_CHAR || c == BACKSPACE_CHAR)) {
            cioEraseBackward(1);
            count--;
        }
        else if (c == CARRIAGE_RETURN) {
            line[count] = NULL_CHAR;
            line_is_complete = true;
        }
        else if (count < lineSize - 1) {
            cputc(c);
            line[count++] = c;
        }
        else {
            line[count] = NULL_CHAR;
            line_is_complete = true;
        }
    }
    cputs("\r\n");
    return line;
}

