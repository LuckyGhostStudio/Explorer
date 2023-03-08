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
		m_SelectionObject = {};	//�ÿվɳ�����ѡ������
	}
	
	void SceneHierarchyPanel::OnImGuiRender()
	{
		//Hierarchy���
		ImGui::Begin("Hierarchy");

		//������������ʵ�壬������each�ڵĺ���
		m_Scene->m_Registry.each([&](auto objectID)
		{
			Object object{ objectID, m_Scene.get() };
			DrawObjectNode(object);		//����ʵ����
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {	//�����ͣ�ڸô��� && ������ ������հ�λ�ã�
			m_SelectionObject = {};	//ȡ��ѡ�У��ÿ�ѡ������
		}

		//��������
		if (ImGui::BeginPopupContextWindow(0, 1, false)) {	//�Ҽ�������ڰ����򵯳��˵���- �Ҽ� ������������
			if (ImGui::MenuItem("Create Empty Object")) {	//�˵������������
				m_Scene->CreateObject("Empty Object");		//����������
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		//�������
		ImGui::Begin("Properties");
		if (m_SelectionObject) {				//��ѡ�е��������
			DrawComponents(m_SelectionObject);	//�������
		}
		ImGui::End();
	}
	
	void SceneHierarchyPanel::DrawObjectNode(Object object)
	{
		auto& name = object.GetComponent<Name>().m_Name;	//������
		
		//������־�����ƵĽڵ��Ƿ�ѡ�� ����ѡ�еı�־ ��0 | ������ͷʱ�򿪣�
		ImGuiTreeNodeFlags flags = (m_SelectionObject == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;	//ˮƽ���쵽�߿�
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());	//���ڵ㣺���id ����־ �������ʵ������
	
		if (ImGui::IsItemClicked()) {		//����㱻���
			m_SelectionObject = object;		//Object��ѡ��
		}

		//ɾ������
		bool objectDeleted = false;	//�����Ƿ���ɾ��
		if (ImGui::BeginPopupContextItem()) {			//�Ҽ������������
			if (ImGui::MenuItem("Delete Object")) {		//�˵��ɾ������
				objectDeleted = true;					//������Ϊ��ɾ������Ⱦ���������UI ��ɾ��������
			}
			ImGui::EndPopup();
		}

		if (opened) {			//������Ѵ�
			ImGui::TreePop();	//չ�����
		}

		if (objectDeleted) {
			m_Scene->DestroyEntity(object);		//ɾ������
			if (m_SelectionObject == object) {	//ɾ��������Ϊ��ѡ������
				m_SelectionObject = {};			//�����ѡ������
			}
		}
	}

	/// <summary>
	/// ����Vector3�ؼ�
	/// </summary>
	/// <param name="label">��ǩ</param>
	/// <param name="values">ֵ</param>
	/// <param name="resetValue">����ֵ</param>
	/// <param name="columnWidth">ÿ�п��</param>
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];	//���壺0������

		ImGui::PushID(label.c_str());	//���ÿؼ���ǩ

		//��ǩ��
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);	//����0���п�
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());			//����3���п�
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });	//Var��ʽ

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//�и� = �����С + �߿�.y * 2
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };	//����ֵ��ť��С

		//X����UI
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));		//��ť��ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f));	//�����ͣ�ڰ�ťʱ����ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));	//��ť������ɫ
		ImGui::PushFont(boldFont);					//���ô���
		if (ImGui::Button("X", buttonSize)) {		//X��ť����
			values.x = resetValue;					//����x����
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();							//��ͬһ��
		ImGui::DragFloat("##X", &values.x, 0.1f);	//X������ ##����ʾ��ǩ �϶�����0.1
		ImGui::PopItemWidth();						//�Ƴ���һ���п�
		ImGui::SameLine();

		//Y����UI
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));			//��ť��ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));	//�����ͣ�ڰ�ťʱ����ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));	//��ť������ɫ
		ImGui::PushFont(boldFont);					//���ô���
		if (ImGui::Button("Y", buttonSize)) {		//Y��ť����
			values.y = resetValue;					//����Y����
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();							//��ͬһ��
		ImGui::DragFloat("##Y", &values.y, 0.1f);	//Y������ ##����ʾ��ǩ �϶�����0.1
		ImGui::PopItemWidth();						//�Ƴ���һ���п�
		ImGui::SameLine();

		//Z����UI
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));		//��ť��ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));	//�����ͣ�ڰ�ťʱ����ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f));	//��ť������ɫ
		ImGui::PushFont(boldFont);					//���ô���
		if (ImGui::Button("Z", buttonSize)) {		//Z��ť����
			values.z = resetValue;					//����Z����
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();							//��ͬһ��
		ImGui::DragFloat("##Z", &values.z, 0.1f);	//Z������ ##����ʾ��ǩ �϶�����0.1
		ImGui::PopItemWidth();						//�Ƴ���һ���п�

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();	//�����ؼ���ǩ
	}

	/// <summary>
	/// �������
	/// </summary>
	/// <typeparam name="T">�������</typeparam>
	/// <typeparam name="UIFunction">������ܺ�������</typeparam>
	/// <param name="name">�����</param>
	/// <param name="entity">ʵ��</param>
	/// <param name="uiFunction">������ܺ���</param>
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Object object, UIFunction uiFunction)
	{
		//���ڵ��־����|���|���쵽�ұ�|�����ص�|��ܱ߿�
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		//T�������
		if (object.HasComponent<T>()) {
			auto& component = object.GetComponent<T>();	//������
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();	//���������С

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));	//���ñ߿���ʽ
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//�и� = �����С + �߿�y * 2
			ImGui::Separator();	//�ָ���
			//�����㣺�����Ĺ�ϣֵ��Ϊ���id
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);	//ͬһ�У�������������
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {	//������ð�ť
				ImGui::OpenPopup("ComponentSettings");	//�򿪵�����
			}

			//�Ƴ����
			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings")) {	//��Ⱦ������
				//�Ƴ�����˵���
				if (ImGui::MenuItem("Remove Component")) {
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

	void SceneHierarchyPanel::DrawComponents(Object object)
	{
		//Name���
		if (object.HasComponent<Name>()) {
			auto& name = object.GetComponent<Name>().m_Name;	//������

			char buffer[256];								//���������buffer
			memset(buffer, 0, sizeof(buffer));				//��buffer����
			strcpy_s(buffer, sizeof(buffer), name.c_str());	//buffer = name
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {	//�����������������ݸı�ʱ
				name = std::string(buffer);	//����name��Ϊbuffer
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);	//�����ұ߿��-1

		//��������ť
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");	//�򿪵�����
		}

		if (ImGui::BeginPopup("AddComponent")) {	//��Ⱦ������
			//���Camera���
			if (ImGui::MenuItem("Camera")) {
				m_SelectionObject.AddComponent<Camera>();
				ImGui::CloseCurrentPopup();
			}
			//���SpriteRenderer���
			if (ImGui::MenuItem("Sprite Renderer")) {
				m_SelectionObject.AddComponent<SpriteRenderer>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		//����Transform���
		DrawComponent<Transform>("Transform", object, [](Transform& transform)
		{
			DrawVec3Control("Position", transform.m_Position);	//λ��
			DrawVec3Control("Rotation", transform.m_Rotation);	//��ת
			DrawVec3Control("Scale", transform.m_Scale, 1.0f);	//���ţ�Ĭ��ֵ1.0f
		});

		//����Camera���
		DrawComponent<Camera>("Camera", object, [](Camera& camera)
		{
			ImGui::Checkbox("Main Camera", &camera.m_Primary);	//��������ÿ�

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };	//ͶӰ���ͣ�͸�� ���� 
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];	//��ǰͶӰ����

			//������ ѡ��ͶӰ����
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];	//��ѡ�У���ǰͶӰ����==��i��ͶӰ����
					//��ѡ�������ı�ʱ��ͶӰ���� ��ѡ��
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];		//���õ�ǰͶӰ����
						camera.SetProjectionType((Camera::ProjectionType)i);		//�������ͶӰ����
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
					}
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == Camera::ProjectionType::Perspective) {	//͸��ͶӰ
				float verticalFov = camera.GetFOV();	//��ֱ�Ž�
				if (ImGui::DragFloat("Vertical Fov", &verticalFov)) {
					camera.SetFOV(verticalFov);
				}
			}

			if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {	//����ͶӰ
				float size = camera.GetSize();		//�ߴ�
				if (ImGui::DragFloat("Size", &size)) {
					camera.SetSize(size);
				}
			}

			float nearClip = camera.GetNearClip();	//���ü�ƽ��
			if (ImGui::DragFloat("Near", &nearClip)) {
				camera.SetNearClip(nearClip);
			}

			float farClip = camera.GetFarClip();	//Զ�ü�ƽ��
			if (ImGui::DragFloat("Far", &farClip)) {
				camera.SetFarClip(farClip);
			}
		});

		//����SpriteRenderer���
		DrawComponent<SpriteRenderer>("Sprite Renderer", object, [](SpriteRenderer& spriteRenderer)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.m_Color));
		});
	}
}