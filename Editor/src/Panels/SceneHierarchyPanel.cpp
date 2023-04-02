#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Explorer/Components/Components.h"
#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"

#include "Explorer/Utils/PlatformUtils.h"

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
			Object object;
			
			if (ImGui::MenuItem("Create Empty")) {		//�˵������������
				object = m_Scene->CreateEmptyObject("Object");	//����������
			}

			//���˵��3D����
			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Cube")) {		//�Ӳ˵������Cube
					object = m_Scene->CreateMeshObject("Cube", Mesh::Type::Cube);
				}
				
				ImGui::EndMenu();
			}


			//���˵����Դ
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Directional Light")) {		//�Ӳ˵������ƽ�й�Դ
					object = m_Scene->CreateLightObject(Light::Type::Directional);
				}
				if (ImGui::MenuItem("Point Light")) {			//�Ӳ˵���������Դ
					object = m_Scene->CreateLightObject(Light::Type::Point);
				}
				if (ImGui::MenuItem("Spot Light")) {			//�Ӳ˵�������۹�Դ
					object = m_Scene->CreateLightObject(Light::Type::Spot);
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Camera")) {		//�˵���������
				object = m_Scene->CreateCameraObject();
			}

			m_SelectionObject = object;	//ѡ��������Ϊ�´�������

			ImGui::EndPopup();	//���������˵�
		}

		ImGui::End();

		//�������
		ImGui::Begin("Properties");
		if (m_SelectionObject) {				//��ѡ�е��������
			DrawComponents(m_SelectionObject);	//�������
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedObject(Object object)
	{
		m_SelectionObject = object;
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
		if (ImGui::BeginPopupContextItem()) {		//�Ҽ������������
			if (ImGui::MenuItem("Delete")) {		//�˵��ɾ������
				objectDeleted = true;				//������Ϊ��ɾ������Ⱦ���������UI ��ɾ��������
			}
			ImGui::EndPopup();
		}

		if (opened) {			//������Ѵ�
			ImGui::TreePop();	//չ�����
		}

		if (objectDeleted) {
			m_Scene->DestroyObject(object);		//ɾ������
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
		ImGui::DragFloat("##X", &values.x, 0.01f);	//X������ ##����ʾ��ǩ �϶�����0.01
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
		ImGui::DragFloat("##Y", &values.y, 0.01f);	//Y������ ##����ʾ��ǩ �϶�����0.01
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
		ImGui::DragFloat("##Z", &values.z, 0.01f);	//Z������ ##����ʾ��ǩ �϶�����0.01
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

			ImGui::Separator();	//�ָ���
		}
	}

/*	template<typename T, typename UIFunction>
	static void DrawDropdownList(const std::string& name, const std::vector<std::string>& itemStringValues, const uint32_t itemCount, int currentItemIndex, UIFunction uiFunction)
	{
		const char* itemStringCharValues[itemCount] = itemStringValues[currentItemIndex].c_str(); //��ǰ��ֵ
		const char* currentItemStringValue = itemStringCharValues[itemCount];

		//������ ѡ��
		if (ImGui::BeginCombo(name.c_str(), currentItemStringValue)) {
			//���ұ�ѡ����
			for (int i = 0; i < itemCount; i++) {
				bool isSelected = currentItemStringValue == itemStringCharValues[i];	//��ѡ�У���ǰ��==��i��
				//��ѡ�������ı�ʱ��i����ѡ��
				if (ImGui::Selectable(itemStringCharValues[i], isSelected)) {
					currentItemStringValue = itemStringCharValues[i];	//���õ�ǰ��Ϊѡ����i

					uiFunction((T)i);	//����������ѡ��ѡ�й��ܺ���
				}

				if (isSelected) {
					ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
				}
			}
			ImGui::EndCombo();
		}
	}*/

	void SceneHierarchyPanel::DrawComponents(Object object)
	{
		//Name���
		if (object.HasComponent<Name>()) {
			auto& name = object.GetComponent<Name>().m_Name;	//������

			char buffer[256];								//���������buffer
			memset(buffer, 0, sizeof(buffer));				//��buffer����
			strcpy_s(buffer, sizeof(buffer), name.c_str());	//buffer = name

			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {	//�����������������ݸı�ʱ
				name = std::string(buffer);								//����name��Ϊbuffer
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
			//���Light���
			if (ImGui::MenuItem("Light")) {
				m_SelectionObject.AddComponent<Light>();
				ImGui::CloseCurrentPopup();
			}
			//���Mesh���
			if (ImGui::MenuItem("Mesh")) {
				m_SelectionObject.AddComponent<Mesh>();
				ImGui::CloseCurrentPopup();
			}
			//���Material���
			if (ImGui::MenuItem("Material")) {
				m_SelectionObject.AddComponent<Material>();
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

		ImGui::Separator();	//�ָ���

		//����Transform���
		DrawComponent<Transform>("Transform", object, [](Transform& transform)
		{
			DrawVec3Control("Position", transform.m_Position);	//λ��
			DrawVec3Control("Rotation", transform.m_Rotation);	//��ת
			DrawVec3Control("Scale", transform.m_Scale, 1.0f);	//���ţ�Ĭ��ֵ1.0f
		});

		//����Light���
		DrawComponent<Light>("Light", object, [](Light& light)
		{
			const char* types[] = { "Directional", "Point", "Spot"};	//��Դ���ͣ�͸�� ���� 
			const char* currentType = types[(int)light.GetType()];		//��ǰ��Դ����

			//������ ѡ���Դ����
			if (ImGui::BeginCombo("Type", currentType)) {
				//����ѡ����
				for (int i = 0; i < 3; i++) {
					bool isSelected = currentType == types[i];	//��ѡ�У���ǰ��Դ����==��i����Դ����
					//��ѡ�������ı�ʱ����Դ���� ��ѡ��
					if (ImGui::Selectable(types[i], isSelected)) {
						currentType = types[i];			//���õ�ǰ��Դ����
						light.SetType((Light::Type)i);	//���ù�Դ����
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
					}
				}
				ImGui::EndCombo();
			}

			//���Դ | �۹�Դ
			if (light.GetType() == Light::Type::Point || light.GetType() == Light::Type::Spot) {
				float range = light.GetRange();		//���հ뾶
				if (ImGui::DragFloat("Range", &range, 0.01f, 0.0f, 1000.0f)) {	//�϶�����0.01
					light.SetRange(range);
				}
			}

			//�۹�Դ
			if (light.GetType() == Light::Type::Spot) {
				float spotOuterAngle = light.GetSpotOuterAngle();	//Spot���Žǣ���Ӱ���Ե��

				if (ImGui::SliderFloat("Spot Angle", &spotOuterAngle, 1.0f, 179.0f)) {	//Outer = [1, 179]
					light.SetSpotOuterAngle(spotOuterAngle);
				}

				light.SetSpotInnerAngle(spotOuterAngle - spotOuterAngle / 12.0f);	//�������Ž� = Outer - Outer / 12
			}

			ImGui::ColorEdit3("Color", glm::value_ptr(light.m_Color));	//�ƹ���ɫѡ����

			float intensity = light.GetIntensity();		//����ǿ��
			if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 1000.0f)) {	//�϶�����0.01
				light.SetIntensity(intensity);
			}

			ImGui::Checkbox("Render Shadow", &light.m_RenderShadow);	//�Ƿ���Ⱦ��Ӱ���ÿ�
		});

		//����Camera���
		DrawComponent<Camera>("Camera", object, [](Camera& camera)
		{
			ImGui::Checkbox("Main Camera", &camera.m_Primary);	//��������ÿ�

			const char* clearFlags[] = { "Color", "Skybox" };						//������־����ɫ ��պ� 
			const char* currentClearFlag = clearFlags[(int)camera.GetClearFlag()];	//��ǰ������־

			//������ ѡ��������־
			if (ImGui::BeginCombo("Clear Flags", currentClearFlag)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentClearFlag == clearFlags[i];	//��ѡ�У���ǰ������־==��i��������־
					//��ѡ�������ı�ʱ��������־ ��ѡ��
					if (ImGui::Selectable(clearFlags[i], isSelected)) {
						currentClearFlag = clearFlags[i];			//���õ�ǰ������־
						camera.SetClearFlag((Camera::ClearFlag)i);	//�������������־
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
					}
				}
				ImGui::EndCombo();
			}

			ImGui::ColorEdit4("Background", glm::value_ptr(camera.m_BackgroundColor));	//������ɫѡ����

			const char* projectionTypes[] = { "Perspective", "Orthographic" };						//ͶӰ���ͣ�͸�� ���� 
			const char* currentProjectionType = projectionTypes[(int)camera.GetProjectionType()];	//��ǰͶӰ����

			//������ ѡ��ͶӰ����
			if (ImGui::BeginCombo("Projection", currentProjectionType)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionType == projectionTypes[i];	//��ѡ�У���ǰͶӰ����==��i��ͶӰ����
					//��ѡ�������ı�ʱ��ͶӰ���� ��ѡ��
					if (ImGui::Selectable(projectionTypes[i], isSelected)) {
						currentProjectionType = projectionTypes[i];				//���õ�ǰͶӰ����
						camera.SetProjectionType((Camera::ProjectionType)i);	//�������ͶӰ����
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
					}
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == Camera::ProjectionType::Perspective) {	//͸��ͶӰ
				float verticalFov = camera.GetFOV();	//��ֱ�Ž�
				if (ImGui::SliderFloat("Vertical Fov", &verticalFov, 1.0f, 179.0f)) {
					camera.SetFOV(verticalFov);
				}
			}

			if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {	//����ͶӰ
				float size = camera.GetSize();		//�ߴ�
				if (ImGui::DragFloat("Size", &size)) {
					camera.SetSize(size);
				}
			}

			float nearClip = camera.GetNearClip();	//���ü�ƽ��	[0.01, far]
			float farClip = camera.GetFarClip();	//Զ�ü�ƽ��	[near, +��]
			
			if (ImGui::DragFloat("Near", &nearClip, 0.1f, 0.01f, farClip)) {
				camera.SetNearClip(nearClip);
			}

			if (ImGui::DragFloat("Far", &farClip, 0.1f, nearClip, 10000.0f)) {
				camera.SetFarClip(farClip);
			}
		});

		//����Mesh���
		DrawComponent<Mesh>("Mesh", object, [](Mesh& mesh)
		{
			const char* meshTypes[] = { "None (Mesh)", "Other", "Cube", "Sphere", "Capsule", "Cylinder", "Plane"};
			const char* currentMeshType = meshTypes[(int)mesh.GetType()];	//��ǰ��������

			//������ ѡ����������
			if (ImGui::BeginCombo("Mesh", currentMeshType)) {
				for (int i = 0; i < 7; i++) {
					bool isSelected = currentMeshType == meshTypes[i];	//��ѡ�У���ǰ��������==��i����������
					//��ѡ�������ı�ʱ���������� ��ѡ��
					if (ImGui::Selectable(meshTypes[i], isSelected)) {
						currentMeshType = meshTypes[i];		//���õ�ǰ��������
						mesh.SetType((Mesh::Type)i);		//������������
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
					}
				}
				ImGui::EndCombo();
			}
			//TODO��if Type == Other then ��Ӱ�ť ���ļ�����Mesh
		});

		//����Material���
		DrawComponent<Material>("Material", object, [](Material& material)
		{
			//TODO:���Shaderѡ���� ѡ����ɫ�����������ɫ��
			const char* shaders[] = { "Standard" };
			const char* currentShader = shaders[0];	//��ǰ��ɫ������

			//������ ѡ����ɫ������
			if (ImGui::BeginCombo("Shader", currentShader)) {
				for (int i = 0; i < 1; i++) {
					bool isSelected = currentShader == shaders[i];	//��ѡ�У���ǰ��ɫ������==��i����ɫ������
					//��ѡ�������ı�ʱ����ɫ������ ��ѡ��
					if (ImGui::Selectable(shaders[i], isSelected)) {
						currentShader = shaders[i];			//���õ�ǰ��ɫ������
						//TODO:����Shader
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();	//����Ĭ��ѡ����
					}
				}
				ImGui::EndCombo();
			}

			uint32_t albedoTextureID = material.GetAlbedoTextureID();	//Albedo��ͼID

			//Albedo����ѡ��&Ԥ����ť
			if (ImGui::ImageButton((void*)albedoTextureID, ImVec2(22, 22), ImVec2(0, 1), ImVec2(1, 0), 2)) {
				std::string filepath = FileDialogs::OpenFile("Albedo Texture(*.png)\0*.png\0");	//���ļ��Ի����ļ�������\0 �ļ�����.png��
				if (!filepath.empty()) {
					material.SetAlbedoTexture(filepath);	//����Albedo��ͼ
				}
			}

			ImGui::SameLine();
			ImGui::Text("Albedo");
			ImGui::SameLine();
			ImGui::ColorEdit4("##Color", glm::value_ptr(material.m_Color), ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			ImGui::Button("Picker");	//ȡɫ��ť��TODO:���ʰɫ����

			uint32_t specularTextureID = material.GetSpecularTextureID();	//Specular��ͼID

			//Specular����ѡ��&Ԥ����ť
			if (ImGui::ImageButton((void*)specularTextureID, ImVec2(22, 22), ImVec2(0, 1), ImVec2(1, 0), 2)) {
				std::string filepath = FileDialogs::OpenFile("Specular Texture(*.png)\0*.png\0");	//���ļ��Ի����ļ�������\0 �ļ�����.png��
				if (!filepath.empty()) {
					material.SetSpecularTexture(filepath);	//����Specular��ͼ
				}
			}

			ImGui::SameLine();
			ImGui::Text("Specular");
			ImGui::SameLine();

			float shininess = material.GetShininess();		//�����
			if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 648.0f)) {
				material.SetShininess(shininess);
			}

			ImGui::DragFloat2("Tiling", glm::value_ptr(material.m_Tiling), 0.1f);	//����ƽ������
			ImGui::DragFloat2("Offset", glm::value_ptr(material.m_Offset), 0.1f);	//����ƫ����
		});

		//����SpriteRenderer���
		DrawComponent<SpriteRenderer>("Sprite Renderer", object, [](SpriteRenderer& spriteRenderer)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.m_Color));
		});
	}
}