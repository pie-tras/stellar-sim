#include "global.h"
#include "renderer/window_util.h"
#include "renderer/shader_util.h"
#include "main/file_util.h"
#include <math.h>

#define NUM_PARTICLES 1024*1024
#define WORK_GROUP_SIZE 128

struct pos {
    float x, y, z, w; // positions
};

struct vel {
    float vx, vy, vz, vw; // velocities
};

void loadPoints(GLuint posSSbo) {
    unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    float* buffer = malloc(sizeof(float) * 3 * NUM_PARTICLES);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * 3 * sizeof(float), sizeof(float), buffer);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * 3 * sizeof(float), posSSbo, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void computeChecks() {
    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    printf("max global (total) work group counts x:%i y:%i z:%i\n",
    work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    int work_grp_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
    work_grp_size[0], work_grp_size[1], work_grp_size[2]);

    int work_grp_inv;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    printf("max local work group invocations %i\n", work_grp_inv);
}

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
    GLFWwindow* window = createWindow(1200, 780, "Test");

    time_t t;
    srand((unsigned) time(&t));

    computeChecks();

    GLuint posSSbo;
    GLuint velSSbo;

    glGenBuffers( 1, &posSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, posSSbo );
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct pos), NULL, GL_STATIC_DRAW );
    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT ; // the invalidate makes a big difference when re-writing
    struct pos *points = (struct pos *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct pos), bufMask );
    for( int i = 0; i < NUM_PARTICLES; i++ ) {
        points[ i ].x = Ranf( -100, 100 );
        points[ i ].y = Ranf( -100, 100 );
        points[ i ].z = Ranf( -100, 100 );
        points[ i ].w = 1.;
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    glGenBuffers( 1, &velSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, velSSbo );
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct vel), NULL, GL_STATIC_DRAW );
    struct vel *vels = (struct vel *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct vel), bufMask );
    for( int i = 0; i < NUM_PARTICLES; i++ ) {
        vels[ i ].vx = Ranf( -10, 10 );
        vels[ i ].vy = Ranf( -10, 10 );
        vels[ i ].vz = Ranf( -10, 10 );
        vels[ i ].vw = 0.;
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    struct compShader* compute_test = createCompShader("test");
    struct shader* quad_shader = createShader("quad");
    struct shader* particle_shader = createShader("particle");
    startShader(particle_shader);
    unsigned int mvp_location = glGetUniformLocation(particle_shader->programID, "u_mvp");

    vec3 cameraPos = {-3, -3, -5};
    vec3 position = {0, 0, -1};
    vec3 scale = {2, 2, 2};

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    loadPoint();

    while (!glfwWindowShouldClose(window))
    {
        {
            glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, posSSbo );
            glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, velSSbo );

            startCompShader(compute_test);
            glDispatchCompute(NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1);
        }

        glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

        // {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        loadPoints();
        startShader(particle_shader);
        loadUniforms(position, scale, cameraPos, mvp_location);
        glDrawArrays(GL_POINTS, 0, 4);
        //}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanCompShader(compute_test);
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    return 0;
}