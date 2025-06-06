#pragma once

#include "Layer.h"
#include <Shader.h>
#include <Texture.h>

#include "CameraController.h"
#include "Event/ApplicationEvent.h"

class ExampleLayer :public LearnOpenGL::Layer
{
public:
	ExampleLayer(const std::string& debugName);

	// Inherited via Layer
	void OnAttach() override;
	void OnDetach() override;
	void OnImguiRender() override;
	void OnUpdate(const LearnOpenGL::Timestep& timestep) override;
	void OnEvent(LearnOpenGL::Event& event) override;
private:
	LearnOpenGL::Shader m_TriangleShader;
	unsigned int m_EBO, m_VBO, m_VAO;
	LearnOpenGL::Texture m_WallTexture;
	LearnOpenGL::Texture m_AwesomefaceTexture;
	float m_ClearColor[3] = { 0.0f, 1.0f, 0.8f };
	float m_TriangleColor[3] = { 1.0f, 1.0f, 1.0f };
	float m_TextureMixLinear;
	float m_Translate[3] = { 0.0f, 0.0f, 0.0f };
	float m_Rotate[3] = { 0.0f, 0.0f, 0.0f };
	float m_Scale[3] = { 1.0f, 1.0f, 1.0f };
	LearnOpenGL::CameraController m_CameraController;
};

