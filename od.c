#include <stdio.h>  /* FILE, fopen, fclose, fread */
#include <stdint.h> /* uint8_t */
#include <stdlib.h> /* EXIT_SUCCESS */
#include <string.h> /* strlen */

/* ProDOS MAX_PATH does not include the terminator character.*/
#define MAX_PATH 64
/* We need space for the terminator character and also need to be a power of 2.*/
#define STR_CAPACITY (2*MAX_PATH)
#define BUF_SIZE 256
#define AT_EOLN(count) (count % 16 == 0)
#define MAX_COLUMNS 16
#define TRACE(s) printf(s)
#define TRACE1(s,p1) printf(s,p1)
#define TRACE2(s,p1,p2) printf(s,p1,p2)

/* Function declarations */
static void read_input_file_name(void);
static uint8_t chomp(char *line);
static void hex_dump(char *file_name);
static void hex_dump_stream(FILE *f);
static void write_bytes(char *bytes, size_t byte_count);

/* Global variables */
static char G_input_file_name[STR_CAPACITY];
static char G_buffer[BUF_SIZE];
static size_t G_total_byte_count = 0;
static uint8_t G_column = 1;

int main(int argc, char *argv[])
{
    /* Must be at least as big as argc or below loop will run forever.*/
    int i;

    if (argc <= 1) {
        /* There are no command line arguments specifying the input file.*/
        read_input_file_name();
        hex_dump(G_input_file_name);
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
    printf("File:");
    fgets(G_input_file_name, MAX_PATH+1, stdin);
    chomp(G_input_file_name);
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
    /* The size_t type is a 2-byte unsigned int for cc65. This is
       what strlen returns. So, defining these variables as ints
       would limit their max value to less than that of size_t,
       which would cause failures with large values. The other
       option would be a long int, but that would be less efficient.*/
    size_t i;
    size_t count = 0;

    i = strlen(line);
    /* i cannot be allowed to go negative because it is unsigned.
       i must be big enough to store the max size_t value because
       that is what strlen returns.
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

static void hex_dump(char *file_name)
{
    FILE *f;

    f = fopen(file_name, "rb");
    if (f) {
        hex_dump_stream(f);
        fclose(f);
    }
    else {
        perror(file_name);
    }
}

static void hex_dump_stream(FILE *f)
{
    /* size_t is a 2-byte unsigned int. */
    size_t reed_count;

    while (!feof(f) && !ferror(f)) {
        reed_count = fread(G_buffer, sizeof(char), BUF_SIZE, f);

        /* Check for errors now so there is no chance of errno being reset. */
        if (ferror(f)) {
            perror("Read error");
        }

        if (reed_count > 0) {
            /* Always write out what was read, regardless of error or eof. */
            write_bytes(G_buffer, reed_count);
        }
    }
    /* Provide a final carriage return if we don't already have one. */
    if (G_column > 1) {
        printf("\n");
    }
}

static void write_bytes(char *bytes, size_t byte_count)
{
    size_t i = 0;

    for (/* i = 0 */ ; i < byte_count; ++i, ++G_total_byte_count) {
        if (G_column == 1) {
            printf("%06zx", G_total_byte_count);
        }

        printf(" %02hhx", bytes[i]);

        if (G_column == MAX_COLUMNS) {
            /* At end of line. */
            printf("\n");
            G_column = 1;
        }
        else {
            ++G_column;
        }
    }
}

