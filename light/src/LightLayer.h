#pragma once

#include "Layer.h"
#include <Shader.h>
#include <Texture.h>

#include "CameraController.h"
#include "Event/ApplicationEvent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class LightLayer :public LearnOpenGL::Layer
{
public:
	LightLayer(const std::string& debugName);

	// Inherited via Layer
	void OnAttach() override;
	void OnDetach() override;
	void OnImguiRender() override;
	void OnUpdate(const LearnOpenGL::Timestep& timestep) override;
	void OnEvent(LearnOpenGL::Event& event) override;
private:
	float m_ClearColor[3] = { 0.0f, 0.0f, 0.0f };

	unsigned int m_CubeEBO, m_CubeVBO, m_CubeVAO;
	LearnOpenGL::Shader m_CubeShader;
	LearnOpenGL::Texture m_CubeDiffuseTexture;
	LearnOpenGL::Texture m_CubeSpecularTexture;
	float m_CubeColor[3] =		{ 0.8f, 0.2f, 0.5f };
	float m_CubeTranslate[3] =	{ 0.0f, 0.0f, 0.0f };
	float m_CubeRotate[3] =		{ 0.0f, 0.0f, 0.0f };
	float m_CubeScale[3] =		{ 1.0f, 1.0f, 1.0f };

	unsigned int m_LightEBO, m_LightVBO, m_LightVAO;
	LearnOpenGL::Shader m_LightShader;
	float m_LightColor[3] =		{ 1.0f, 1.0f, 1.0f };
	float m_LightTranslate[3] = { 0.0f, 0.7f, 0.5f };
	float m_LightRotate[3] =	{ 0.0f, 0.0f, 0.0f };
	float m_LightScale[3] =		{ 0.1f, 0.1f, 0.1f };

	int m_CubeShininess = 256;

	struct LightMaterial
	{
		float ambientStrength;
		float diffuseStrength;
		float specularStrength;
	};

	LightMaterial m_LightMaterial;

	LearnOpenGL::CameraController m_CameraController;
};

