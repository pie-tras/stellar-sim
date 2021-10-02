struct loaded_file {
    char* content;
    long size;
};

struct loaded_file* readFile(const char* path);
void cleanLoadedFile(struct loaded_file* loaded);