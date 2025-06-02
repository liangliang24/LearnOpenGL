#pragma once
#include "Layer.h"
namespace LearnOpenGL
{
	class LayerManager
	{
	public:
		LayerManager();
		~LayerManager();
		void PushLayer(Layer* layer);
		void PushBackLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PopBackLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex;
	};
}

