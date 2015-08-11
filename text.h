#ifndef TEXT_H
#define TEXT_H

struct _Text {
    /* private */
    char *fileName;
    LineList *lines;
    uint8_t x;
    uint8_t y;
};

typedef struct _Text Text;

extern void txOpen(Text *t, char *fileName);
extern void txSave(Text *t);
extern void txClose(Text *t);
extern void txIns(Text *t, uint8_t c);
extern void txDel(Text *t, uint8_t count);
extern void txUp(Text *t);
extern void txDown(Text *t);
extern void txRight(Text *t);
extern void txLeft(Text *t);

#endif

