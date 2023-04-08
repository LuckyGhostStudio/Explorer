#include "exppch.h"
#include "UI.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	void UI::DrawColorEditor3(const std::string& label, float* color, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//设置控件ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

		ImGui::Columns(2, 0, false);	//设置为 两列 id 边界取消显示
		ImGui::SetColumnWidth(0, labelColumnWidth);	//设置0号列宽

		ImGui::Text(label.c_str());			//控件名（0号列）

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//设置1号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
		ImGui::ColorEdit3("##None", color);	//色调编辑器（1号列）
		ImGui::PopItemWidth();
			
		ImGui::Columns(1);				//设置为一列

		ImGui::PopID();
	}

	void UI::DrawColorEditor4(const std::string& label, float* color, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//设置控件ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

		ImGui::Columns(2, 0, false);	//设置为 两列 id 边界取消显示
		ImGui::SetColumnWidth(0, labelColumnWidth);	//设置0号列宽

		ImGui::Text(label.c_str());			//控件名（0号列）

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//设置1号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
		ImGui::ColorEdit4("##None", color);	//色调编辑器（1号列）
		ImGui::PopItemWidth();

		ImGui::Columns(1);				//设置为一列

		ImGui::PopID();
	}
	
	void UI::DrawSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//设置控件ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

		ImGui::Columns(2, 0, false);	//设置为 两列 id 边界取消显示
		ImGui::SetColumnWidth(0, labelColumnWidth);	//设置0号列宽

		ImGui::Text(label.c_str());			//控件名（0号列）

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//设置1号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
		//滑动条（1号列）
		switch (valueType)
		{
			case ValueType::Float:
				ImGui::SliderFloat("##Float", value, minValue, maxValue);
				break;
			case ValueType::Angle:
				ImGui::SliderAngle("##Angle", value, minValue, maxValue);	//弧度
				break;
		}
		ImGui::PopItemWidth();

		ImGui::Columns(1);				//设置为一列

		ImGui::PopID();
	}
	
	void UI::DrawDrag(const std::string& label, float* value, float valueSpeed, ValueType valueType, float minValue, float maxValue, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//设置控件ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

		ImGui::Columns(2, 0, false);	//设置为 两列 id 边界取消显示
		ImGui::SetColumnWidth(0, labelColumnWidth);	//设置0号列宽

		ImGui::Text(label.c_str());			//控件名（0号列）

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//设置1号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
		//拖动条（1号列）
		switch (valueType)
		{
			case ValueType::Float:
				ImGui::DragFloat("##Float", value, valueSpeed, minValue, maxValue);
				break;
			case ValueType::Float2:
				ImGui::DragFloat2("##Float2", value, valueSpeed, minValue, maxValue);
				break;
		}
		ImGui::PopItemWidth();

		ImGui::Columns(1);				//设置为一列

		ImGui::PopID();
	}
	
	void UI::DrawCheckBox(const std::string& label, bool* value, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//设置控件ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

		ImGui::Columns(2, 0, false);	//设置为 两列 id 边界取消显示
		ImGui::SetColumnWidth(0, labelColumnWidth);	//设置0号列宽

		ImGui::Text(label.c_str());			//控件名（0号列）

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//设置1号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
		ImGui::Checkbox("##None", value);	//勾选框（1号列）
		ImGui::PopItemWidth();

		ImGui::Columns(1);				//设置为一列

		ImGui::PopID();
	}
}