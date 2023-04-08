#include "exppch.h"
#include "UI.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	void UI::DrawColorEditor3(const std::string& label, float* color, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//���ÿؼ�ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

		ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�ȡ����ʾ
		ImGui::SetColumnWidth(0, labelColumnWidth);	//����0���п�

		ImGui::Text(label.c_str());			//�ؼ�����0���У�

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//����1���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
		ImGui::ColorEdit3("##None", color);	//ɫ���༭����1���У�
		ImGui::PopItemWidth();
			
		ImGui::Columns(1);				//����Ϊһ��

		ImGui::PopID();
	}

	void UI::DrawColorEditor4(const std::string& label, float* color, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//���ÿؼ�ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

		ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�ȡ����ʾ
		ImGui::SetColumnWidth(0, labelColumnWidth);	//����0���п�

		ImGui::Text(label.c_str());			//�ؼ�����0���У�

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//����1���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
		ImGui::ColorEdit4("##None", color);	//ɫ���༭����1���У�
		ImGui::PopItemWidth();

		ImGui::Columns(1);				//����Ϊһ��

		ImGui::PopID();
	}
	
	void UI::DrawSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//���ÿؼ�ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

		ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�ȡ����ʾ
		ImGui::SetColumnWidth(0, labelColumnWidth);	//����0���п�

		ImGui::Text(label.c_str());			//�ؼ�����0���У�

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//����1���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
		//��������1���У�
		switch (valueType)
		{
			case ValueType::Float:
				ImGui::SliderFloat("##Float", value, minValue, maxValue);
				break;
			case ValueType::Angle:
				ImGui::SliderAngle("##Angle", value, minValue, maxValue);	//����
				break;
		}
		ImGui::PopItemWidth();

		ImGui::Columns(1);				//����Ϊһ��

		ImGui::PopID();
	}
	
	void UI::DrawDrag(const std::string& label, float* value, float valueSpeed, ValueType valueType, float minValue, float maxValue, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//���ÿؼ�ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

		ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�ȡ����ʾ
		ImGui::SetColumnWidth(0, labelColumnWidth);	//����0���п�

		ImGui::Text(label.c_str());			//�ؼ�����0���У�

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//����1���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
		//�϶�����1���У�
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

		ImGui::Columns(1);				//����Ϊһ��

		ImGui::PopID();
	}
	
	void UI::DrawCheckBox(const std::string& label, bool* value, float labelColumnWidth, float widgetOffset)
	{
		ImGui::PushID(label.c_str());	//���ÿؼ�ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

		ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�ȡ����ʾ
		ImGui::SetColumnWidth(0, labelColumnWidth);	//����0���п�

		ImGui::Text(label.c_str());			//�ؼ�����0���У�

		ImGui::NextColumn();

		ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//����1���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
		ImGui::Checkbox("##None", value);	//��ѡ��1���У�
		ImGui::PopItemWidth();

		ImGui::Columns(1);				//����Ϊһ��

		ImGui::PopID();
	}
}