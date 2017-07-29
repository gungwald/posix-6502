#include <ctype.h>      /* isgraph */
#include <errno.h>      /* errno */
#include <stdbool.h>    /* bool, true, false */
#include <string.h>     /* strlen */
#include <stdio.h>      /* stdin, stdout, printf, puts, fgets, fopen, 
                           fclose, FILENAME_MAX,EOF */
#include <apple2.h>     /* _filetype, _auxtype */

#define PATH_CAPACITY   (FILENAME_MAX+1)
#define LINE_CAPACITY   256
#define IS_EOF_LINE(s)  (strcmp(s, "\x04\n") == 0)

void printError(const char *op, const char *param, int errorNum);
void fcat(FILE *in, const char *inName, FILE *out, const char *outName);
FILE *openTextFile(const char *name, const char *mode);
FILE *openFile(const char *name, const char *mode, FILE *fallback);
void checkForError(FILE *f, const char *name, const char *function);
void closeFile(FILE *f, const char *name);
void cat(char *inputName, char *outputName);
char *readLine(FILE *f, const char *inputName, char *line);
bool writeStuff(FILE *out, const char *outName, const char *data);
char *chomp(char *line);
void dumpString(const char *label, const char *s, size_t size);

char line[LINE_CAPACITY];
char inputFileName[PATH_CAPACITY];
char outputFileName[PATH_CAPACITY];
char blah[PATH_CAPACITY];

void dumpString(const char *label, const char *s, size_t size)
{
    size_t i = 0;
    char c;

    printf("%s: length=%d: ", label, strlen(s));

    for (c = s[i]; i < size; c = s[++i])
        if (isgraph(c))
            printf("%c ", c);
        else
            printf("0x%02X ", c);

    putchar('\r');
}

/* A newline represents Enter/Return for cc65 */
char *chomp(char *line)
{
    size_t i;

    i = strlen(line);
    while (i > 0)
        if (line[--i] == '\r' || line[i] == '\n')
            line[i] = '\0';
        else
            break;

    return line;
}

void printError(const char *op, const char *param, int errorNum)
{
    fprintf(stderr, "%s: %s: %s\n", op, param, strerror(errorNum));
}

char *readLine(FILE *f, const char *inputName, char *line)
{
    char *result;

    if ((result = fgets(line, LINE_CAPACITY, f)) == NULL)
        if (ferror(f))
            printError("fgets", inputName, errno);

    return result;
}

/* The name 'write' can't be used or all output will fail. */
bool writeStuff(FILE *out, const char *outName, const char *data)
{
    size_t len;
    bool ok = true;

    len = strlen(data);
    if (fwrite(data, 1, len, out) < len)
        if (! (ok = (ferror(out) == 0)))
            printError("fwrite", outName, errno);

    return ok;
}

void fcat(FILE *in, const char *inName, FILE *out, const char *outName)
{
    while (readLine(in, inName, line) != NULL)
        if (IS_EOF_LINE(line) || ! writeStuff(out, outName, line))
            break;
}

FILE *openTextFile(const char *name, const char *mode)
{
    /* Apple2-specific file types from apple2.h. */
    _filetype = PRODOS_T_TXT;
    _auxtype  = PRODOS_AUX_T_TXT_SEQ;
    return fopen(name, mode);
}

FILE *openFile(const char *name, const char *mode, FILE *fallback)
{
    FILE *f = NULL;

    if (strlen(name) == 0)
        f = fallback;
    else if ((f = openTextFile(name, mode)) == NULL)
        printError("fopen", name, errno);

    return f;
}

void closeFile(FILE *f, const char *name)
{
    if (fclose(f) == EOF)
        printError("fclose", name, errno);
}

void cat(char *inputName, char *outputName)
{
    FILE *in, *out;

    if ((in = openFile(inputName, "r", stdin)) != NULL) {
        if (in == stdin)
            inputName = "stdin";

        if ((out = openFile(outputName, "w", stdout)) != NULL) {
            if (out = stdout)
                outputName = "stdout";

            fcat(in, inputName, out, outputName);

            if (out != stdout)
                closeFile(out, outputName);
        }
        if (in != stdin)
            closeFile(in, inputName);
    }
}

void main()
{
    puts("Enter the input file name or nothing for keyboard input.");
    printf(">");
    fgets(inputFileName, PATH_CAPACITY, stdin);
    chomp(inputFileName);

    /* TODO: Accept multiple input files */
    if (strlen(inputFileName) > 0) {
        puts("Enter additional input file name or nothing to end input.");
        printf(">");
        fgets(blah, PATH_CAPACITY, stdin);
    }

    puts("Enter output file name or nothing for screen output.");
    printf(">");
    fgets(outputFileName, PATH_CAPACITY, stdin);
    chomp(outputFileName);

    cat(inputFileName, outputFileName);
}

