#include "pch.h"
#include "CameraController.h"
#include "Application.h"
#include "Event/MouseEvent.h"

namespace LearnOpenGL
{
	CameraController::CameraController()
		:m_Camera(Camera()),
		m_CameraSpeed(0.5f),
		m_MouseSensitivity(0.5f)
	{
	}

	CameraController::CameraController(const glm::vec3& vec3)
		: m_Camera(Camera(vec3)),
		m_CameraSpeed(0.5f),
		m_MouseSensitivity(0.5f)
	{

	}

	CameraController::CameraController(const glm::vec3& vec3, const float pitch)
		: m_Camera(Camera(vec3, pitch)),
		m_CameraSpeed(0.5f),
		m_MouseSensitivity(0.5f)
	{

	}

	CameraController::~CameraController()
	{

	}

	void CameraController::OnUpdate(const Timestep& timestep)
	{
		GLFWwindow* window = LearnOpenGL::Application::GetInstance()->GetActiveWindow().GetWindow();
		if (m_IsFirstUpdate)
		{
			m_IsFirstUpdate = false;
			glfwGetCursorPos(window, &m_LastPosX, &m_LastPosY);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera.AddCameraPosForward(m_CameraSpeed * timestep);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera.AddCameraPosRight(-m_CameraSpeed * timestep);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera.AddCameraPosForward(-m_CameraSpeed * timestep);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera.AddCameraPosRight(m_CameraSpeed * timestep);
		
		double posX, posY;
		glfwGetCursorPos(window, &posX, &posY);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			double xOffset = posX - m_LastPosX;
			double yOffset = posY - m_LastPosY;
			m_Camera.AddPitch(-yOffset * m_MouseSensitivity);
			m_Camera.AddYaw(xOffset * m_MouseSensitivity);
		}

		m_LastPosX = posX;
		m_LastPosY = posY;

	}

	void CameraController::OnEvent(Event& event)
	{
		EventDispatcher eventDispatcher(event);

		eventDispatcher.Dispatch<WindowResizeEvent>(std::bind(&CameraController::OnWindowResize, this, std::placeholders::_1));
	}

	bool CameraController::OnWindowResize(WindowResizeEvent& event)
	{
		m_Camera.UpdateProjectionMatrix();
		return false;
	}
}
