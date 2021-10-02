#include "global.h"
#include "renderer/window_util.h"
#include "renderer/shader_util.h"
#include "main/file_util.h"


int main(void) {
    GLFWwindow* window = createWindow(800, 600, "Test");

	float vertices[] = {
    	-0.5f, -0.5f, 0.0f,
     	0.5f, -0.5f, 0.0f,
    	0.0f,  0.5f, 0.0f
	};

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	struct shader* particle_shader = createShader("particle");

    startShader(particle_shader);
    glBindVertexArray(VAO);
 
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT);
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanShader(particle_shader);
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    return 0;
}