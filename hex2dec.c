#include <stdio.h>  /* printf, fgets, perror */
#include <string.h> /* strrchr */
#include <stdlib.h> /* strtoul */
#include <errno.h>  /* errno */

#define NUM_SIZE 32

/* Function prototypes */
static void fixup(char *s);
static void dump(char *s);
static void remove_char_at(char *s);

/* Global variables */
static char hex[NUM_SIZE];
static unsigned long int decimal;
static char *first_invalid_char;

void main(void)
{
    printf("Hexidecimal number:");
    fgets(hex, NUM_SIZE, stdin);
    dump(hex);
    fixup(hex);
    dump(hex);

    /* Can't tell strtoul valid from invalid return value, so checking errno
       is required, which requires it to be preset to 0. */
    errno = 0;
    decimal = strtoul(hex, &first_invalid_char, 16);
    if (errno == 0) {
        printf("$%s=%lu\n", hex, decimal);
        if (*first_invalid_char != '\0') {
            printf("Conversion ended prematurely at '%c'=$%02hhX with '%s' remaining.\n", first_invalid_char[0], first_invalid_char[0], first_invalid_char);
        }
    }
    else {
        perror(hex);
    }
}

/**
 * Removes all the trailing line terminators. These are left in by fgets.
 * Applies backspaces. These are not properly hanldled by fgets.
 * The algorithm avoids calling strlen and strcpy multiple times, for
 * example, in the case of a backspace char.
 */
void fixup(char *s)
{
    size_t i = 0, j = 0;
    char c;

    for ( ; s[i] != '\0'; ++i) {
        c = s[i];

        if (c == '\r' || c == '\n') {
            s[j] = '\0';
            /* We're done because we found the line terminator. */
            break;
        }
        else if (c == '\b') {
            if (i > 0) {
                /* A backspace should remove the previous char so move back. */
                --j;
            }
        }
        else {
            /* Only copy the current char if there is a gap between i and j.
               There's no need to copy if i and j are the same index. */
            if (j != i) {
                s[j] = s[i];
            }

            /* s[i] is normal so go ahead and advance j. */
            ++j;
        }
        
    }
}

/**
 * Does not modify the original string terminator char.
 */
void apply_backspace(char *s, size_t backspace_index)
{
    if (backspace_index == 0) {
        /* There's nothing to backspace over so just remove the backspace char
           without damaging memory before the start of the string. */
        strcpy(s, s+1);
    }
    else {
        strcpy(s-1, s+1);
    }
}

/**
 * Does not modify the original string terminator char.
 */
void remove_char_at(char *s)
{
    strcpy(s, s+1);
}

char *make_visible(char c)
{
    static char v[NUM_SIZE];

    if (c == '\n') {
        strcpy(v, "\\n");
    }
    else if (c == '\r') {
        strcpy(v, "\\r");
    }
    else if (c == '\b') {
        strcpy(v, "\\b");
    }
    else {
        snprintf(v, NUM_SIZE, "%c", c);
    }
    return v;
}

void dump(char *s)
{
    size_t i;
    char *v;

    for (i = 0; s[i] != '\0'; ++i) {
        v = make_visible(s[i]);
        printf("'%s'=$%02hhX ", v, s[i]);
    }
    puts("");
}

