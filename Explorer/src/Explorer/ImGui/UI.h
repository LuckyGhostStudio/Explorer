#pragma once

#include <glm/glm.hpp>
#include <string>
//#include <imgui.h>
//#include <imgui.h>
//#include <imgui_internal.h>

namespace Explorer
{
	class UI
	{
	public:
		/// <summary>
		/// 值类型
		/// </summary>
		enum class ValueType
		{
			Float = 0,
			Float2 = 1,
			Angle = 2
		};
	public:
		/// <summary>
		/// 绘制颜色编辑器：vec3
		/// </summary>
		/// <param name="label">标签名</param>
		/// <param name="color">颜色值指针</param>
		/// <param name="labelColumnWidth">标签列宽</param>
		/// <param name="widgetOffset">小部件右边界向左偏移量</param>
		static void DrawColorEditor3(const std::string& label, float* color, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// 绘制颜色编辑器：vec4
		/// </summary>
		/// <param name="label">标签名</param>
		/// <param name="color">颜色值指针</param>
		/// <param name="labelColumnWidth">标签列宽</param>
		/// <param name="widgetOffset">小部件右边界向左偏移量</param>
		static void DrawColorEditor4(const std::string& label, float* color, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// 绘制滑动条：vec1
		/// </summary>
		/// <param name="label">标签名</param>
		/// <param name="value">值</param>
		/// <param name="minValue">最小值</param>
		/// <param name="maxValue">最大值</param>
		/// <param name="valueType">值类型</param>
		/// <param name="labelColumnWidth">标签列宽</param>
		/// <param name="widgetOffset">小部件右边界向左偏移量</param>
		static void DrawSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType = ValueType::Float, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// 绘制拖动条
		/// </summary>
		/// <param name="label">标签名</param>
		/// <param name="value">值</param>
		/// <param name="valueSpeed">拖动精度</param>
		/// <param name="valueType">值类型</param>
		/// <param name="minValue">最小值</param>
		/// <param name="maxValue">最大值</param>
		/// <param name="labelColumnWidth">标签列宽</param>
		/// <param name="widgetOffset">小部件右边界向左偏移量</param>
		static void DrawDrag(const std::string& label, float* value, float valueSpeed = 0.01f, ValueType valueType = ValueType::Float, float minValue = 0.0f, float maxValue = 0.0f, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// 绘制勾选框
		/// </summary>
		/// <param name="label">标签名</param>
		/// <param name="value">值</param>
		/// <param name="labelColumnWidth">标签列宽</param>
		/// <param name="widgetOffset">小部件右边界向左偏移量</param>
		static void DrawCheckBox(const std::string& label, bool* value, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// 绘制图像按钮
		/// </summary>
		/// <typeparam name="Func">按钮监听函数</typeparam>
		/// <param name="label">标签名</param>
		/// <param name="textureID">纹理ID</param>
		/// <param name="size">尺寸</param>
		/// <param name="function">按钮监听函数</param>
		/// <param name="framePadding">边框</param>
		/// <param name="labelColumnWidth">标签列宽</param>
		/// <param name="widgetOffset">小部件右边界向左偏移量</param>
		template<typename Func>
		static void DrawImageButton(const std::string& label, uint32_t textureID, const glm::vec2& size, Func function, float framePadding = 2.0f, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f)
		{
			ImGui::PushID(label.c_str());	//设置控件ID

			float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

			ImGui::Columns(2, 0, false);	//设置为 两列 id 边界取消显示
			ImGui::SetColumnWidth(0, labelColumnWidth);	//设置0号列宽

			ImGui::Text(label.c_str());	//标签（0号列）

			ImGui::NextColumn();

			ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//设置1号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
			//贴图选择&预览按钮（1号列）
			if (ImGui::ImageButton((void*)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding)) {
				function();	//按钮事件函数
			}
			ImGui::PopItemWidth();

			ImGui::Columns(1);				//设置为一列

			ImGui::PopID();
		}

		/// <summary>
		/// 绘制可选择下拉列表
		/// </summary>
		/// <typeparam name="Func">某项选中时回调函数</typeparam>
		/// <param name="label">标签名</param>
		/// <param name="currentValue">当前值</param>
		/// <param name="values">所有值</param>
		/// <param name="valueCount">值个数</param>
		/// <param name="function">某项选中时回调函数</param>
		/// <param name="labelColumnWidth">标签列宽</param>
		/// <param name="widgetOffset">小部件右边界向左偏移量</param>
		template<typename Func>
		static void DrawDropdownList(const std::string& label, const char* currentValue, const char* values[], uint32_t valueCount, Func function, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f)
		{
			ImGui::PushID(label.c_str());	//设置控件ID

			float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

			ImGui::Columns(2, 0, false);	//设置为 两列 id 边界取消显示
			ImGui::SetColumnWidth(0, labelColumnWidth);	//设置0号列宽

			ImGui::Text(label.c_str());	//标签（0号列）

			ImGui::NextColumn();

			ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//设置1号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
			//下拉框（1号列）
			if (ImGui::BeginCombo("##None", currentValue)) {
				//查找选中项
				for (int i = 0; i < valueCount; i++) {
					bool isSelected = currentValue == values[i];	//被选中：当前值==第i个值
					//可选择项，该项改变时：第i项为选中项
					if (ImGui::Selectable(values[i], isSelected)) {
						currentValue = values[i];		//设置当前值
						function(i, currentValue);		//i项选中事件函数
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//设置默认选中项
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::Columns(1);				//设置为一列

			ImGui::PopID();
		}

		/// <summary>
		/// 绘制树节点
		/// </summary>
		/// <typeparam name="T">结点ID类型</typeparam>
		/// <typeparam name="Func">结点打开调用函数</typeparam>
		/// <param name="label">结点标签名</param>
		/// <param name="bold">标签名是否是粗体</param>
		/// <param name="function">结点打开调用函数</param>
		template<typename T, typename Func>
		static void DrawTreeNode(const std::string& label, bool bold, Func function)
		{
			ImGui::PushID(label.c_str());	//设置控件ID

			ImGuiIO& io = ImGui::GetIO();
			auto font = io.Fonts->Fonts[0];	//字体：0号 粗体

			if (!bold) font = io.Fonts->Fonts[1];	//1号 默认字体

			//树节点标志：打开|框架|延伸到右边|允许重叠|框架边框
			ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();	//可用区域大小

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));	//设置边框尺寸
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//结点行高 = 字体大小 + 边框y * 2

			ImGui::PushFont(font);	//设置结点文本字体为粗体
			//结点：T类的哈希值作为结点id
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
			ImGui::PopFont();
			ImGui::PopStyleVar();
			
			//结点已打开
			if (opened) {
				function(lineHeight);	//执行结点打开后的操作
				ImGui::TreePop();	//展开结点
			}

			ImGui::PopID();
		}
	};
}