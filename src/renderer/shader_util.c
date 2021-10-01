#include "global.h"
#include "main/file_util.h"

void parseShaders(char* vertex_path, char* fragment_path) {
    struct loaded_file* vertex_src = readFile(vertex_path);
    struct loaded_file* fragment_src = readFile(vertex_path);
}

void createShader(const char* key) {
    char* base_path = "res/shader";
    parseShaders(strcat(strcat(base_path, key), ".vert.glsl"), strcat(strcat(base_path, key), ".frag.glsl"));
    //compile
    //create
    //return shader struct
}

//Create compute shaders