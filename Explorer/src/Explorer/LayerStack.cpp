#include "exppch.h"
#include "LayerStack.h"

namespace Explorer
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();	//插入迭代器初始化为 begin
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
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);	//插入layer到 m_LayerInsert位置
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
			// TODO: 质疑--的作用
			m_LayerInsert--;
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