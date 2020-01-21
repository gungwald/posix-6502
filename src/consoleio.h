#ifndef APLEISJAPLEJ_CONSOLEIO_H
#define APLEISJAPLEJ_CONSOLEIO_H

#define cio_set_cursor(x)     cioSetCursor(x)
#define cio_erase_backward(x) cioEraseBackward(x)
#define cio_read_char()       cioReadChar()
#define cio_read_line(x,y)    cioReadLine(x,y)

enum CursorType {CIO_CURSOR_BLOCK, CIO_CURSOR_UNDERSCORE};

extern void cioSetCursor(enum CursorType cursor);
extern char cioEraseBackward(unsigned char numberOfCharacterCells);
extern char cioReadChar();
extern char *cioReadLine(char *line, size_t lineSize);

#endif

