#pragma once
#include "pch.h"
#include "Event/Event.h"
#include "Timestep.h"

namespace LearnOpenGL
{
	class Layer
	{
	public:
		Layer(const std::string& debugName)
			: m_DebugName(debugName) { }
		virtual void OnAttach() {};
		virtual void OnDetach() {};

		virtual void OnImguiRender() {};
		virtual void OnUpdate(const Timestep& timestep) {};
		virtual void OnEvent(Event& event) {};

		virtual std::string GetLayerName() { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}

