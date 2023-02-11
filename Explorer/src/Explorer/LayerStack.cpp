#include "exppch.h"
#include "LayerStack.h"

namespace Explorer
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();	//�����������ʼ��Ϊ begin
	}

	LayerStack::~LayerStack()
	{
		//ɾ�����в�
		for (Layer* layer : m_Layers) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);	//����layer�� m_LayerInsertλ��
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);	//���� overlay �� ջ��
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);	//����layer

		if (it != m_Layers.end()) {	//�ҵ�layer
			m_Layers.erase(it);		//�Ƴ�itָ���layer
			// TODO: ����--������
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);	//����overlay

		if (it != m_Layers.end()) {
			m_Layers.erase(it);		//�Ƴ�itָ���overlay
		}
	}
}