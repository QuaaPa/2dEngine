#include <iostream>

#include "graphics/Graphics.h"
#include "graphics/Shader.h"
#include "graphics/FrameBuffer.h"
#include "UI.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;

void framebufferSizeCcallback(GLFWwindow* window, int width, int height);

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCcallback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glm::mat4 model(1.0f);

    float_t m_far = 750.f;
    float_t m_near = 0.01f;
    float_t m_aspect = 1.f;

    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    projection[0][0] = 1.f / m_aspect;
    projection[1][1] = -1.f;
    projection[2][2] = -1.f / (m_far - m_near);
    projection[3][2] = m_near / (m_far - m_near);

    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 color(1.0f);

    float vertices[] = {
        -0.25f, -0.5f, 0.0f,
         0.25f, -0.5f, 0.0f,
        -0.25f,  0.5f, 0.0f,

         0.25f,  0.5f, 0.0f,
        -0.25f,  0.5f, 0.0f,
         0.25f, -0.5f, 0.0f
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");

    FrameBuffer::createFramebuffer(1920, 1080);

    UI::Init(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        FrameBuffer::bindFramebuffer();
        glClear(GL_COLOR_BUFFER_BIT);
        FrameBuffer::unbindFramebuffer();

        UI::Run();

        FrameBuffer::bindFramebuffer();
        FrameBuffer::clearBuffer();
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        FrameBuffer::unbindFramebuffer();

        model = glm::translate(model, glm::vec3(UI::GetData().m_Position[0], UI::GetData().m_Position[1], UI::GetData().m_Position[2]));
        // model = glm::rotate(model, glm::radians(1.0f), glm::vec3(UI::GetData().m_Rotation[0], UI::GetData().m_Rotation[1], UI::GetData().m_Rotation[2]));
        model = glm::scale(model, glm::vec3(UI::GetData().m_Scale[0], UI::GetData().m_Scale[1], UI::GetData().m_Scale[2]));

        color = glm::vec3(UI::GetData().m_Color[0], UI::GetData().m_Color[1], UI::GetData().m_Color[2]);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("color", color);

        UI::Render();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    UI::Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

}

void framebufferSizeCcallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    FrameBuffer::rescaleFramebuffer((float)width, (float)height);
};
