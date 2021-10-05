#include "global.h"
#include "renderer/window_util.h"
#include "renderer/shader_util.h"
#include "main/file_util.h"
#include <math.h>

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

void initPositions(long double* positions, long double* velocities, int width, int height, int depth) {
    int index = 0;
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                positions[index] = (long double) x;
                positions[index + 1] = (long double) y;
                positions[index + 2] = (long double) z;
                velocities[index] = 0;
                velocities[index + 1] = 0;
                velocities[index + 2] = 0;
                index += 3;
            }
        }
    }
}

void updatePositons(long double* positions, long double* velocities, int particle_count) {
    long double sumx = 0;
    long double sumy = 0;
    long double sumz = 0;
    for(int i = 0; i < particle_count * 3; i+=3){
        sumx += positions[i];
        sumy += positions[i + 1];
        sumz += positions[i + 2];
   //     printf("V: %f, %f, %f\n", positions[i], positions[i + 1], positions[i + 2]);
    }

    long double cmx = sumx / particle_count;
    long double cmy = sumy / particle_count;
    long double cmz = sumz / particle_count;

   //printf("sum: %f, %f, %f\n", sumx, sumy, sumz);
  // printf("cm: %f, %f, %f\n", cmx, cmy, cmz);
  //  exit(0);

    long double G = 6.67 * pow(10, -6);
    long double dt = 0.1;

    for(int i = 0; i < particle_count * 3; i+=3){
        long double rx = cmx - positions[i];
        long double ry = cmy - positions[i + 1];
        long double rz = cmz - positions[i + 2];
        long double rsqrd = (rx * rx) + (ry * ry) + (rz * rz);
        long double a = 0;
        long double ax = 0;
        long double ay = 0;
        long double az = 0;
        if(rsqrd != 0) {
            a = (G * (particle_count - 1)) / rsqrd;
            long double r = sqrt(rsqrd);
            ax = a * (rx / r);
            ay = a * (ry / r);
            az = a * (rz / r);
            //if(i == 3)
         //   printf("a: %Le, %Le, %Le\n", ax, ay, az);
        }

        // Gravitational perturbation
        rx = -positions[i];
        ry = -positions[i + 1];
        rz = -positions[i + 2];
        rsqrd = (rx * rx) + (ry * ry) + (rz * rz);
        if(rsqrd != 0) {
            a = (G * (100000000LL)) / rsqrd;
            long double r = sqrt(rsqrd);
            ax += a * (rx / r);
            ay += a * (ry / r);
            az += a * (rz / r);
        }

        // if( rsqrd < 0.01 ) {
        //     velocities[i] ;
        //     velocities[i + 1] = 0;
        //     velocities[i + 2] = 0;
        // }

       // printf("a: %f, %f, %f\n", ax, ay, az);
     //   printf("v: %f, %f, %f\n", velocities[i], velocities[i + 1], velocities[i + 2]);

        positions[i] += velocities[i] * dt;
        positions[i + 1] += velocities[i + 1] * dt;
        positions[i + 2] += velocities[i + 2] * dt;
        velocities[i] += ax * dt;
        velocities[i + 1] += ay * dt;
        velocities[i + 2] += az * dt;
    }
 //  printf("v: %Le, %Le, %Le\n", velocities[0], velocities[1], velocities[2]);
}

void updateVertices(double* vertices, long double* positions, long double* velocities, int width, int height, int depth, unsigned int VBO) {
    int index = 0;

    for (int i = 0; i < width * height * depth * 3; i+=3) {

        double velcolor = (velocities[i] * velocities[i]) + (velocities[i + 1] * velocities[i + 1]) + (velocities[i + 2] * velocities[i + 2]);
        velcolor = velcolor / 50000;
        if(velcolor > 1.0) velcolor = 1.0;

        vertices[index] = ((double) (positions[i] - 0.5) / width);
        vertices[index + 1] = ((double) (positions[i + 1] - 1.0) / height);
        vertices[index + 2] = ((double) positions[i + 2] / depth);
        vertices[index + 3] = ((double) velcolor);
        vertices[index + 4] = ((double) 0.0);
        vertices[index + 5] = ((double) 1.0 - velcolor);

        vertices[index + 6] = ((double) (positions[i] + 0.5) / width);
        vertices[index + 7] = ((double) (positions[i + 1] - 1.0) / height);
        vertices[index + 8] = ((double) positions[i + 2] / depth);
        vertices[index + 9] = ((double) velcolor);
        vertices[index + 10] = ((double) 0.0);
        vertices[index + 11] = ((double) 1.0 - velcolor);

        vertices[index + 12] = ((double) positions[i] / width);
        vertices[index + 13] = ((double) positions[i + 1] / height);
        vertices[index + 14] = ((double) positions[i + 2] / depth);
        vertices[index + 15] = ((double) velcolor);
        vertices[index + 16] = ((double) 0.0);
        vertices[index + 17] = ((double) 1.0 - velcolor);
        index += 18;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 18 * width * height * depth, vertices, GL_STATIC_DRAW);
}


int main(void) {
    GLFWwindow* window = createWindow(1200, 780, "Test");

    time_t t;
    srand((unsigned) time(&t));

    int width = 75;
    int height = 75;
    int depth = 75;
    int particle_count = width * height * depth;

    long double* positions = malloc(sizeof(long double) * particle_count * 3);
    long double* velocities = malloc(sizeof(long double) * particle_count * 3);
    double* vertices = malloc(sizeof(double) * 3 * 6 * particle_count);

    initPositions(positions, velocities, width, height, depth);

    printf("init\n");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    updateVertices(vertices, positions, velocities, width, height, depth, VBO);
    printf("update vertices\n");

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (const void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (const void*)24);

	struct shader* particle_shader = createShader("particle");

    startShader(particle_shader);
    glBindVertexArray(VAO);

    vec3 cameraPos = {-3, -3, -5};
    vec3 position = {0, 0, -1};
    vec3 scale = {2, 2, 2};

    unsigned int mvp_location = glGetUniformLocation(particle_shader->programID, "u_mvp");

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        updatePositons(positions, velocities, particle_count);
        updateVertices(vertices, positions, velocities, width, height, depth, VBO);
        loadUniforms(position, scale, cameraPos, mvp_location);
        glDrawArrays(GL_TRIANGLES, 0, particle_count * 3);

       // cameraPos[2] += 0.001f;
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