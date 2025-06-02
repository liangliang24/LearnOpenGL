#include "pch.h"
#include "Application.h"

namespace LearnOpenGL
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		:m_Window(std::make_unique<WindowsWindow>()),
		m_ImguiLayer(new ImguiLayer("Imgui Layer")),
		m_Running(true),
		m_LastFrameTime(0.0f)
	{
		if (s_Instance)
		{
			spdlog::error("Already have s_Instance");
		}
		s_Instance = this;

		m_Window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_Window->Init();

		PushBackLayer(m_ImguiLayer);
	}
	Application::~Application()
	{
	}
	void Application::Init()
	{
		m_Window->Init();
	}
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnCloseWindow, this, std::placeholders::_1));

		for (auto it = m_LayerManager.end(); it != m_LayerManager.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}


	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerManager.PushLayer(layer);
	}
	void Application::PushBackLayer(Layer* layer)
	{
		m_LayerManager.PushBackLayer(layer);
	}
	void Application::PopLayer(Layer* layer)
	{
		m_LayerManager.PopLayer(layer);
	}
	void Application::Run()
	{
		while (m_Running)
		{
			float time = glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* it : m_LayerManager)
			{
				it->OnUpdate(timestep);
			}

			m_ImguiLayer->Begin();
			for (Layer* it : m_LayerManager)
			{
				it->OnImguiRender();
			}
			m_ImguiLayer->End();

			m_Window->OnUpdate();
		}
	}
	bool Application::OnCloseWindow(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}