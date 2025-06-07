#include "pch.h"
#include "LightLayer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"



LightLayer::LightLayer(const std::string& debugName)
    : Layer(debugName),
    m_CameraController(LearnOpenGL::CameraController(glm::vec3(0.0f, 2.0f, 3.0f), -30.0f))
{
	m_LightMaterial.ambientStrength = 0.1f;
	m_LightMaterial.diffuseStrength = 0.5f;
	m_LightMaterial.specularStrength = 1.0f;
}

void LightLayer::OnAttach()
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
	glEnable(GL_DEPTH_TEST);

	{

		m_CubeShader = LearnOpenGL::Shader("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");
		m_CubeDiffuseTexture = LearnOpenGL::Texture("assets/textures/DiffuseTexture.png");
		m_CubeSpecularTexture= LearnOpenGL::Texture("assets/textures/SpecularTexture.png");
		float vertices[] =
		{
			-0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,	-1.0f,   0.0f,   0.0f,
			0.5f,   -0.5f,  0.0f,   0.0f,   0.0f,	-1.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   0.0f,   0.0f,   0.0f,	-1.0f,   1.0f,   1.0f,
			-0.5f,  0.5f,   0.0f,   0.0f,   0.0f,	-1.0f,   0.0f,   1.0f,

			-0.5f,  -0.5f,  1.0f,   0.0f,   0.0f,	1.0f,   0.0f,   0.0f,
			0.5f,   -0.5f,  1.0f,   0.0f,   0.0f,	1.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   1.0f,   0.0f,   0.0f,	1.0f,   1.0f,   1.0f,
			-0.5f,  0.5f,   1.0f,   0.0f,   0.0f,	1.0f,   0.0f,   1.0f,

			-0.5f,  -0.5f,  0.0f,   0.0f,   -1.0f,	0.0f,   0.0f,   0.0f,
			0.5f,   -0.5f,  0.0f,   0.0f,   -1.0f,	0.0f,   1.0f,   0.0f,
			0.5f,	-0.5f,   1.0f,  0.0f,   -1.0f,	0.0f,   1.0f,   1.0f,
			-0.5f,  -0.5f,  1.0f,   0.0f,   -1.0f,	0.0f,   0.0f,   1.0f,

			-0.5f,  0.5f,   0.0f,   0.0f,   1.0f,	0.0f,   0.0f,   0.0f,
			0.5f,   0.5f,   0.0f,   0.0f,   1.0f,	0.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   1.0f,   0.0f,   1.0f,	0.0f,   1.0f,   1.0f,
			-0.5f,  0.5f,   1.0f,   0.0f,   1.0f,	0.0f,   0.0f,   1.0f,

			-0.5f,  -0.5f,  0.0f,   -1.0f,   0.0f,	0.0f,   0.0f,   0.0f,
			-0.5f,  0.5f,   0.0f,   -1.0f,   0.0f,	0.0f,   1.0f,   0.0f,
			-0.5f,  0.5f,   1.0f,   -1.0f,   0.0f,	0.0f,   1.0f,   1.0f,
			-0.5f,  -0.5f,  1.0f,   -1.0f,   0.0f,	0.0f,   0.0f,   1.0f,

			0.5f,  -0.5f,   0.0f,   1.0f,   0.0f,	0.0f,   0.0f,   0.0f,
			0.5f,   0.5f,   0.0f,   1.0f,   0.0f,	0.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   1.0f,   1.0f,   0.0f,	0.0f,   1.0f,   1.0f,
			0.5f,  -0.5f,   1.0f,   1.0f,   0.0f,	0.0f,   0.0f,   1.0f
		};

		unsigned int indices[] =
		{
			0,  1,  3,
			1,  2,  3,

			4,  5,  7,
			5,  6,  7,

			8,  9,  11,
			9,  10, 11,

			12, 13, 15,
			13, 14, 15,

			16, 17, 19,
			17, 18, 19,

			20, 21, 23,
			21, 22, 23,
		};


		glGenBuffers(1, &m_CubeVBO);
		glGenVertexArrays(1, &m_CubeVAO);
		glGenBuffers(1, &m_CubeEBO);
		glBindVertexArray(m_CubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CubeEBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_CubeShader.UseShader();
		m_CubeShader.SetUniform1i("u_Material.diffuse", 0);
		m_CubeShader.SetUniform1i("u_Material.specular", 1);
	}

	{
		float vertices[] =
		{
			-0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,
			0.5f,   -0.5f,  0.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   0.0f,   1.0f,   1.0f,
			-0.5f,  0.5f,   0.0f,   0.0f,   1.0f,

			-0.5f,  -0.5f,  1.0f,   0.0f,   0.0f,
			0.5f,   -0.5f,  1.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   1.0f,   1.0f,   1.0f,
			-0.5f,  0.5f,   1.0f,   0.0f,   1.0f,

			-0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,
			0.5f,   -0.5f,  0.0f,   1.0f,   0.0f,
			0.5f,  -0.5f,   1.0f,   1.0f,   1.0f,
			-0.5f,  -0.5f,  1.0f,   0.0f,   1.0f,

			-0.5f,  0.5f,   0.0f,   0.0f,   0.0f,
			0.5f,   0.5f,   0.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   1.0f,   1.0f,   1.0f,
			-0.5f,  0.5f,   1.0f,   0.0f,   1.0f,

			-0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,
			-0.5f,  0.5f,   0.0f,   1.0f,   0.0f,
			-0.5f,  0.5f,   1.0f,   1.0f,   1.0f,
			-0.5f,  -0.5f,  1.0f,   0.0f,   1.0f,

			0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,
			0.5f,   0.5f,   0.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   1.0f,   1.0f,   1.0f,
			0.5f,  -0.5f,   1.0f,   0.0f,   1.0f
		};

		unsigned int indices[] =
		{
			0,  1,  3,
			1,  2,  3,

			4,  5,  7,
			5,  6,  7,

			8,  9,  11,
			9,  10, 11,

			12, 13, 15,
			13, 14, 15,

			16, 17, 19,
			17, 18, 19,

			20, 21, 23,
			21, 22, 23,
		};

		m_LightShader = LearnOpenGL::Shader("assets/shaders/LightVertexShader.glsl", "assets/shaders/LightFragmentShader.glsl");

		glGenBuffers(1, &m_LightVBO);
		glGenVertexArrays(1, &m_LightVAO);
		glGenBuffers(1, &m_LightEBO);
		glBindVertexArray(m_LightVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_LightVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LightEBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void LightLayer::OnDetach()
{
    glDeleteBuffers(1, &m_CubeEBO);
    glDeleteBuffers(1, &m_CubeVBO);
    glDeleteVertexArrays(1, &m_CubeVAO);
}

void LightLayer::OnImguiRender()
{
    ImGui::Begin("Hello World!");

    ImGui::ColorEdit3("background color", m_ClearColor);

	static int presetMaterialIdx = 1;
	if (ImGui::CollapsingHeader("Cube"))
	{
		ImGui::SeparatorText("Basic property");

		ImGui::ColorEdit3("Cube color", m_CubeColor);
		ImGui::DragFloat3("Cube Translate", m_CubeTranslate, 0.01f);
		ImGui::DragFloat3("Cube Rotate", m_CubeRotate, 1.0f, -180.0f, 180.f);
		ImGui::DragFloat3("Cube Scale", m_CubeScale, 0.01f);

		ImGui::SeparatorText("Material");

		ImGui::DragInt("Cube shininess", &m_CubeShininess, 1, 32, 2048);

		ImGui::SeparatorText("Preset material");
		
		const char* presetMaterialName[] =
		{
			"gold",
			"pearl",
			"red plastic"
		};

		const char* presetMaterialValue = presetMaterialName[presetMaterialIdx];

		if (ImGui::BeginCombo("Preset material combo", presetMaterialValue))
		{
			for (int i = 0; i < IM_ARRAYSIZE(presetMaterialName); ++i)
			{
				const bool isSelected = (presetMaterialIdx == i);
				if (ImGui::Selectable(presetMaterialName[i], isSelected))
				{
					presetMaterialIdx = i;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::SeparatorText("Basic property");

		ImGui::ColorEdit3("Light color", m_LightColor);
		ImGui::DragFloat3("Light Translate", m_LightTranslate, 0.01f);
		ImGui::DragFloat3("Light Rotate", m_LightRotate, 1.0f, -180.0f, 180.f);
		ImGui::DragFloat3("Light Scale", m_LightScale, 0.01f);

		ImGui::SeparatorText("Material");
		ImGui::DragFloat("Light ambient", &m_LightMaterial.ambientStrength, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Light diffuse", &m_LightMaterial.diffuseStrength, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Light specular", &m_LightMaterial.specularStrength, 0.01f, 0.0f, 1.0f);
	}
   
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

void LightLayer::OnUpdate(const LearnOpenGL::Timestep& timestep)
{
    GLFWwindow* window = LearnOpenGL::Application::GetInstance()->GetActiveWindow().GetWindow();

    m_CameraController.OnUpdate(timestep);

	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		glBindVertexArray(m_CubeVAO);
		m_CubeShader.UseShader();
		glm::mat4 trans = glm::mat4(1.0f);

		trans = glm::translate(trans, glm::vec3(m_CubeTranslate[0], m_CubeTranslate[1], m_CubeTranslate[2]));
		trans = glm::rotate(trans, glm::radians(m_CubeRotate[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(m_CubeRotate[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(m_CubeRotate[2]), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(m_CubeScale[0], m_CubeScale[1], m_CubeScale[2]));
		m_CubeShader.SetUniformMatrix4fv("u_Transform", 1, GL_FALSE, glm::value_ptr(trans));
		m_CubeShader.SetUniformMatrix4fv("u_View", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()));
		m_CubeShader.SetUniformMatrix4fv("u_Projection", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetProjectionMatrix()));
		m_CubeShader.SetUniform3f("u_Color", m_CubeColor[0], m_CubeColor[1], m_CubeColor[2]);
		m_CubeShader.SetUniform3f("u_LightColor", m_LightColor[0], m_LightColor[1], m_LightColor[2]);
		m_CubeShader.SetUniform1i("u_Material.shininess", m_CubeShininess);
		m_CubeShader.SetUniform3f("u_Light.position", m_LightTranslate[0], m_LightTranslate[1], m_LightTranslate[2]);
		m_CubeShader.SetUniform1f("u_Light.ambient", m_LightMaterial.ambientStrength);
		m_CubeShader.SetUniform1f("u_Light.diffuse", m_LightMaterial.diffuseStrength);
		m_CubeShader.SetUniform1f("u_Light.specular", m_LightMaterial.specularStrength);
		glm::vec3 cameraPos = m_CameraController.GetCamera().GetCameraPos();
		m_CubeShader.SetUniform3f("u_CameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
		m_CubeDiffuseTexture.ActiveTexture(GL_TEXTURE0);
		m_CubeSpecularTexture.ActiveTexture(GL_TEXTURE1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	{
		glBindVertexArray(m_LightVAO);
		m_LightShader.UseShader();
		glm::mat4 trans = glm::mat4(1.0f);

		trans = glm::translate(trans, glm::vec3(m_LightTranslate[0], m_LightTranslate[1], m_LightTranslate[2]));
		trans = glm::rotate(trans, glm::radians(m_LightRotate[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(m_LightRotate[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(m_LightRotate[2]), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(m_LightScale[0], m_LightScale[1], m_LightScale[2]));
		m_LightShader.SetUniformMatrix4fv("u_Transform", 1, GL_FALSE, glm::value_ptr(trans));
		m_LightShader.SetUniformMatrix4fv("u_View", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetViewMatrix()));
		m_LightShader.SetUniformMatrix4fv("u_Projection", 1, GL_FALSE, glm::value_ptr(m_CameraController.GetCamera().GetProjectionMatrix()));
		m_LightShader.SetUniform4f("u_Color", m_LightColor[0], m_LightColor[1], m_LightColor[2], 1.0f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void LightLayer::OnEvent(LearnOpenGL::Event& event)
{
    LearnOpenGL::EventDispatcher eventDispatcher(event);

    eventDispatcher.Dispatch<LearnOpenGL::WindowResizeEvent>([](LearnOpenGL::WindowResizeEvent& windowResizeEvent)
        {
            glViewport(0, 0, windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight());
            return false;
        });

    m_CameraController.OnEvent(event);
}
