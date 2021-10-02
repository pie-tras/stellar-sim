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

	float vertices[] = {
    	-0.25f, -0.25f, 0.0f, 0.18f, 0.6f, 0.96f,
     	0.25f, -0.25f, 0.0f, 0.18f, 0.6f, 0.96f,
    	0.0f,  0.25f, 0.0f, 0.18f, 0.6f, 0.96f,

        0.25f, 0.25f, 0.5f, 1.0f, 0.93f, 0.24f,
     	0.75f, 0.25f, 0.5f, 1.0f, 0.93f, 0.24f,
    	0.5f,  0.75f, 0.5f, 1.0f, 0.93f, 0.24f
	};

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)12);
    glEnableVertexAttribArray(1);

	struct shader* particle_shader = createShader("particle");

    startShader(particle_shader);
    glBindVertexArray(VAO);

    vec3 cameraPos = {0, 0, 0};
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
        glDrawArrays(GL_TRIANGLES, 0, 6);

        cameraPos[2] -= 0.1f;
        printf("%f\n", cameraPos[2]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanShader(particle_shader);
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    return 0;
}