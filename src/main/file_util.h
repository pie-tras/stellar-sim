struct loaded_file {
    char* content;
    int length;
};

struct loaded_file* readFile(const char* path);