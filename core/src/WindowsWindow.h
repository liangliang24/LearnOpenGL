#pragma once
#include <pch.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Event/Event.h"

namespace LearnOpenGL
{
	class WindowsWindow
	{
	public:
		WindowsWindow();
		~WindowsWindow();

		void Init();
		void OnUpdate();
		void SetEventCallBack(const std::function<void(Event&)>& callback) { m_Data.EventCallback = callback; }

		GLFWwindow* GetWindow() { return m_Window; }

		struct WindowData
		{
			double MouseX, MouseY;

			std::function<void(Event&)> EventCallback;
		};
	private:
		GLFWwindow* m_Window;
		WindowData m_Data;
	};
}


