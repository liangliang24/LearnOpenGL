#include "pch.h"
#include "ExampleLayer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

ExampleLayer::ExampleLayer(const std::string& debugName)
    : Layer(debugName),
    m_TextureMixLinear(0.5f),
    m_CameraController(LearnOpenGL::CameraController(glm::vec3(0.0f, 0.0f, 3.0f)))
{
}

void ExampleLayer::OnAttach()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
    }
    spdlog::info("Initialize GLAD successfully");

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	spdlog::info("Maximum nr of vertex attributes supported: {0}", nrAttributes);

    m_TriangleShader = LearnOpenGL::Shader("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");
	m_WallTexture = LearnOpenGL::Texture("assets/textures/wall.jpg");
	m_AwesomefaceTexture = LearnOpenGL::Texture("assets/textures/awesomeface.png");

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

    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_EBO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_TriangleShader.UseShader();
    m_TriangleShader.SetUniform1i("u_Texture1", 0);
    m_TriangleShader.SetUniform1i("u_Texture2", 1);
}

void ExampleLayer::OnDetach()
{
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ExampleLayer::OnImguiRender()
{
    ImGui::Begin("Hello World!");

    ImGui::ColorEdit3("background color", m_ClearColor);
    ImGui::ColorEdit4("triangle color", m_TriangleColor);

    ImGui::DragFloat("Texture Mix Linear", &m_TextureMixLinear, 0.01f, 0.0f, 1.0f);

    ImGui::DragFloat3("Translate", m_Translate, 0.01f);
    ImGui::DragFloat3("Rotate", m_Rotate, 1.0f, -180.0f, 180.f);
    ImGui::DragFloat3("Scale", m_Scale, 0.01f);

    float cameraSpeed = m_CameraController.GetCameraSpeed();
    ImGui::DragFloat("C<amera Speed", (float*)&cameraSpeed);
    m_CameraController.SetCameraSpeed(cameraSpeed);

    float sensitivity = m_CameraController.GetMouseSensitivity();
    ImGui::DragFloat("Mouse Sensiticity", (float*)&sensitivity);
    m_CameraController.SetMouseSensitivity(sensitivity);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::End();
}

void ExampleLayer::OnUpdate(const LearnOpenGL::Timestep& timestep)
{
    GLFWwindow* window = LearnOpenGL::Application::GetInstance()->GetActiveWindow().GetWindow();

    m_CameraController.OnUpdate(timestep);

    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_TriangleShader.UseShader();
    glm::mat4 trans = glm::mat4(1.0f);

    trans = glm::translate(trans, glm::vec3(m_Translate[0], m_Translate[1], m_Translate[2]));
    trans = glm::rotate(trans, glm::radians(m_Rotate[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(m_Rotate[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(m_Rotate[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(m_Scale[0], m_Scale[1], m_Scale[2]));
    m_TriangleShader.SetUniformMatrix4fv("u_Transform", 1, GL_FALSE, glm::value_ptr(trans));
    m_TriangleShader.SetUniformMatrix4fv("u_View", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()));
    m_TriangleShader.SetUniformMatrix4fv("u_Projection", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetProjectionMatrix()));
    m_TriangleShader.SetUniform4f("u_Color", m_TriangleColor[0], m_TriangleColor[1], m_TriangleColor[2], 1.0f);
    m_TriangleShader.SetUniform1f("u_TextureMixLinear", m_TextureMixLinear);
    m_WallTexture.ActiveTexture(GL_TEXTURE0);
    m_AwesomefaceTexture.ActiveTexture(GL_TEXTURE1);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void ExampleLayer::OnEvent(LearnOpenGL::Event& event)
{
    LearnOpenGL::EventDispatcher eventDispatcher(event);

    eventDispatcher.Dispatch<LearnOpenGL::WindowResizeEvent>([](LearnOpenGL::WindowResizeEvent& windowResizeEvent)
        {
            glViewport(0, 0, windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight());
            return false;
        });

    m_CameraController.OnEvent(event);
}
