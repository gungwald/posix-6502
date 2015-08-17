#include <stdio.h>
#include <stdint.h> /* uint8_t */
#include <stdlib.h> /* EXIT_SUCCESS */
#include <string.h> /* strlen */

/* ProDOS MAX_PATH does not include the terminator character.*/
#define MAX_PATH 64
/* We need space for the terminator character and also need to be a power of 2.*/
#define STR_CAPACITY (2*MAX_PATH)

static void read_input_file_name(void);
static uint8_t chomp(char *line);
static void hex_dump(char *file_name);

static char input_file_name[STR_CAPACITY];

int main(int argc, char *argv[])
{
    /** Must be at least as big as argc or below loop will run forever.*/
    int i;

    if (argc <= 1) {
        /* There are no command line arguments specifying the input file.*/
        read_input_file_name();
        hex_dump(input_file_name);
    }
    else {
        for (i = 1; i < argc; ++i) {
            hex_dump(argv[i]);
        }
    }
    return EXIT_SUCCESS;
}

static void read_input_file_name()
{
    fgets(input_file_name, MAX_PATH+1, stdin);
    chomp(input_file_name);
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
static uint8_t chomp(char *line)
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

