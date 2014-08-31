#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "suffix-array.h"

void print_array(int *array, int length)
{
    int i;
    for (i = 0; i < length; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    char *line = NULL;
    size_t len;
    ssize_t read = 0;

    while((read = getline(&line, &len, stdin)) != -1) {
        line[--read] = '\0';
        printf("string is: '%s'\n", line);
        int *suffix = build_suffix_array(line);
        print_array(suffix, read);
        free(suffix);
        free(line);
        line = NULL;
    }

    return 0;
}
