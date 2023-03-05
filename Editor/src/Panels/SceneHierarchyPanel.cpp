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
		//Hierarchy面板
		ImGui::Begin("Hierarchy");

		//遍历场景所有实体，并调用each内的函数
		m_Scene->m_Registry.each([&](auto objectID)
		{
			Object object{ objectID, m_Scene.get() };
			DrawObjectNode(object);		//绘制实体结点
		});

		ImGui::End();

		//属性面板
		ImGui::Begin("Properties");
		if (m_SelectionObject) {				//被选中的物体存在
			DrawComponents(m_SelectionObject);	//绘制组件
		}
		ImGui::End();
	}
	
	void SceneHierarchyPanel::DrawObjectNode(Object object)
	{
		auto& name = object.GetComponent<Name>().m_Name;	//物体名
		
		//树结点标志（绘制的节点是否被选中 ？被选中的标志 ：0 | 单击箭头时打开）
		ImGuiTreeNodeFlags flags = (m_SelectionObject == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());	//树节点：结点id 结点标志 结点名（实体名）
	
		if (ImGui::IsItemClicked()) {		//树结点被点击
			m_SelectionObject = object;		//Object被选中
		}

		if (opened) {			//树结点已打开
			ImGui::TreePop();	//展开结点
		}
	}

	void SceneHierarchyPanel::DrawComponents(Object object)
	{
		if (object.HasComponent<Name>()) {	//名字组件存在
			auto& name = object.GetComponent<Name>().m_Name;	//物体名

			char buffer[256];								//输入框内容buffer
			memset(buffer, 0, sizeof(buffer));				//将buffer置零
			strcpy_s(buffer, sizeof(buffer), name.c_str());	//buffer = name
			if (ImGui::InputText("Name", buffer, sizeof(buffer))) {	//创建输入框，输入框内容改变时
				name = std::string(buffer);	//物体name设为buffer
			}
		}
		if (object.HasComponent<Transform>()) {	//Transform组件存在
			//Transform组件结点：Transform组件类的哈希值作为结点id 默认打开
			if (ImGui::TreeNodeEx((void*)typeid(Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = object.GetComponent<Transform>();

				ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Position), 0.1f);	//位置：拖动速度0.1
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 0.1f);	//旋转：拖动速度0.1
				ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.1f);		//缩放：拖动速度0.1

				ImGui::TreePop();	//展开结点
			}

		}
	}
}