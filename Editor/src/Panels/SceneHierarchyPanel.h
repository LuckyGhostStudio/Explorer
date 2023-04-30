#pragma once

#include "Explorer/Scene/Scene.h"
#include "Explorer/Core/Core.h"
#include "Explorer/Core/Log.h"
#include "Explorer/Scene/Object.h"
#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// ��������㼶���
	/// </summary>
	class SceneHierarchyPanel
	{
	private:
		std::shared_ptr<Scene> m_Scene;	//�����ʾ�ĳ���
		Object m_SelectionObject;		//��ѡ����ʵ��

		std::shared_ptr<Texture2D> m_SettingsIcon;	//���ð�ťͼ��
	private:
		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="Object">����</param>
		void DrawObjectNode(Object object);

		/// <summary>
		/// ����ʵ����������
		/// </summary>
		/// <param name="object">����</param>
		void DrawComponents(Object object);

		/// <summary>
		/// �������
		/// </summary>
		/// <typeparam name="T">�������</typeparam>
		/// <typeparam name="UIFunction">������ܺ�������</typeparam>
		/// <param name="entity">ʵ��</param>
		/// <param name="uiFunction">������ܺ���</param>
		template<typename T, typename UIFunction>
		void DrawComponent(Object object, UIFunction uiFunction);
	public:
		SceneHierarchyPanel() = default;

		/// <summary>
		/// ��������㼶���
		/// </summary>
		/// <param name="context">����</param>
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// ���ó�������
		/// </summary>
		/// <param name="context">����</param>
		void SetScene(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// ��ȾImGui
		/// </summary>
		void OnImGuiRender();

		/// <summary>
		/// ���ر�ѡ������
		/// </summary>
		/// <returns>��ѡ������</returns>
		Object GetSelectedObject() const { return m_SelectionObject; }

		/// <summary>
		/// ���ñ�ѡ������
		/// </summary>
		/// <param name="object">��ѡ������</param>
		void SetSelectedObject(Object object);
	};
	
	template<typename T, typename UIFunction>
	inline void SceneHierarchyPanel::DrawComponent(Object object, UIFunction uiFunction)
	{
		//T�������
		if (object.HasComponent<T>()) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];	//���壺0������

			//���ڵ��־����|���|���쵽�ұ�|�����ص�|��ܱ߿�
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			auto& component = object.GetComponent<T>();	//������
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();	//���������С

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));	//���ñ߿�ߴ�
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//�и� = �����С + �߿�y * 2

			ImVec2 nodePos = ImGui::GetCursorPos();	//������λ������
			//�����㣺�����Ĺ�ϣֵ��Ϊ���id
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "##Component");
			ImGui::PopStyleVar();

			uint32_t iconID = component.GetIcon()->GetRendererID();	//���ͼ��ID

			ImGui::SameLine();
			ImGui::SetCursorPos(ImVec2(nodePos.x + 28, nodePos.y + 5));					//����Iconλ��
			ImGui::Image((void*)iconID, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));	//���ͼ��ͼƬ

			//��ѡ����ڣ���ʾ��ѡ��
			if (component.GetSelectableEnable()) {
				ImGui::SameLine();
				ImGui::SetCursorPos(ImVec2(nodePos.x + 52, nodePos.y + 4));	//����Checkboxλ��
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.1, 0.1));
				ImGui::Checkbox("##Checkbox", &component.GetEnable_Ref());	//������ù�ѡ��
				ImGui::PopStyleVar();
			}
			ImGui::SameLine();

			ImGui::SetCursorPos(ImVec2(nodePos.x + 52 + 26, nodePos.y));	//����Textλ��
			ImGui::PushFont(boldFont);	//���ý���ı�����Ϊ����
			ImGui::Text(component.GetName().c_str());	//�����
			ImGui::PopFont();
			ImGui::SameLine();

			ImGui::SetCursorPos(ImVec2(nodePos.x + ImGui::GetWindowContentRegionWidth() - lineHeight, nodePos.y + 2));	//����λ��
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2431f, 0.2431f, 0.2431f, 1.0f));		//��ť��ɫ
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.26f, 0.26f, 1.0f));		//�����ͣ�ڰ�ťʱ����ɫ
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.26f, 0.26f, 0.26f, 1.0f));	//��ť������ɫ
			ImGui::PushFont(boldFont);					//���ô���
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight - 4))) {	//������ð�ť TODO:����Image
				ImGui::OpenPopup("ComponentSettings");	//�򿪵�����
			}
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);

			//�Ƴ����
			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings")) {	//��Ⱦ������
				bool disableRemove = false;	//�����Ƴ�
				//Transform
				if (component.GetName() == "Transform") {
					disableRemove = true;
				}

				//�Ƴ�����˵���
				if (ImGui::MenuItem("Remove Component", "", false, !disableRemove)) {
					componentRemoved = true;	//������Ϊ�Ƴ�
				}
				ImGui::EndPopup();
			}

			if (opened) {
				uiFunction(component);	//����������ܺ��������Ƹ������ͬ�Ĳ���
				ImGui::TreePop();	//չ�����
			}

			if (componentRemoved) {
				object.RemoveComponent<T>();	//�Ƴ�T���
			}
		}
	}
}