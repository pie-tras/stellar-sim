struct shader{
    unsigned int programID;
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;
};

struct compShader{
    unsigned int programID;
    unsigned int compShaderID;
};

struct shader* createShader(char* key);

struct compShader* createCompShader(char* key);

void startShader(struct shader* shader);

void startCompShader(struct compShader* shader);

void stopShader();

void cleanShader(struct shader* shader);

void cleanCompShader(struct compShader* shader);