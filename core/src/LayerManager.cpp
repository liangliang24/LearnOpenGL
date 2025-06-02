#include "pch.h"
#include "LayerManager.h"

namespace LearnOpenGL
{
	LayerManager::LayerManager()
		:m_LayerInsertIndex(0)
	{
	}

	LayerManager::~LayerManager()
	{
		for (Layer* it : m_Layers)
		{
			it->OnDetach();
			delete it;
		}
	}

	void LayerManager::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerManager::PushBackLayer(Layer* layer)
	{
		m_Layers.emplace_back(layer);
		layer->OnAttach();
	}

	void LayerManager::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerManager::PopBackLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.pop_back();
		}
	}
}
