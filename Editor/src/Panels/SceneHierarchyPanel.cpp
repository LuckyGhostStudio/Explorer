#include "SceneHierarchyPanel.h"

#include "Explorer/Scene/Components.h"

#include <imgui/imgui.h>

namespace Explorer
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
	{
		SetScene(scene);
	}
	
	void SceneHierarchyPanel::SetScene(const std::shared_ptr<Scene>& scene)
	{
		m_Scene = scene;
	}
	
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		//������������ʵ�壬������each�ڵĺ���
		m_Scene->m_Registry.each([&](auto ObjectID)
		{
			Object Object{ ObjectID, m_Scene.get() };
			DrawObjectNode(Object);		//����ʵ����
		});

		ImGui::End();
	}
	
	void SceneHierarchyPanel::DrawObjectNode(Object Object)
	{
		auto& name = Object.GetComponent<Name>().m_Name;	//������
		
		//������־�����ƵĽڵ��Ƿ�ѡ�� ����ѡ�еı�־ ��0 | ������ͷʱ�򿪣�
		ImGuiTreeNodeFlags flags = (m_SelectionObject == Object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)Object, flags, name.c_str());	//���ڵ㣺���id ����־ �������ʵ������
	
		if (ImGui::IsItemClicked()) {		//����㱻���
			m_SelectionObject = Object;		//Object��ѡ��
		}

		if (opened) {			//������Ѵ�
			ImGui::TreePop();	//չ�����
		}
	}
}