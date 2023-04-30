#pragma once

#include "Explorer/Scene/Scene.h"
#include "Explorer/Core/Core.h"
#include "Explorer/Core/Log.h"
#include "Explorer/Scene/Object.h"
#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// 场景对象层级面板
	/// </summary>
	class SceneHierarchyPanel
	{
	private:
		std::shared_ptr<Scene> m_Scene;	//面板显示的场景
		Object m_SelectionObject;		//被选择中实体

		std::shared_ptr<Texture2D> m_SettingsIcon;	//设置按钮图标
	private:
		/// <summary>
		/// 绘制物体结点
		/// </summary>
		/// <param name="Object">物体</param>
		void DrawObjectNode(Object object);

		/// <summary>
		/// 绘制实体的所有组件
		/// </summary>
		/// <param name="object">物体</param>
		void DrawComponents(Object object);

		/// <summary>
		/// 绘制组件
		/// </summary>
		/// <typeparam name="T">组件类型</typeparam>
		/// <typeparam name="UIFunction">组件功能函数类型</typeparam>
		/// <param name="entity">实体</param>
		/// <param name="uiFunction">组件功能函数</param>
		template<typename T, typename UIFunction>
		void DrawComponent(Object object, UIFunction uiFunction);
	public:
		SceneHierarchyPanel() = default;

		/// <summary>
		/// 场景对象层级面板
		/// </summary>
		/// <param name="context">场景</param>
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// 设置场景参数
		/// </summary>
		/// <param name="context">场景</param>
		void SetScene(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// 渲染ImGui
		/// </summary>
		void OnImGuiRender();

		/// <summary>
		/// 返回被选中物体
		/// </summary>
		/// <returns>被选中物体</returns>
		Object GetSelectedObject() const { return m_SelectionObject; }

		/// <summary>
		/// 设置被选中物体
		/// </summary>
		/// <param name="object">被选中物体</param>
		void SetSelectedObject(Object object);
	};
	
	template<typename T, typename UIFunction>
	inline void SceneHierarchyPanel::DrawComponent(Object object, UIFunction uiFunction)
	{
		//T组件存在
		if (object.HasComponent<T>()) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];	//粗体：0号字体

			//树节点标志：打开|框架|延伸到右边|允许重叠|框架边框
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			auto& component = object.GetComponent<T>();	//获得组件
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();	//可用区域大小

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));	//设置边框尺寸
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//行高 = 字体大小 + 边框y * 2

			ImVec2 nodePos = ImGui::GetCursorPos();	//组件结点位置坐标
			//组件结点：组件类的哈希值作为结点id
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "##Component");
			ImGui::PopStyleVar();

			uint32_t iconID = component.GetIcon()->GetRendererID();	//组件图标ID

			ImGui::SameLine();
			ImGui::SetCursorPos(ImVec2(nodePos.x + 28, nodePos.y + 5));					//设置Icon位置
			ImGui::Image((void*)iconID, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));	//组件图标图片

			//勾选框存在：显示勾选框
			if (component.GetSelectableEnable()) {
				ImGui::SameLine();
				ImGui::SetCursorPos(ImVec2(nodePos.x + 52, nodePos.y + 4));	//设置Checkbox位置
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.1, 0.1));
				ImGui::Checkbox("##Checkbox", &component.GetEnable_Ref());	//组件启用勾选框
				ImGui::PopStyleVar();
			}
			ImGui::SameLine();

			ImGui::SetCursorPos(ImVec2(nodePos.x + 52 + 26, nodePos.y));	//设置Text位置
			ImGui::PushFont(boldFont);	//设置结点文本字体为粗体
			ImGui::Text(component.GetName().c_str());	//组件名
			ImGui::PopFont();
			ImGui::SameLine();

			ImGui::SetCursorPos(ImVec2(nodePos.x + ImGui::GetWindowContentRegionWidth() - lineHeight, nodePos.y + 2));	//设置位置
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2431f, 0.2431f, 0.2431f, 1.0f));		//按钮颜色
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.26f, 0.26f, 1.0f));		//鼠标悬停在按钮时的颜色
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.26f, 0.26f, 0.26f, 1.0f));	//按钮按下颜色
			ImGui::PushFont(boldFont);					//设置粗体
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight - 4))) {	//组件设置按钮 TODO:设置Image
				ImGui::OpenPopup("ComponentSettings");	//打开弹出框
			}
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);

			//移除组件
			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings")) {	//渲染弹出框
				bool disableRemove = false;	//禁用移除
				//Transform
				if (component.GetName() == "Transform") {
					disableRemove = true;
				}

				//移除组件菜单项
				if (ImGui::MenuItem("Remove Component", "", false, !disableRemove)) {
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
}