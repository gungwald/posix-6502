
struct _Line {
    uint8_t capacity;
    uint8_t length;
    char *line;
};

typedef struct _Line Line;

Line *newLine();
void delLine(Line *l);
void insChar(Line *l, char c);

