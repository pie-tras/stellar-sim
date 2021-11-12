#include "global.h"
#include "shader_util.h"
#include "main/file_util.h"

unsigned int compileShader(unsigned int type, const char* src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        printf("Failed to compile ");
        printf("%s", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        printf(" shader!\n");
        printf("%s\n", message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

struct compShader* createCompShader(char* key){
    char* base_path = "res/shader/";
    char comp_path[64];
    strcpy(comp_path, base_path);

    strcat(comp_path, key);
    strcat(comp_path, ".comp.glsl");

    struct loaded_file* comp_src = readFile(comp_path);

    unsigned int programID = glCreateProgram();
    unsigned int compShaderID = compileShader(GL_COMPUTE_SHADER, comp_src->content);
   
    glAttachShader(programID, compShaderID);
    glLinkProgram(programID);
    glValidateProgram(programID);
    
    struct compShader* s = malloc(sizeof(s));
    s->programID = programID;
    s->compShaderID = compShaderID;

    cleanLoadedFile(comp_src);

    return s;
}

struct shader* createShader(char* key) {
    char* base_path = "res/shader/";
    char vert_path[64];
    char frag_path[64];
    strcpy(vert_path, base_path);
    strcpy(frag_path, base_path);

    strcat(vert_path, key);
    strcat(vert_path, ".vert.glsl");
    strcat(frag_path, key);
    strcat(frag_path, ".frag.glsl");

    struct loaded_file* vertex_src = readFile(vert_path);
    struct loaded_file* fragment_src = readFile(frag_path);

    unsigned int programID = glCreateProgram();
    unsigned int vertexShaderID = compileShader(GL_VERTEX_SHADER, vertex_src->content);
    unsigned int fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragment_src->content);

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    glValidateProgram(programID);
    
    struct shader* s = malloc(sizeof(s));
    s->programID = programID;
    s->vertexShaderID = vertexShaderID;
    s->fragmentShaderID = fragmentShaderID;

    cleanLoadedFile(vertex_src);
    cleanLoadedFile(fragment_src);

    return s;
}

void startShader(struct shader* shader) {
    glUseProgram(shader->programID);
}

void startCompShader(struct compShader* shader) {
    glUseProgram(shader->programID);
}

void stopShader(){
    glUseProgram(0);
}

void cleanShader(struct shader* shader) {
    stopShader();
    glDetachShader(shader->programID, shader->vertexShaderID);
    glDetachShader(shader->programID, shader->fragmentShaderID);
    glDeleteShader(shader->vertexShaderID);
    glDeleteShader(shader->fragmentShaderID);
    glDeleteProgram(shader->programID);

    free(shader);
}

void cleanCompShader(struct compShader* shader) {
    stopShader();
    glDetachShader(shader->programID, shader->compShaderID);
    glDeleteShader(shader->compShaderID);
    glDeleteProgram(shader->programID);

    free(shader);
}