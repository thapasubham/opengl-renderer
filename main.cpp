#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
void frame_resize_viewport(GLFWwindow *, int, int);
void process_Input(GLFWwindow *);

float vertices[] = {
    // positions         // colors           // texture coords
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top right
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
};
float verticesB[] = {
    // positions         // colors           // texture coords
    0.5f, -0.5f, 0.2f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.2f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f    // top
};

float textureCoordinate[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f

};

unsigned int indices[] = {0, 1, 2, 1, 3, 2};
unsigned int indicesB[] = {0, 1, 2};

int main()
{
    // Initialize image loading
    int width, height, nrChannels;
    unsigned char *imgData = stbi_load("Assets/wall.jpg", &width, &height, &nrChannels, 0);

    if (!imgData)
    {
        std::cout << "FAILED TO LOAD IMAGE" << std::endl;
    }
    else
    {
        std::cout << "TEXTURE LOADED CORRECTLY" << std::endl;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *glfwWindow = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);

    if (glfwWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(glfwWindow);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(glfwWindow, frame_resize_viewport);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (imgData)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(imgData);

    VAO vao1;
    vao1.Bind();
    VBO vbo1(vertices, sizeof(vertices));
    EBO ebo1(indices, sizeof(indices));

    vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    VAO vao2;
    vao2.Bind();
    VBO vbo2(verticesB, sizeof(verticesB));
    EBO ebo2(indicesB, sizeof(indicesB));

    vao2.LinkAttrib(vbo2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    vao2.LinkAttrib(vbo2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    vao2.LinkAttrib(vbo2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    vao2.Unbind();
    vbo2.Unbind();
    ebo2.Unbind();

    Shader staticShader("shaders/default.vert", "shaders/default.frag");
    Shader animatedShader("shaders/animated.vert", "shaders/animated.frag");

    while (!glfwWindowShouldClose(glfwWindow))
    {
        float timeValue = (float)glfwGetTime();

        process_Input(glfwWindow);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        staticShader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        vao1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        vao1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, texture);

        animatedShader.use();
        animatedShader.setFloat("timeValue", timeValue);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        vao2.Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        vao2.Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    vao2.Delete();
    vbo2.Delete();
    ebo2.Delete();
    glDeleteTextures(1, &texture);

    std::cout << "Terminating Program" << std::endl;
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
    return 0;
}

void frame_resize_viewport(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_Input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}