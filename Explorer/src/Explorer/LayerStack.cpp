#include "exppch.h"
#include "LayerStack.h"

namespace Explorer
{
	LayerStack::LayerStack()
	{
		
	}

	LayerStack::~LayerStack()
	{
		//删除所有层
		for (Layer* layer : m_Layers) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);		//插入layer到 m_LayerInsertIndex位置
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);	//插入 overlay 到 栈顶
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);	//查找layer

		if (it != m_Layers.end()) {	//找到layer
			m_Layers.erase(it);		//移除it指向的layer
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);	//查找overlay

		if (it != m_Layers.end()) {
			m_Layers.erase(it);		//移除it指向的overlay
		}
	}
}