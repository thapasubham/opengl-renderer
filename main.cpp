 #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frame_resize_viewport(GLFWwindow*, int, int);
void process_Input(GLFWwindow*);

float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};
float verticesB[] = {
    // positions         // colors
     0.5f, -0.5f, 0.2f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.2f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.2f,  0.0f, 0.0f, 1.0f    // top 
};
unsigned int indicesB[] = {
    0, 1, 2
};
unsigned int indices[] = {
    0, 1, 2
};
float verticesC[] = {
    // positions         // colors
     0.5f, -0.5f, 0.1f,  1.0f, 1.0f, 0.0f,  // bottom right (between triangles A & B)
    -0.5f, -0.5f, 0.1f,  1.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.1f,  1.0f, 1.0f, 0.0f   // top
};
unsigned int indicesC[] = { 0, 1, 2 };
unsigned int fillShader() {
   const char* vertexShaderSource = R"(
      #version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform float timeValue;

void main() { 
    vec3 center = vec3(0.0, 0.0, 0.0);
    vec3 pos = aPos - center;

    float angle = timeValue;
    float sinA = sin(angle);
    float cosA = cos(angle);

    mat3 rotation = mat3(
        cosA,  0.0,sinA,
        0.0,   1.0,  0.0,
        -sinA,   0.0,cosA
    );
pos.z += sin(timeValue) * 0.5;
    pos = rotation * pos;

    // optional circular offset
    vec2 offset = vec2(sin(timeValue) * 0.5, cos(timeValue) * 0.5);
  pos.xy += offset;

    gl_Position = vec4(pos + center, 1.0);

    vertexColor = ((pos + aPos) / 2.0) + 0.5; 

 }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
in vec3 shader;
        out vec4 FragColor;
        void main() { FragColor = vec4(vertexColor,1.0); }
    )";
    
    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(VertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout<< "Error compiling"<<infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
  
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling :fragment shader" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, VertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error linking shader" << infoLog << std::endl;

    }
    glDeleteShader(VertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int shader() {
    const char* vertexShaderSource = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        void main() { gl_Position = vec4(aPos, 1.0); }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() { FragColor = vec4(1.0, 0.5, 0.45, 1.0); }
    )";

    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(VertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling :fragment shader" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, VertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error linking shader" << infoLog << std::endl;

    }
    glDeleteShader(VertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* glfwWindow = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);

    if (glfwWindow == NULL)
    {

        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(glfwWindow);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(glfwWindow, frame_resize_viewport);
    unsigned int VAO, VAO2;
    unsigned int VBO, VBO2;
    unsigned int EBO, EBO2;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesB), indicesB, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int shaderProgram = shader();
    unsigned int fillShaderProgram=  fillShader();
    //int vertexColorLocation = glGetUniformLocation(fillShaderProgram, "ourColor");
    int timeUniform = glGetUniformLocation(fillShaderProgram, "timeValue");
    while (!glfwWindowShouldClose(glfwWindow)) {
        float timeValue = glfwGetTime();
        float red = (cos(timeValue) / 2.0f) + 0.5f;
        float green = (sin(timeValue) / 2.0f) + 0.5f;
        float blue = (cos(timeValue + 2.0f) / 2.0f) + 0.5f;

        float offsetX = sin(timeValue) * 0.5f;
        float offsety = cos(timeValue) * 0.5f;

        process_Input(glfwWindow);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);


        //glUniform4f(vertexColorLocation, red, green, blue, 1.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        glUseProgram(fillShaderProgram);
        glUniform1f(timeUniform, timeValue);

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(fillShaderProgram);
    std::cout << "Terminating Program" << std::endl;
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
    return 0;
}

void frame_resize_viewport(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_Input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}