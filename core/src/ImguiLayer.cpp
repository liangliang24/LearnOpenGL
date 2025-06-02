#include "pch.h"
#include "ImguiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Application.h"
namespace LearnOpenGL
{
	ImguiLayer::ImguiLayer(const std::string& debugName)
		:Layer(debugName)
	{

	}
	void ImguiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImguiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImguiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(Application::GetInstance()->GetActiveWindow().GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void ImguiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiLayer::OnImguiRender()
	{
	}

	void ImguiLayer::OnUpdate(const Timestep& timestep)
	{
	}

	void ImguiLayer::OnEvent(Event& event)
	{
	}
}
