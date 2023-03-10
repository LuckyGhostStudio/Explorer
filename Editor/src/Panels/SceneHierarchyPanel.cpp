#include "SceneHierarchyPanel.h"

#include "Explorer/Scene/Components.h"
#include "Explorer/Renderer/Camera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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
		m_SelectionObject = {};	//置空旧场景被选中物体
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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {	//鼠标悬停在该窗口 && 点击鼠标 （点击空白位置）
			m_SelectionObject = {};	//取消选中：置空选中物体
		}

		//创建物体
		if (ImGui::BeginPopupContextWindow(0, 1, false)) {	//右键点击窗口白区域弹出菜单：- 右键 不在物体项上
			if (ImGui::MenuItem("Create Empty Object")) {	//菜单项：创建空物体
				m_Scene->CreateObject("Empty Object");		//创建空物体
			}
			ImGui::EndPopup();
		}

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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;	//水平延伸到边框
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());	//树节点：结点id 结点标志 结点名（实体名）
	
		if (ImGui::IsItemClicked()) {		//树结点被点击
			m_SelectionObject = object;		//Object被选中
		}

		//删除物体
		bool objectDeleted = false;	//物体是否已删除
		if (ImGui::BeginPopupContextItem()) {			//右键点击该物体结点
			if (ImGui::MenuItem("Delete Object")) {		//菜单项：删除物体
				objectDeleted = true;					//物体标记为已删除：渲染结束后面的UI 再删除该物体
			}
			ImGui::EndPopup();
		}

		if (opened) {			//树结点已打开
			ImGui::TreePop();	//展开结点
		}

		if (objectDeleted) {
			m_Scene->DestroyEntity(object);		//删除物体
			if (m_SelectionObject == object) {	//删除的物体为已选中物体
				m_SelectionObject = {};			//清空已选中物体
			}
		}
	}

	/// <summary>
	/// 绘制Vector3控件
	/// </summary>
	/// <param name="label">标签</param>
	/// <param name="values">值</param>
	/// <param name="resetValue">重置值</param>
	/// <param name="columnWidth">每列宽度</param>
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];	//粗体：0号字体

		ImGui::PushID(label.c_str());	//设置控件标签

		//标签列
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);	//设置0号列宽
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());			//设置3个列宽
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });	//Var样式

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//行高 = 字体大小 + 边框.y * 2
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };	//重置值按钮大小

		//X分量UI
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));		//按钮颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f));	//鼠标悬停在按钮时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));	//按钮按下颜色
		ImGui::PushFont(boldFont);					//设置粗体
		if (ImGui::Button("X", buttonSize)) {		//X按钮按下
			values.x = resetValue;					//重置x分量
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();							//在同一行
		ImGui::DragFloat("##X", &values.x, 0.1f);	//X分量列 ##不显示标签 拖动精度0.1
		ImGui::PopItemWidth();						//推出第一个列宽
		ImGui::SameLine();

		//Y分量UI
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));			//按钮颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));	//鼠标悬停在按钮时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));	//按钮按下颜色
		ImGui::PushFont(boldFont);					//设置粗体
		if (ImGui::Button("Y", buttonSize)) {		//Y按钮按下
			values.y = resetValue;					//重置Y分量
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();							//在同一行
		ImGui::DragFloat("##Y", &values.y, 0.1f);	//Y分量列 ##不显示标签 拖动精度0.1
		ImGui::PopItemWidth();						//推出第一个列宽
		ImGui::SameLine();

		//Z分量UI
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));		//按钮颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));	//鼠标悬停在按钮时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f));	//按钮按下颜色
		ImGui::PushFont(boldFont);					//设置粗体
		if (ImGui::Button("Z", buttonSize)) {		//Z按钮按下
			values.z = resetValue;					//重置Z分量
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();							//在同一行
		ImGui::DragFloat("##Z", &values.z, 0.1f);	//Z分量列 ##不显示标签 拖动精度0.1
		ImGui::PopItemWidth();						//推出第一个列宽

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();	//弹出控件标签
	}

	/// <summary>
	/// 绘制组件
	/// </summary>
	/// <typeparam name="T">组件类型</typeparam>
	/// <typeparam name="UIFunction">组件功能函数类型</typeparam>
	/// <param name="name">组件名</param>
	/// <param name="entity">实体</param>
	/// <param name="uiFunction">组件功能函数</param>
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Object object, UIFunction uiFunction)
	{
		//树节点标志：打开|框架|延伸到右边|允许重叠|框架边框
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		//T组件存在
		if (object.HasComponent<T>()) {
			auto& component = object.GetComponent<T>();	//获得组件
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();	//可用区域大小

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));	//设置边框样式
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//行高 = 字体大小 + 边框y * 2
			ImGui::Separator();	//分隔符
			//组件结点：组件类的哈希值作为结点id
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);	//同一行：可用区域左移
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {	//组件设置按钮
				ImGui::OpenPopup("ComponentSettings");	//打开弹出框
			}

			//移除组件
			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings")) {	//渲染弹出框
				//移除组件菜单项
				if (ImGui::MenuItem("Remove Component")) {
					componentRemoved = true;	//组件标记为移除
				}
				ImGui::EndPopup();
			}

			if (opened) {
				uiFunction(component);	//调用组件功能函数：绘制该组件不同的部分
				ImGui::TreePop();	//展开结点
			}

			if (componentRemoved) {
				object.RemoveComponent<T>();	//移除T组件
			}
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
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {	//创建输入框，输入框内容改变时
				name = std::string(buffer);	//物体name设为buffer
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);	//沿着右边框宽-1

		//添加组件按钮
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");	//打开弹出框
		}

		if (ImGui::BeginPopup("AddComponent")) {	//渲染弹出框
			//添加Camera组件
			if (ImGui::MenuItem("Camera")) {
				m_SelectionObject.AddComponent<Camera>();
				ImGui::CloseCurrentPopup();
			}
			//添加SpriteRenderer组件
			if (ImGui::MenuItem("Sprite Renderer")) {
				m_SelectionObject.AddComponent<SpriteRenderer>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		//绘制Transform组件
		DrawComponent<Transform>("Transform", object, [](Transform& transform)
		{
			DrawVec3Control("Position", transform.m_Position);	//位置
			DrawVec3Control("Rotation", transform.m_Rotation);	//旋转
			DrawVec3Control("Scale", transform.m_Scale, 1.0f);	//缩放：默认值1.0f
		});

		//绘制Camera组件
		DrawComponent<Camera>("Camera", object, [](Camera& camera)
		{
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
		});

		//绘制SpriteRenderer组件
		DrawComponent<SpriteRenderer>("Sprite Renderer", object, [](SpriteRenderer& spriteRenderer)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.m_Color));
		});
	}
}