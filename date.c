/* Prints the ProDOS date and time to the screen. */

#include <stdio.h>
#include <stdlib.h>     /* EXIT_SUCCESS */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>     /* strlen */
#include <time.h>       /* time, ctime */
#include <errno.h>

/*
          49041 ($BF91)     49040 ($BF90)

         7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0
        +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
 DATE:  |    year     |  month  |   day   |
        +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+

         7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0
        +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
 TIME:  |    hour       | |    minute     |
        +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+

          49043 ($BF93)     49042 ($BF92)
*/

uint8_t *MONTH_DAY_ADDR = (uint8_t *) 0xbf90;
uint8_t *YEAR_ADDR      = (uint8_t *) 0xbf91;
uint8_t *MINUTE_ADDR    = (uint8_t *) 0xbf92;
uint8_t *HOUR_ADDR      = (uint8_t *) 0xbf93;

const uint8_t BITS_0000_0001 = (uint8_t) 0x01; /*   1 */
const uint8_t BITS_0000_1000 = (uint8_t) 0x08; /*   8 */
const uint8_t BITS_0001_1111 = (uint8_t) 0x1f; /*  31 */
const uint8_t BITS_1110_0000 = (uint8_t) 0xe0; /* 224 */
const uint8_t BITS_1111_1110 = (uint8_t) 0xfe; /* 254 */

static void usage(void); /* cc65 requires "void" here to match an empty definition.*/
static void display_date_time(void);
static void set_date_time(const char *date_time);
static void set_date(uint8_t month, uint8_t day, uint8_t year);
static void set_time(uint8_t hour, uint8_t minute);
static void print_error(const char *message);

int main(int argc, char *argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1],"/?")==0 || strcmp(argv[1],"-h")==0) {
            usage();
        }
        else {
            set_date_time(argv[1]);
        }
    }
    else {
        display_date_time();
    }
    exit(EXIT_SUCCESS);
}

void usage()
{
    puts("To display the date:\n\n    -date\n");
    puts("To set the date:\n\n    bload date\n    call 2051:rem MMDD[hhmm]YY\n");
    puts("Hours must be in military time.");
}

void display_date_time()
{
    time_t now;
    char *formatted_time;

    if (*MONTH_DAY_ADDR == 0) {
        puts("The date and time have not been set.");
    }
    else if (time(&now) == -1 ) {
        printf("errno=%d\n", errno);
        perror("time");
    }
    else {
        formatted_time = ctime(&now);
        if (formatted_time == NULL) {
            perror("ctime");
        }
        else {
            printf(formatted_time);
        }
    }
}

void print_error(const char *message)
{
    fprintf(stderr, "%s\n", message);
}

void set_date(uint8_t month, uint8_t day, uint8_t year)
{
    uint8_t month_day_byte;
    uint8_t year_month_byte;

    month_day_byte = month << 5;
    month_day_byte += day & BITS_0001_1111;

    year_month_byte = year << 1;
    year_month_byte += month & BITS_0000_1000;

    /* Set the ProDOS memory locations for month and year. */
    *MONTH_DAY_ADDR = month_day_byte;
    *YEAR_ADDR = year_month_byte;
}

void set_time(uint8_t hour, uint8_t minute)
{
    *MINUTE_ADDR = minute;
    *HOUR_ADDR = hour;
}

void set_date_time(const char *date_time_input)
{
    uint8_t len;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t year;
    size_t conversion_count;

    len = strlen(date_time_input);
    if (len == 6) {
        /* This is just the date without the time. */
        conversion_count = sscanf(date_time_input, "%2hhu%2hhu%2hhu", &month, &day, &year);

        if (conversion_count == 3) {
            set_date(month, day, year);
        }
        else {
            print_error("Invalid date input format");
        }
    }
    else if (len < 10) {
        print_error("You have not provided enough digits.");
    }
    else if (len > 10) {
        print_error("You have provided too many digits.");
    }
    else {
        conversion_count = sscanf(date_time_input, "%2hhu%2hhu%2hhu%2hhu%2hhu", 
            &month, &day, &hour, &minute, &year);

        if (conversion_count == 5) {
            set_date(month, day, year);
            set_time(hour, minute);
        }
        else {
            print_error("Invalid date input format");
            usage();
        }
    }
}

