#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace LearnOpenGL
{
	class Camera
	{
	public:
		Camera();
		Camera(const glm::vec3& cameraPos);
		Camera(const glm::vec3& cameraPos, const float pitch, const float yaw);
		~Camera();

		float GetFOV() { return m_FOV; }
		void SetFOV(const float& fov);
		float GetNearPlant() { return m_NearPlant; }
		void SetNearPlant(const float& nearPlant);
		float GetFarPlant() { return m_FarPlant; }
		void SetFarPlant(const float& farPlant);
		const glm::vec3& GetCameraPos() { return m_CameraPos; }
		const glm::vec3& GetCameraDirection() { return m_CameraDirection; }
		void SetCameraPos(const glm::vec3& cameraPos);
		void AddCameraPosForward(const float& offset);
		void AddCameraPosRight(const float& offset);
		void AddPitch(const float& offset);
		void AddYaw(const float& offset);
		const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		void UpdateCameraDirection();
		void UpdateViewMatrix() { m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraDirection, m_CameraUp); }
		void UpdateProjectionMatrix();

	private:
		glm::vec3 m_CameraPos, m_CameraDirection, m_CameraUp;
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix;
		float m_CameraPitch, m_CameraYaw, m_FOV;
		float m_NearPlant;
		float m_FarPlant;
	};
}

