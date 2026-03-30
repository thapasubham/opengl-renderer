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
void mouse_callback(GLFWwindow* , double , double );
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
double mouseX, mouseY;
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

    int width2, height2, nrChannels2;
    unsigned char* imgData2 = stbi_load("Assets/weather.jpg", &width2, &height2, &nrChannels2, 0);
    if (!imgData2)
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
    glfwSetCursorPosCallback(glfwWindow, mouse_callback);

    unsigned int texture, texture2;
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

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (imgData2)
    {
        GLenum format2 = GL_RGB;
        if (nrChannels2 == 1)
            format2 = GL_RED;
        else if (nrChannels2 == 3)
            format2 = GL_RGB;
        else if (nrChannels2 == 4)
            format2 = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format2, width2, height2, 0, format2, GL_UNSIGNED_BYTE, imgData2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(imgData);
    stbi_image_free(imgData2);

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
        staticShader.setVec2("mousePos", ((float)mouseX / 800.0f) * 2.0f - 1.0f, 1.0f - ((float)mouseY / 600.0f) * 2.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        vao1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        vao1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        animatedShader.use();
        animatedShader.setInt("ourTexture", 0);
        animatedShader.setInt("ourTexture2", 1);
        animatedShader.setFloat("timeValue", timeValue);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouseX = xpos;
    mouseY = ypos;
}
void process_Input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {   
        glfwSetWindowShouldClose(window, true);
    }
}