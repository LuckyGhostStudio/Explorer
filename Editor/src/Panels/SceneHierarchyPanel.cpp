#include "SceneHierarchyPanel.h"

#include "Explorer/Scene/Components.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
		//Hierarchy���
		ImGui::Begin("Hierarchy");

		//������������ʵ�壬������each�ڵĺ���
		m_Scene->m_Registry.each([&](auto objectID)
		{
			Object object{ objectID, m_Scene.get() };
			DrawObjectNode(object);		//����ʵ����
		});

		ImGui::End();

		//�������
		ImGui::Begin("Properties");
		if (m_SelectionObject) {				//��ѡ�е��������
			DrawComponents(m_SelectionObject);	//�������
		}
		ImGui::End();
	}
	
	void SceneHierarchyPanel::DrawObjectNode(Object object)
	{
		auto& name = object.GetComponent<Name>().m_Name;	//������
		
		//������־�����ƵĽڵ��Ƿ�ѡ�� ����ѡ�еı�־ ��0 | ������ͷʱ�򿪣�
		ImGuiTreeNodeFlags flags = (m_SelectionObject == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());	//���ڵ㣺���id ����־ �������ʵ������
	
		if (ImGui::IsItemClicked()) {		//����㱻���
			m_SelectionObject = object;		//Object��ѡ��
		}

		if (opened) {			//������Ѵ�
			ImGui::TreePop();	//չ�����
		}
	}

	void SceneHierarchyPanel::DrawComponents(Object object)
	{
		if (object.HasComponent<Name>()) {	//�����������
			auto& name = object.GetComponent<Name>().m_Name;	//������

			char buffer[256];								//���������buffer
			memset(buffer, 0, sizeof(buffer));				//��buffer����
			strcpy_s(buffer, sizeof(buffer), name.c_str());	//buffer = name
			if (ImGui::InputText("Name", buffer, sizeof(buffer))) {	//�����������������ݸı�ʱ
				name = std::string(buffer);	//����name��Ϊbuffer
			}
		}
		if (object.HasComponent<Transform>()) {	//Transform�������
			//Transform�����㣺Transform�����Ĺ�ϣֵ��Ϊ���id Ĭ�ϴ�
			if (ImGui::TreeNodeEx((void*)typeid(Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = object.GetComponent<Transform>();

				ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Position), 0.1f);	//λ�ã��϶��ٶ�0.1
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 0.1f);	//��ת���϶��ٶ�0.1
				ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.1f);		//���ţ��϶��ٶ�0.1

				ImGui::TreePop();	//չ�����
			}

		}
	}
}