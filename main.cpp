#include <iostream>
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

int  init(GLFWwindow** window);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
}; 

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main() {
    GLFWwindow* window = nullptr;
    // pass in double pointer
    if (init(&window)) {
        std::cout << "Error: Initialization failed" << std::endl;
    }

    // create vertex attribute object
    // VAO that stores our vertex attribute configuration and which VBO to use
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // create vertex buffer object
    unsigned int VBO;
    // assign id
    glGenBuffers(1, &VBO);
    // bind to array_buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copy vertex data into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // create vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check if it worked
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: Vertex shader compilation failed\n" << infoLog << std::endl;
    }

    // create fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check if it worked
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error: Fragment shader compilation failed\n" << infoLog << std::endl;
    }

    // create overall shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // link shaders 
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error: Shader program link failed\n" << infoLog << std::endl;
    }

    // can delete the shaders after we've linked them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    // use shader
    glUseProgram(shaderProgram);

    // tell opengl how to interepret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 


    // runtime loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        // DRAW
        glUseProgram(shaderProgram);            // shader
        glBindVertexArray(VAO);                 // bind
        glDrawArrays(GL_TRIANGLES, 0, 3);       // draw
        glBindVertexArray(0);                   // unbind

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // delete stuff
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

int init(GLFWwindow** window) {
    // init glfw
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    if (!glfwInit()) {
        const char* description;
        int code = glfwGetError(&description);
        std::cout << "Error: Failed to init GLFW\n - (" << code << ") " << 
                     description << std::endl;
        return -1;
    }
    // glfw hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // create window
    *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Error: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // create context
    glfwMakeContextCurrent(*window);

    // set viewport
    glViewport(0, 0, 800, 600);

    // set callback for window resize
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

    // check if epoxy works
    // std::cout << "Using Expoxy version: " << epoxy_gl_version() << std::endl;

    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


