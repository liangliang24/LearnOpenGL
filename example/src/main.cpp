#include "pch.h"

#include "Application.h"
#include "ExampleLayer.h"

class Example :public LearnOpenGL::Application
{
public:
    Example()
        : Application()
    {
        PushLayer(new ExampleLayer("Example"));
    }
    virtual ~Example() = default;
};

int main()
{
    std::shared_ptr<LearnOpenGL::Application> app = std::make_shared<Example>();
    app->Run();
    return 0;
}

/*
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utility>
#include <algorithm>

struct WindowData
{
    double xPos, yPos;
}windowData;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    spdlog::info("resize width:{0}�� height:{1}", width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        spdlog::info("press key escape");
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearningOpenGL", NULL, NULL);
    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    spdlog::info("Create GLFW window successfully!");
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }
    spdlog::info("Initialize GLAD successfully");

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetWindowUserPointer(window, &windowData);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
        {
            windowData = *(WindowData*)glfwGetWindowUserPointer(window);
            windowData.xPos = xPos;
            windowData.yPos = yPos;
        });

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    spdlog::info("Maximum nr of vertex attributes supported: {0}", nrAttributes);

    ImVec4 clearColor = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
    ImVec4 triangleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float textureMixLinear = 0.5f;
    float translate[3] = { 0.0f, 0.0f, 0.0f };
    float rotate[3] = { 0.0f, 0.0f, 0.0f };
    float scale[3] = { 1.0f, 1.0f, 1.0f };
    float fov = 45.0f;
    float nearPlant = 0.1f;
    float farPlant = 100.0f;
    float cameraSpeed = 5.0f;
    float cursorLastXPos = 0.0f;
    float cursorLastYPos = 0.0f;

    LearnOpenGL::Shader triangleShader = LearnOpenGL::Shader("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");
    LearnOpenGL::Texture wallTexture = LearnOpenGL::Texture("assets/textures/wall.jpg");
    LearnOpenGL::Texture awesomefaceTexture = LearnOpenGL::Texture("assets/textures/awesomeface.png");

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

    float cameraPitch = 0.0f;
    float cameraYaw = -90.0f;
    float mouseSensitivity = 0.05f;

    float vertices[] =
    {
        -0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,
        0.5f,   -0.5f,  0.0f,   1.0f,   0.0f,
        0.5f,   0.5f,   0.0f,   1.0f,   1.0f,
        -0.5f,  0.5f,   0.0f,   0.0f,   1.0f
    };

    unsigned int indices[] =
    {
        0,  1,  3,
        1,  2,  3
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    triangleShader.UseShader();
    triangleShader.SetUniform1i("u_Texture1", 0);
    triangleShader.SetUniform1i("u_Texture2", 1);

    float lastFrame = 0.0f;
    
    glfwGetCursorPos(window, &windowData.xPos, &windowData.yPos);
    cursorLastXPos = windowData.xPos;
    cursorLastYPos = windowData.yPos;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraDirection * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraDirection * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed * deltaTime;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Hello World!");

            if (ImGui::ColorEdit3("background color", (float*)&clearColor))
            {
                spdlog::info("set background color:{0}, {1}, {2}", clearColor.x, clearColor.y, clearColor.z);
            }

            if (ImGui::ColorEdit4("triangle color", (float*)&triangleColor))
            {
                spdlog::info("set triangle color:{0}, {1}, {2}, {3}", triangleColor.x, triangleColor.y, triangleColor.z, triangleColor.w);
            }

            ImGui::DragFloat("Texture Mix Linear", (float*)&textureMixLinear, 0.01f, 0.0f, 1.0f);

            ImGui::DragFloat3("Translate", translate, 0.01f);
            ImGui::DragFloat3("Rotate", rotate, 1.0f, -180.0f, 180.f);
            ImGui::DragFloat3("Scale", scale, 0.01f);
            ImGui::DragFloat("FOV",(float*)&fov);
            ImGui::DragFloat("NearPlant", (float*)&nearPlant);
            ImGui::DragFloat("FarPlant", (float*)&farPlant);
            ImGui::DragFloat("Camera Speed", (float*)&cameraSpeed);
            ImGui::DragFloat("Mouse Sensiticity", (float*)&mouseSensitivity);
     
            ImGui::Text("delta time:%.3f", deltaTime);
            ImGui::Text("Camera Direction:%f, %f, %f", cameraDirection.x, cameraDirection.y, cameraDirection.z);
            ImGui::Text("Camera Pitch:%f, Camera Yase:%.f", cameraPitch, cameraYaw);

            ImGui::End();
        }

        ImGui::Render();
        float xOffset = windowData.xPos - cursorLastXPos;
        float yOffset = cursorLastYPos - windowData.yPos;
        cursorLastXPos = windowData.xPos;
        cursorLastYPos = windowData.yPos;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            cameraPitch += yOffset * mouseSensitivity;
            cameraYaw += xOffset * mouseSensitivity;
            if (cameraPitch > 89.0f) cameraPitch = 89.0f;
            if (cameraPitch < -89.0f) cameraPitch = -89.0f;
        }

        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShader.UseShader();
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(translate[0], translate[1], translate[2]));
        trans = glm::rotate(trans, glm::radians(rotate[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotate[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotate[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3(scale[0], scale[1], scale[2]));
        cameraDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraDirection.y = sin(glm::radians(cameraPitch));
        cameraDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraDirection = glm::normalize(cameraDirection);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, nearPlant, farPlant);
        triangleShader.SetUniformMatrix4fv("u_Transform", 1, GL_FALSE, glm::value_ptr(trans));
        triangleShader.SetUniformMatrix4fv("u_View", 1, GL_FALSE, glm::value_ptr(view));
        triangleShader.SetUniformMatrix4fv("u_Projection", 1, GL_FALSE, glm::value_ptr(projection));
        triangleShader.SetUniform4f("u_Color", triangleColor.x, triangleColor.y, triangleColor.z, triangleColor.w);
        triangleShader.SetUniform1f("u_TextureMixLinear", textureMixLinear);
        wallTexture.ActiveTexture(GL_TEXTURE0);
        awesomefaceTexture.ActiveTexture(GL_TEXTURE1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
	return 0;
}
*/