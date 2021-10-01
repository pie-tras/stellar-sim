#include "global.h"
#include "renderer/window_util.h"
#include "main/file_util.h"

int main(void) {
    GLFWwindow* window = createWindow(800, 600, "Test");

	// float vertices[] = {
    // 	-0.5f, -0.5f, 0.0f,
    //  	0.5f, -0.5f, 0.0f,
    // 	0.0f,  0.5f, 0.0f
	// };

	// unsigned int VBO;
	// glGenBuffers(1, &VBO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	struct loaded_file* lf = readFile("res/shader/particle.fragment.glsl");
	printf("%s\n LEN: %d", lf->content, lf->length);
 
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT);
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    return 0;
}