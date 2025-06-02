#pragma once
#include "WindowsWindow.h"
#include "LayerManager.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "ImguiLayer.h"

namespace LearnOpenGL
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Init();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushBackLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void Run();

		bool OnCloseWindow(WindowCloseEvent& event);
		static Application* GetInstance() { return s_Instance; }
		WindowsWindow& GetActiveWindow() { return *m_Window; }
	private:
		std::unique_ptr<WindowsWindow> m_Window;
		ImguiLayer* m_ImguiLayer;
		LayerManager m_LayerManager;
		bool m_Running;
		float m_LastFrameTime;

		static Application* s_Instance;
	};
}

