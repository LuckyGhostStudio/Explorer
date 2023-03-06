#include "SceneHierarchyPanel.h"

#include "Explorer/Scene/Components.h"
#include "Explorer/Renderer/Camera.h"

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
		//Name组件
		if (object.HasComponent<Name>()) {
			auto& name = object.GetComponent<Name>().m_Name;	//物体名

			char buffer[256];								//输入框内容buffer
			memset(buffer, 0, sizeof(buffer));				//将buffer置零
			strcpy_s(buffer, sizeof(buffer), name.c_str());	//buffer = name
			if (ImGui::InputText("Name", buffer, sizeof(buffer))) {	//创建输入框，输入框内容改变时
				name = std::string(buffer);	//物体name设为buffer
			}
		}

		//Transform组件
		if (object.HasComponent<Transform>()) {
			//Transform组件结点：Transform组件类的哈希值作为结点id 默认打开
			if (ImGui::TreeNodeEx((void*)typeid(Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = object.GetComponent<Transform>();

				ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Position), 0.1f);	//位置：拖动速度0.1
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 0.1f);	//旋转：拖动速度0.1
				ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.1f);		//缩放：拖动速度0.1

				ImGui::TreePop();	//展开结点
			}
		}

		//Camera组件
		if (object.HasComponent<Camera>()) {
			//Camera组件结点：Camera组件类的哈希值作为结点id 默认打开
			if (ImGui::TreeNodeEx((void*)typeid(Camera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& camera = object.GetComponent<Camera>();

				ImGui::Checkbox("Main Camera", &camera.m_Primary);	//主相机设置框

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };	//投影类型：透视 正交 
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];	//当前投影类型

				//下拉框 选择投影类型
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < 2; i++) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];	//被选中：当前投影类型==第i个投影类型
						//可选择项，该项改变时：投影类型 已选中
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];		//设置当前投影类型
							camera.SetProjectionType((Camera::ProjectionType)i);		//设置相机投影类型
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();	//设置默认选中项
						}
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == Camera::ProjectionType::Perspective) {	//透视投影
					float verticalFov = camera.GetFOV();	//垂直张角
					if (ImGui::DragFloat("Vertical Fov", &verticalFov)) {
						camera.SetFOV(verticalFov);
					}
				}

				if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {	//正交投影
					float size = camera.GetSize();		//尺寸
					if (ImGui::DragFloat("Size", &size)) {
						camera.SetSize(size);
					}
				}

				float nearClip = camera.GetNearClip();	//近裁剪平面
				if (ImGui::DragFloat("Near", &nearClip)) {
					camera.SetNearClip(nearClip);
				}

				float farClip = camera.GetFarClip();	//远裁剪平面
				if (ImGui::DragFloat("Far", &farClip)) {
					camera.SetFarClip(farClip);
				}

				ImGui::TreePop();	//展开结点
			}
		}
	}
}