#include "iostream"

#include "linmath.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Camera.h"
#include "Util.h"


static const char* vertex_shader_text = 
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec3 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char* fragment_shader_text =
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

const float PI = 3.14;

void error_callback(int error, const char* description)
{
    printf(description);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 960, "Hello World", NULL, NULL);
    if (!window)
    {
        printf("no window");
        glfwTerminate();
        return -1;
    }

    // /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        printf("GLEW error: %s", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    float verts[] = {
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    int indecies[] = {
        0, 1, 2,
        1, 0, 3
    };

    Vertex vertecies[] = {
        { {0.5, -0.5, 0.0}, {0.0, 0.0, 1.0} },
        { {-0.5, 0.5, 0.0}, {1.0, 0.0, 0.0} },
        { {-0.5, -0.5, 0.0}, {0.0, 1.0, 0.0} },
        { {0.5, 0.5, 0.0}, {0.0, 1.0, 0.0} }
    };

    GLuint vertex_buffer, element_buffer;
    GLuint vertex_shader, fragment_shader;
    GLuint program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSwapInterval(1);

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_buffer);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    Camera cam;
    cam.setEye(0.0f, 0.0f, 5.0f);
    double initialTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currTime = glfwGetTime();
        float ratio, fov_y;
        int width, height;
        mat4x4 model, view, proj, mvp;
        quat rot;
        vec3 up = {0.0f, 1.0f, 0.0f};

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        fov_y = 2 * atanf(height / 2.0f);
        
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mat4x4_identity(model);
        //mat4x4_translate(model, 0.f + glfwGetTime(), 0.f, 0.f);
        mat4x4_rotate_Y(model, model, glfwGetTime());
        //mat4x4o_mul_quat(model, model, rot);

        //mat4x4_ortho(proj, -ratio, ratio, -1.f, 1.f, -1.f, 1.f);
        mat4x4_perspective(proj, 45, ratio, 0.0f, 10.0f);
        mat4x4_look_at(view, cam.getEye(), cam.getForward(), cam.getUp());
        mat4x4_mul(mvp, view, model);
        mat4x4_mul(mvp, proj, mvp);
        
        
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color))); 

        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawElements(GL_TRIANGLES, sizeof(indecies) / sizeof(int), GL_UNSIGNED_INT, (void *) 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        initialTime = currTime;
    }

    glfwTerminate();
    return 0;
}