#include "pch.h"
#include "BatchRender.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

BatchRender::BatchRender(const std::string& debugName)
    : Layer(debugName),
    m_CameraController(LearnOpenGL::CameraController(glm::vec3(0.0f, 0.0f, 3.0f)))
{
}

void BatchRender::OnAttach()
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
    m_AwesomefaceTexture = LearnOpenGL::Texture("assets/textures/awesomeface.png");
    m_BorrowmoneyTexture = LearnOpenGL::Texture("assets/textures/BorrowMoney.jpg");

    /*float vertices[] =
    {
        -1.0f,  -0.5f,  0.0f,   1.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   -0.5f,  0.0f,   1.0f,   0.0f,   0.0f,   1.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.5f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f,   1.0f,   1.0f,   0.0f,
        -1.0f,  0.5f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,

        1.0f,  -0.5f,  0.0f,   1.0f,   0.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f,
        2.0f,  -0.5f,  0.0f,   1.0f,   0.0f,   1.0f,   1.0f,   1.0f,   0.0f,   1.0f,
        2.0f,  0.5f,   0.0f,   1.0f,   0.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,
        1.0f,  0.5f,   0.0f,   1.0f,   0.0f,   1.0f,   1.0f,   0.0f,   1.0f,   1.0f,
    };*/

	const size_t MaxQuadCount = 1000;
	const size_t MaxVertexCount = MaxQuadCount * 4;
	const size_t MaxIndexCount = MaxQuadCount * 6;

    /*unsigned int indices[] =
    {
        0,  2,  3,  0,  1,  2,
        4,  6,  7,  4,  5,  6
    };*/

    uint32_t indices[MaxIndexCount];
    uint32_t offset = 0;
    for (size_t i = 0; i < MaxIndexCount; i += 6)
    {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;
		indices[i + 3] = offset + 0;
		indices[i + 4] = offset + 2;
		indices[i + 5] = offset + 3;

        offset += 4;
    }

    glEnable(GL_DEPTH_TEST);

    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_EBO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxVertexCount, nullptr, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));
	glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_TriangleShader.UseShader();
    int samplers[2] = { 0, 1 };
    m_TriangleShader.SetUniform1iv("u_Textures", 2, samplers);
}

void BatchRender::OnDetach()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void BatchRender::OnImguiRender()
{
    ImGui::Begin("Hello World!");

    ImGui::ColorEdit3("background color", m_ClearColor);

    ImGui::DragFloat3("Translate", m_Translate, 0.01f);
    ImGui::DragFloat3("Rotate", m_Rotate, 1.0f, -180.0f, 180.f);
    ImGui::DragFloat3("Scale", m_Scale, 0.01f);

    float cameraSpeed = m_CameraController.GetCameraSpeed();
    ImGui::DragFloat("Camera Speed", (float*)&cameraSpeed);
    m_CameraController.SetCameraSpeed(cameraSpeed);

    float sensitivity = m_CameraController.GetMouseSensitivity();
    ImGui::DragFloat("Mouse Sensiticity", (float*)&sensitivity);
    m_CameraController.SetMouseSensitivity(sensitivity);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::End();
}

void BatchRender::OnUpdate(const LearnOpenGL::Timestep& timestep)
{
    GLFWwindow* window = LearnOpenGL::Application::GetInstance()->GetActiveWindow().GetWindow();

    m_CameraController.OnUpdate(timestep);

	/*glMapBuffer()
	glUnMapBuffer()*/

	std::array<Vertex, 1000> vertices;
	Vertex* vertex = vertices.data();
	int indexCount = 0;
	for (int x = 0; x < 5; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			vertex = CreateQuad(vertex, x, y, (x + y) % 2, 1.0f);
			indexCount += 6;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_TriangleShader.UseShader();

	m_AwesomefaceTexture.ActiveTexture(GL_TEXTURE0);
	m_BorrowmoneyTexture.ActiveTexture(GL_TEXTURE1);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(m_Translate[0], m_Translate[1], m_Translate[2]));
    trans = glm::rotate(trans, glm::radians(m_Rotate[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(m_Rotate[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(m_Rotate[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(m_Scale[0], m_Scale[1], m_Scale[2]));
    m_TriangleShader.SetUniformMatrix4fv("u_Transform", 1, GL_FALSE, glm::value_ptr(trans));
    m_TriangleShader.SetUniformMatrix4fv("u_View", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()));
    m_TriangleShader.SetUniformMatrix4fv("u_Projection", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetProjectionMatrix()));
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void BatchRender::OnEvent(LearnOpenGL::Event& event)
{
    LearnOpenGL::EventDispatcher eventDispatcher(event);

    eventDispatcher.Dispatch<LearnOpenGL::WindowResizeEvent>([](LearnOpenGL::WindowResizeEvent& windowResizeEvent)
        {
            glViewport(0, 0, windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight());
            return false;
        });

    m_CameraController.OnEvent(event);
}

BatchRender::Vertex* BatchRender::CreateQuad(Vertex* vertex, float x, float y, float textureID, float scale)
{
	vertex->Position[0] = x;
	vertex->Position[1] = y;
	vertex->Position[2] = 0;
	vertex->Color[0] = 1.0f;
	vertex->Color[1] = 1.0f;
	vertex->Color[2] = 1.0f;
	vertex->Color[3] = 1.0f;
	vertex->TexCoords[0] = 0.0f;
	vertex->TexCoords[1] = 0.0f;
	vertex->TexID = textureID;
    vertex++;

	vertex->Position[0] = x + scale;
	vertex->Position[1] = y;
	vertex->Position[2] = 0;
	vertex->Color[0] = 1.0f;
	vertex->Color[1] = 1.0f;
	vertex->Color[2] = 1.0f;
	vertex->Color[3] = 1.0f;
	vertex->TexCoords[0] = 1.0f;
	vertex->TexCoords[1] = 0.0f;
	vertex->TexID = textureID;
	vertex++;

	vertex->Position[0] = x + scale;
	vertex->Position[1] = y + scale;
	vertex->Position[2] = 0;
	vertex->Color[0] = 1.0f;
	vertex->Color[1] = 1.0f;
	vertex->Color[2] = 1.0f;
	vertex->Color[3] = 1.0f;
	vertex->TexCoords[0] = 1.0f;
	vertex->TexCoords[1] = 1.0f;
	vertex->TexID = textureID;
	vertex++;

	vertex->Position[0] = x;
	vertex->Position[1] = y + scale;
	vertex->Position[2] = 0;
	vertex->Color[0] = 1.0f;
	vertex->Color[1] = 1.0f;
	vertex->Color[2] = 1.0f;
	vertex->Color[3] = 1.0f;
	vertex->TexCoords[0] = 0.0f;
	vertex->TexCoords[1] = 1.0f;
	vertex->TexID = textureID;
	vertex++;

    return vertex;
}

