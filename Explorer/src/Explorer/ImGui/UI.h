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
		/// ֵ����
		/// </summary>
		enum class ValueType
		{
			Float = 0,
			Float2 = 1,
			Angle = 2
		};
	public:
		/// <summary>
		/// ������ɫ�༭����vec3
		/// </summary>
		/// <param name="label">��ǩ��</param>
		/// <param name="color">��ɫֵָ��</param>
		/// <param name="labelColumnWidth">��ǩ�п�</param>
		/// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
		static void DrawColorEditor3(const std::string& label, float* color, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// ������ɫ�༭����vec4
		/// </summary>
		/// <param name="label">��ǩ��</param>
		/// <param name="color">��ɫֵָ��</param>
		/// <param name="labelColumnWidth">��ǩ�п�</param>
		/// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
		static void DrawColorEditor4(const std::string& label, float* color, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// ���ƻ�������vec1
		/// </summary>
		/// <param name="label">��ǩ��</param>
		/// <param name="value">ֵ</param>
		/// <param name="minValue">��Сֵ</param>
		/// <param name="maxValue">���ֵ</param>
		/// <param name="valueType">ֵ����</param>
		/// <param name="labelColumnWidth">��ǩ�п�</param>
		/// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
		static void DrawSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType = ValueType::Float, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// �����϶���
		/// </summary>
		/// <param name="label">��ǩ��</param>
		/// <param name="value">ֵ</param>
		/// <param name="valueSpeed">�϶�����</param>
		/// <param name="valueType">ֵ����</param>
		/// <param name="minValue">��Сֵ</param>
		/// <param name="maxValue">���ֵ</param>
		/// <param name="labelColumnWidth">��ǩ�п�</param>
		/// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
		static void DrawDrag(const std::string& label, float* value, float valueSpeed = 0.01f, ValueType valueType = ValueType::Float, float minValue = 0.0f, float maxValue = 0.0f, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// ���ƹ�ѡ��
		/// </summary>
		/// <param name="label">��ǩ��</param>
		/// <param name="value">ֵ</param>
		/// <param name="labelColumnWidth">��ǩ�п�</param>
		/// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
		static void DrawCheckBox(const std::string& label, bool* value, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f);

		/// <summary>
		/// ����ͼ��ť
		/// </summary>
		/// <typeparam name="Func">��ť��������</typeparam>
		/// <param name="label">��ǩ��</param>
		/// <param name="textureID">����ID</param>
		/// <param name="size">�ߴ�</param>
		/// <param name="function">��ť��������</param>
		/// <param name="framePadding">�߿�</param>
		/// <param name="labelColumnWidth">��ǩ�п�</param>
		/// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
		template<typename Func>
		static void DrawImageButton(const std::string& label, uint32_t textureID, const glm::vec2& size, Func function, float framePadding = 2.0f, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f)
		{
			ImGui::PushID(label.c_str());	//���ÿؼ�ID

			float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

			ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�ȡ����ʾ
			ImGui::SetColumnWidth(0, labelColumnWidth);	//����0���п�

			ImGui::Text(label.c_str());	//��ǩ��0���У�

			ImGui::NextColumn();

			ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//����1���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
			//��ͼѡ��&Ԥ����ť��1���У�
			if (ImGui::ImageButton((void*)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding)) {
				function();	//��ť�¼�����
			}
			ImGui::PopItemWidth();

			ImGui::Columns(1);				//����Ϊһ��

			ImGui::PopID();
		}

		/// <summary>
		/// ���ƿ�ѡ�������б�
		/// </summary>
		/// <typeparam name="Func">ĳ��ѡ��ʱ�ص�����</typeparam>
		/// <param name="label">��ǩ��</param>
		/// <param name="currentValue">��ǰֵ</param>
		/// <param name="values">����ֵ</param>
		/// <param name="valueCount">ֵ����</param>
		/// <param name="function">ĳ��ѡ��ʱ�ص�����</param>
		/// <param name="labelColumnWidth">��ǩ�п�</param>
		/// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
		template<typename Func>
		static void DrawDropdownList(const std::string& label, const char* currentValue, const char* values[], uint32_t valueCount, Func function, float labelColumnWidth = 120.0f, float widgetOffset = 20.0f)
		{
			ImGui::PushID(label.c_str());	//���ÿؼ�ID

			float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

			ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�ȡ����ʾ
			ImGui::SetColumnWidth(0, labelColumnWidth);	//����0���п�

			ImGui::Text(label.c_str());	//��ǩ��0���У�

			ImGui::NextColumn();

			ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset);	//����1���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
			//������1���У�
			if (ImGui::BeginCombo("##None", currentValue)) {
				//����ѡ����
				for (int i = 0; i < valueCount; i++) {
					bool isSelected = currentValue == values[i];	//��ѡ�У���ǰֵ==��i��ֵ
					//��ѡ�������ı�ʱ����i��Ϊѡ����
					if (ImGui::Selectable(values[i], isSelected)) {
						currentValue = values[i];		//���õ�ǰֵ
						function(i, currentValue);		//i��ѡ���¼�����
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::Columns(1);				//����Ϊһ��

			ImGui::PopID();
		}

		/// <summary>
		/// �������ڵ�
		/// </summary>
		/// <typeparam name="T">���ID����</typeparam>
		/// <typeparam name="Func">���򿪵��ú���</typeparam>
		/// <param name="label">����ǩ��</param>
		/// <param name="bold">��ǩ���Ƿ��Ǵ���</param>
		/// <param name="function">���򿪵��ú���</param>
		template<typename T, typename Func>
		static void DrawTreeNode(const std::string& label, bool bold, Func function)
		{
			ImGui::PushID(label.c_str());	//���ÿؼ�ID

			ImGuiIO& io = ImGui::GetIO();
			auto font = io.Fonts->Fonts[0];	//���壺0�� ����

			if (!bold) font = io.Fonts->Fonts[1];	//1�� Ĭ������

			//���ڵ��־����|���|���쵽�ұ�|�����ص�|��ܱ߿�
			ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();	//���������С

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));	//���ñ߿�ߴ�
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//����и� = �����С + �߿�y * 2

			ImGui::PushFont(font);	//���ý���ı�����Ϊ����
			//��㣺T��Ĺ�ϣֵ��Ϊ���id
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
			ImGui::PopFont();
			ImGui::PopStyleVar();
			
			//����Ѵ�
			if (opened) {
				function(lineHeight);	//ִ�н��򿪺�Ĳ���
				ImGui::TreePop();	//չ�����
			}

			ImGui::PopID();
		}
	};
}