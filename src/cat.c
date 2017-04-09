#include <stdio.h>

#define MAX_STR_SIZE 256

char line[MAX_STR_SIZE];

void fcat(FILE *f)
{
    while (fgets(line, MAX_STR_SIZE, f) != NULL) {
        printf(line);
    }
}

void cat(char *name)
{
    FILE *f;

    f = fopen(name, "r");
    if (f) {
        fcat(f);
        fclose(f);
    }
    else {
        perror(name);
    }
}

void main(int argc, char *argv[])
{
    int i;

    if (argc <= 1) {
        fcat(stdin);
    }
    else {
        for (i = 1; i < argc; i++) {
            cat(argv[i]);
        }
    }
}

