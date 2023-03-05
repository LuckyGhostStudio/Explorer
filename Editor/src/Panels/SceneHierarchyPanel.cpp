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

		//遍历场景所有实体，并调用each内的函数
		m_Scene->m_Registry.each([&](auto ObjectID)
		{
			Object Object{ ObjectID, m_Scene.get() };
			DrawObjectNode(Object);		//绘制实体结点
		});

		ImGui::End();
	}
	
	void SceneHierarchyPanel::DrawObjectNode(Object Object)
	{
		auto& name = Object.GetComponent<Name>().m_Name;	//物体名
		
		//树结点标志（绘制的节点是否被选中 ？被选中的标志 ：0 | 单击箭头时打开）
		ImGuiTreeNodeFlags flags = (m_SelectionObject == Object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)Object, flags, name.c_str());	//树节点：结点id 结点标志 结点名（实体名）
	
		if (ImGui::IsItemClicked()) {		//树结点被点击
			m_SelectionObject = Object;		//Object被选中
		}

		if (opened) {			//树结点已打开
			ImGui::TreePop();	//展开结点
		}
	}
}