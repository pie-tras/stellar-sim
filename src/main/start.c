#include "global.h"
#include "renderer/window_util.h"
#include "renderer/shader_util.h"
#include "main/file_util.h"

void loadUniforms(vec3 position, vec3 scale, vec3 cameraPos, unsigned int mvp_location) {
    mat4 projection;
	mat4 model;
    mat4 view;
    glm_mat4_identity(projection);
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    
	glm_perspective(1.6, 800 / 600, 0.001f, 10000.0f, projection);
	glm_translate(model, position);
    glm_scale(model, scale);

    glm_translate(view, cameraPos);

    mat4 mvp;
    glm_mat4_mulN((mat4 *[]){&projection, &view, &model}, 3, mvp);

	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp[0]);
}


int main(void) {
    GLFWwindow* window = createWindow(800, 600, "Test");

    time_t t;
    srand((unsigned) time(&t));

    int width = 175;
    int height = 175;
    int depth = 175;
    int particle_count = width * height * depth;

    double* vertices = malloc(sizeof(double) * 3 * 6 * particle_count);

    int index = 0;

    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                vertices[index] = ((double) (x - 0.5) / width);
                vertices[index + 1] = ((double) (y - 1.0) / height);
                vertices[index + 2] = ((double) z / depth);
                vertices[index + 3] = ((double) rand() / RAND_MAX);
                vertices[index + 4] = ((double) rand() / RAND_MAX);
                vertices[index + 5] = ((double) rand() / RAND_MAX);

                vertices[index + 6] = ((double) (x + 0.5) / width);
                vertices[index + 7] = ((double) (y - 1.0) / height);
                vertices[index + 8] = ((double) z / depth);
                vertices[index + 9] = ((double) rand() / RAND_MAX);
                vertices[index + 10] = ((double) rand() / RAND_MAX);
                vertices[index + 11] = ((double) rand() / RAND_MAX);

                vertices[index + 12] = ((double) x / width);
                vertices[index + 13] = ((double) y / height);
                vertices[index + 14] = ((double) z / depth);
                vertices[index + 15] = ((double) rand() / RAND_MAX);
                vertices[index + 16] = ((double) rand() / RAND_MAX);
                vertices[index + 17] = ((double) rand() / RAND_MAX);
                index += 18;
            }
        }
    }

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 18 * particle_count, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (const void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (const void*)24);

	struct shader* particle_shader = createShader("particle");

    startShader(particle_shader);
    glBindVertexArray(VAO);

    vec3 cameraPos = {-1, -1, -2};
    vec3 position = {0, 0, -2};
    vec3 scale = {2, 2, 2};

    unsigned int mvp_location = glGetUniformLocation(particle_shader->programID, "u_mvp");

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        loadUniforms(position, scale, cameraPos, mvp_location);
        glDrawArrays(GL_TRIANGLES, 0, particle_count * 3);

        cameraPos[2] += 0.01f;
      //  printf("%f\n", cameraPos[2]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free(vertices);

    cleanShader(particle_shader);
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    return 0;
}