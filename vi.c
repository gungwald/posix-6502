#include <stdint.h>
#include <stdbool.h>
#include <conio.h>

void nav_mode(void);
void ins_mode(void);
void move_down(void);

uint8_t max_x, max_y;


void main()
{
    char c;

    clrscr();
    screensize(&max_x, &max_y);
    /*control_mode(); */

    while (c != 'q') {
        c = cgetc();
        cputc(c);
    }
}

void nav_mode()
{
    char c;

    while (true)
    {
        c = cgetc();
        switch (c) {
            case 'i':
                /*ins_mode();*/
                break;
            case 'j':
                /*move_down();*/
                break;
            default:
                break;
        }
    }
}
