#include "pch.h"
#include "Camera.h"
#include "Application.h"
#include "GLFW/glfw3.h"

namespace LearnOpenGL
{
	Camera::Camera()
		: m_CameraPos(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_CameraPitch(0.0f),
		m_CameraYaw(-90.0f),
		m_FOV(45.0f),
		m_NearPlant(0.1f),
		m_FarPlant(100.0f)
	{
		UpdateCameraDirection();
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	Camera::Camera(const glm::vec3& cameraPos)
		: m_CameraPos(cameraPos),
		m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_CameraPitch(0.0f),
		m_CameraYaw(-90.0f),
		m_FOV(45.0f),
		m_NearPlant(0.1f),
		m_FarPlant(100.0f)
	{
		UpdateCameraDirection();
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	Camera::Camera(const glm::vec3& cameraPos, const float pitch)
		: m_CameraPos(cameraPos),
		m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_CameraPitch(pitch),
		m_CameraYaw(-90.0f),
		m_FOV(45.0f),
		m_NearPlant(0.1f),
		m_FarPlant(100.0f)
	{
		UpdateCameraDirection();
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	Camera::~Camera()
	{
	}

	void Camera::SetFOV(const float& fov)
	{
		m_FOV = fov;
		UpdateProjectionMatrix();
	}

	void Camera::SetNearPlant(const float& nearPlant)
	{
		m_NearPlant = nearPlant;
		UpdateProjectionMatrix();
	}

	void Camera::SetFarPlant(const float& farPlant)
	{
		m_FarPlant = farPlant;
		UpdateProjectionMatrix();
	}

	void Camera::SetCameraPos(const glm::vec3& cameraPos)
	{
		m_CameraPos = cameraPos;
		UpdateViewMatrix();
	}

	void Camera::AddCameraPosForward(const float& offset)
	{
		m_CameraPos += offset * m_CameraDirection;
		UpdateViewMatrix();
	}

	void Camera::AddCameraPosRight(const float& offset)
	{
		m_CameraPos += offset * glm::normalize(glm::cross(m_CameraDirection, m_CameraUp));
		UpdateViewMatrix();
	}

	void Camera::AddPitch(const float& offset)
	{
		m_CameraPitch += offset;
		if (m_CameraPitch > 89.0f) m_CameraPitch = 89.0f;
		if (m_CameraPitch < -89.0f) m_CameraPitch = -89.0f;
		UpdateCameraDirection();
		UpdateViewMatrix();
	}

	void Camera::AddYaw(const float& offset)
	{
		m_CameraYaw += offset;
		UpdateCameraDirection();
		UpdateViewMatrix();
	}

	void Camera::UpdateCameraDirection()
	{
		m_CameraDirection.x = cos(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		m_CameraDirection.y = sin(glm::radians(m_CameraPitch));
		m_CameraDirection.z = sin(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		m_CameraDirection = glm::normalize(m_CameraDirection);
	}
	void Camera::UpdateProjectionMatrix()
	{
		int width, height;
		glfwGetWindowSize(Application::GetInstance()->GetActiveWindow().GetWindow(), &width, &height);
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), float(width) / float(height), m_NearPlant, m_FarPlant);
	}
}