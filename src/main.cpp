#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Shader.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    spdlog::info("resize width:{0}£¬ height:{1}", width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        spdlog::info("press key escape");
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
    ImVec4 triangleColor = ImVec4(0.7f, 0.1f, 0.1f, 1.0f);

    Shader triangleShader = Shader("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");

    float vertices[] =
    {
        -0.5f,  -0.5f,  0.0f,
        0.5f,   -0.5f,  0.0f,
        0.5f,   0.5f,   0.0f,
        -0.5f,  0.5f,   0.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

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

            ImGui::End();
        }

        ImGui::Render();

        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShader.UseShader();
        triangleShader.SetUniform4f("uniColor", triangleColor.x, triangleColor.y, triangleColor.z, triangleColor.w);
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
