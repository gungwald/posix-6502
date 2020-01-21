#ifndef APWOEKMFKD_CHARACTERS_H
#define APWOEKMFKD_CHARACTERS_H

/* NULL_CHAR is defined to be the character with ASCII Code zero. This is
   different from the NULL constant defined by C, which is a pointer to 
   address 0. Their values are both zero, but their types are different.
   NULL is type 'void *' while NULL_CHAR is type 'char'. */
const char NULL_CHAR       = '\x00'; 
const char BACKSPACE_CHAR  = '\x08';
const char LINE_FEED       = '\n';   /* Formal name for a new line character */
const char CARRIAGE_RETURN = '\r';
const char DELETE_CHAR     = '\x7f'; /* Decimal 127 */

#endif

