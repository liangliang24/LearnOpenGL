#pragma once

#include "Camera.h"
#include <Timestep.h>
#include <Event/Event.h>
#include "Event/MouseEvent.h"
#include "Event/ApplicationEvent.h"

namespace LearnOpenGL
{
	class CameraController
	{
	public:
		CameraController();
		CameraController(const glm::vec3& vec3);
		CameraController(const glm::vec3& vec3, const float pitch, const float yaw);
		~CameraController();

		float GetMouseSensitivity() { return m_MouseSensitivity; }
		void SetMouseSensitivity(const float& sensitivity) { m_MouseSensitivity = sensitivity; }
		float GetCameraSpeed() { return m_CameraSpeed; }
		void SetCameraSpeed(const float& cameraSpeed) { m_CameraSpeed = cameraSpeed; }

		void OnUpdate(const Timestep& timestep);
		void OnEvent(Event& event);
		bool OnWindowResize(WindowResizeEvent& event);

		Camera& GetCamera() { return m_Camera; }
	private:
		Camera m_Camera;
		float m_MouseSensitivity;
		float m_CameraSpeed;
		double m_LastPosX, m_LastPosY;
		bool m_IsFirstUpdate;
	};
}

