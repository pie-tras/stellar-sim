#include "global.h"
#include "file_util.h"

struct loaded_file* readFile(const char* path) {
    FILE* in_file = fopen(path, "r");

    if (!in_file) {
        printf("Could not load file at path: %s", path);
        return NULL;
    }

    int i = 0;
    int size = 256;
    char* buff = malloc(size);
    char c;
    while((c = fgetc(in_file)) != EOF) {
        buff[i] = c;
        i++;
        if (i == size) {
            size *= 2;
            buff = realloc(buff, size);
        }
    }

    struct loaded_file* loaded = malloc(sizeof(loaded));
    
    loaded->content = malloc(i);
    strcpy(loaded->content, buff);
    loaded->length = i;

    free(buff);
    fclose(in_file);

    return loaded;
}