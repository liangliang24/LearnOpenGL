#pragma once

#include "Layer.h"
#include <Shader.h>
#include <Texture.h>

#include "CameraController.h"
#include "Event/ApplicationEvent.h"

class BatchRender :public LearnOpenGL::Layer
{
public:
	BatchRender(const std::string& debugName);

	// Inherited via Layer
	void OnAttach() override;
	void OnDetach() override;
	void OnImguiRender() override;
	void OnUpdate(const LearnOpenGL::Timestep& timestep) override;
	void OnEvent(LearnOpenGL::Event& event) override;
private:
	LearnOpenGL::Shader m_TriangleShader;
	LearnOpenGL::Texture m_AwesomefaceTexture;
	LearnOpenGL::Texture m_BorrowmoneyTexture;
	unsigned int m_VBO, m_VAO, m_EBO;
	float m_ClearColor[3] = { 0.0f, 0.0f, 0.0f };
	float m_Translate[3] = { 0.0f, 0.0f, 0.0f };
	float m_Rotate[3] = { 0.0f, 0.0f, 0.0f };
	float m_Scale[3] = { 1.0f, 1.0f, 1.0f };
	LearnOpenGL::CameraController m_CameraController;

	struct Vertex
	{
		float Position[3];
		float Color[4];
		float TexCoords[2];
		float TexID;
	};
public:
	static Vertex* CreateQuad(Vertex* vertex, float x, float y, float textureID, float scale);

};

