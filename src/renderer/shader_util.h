struct shader{
    unsigned int programID;
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;
};

struct shader* createShader(char* key);

void startShader(struct shader* shader);

void stopShader();

void cleanShader(struct shader* shader);