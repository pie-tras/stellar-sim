#include "global.h"
#include "file_util.h"

struct loaded_file* readFile(const char* path) {
    FILE *fp;
    long size;
    char *buffer;

    fp = fopen(path, "rb");

    if (!fp) {
        printf("Could not load file at path: %s", path);
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    buffer = calloc(1, size + 1);
    if (!buffer) {
        fclose(fp);
        printf("Memory alloc failed.");
    }

    if (1!=fread(buffer, size, 1, fp)) {
        fclose(fp);
        free(buffer);
        printf("Copy to buffer failed");
    }

    fclose(fp);

    struct loaded_file* loaded = malloc(sizeof(loaded));
    
    loaded->content = calloc(1, size + 1);
    strcpy(loaded->content, buffer);
    loaded->size = size;

    free(buffer);

    return loaded;
}

void cleanLoadedFile(struct loaded_file* loaded){
    free(loaded->content);
    free(loaded);
}