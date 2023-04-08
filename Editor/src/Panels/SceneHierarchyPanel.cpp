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
#include "Explorer/ImGui/UI.h"

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

		ImGui::End();	//Hierarchy

		//�������
		ImGui::Begin("Properties");
		if (m_SelectionObject) {				//��ѡ�е��������
			DrawComponents(m_SelectionObject);	//�������
		}
		ImGui::End();	//Properties

		//���������������
		ImGui::Begin("Environment");
		//TODO:���л������л���������

		Environment& environment = m_Scene->GetEnvironment();	//��������
		Skybox& skybox = environment.GetSkybox();	//��պ�

		UI::DrawCheckBox("Enable Skybox", &environment.GetSkyboxEnable_Ref());	//Enable Skybox�Ƿ�������պ� ��ѡ��

		//Skybox��պ����� ���ڵ�
		UI::DrawTreeNode<Environment>(skybox.GetShader()->GetName() + "(Skybox)", true, [&](float lineHeight)
		{
			UI::DrawColorEditor3("Tint Color", glm::value_ptr(skybox.GetTintColor()));					//Tint Colorɫ�� ��ɫ�༭��
			UI::DrawSlider("Expose", &skybox.GetExpose_Ref(), 0.0f, 8.0f);								//Expose�ع�� ������
			UI::DrawSlider("Rotation", &skybox.GetRotation_Ref(), 0.0f, 360.0f, UI::ValueType::Angle);	//Rotation��תֵz ������

			uint32_t rightTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Right]->GetRendererID();	//RightԤ����ͼID
			uint32_t leftTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Left]->GetRendererID();	//LeftԤ����ͼID
			uint32_t upTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Up]->GetRendererID();		//UpԤ����ͼID
			uint32_t downTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Down]->GetRendererID();	//DownԤ����ͼID
			uint32_t frontTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Front]->GetRendererID();	//FrontԤ����ͼID
			uint32_t backTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Back]->GetRendererID();	//BackԤ����ͼID

			//Cubemap��ͼ���� ���ڵ�
			UI::DrawTreeNode<Cubemap>("Cubemap Settings", false, [&](float lineHeight)
			{
				glm::vec2 textureButtonSize = { 50, 50 };

				//Right(+x)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Right [+X]", rightTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Right[+X] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Right);	//����RightԤ����ͼ
					}
				});

				//Left(-x)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Left [-X]", leftTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Left[-X] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Left);	//����LeftԤ����ͼ
					}
				});

				//Up(+y)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Up [+Y]", upTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Up[+Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Up);	//����UpԤ����ͼ
					}
				});

				//Down(-y)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Down [-Y]", downTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Down[-Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Down);	//����DownԤ����ͼ
					}
				});

				//Front(+z)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Front [+Z]", frontTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Front[+Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Front);	//����FrontԤ����ͼ
					}
				});

				//Back(-z)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Back [-Z]", backTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Back[-Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Back);	//����BackԤ����ͼ
					}
				});
			});

			//CubemapԤ��ͼ ���ڵ�
			UI::DrawTreeNode<Cubemap>("Cubemap Preview", false, [&](float lineHeight)
			{
				ImVec2 size = ImVec2(90, 90);	//Cubemap���Ŵ�С
				int positionX = 35;				//Cubemap Xλ��

				//CubemapԤ��ͼ
				ImGui::SetCursorPosX(positionX);
				ImGui::BeginChild("Cubemap Preview", ImVec2(size.x * 4 + 40, size.y * 3 + 40), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				//Up [+Y]
				ImGui::SetCursorPosX(size.x);	//�����α�Xλ��
				ImGui::Image((void*)upTextureID, size);
				//Left [-X]
				ImGui::SetCursorPosX(0);
				ImGui::Image((void*)leftTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
				ImGui::SameLine();
				//Back [-Z]
				ImGui::SetCursorPosX(size.x);
				ImGui::Image((void*)backTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
				ImGui::SameLine();
				//Right [+X]
				ImGui::SetCursorPosX(size.x * 2);
				ImGui::Image((void*)rightTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
				ImGui::SameLine();
				//Front [+Z]
				ImGui::SetCursorPosX(size.x * 3);
				ImGui::Image((void*)frontTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
				//Down [-Y]
				ImGui::SetCursorPosX(size.x);
				ImGui::Image((void*)downTextureID, size);

				ImGui::PopStyleVar();

				ImGui::EndChild();
			});
		});

		//Environment Lighting������������ ���ڵ�
		UI::DrawTreeNode<Environment>("Environment Lighting", true, [&](float lineHeight)
		{
			const char* sourceTypes[] = { "Skybox","Color" };							//������Դ����
			const char* currentSource = sourceTypes[(int)environment.GetSourceType()];	//��ǰ������Դ����

			//Source������Դ���� ѡ�������б�
			UI::DrawDropdownList("Source", currentSource, sourceTypes, 2, [&](int index, const char* value)
			{
				environment.SetSourceType((Environment::SourceType)index);	//���û�����Դ����
			});

			//��պ�δ����
			if (!environment.GetSkyboxEnable()) {
				UI::DrawColorEditor3("Ambient Color", glm::value_ptr(environment.GetAmbientColor()));	//AmbientColor��������ɫ ��ɫ�༭��
			}
			else {
				//������Դ ��պУ������ӹ�ԴΪ��ɫ�Ļ����⣩
				if (environment.GetSourceType() == Environment::SourceType::Skybox) {
					UI::DrawSlider("Intensity Multiplier", &environment.GetIntensityMultiplier_Ref(), 0.0f, 8.0f, UI::ValueType::Float);	//��ǿ�ȱ��� ������
				}
				//������ԴΪ ��ɫ
				if (environment.GetSourceType() == Environment::SourceType::Color) {
					UI::DrawColorEditor3("Ambient Color", glm::value_ptr(environment.GetAmbientColor()));	//AmbientColor��������ɫ ��ɫ�༭��
				}
			}

		});

		ImGui::End();	//Environment
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
			//TODO:���չ������
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

		ImGui::PushID(label.c_str());	//���ÿؼ�ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����

		//��ǩ��
		ImGui::Columns(2, 0, false);	//����Ϊ ���� id �߽�0
		ImGui::SetColumnWidth(0, columnWidth);	//����0���п�
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, panelWidth - columnWidth - 105);	//����3���п�
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 });	//Var Item�ո�

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//�и� = �����С + �߿�.y * 2
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };	//��ť��С

		//X����UI
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });	//Var Item�ո�
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));		//��ť��ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f));	//�����ͣ�ڰ�ťʱ����ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));	//��ť������ɫ
		ImGui::PushFont(boldFont);					//���ô���
		if (ImGui::Button("X", buttonSize)) {		//X��ť����
			values.x = resetValue;					//����x����
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);	//��ť��ɫ
		ImGui::SameLine();							//��ͬһ��
		ImGui::PopStyleVar();											//Var Item�ո�

		ImGui::DragFloat("##X", &values.x, 0.01f);	//X������ ##����ʾ��ǩ �϶�����0.01
		ImGui::PopItemWidth();						//�Ƴ���һ���п�
		ImGui::SameLine();
		

		//Y����UI
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });	//Var Item�ո�
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));			//��ť��ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));	//�����ͣ�ڰ�ťʱ����ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));	//��ť������ɫ
		ImGui::PushFont(boldFont);					//���ô���
		if (ImGui::Button("Y", buttonSize)) {		//Y��ť����
			values.y = resetValue;					//����Y����
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);	//��ť��ɫ
		ImGui::SameLine();							//��ͬһ��
		ImGui::PopStyleVar();											//Var Item�ո�

		ImGui::DragFloat("##Y", &values.y, 0.01f);	//Y������ ##����ʾ��ǩ �϶�����0.01
		ImGui::PopItemWidth();						//�Ƴ���һ���п�
		ImGui::SameLine();

		//Z����UI
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });	//Var Item�ո�
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));		//��ť��ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));	//�����ͣ�ڰ�ťʱ����ɫ
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f));	//��ť������ɫ
		ImGui::PushFont(boldFont);					//���ô���
		if (ImGui::Button("Z", buttonSize)) {		//Z��ť����
			values.z = resetValue;					//����Z����
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);	//��ť��ɫ
		ImGui::SameLine();							//��ͬһ��
		ImGui::PopStyleVar();											//Var Item�ո�

		ImGui::DragFloat("##Z", &values.z, 0.01f);	//Z������ ##����ʾ��ǩ �϶�����0.01
		ImGui::PopItemWidth();						//�Ƴ���һ���п�

		ImGui::PopStyleVar();

		ImGui::Columns(1);	//����Ϊ1��

		ImGui::PopID();	//�����ؼ���ǩ
	}

	/// <summary>
	/// �������
	/// </summary>
	/// <typeparam name="T">�������</typeparam>
	/// <typeparam name="UIFunction">������ܺ�������</typeparam>
	/// <param name="name">�����</param>
	/// <param name="entity">ʵ��</param>
	/// <param name="enable">�Ƿ�����</param>
	/// <param name="uiFunction">������ܺ���</param>
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Object object, bool enable, UIFunction uiFunction)
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
			
			ImGui::SameLine(lineHeight * 1.3f);
			ImGui::SetCursorPos(ImVec2(nodePos.x + 28, nodePos.y + 4));	//����Checkboxλ��
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.1, 0.1));	
			ImGui::Checkbox("##Checkbox", &enable);	//������ù�ѡ��
			ImGui::SameLine();
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(ImVec2(nodePos.x + 54, nodePos.y));	//����Textλ��
			ImGui::PushFont(boldFont);	//���ý���ı�����Ϊ����
			ImGui::Text(name.c_str());	//�����
			ImGui::PopFont();

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);	//ͬһ�У������иߵ�һ��
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

	void SceneHierarchyPanel::DrawComponents(Object object)
	{
		bool enable = true;
		ImGui::Checkbox("##Enable Object", &enable);	//�Ƿ����ø�Object��TODO:δʵ��
		ImGui::SameLine();

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����
		ImGui::PushItemWidth(panelWidth - 150);
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
		ImGui::PopItemWidth();

		ImGui::SameLine();
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

		ImGui::Separator();	//�ָ���

		//����Transform���
		DrawComponent<Transform>("Transform", object, true, [](Transform& transform)
		{
			DrawVec3Control("Position", transform.GetPosition());	//λ��
			DrawVec3Control("Rotation", transform.GetRotation());	//��ת
			DrawVec3Control("Scale", transform.GetScale(), 1.0f);	//���ţ�Ĭ��ֵ1.0f
		});

		//����Light���
		DrawComponent<Light>("Light", object, true, [](Light& light)
		{
			const char* types[] = { "Directional", "Point", "Spot"};	//��Դ���ͣ�͸�� ���� 
			const char* currentType = types[(int)light.GetType()];		//��ǰ��Դ����

			//Type��Դ����ѡ�������б�
			UI::DrawDropdownList("Type", currentType, types, 3, [&](int index, const char* value)
			{
				light.SetType((Light::Type)index);	//���ù�Դ����
			});

			//���Դ | �۹�Դ
			if (light.GetType() == Light::Type::Point || light.GetType() == Light::Type::Spot) {
				UI::DrawDrag("Range", &light.GetRange_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1000.0f);	//Range���հ뾶 �϶���
			}

			//�۹�Դ
			if (light.GetType() == Light::Type::Spot) {
				UI::DrawSlider("Spot Angle", &light.GetSpotOuterAngle_Ref(), 1.0f, 179.0f, UI::ValueType::Angle);	//SpotAngle���Ž� ������
				light.SetSpotInnerAngle();	//�������Ž� = Outer - Outer / 12
			}

			UI::DrawColorEditor3("Color", glm::value_ptr(light.GetColor()));									//Color�ƹ���ɫ ��ɫ�༭��
			UI::DrawDrag("Intensity", &light.GetIntensity_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1000.0f);	//Intensity����ǿ�� �϶���
			UI::DrawCheckBox("Cast Shadow", &light.GetCastShadow_Ref());										//CastShadow�Ƿ�Ͷ����Ӱ ��ѡ��
		});

		//����Camera���
		DrawComponent<Camera>("Camera", object, true, [](Camera& camera)
		{
			UI::DrawCheckBox("Main Camera", &camera.IsPrimary_Ref());	//MainCamera�Ƿ�������� ��ѡ��

			const char* clearFlags[] = { "Color", "Skybox" };						//������־����ɫ ��պ� 
			const char* currentClearFlag = clearFlags[(int)camera.GetClearFlag()];	//��ǰ������־

			//Clear Flags������־ѡ�������б�
			UI::DrawDropdownList("Clear Flags", currentClearFlag, clearFlags, 2, [&](int index, const char* value)
			{
				camera.SetClearFlag((Camera::ClearFlag)index);	//�������������־
			});

			UI::DrawColorEditor4("Background", glm::value_ptr(camera.GetBackgroundColor()));	//Background������ɫ ��ɫ�༭��

			const char* projectionTypes[] = { "Perspective", "Orthographic" };						//ͶӰ���ͣ�͸�� ���� 
			const char* currentProjectionType = projectionTypes[(int)camera.GetProjectionType()];	//��ǰͶӰ����

			//ProjectionͶӰ����ѡ�������б�
			UI::DrawDropdownList("Projection", currentProjectionType, projectionTypes, 2, [&](int index, const char* value)
			{
				camera.SetProjectionType((Camera::ProjectionType)index);	//�������ͶӰ����
			});

			//͸��ͶӰ
			if (camera.GetProjectionType() == Camera::ProjectionType::Perspective) {	
				UI::DrawSlider("Vertical FOV", &camera.GetFOV_Ref(), 1.0f, 179.0f, UI::ValueType::Angle);	//FOV��ֱ�Ž� ������
			}

			//����ͶӰ
			if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {	
				UI::DrawDrag("Size", &camera.GetSize_Ref());	//Size�ߴ� �϶���
			}

			UI::DrawDrag("Near", &camera.GetNearClip_Ref(), 0.01f, UI::ValueType::Float, 0.01f, camera.GetFarClip() - 0.01f);	//Near���ü�ƽ�� �϶���
			UI::DrawDrag("Far", &camera.GetFarClip_Ref(), 0.01f, UI::ValueType::Float, camera.GetNearClip() + 0.01f, 1000.0f);	//FarԶ�ü�ƽ�� �϶���
		});

		//����Mesh���
		DrawComponent<Mesh>("Mesh", object, true, [](Mesh& mesh)
		{
			const char* meshTypes[] = { "None (Mesh)", "Other", "Cube", "Sphere", "Capsule", "Cylinder", "Plane"};
			const char* currentMeshType = meshTypes[(int)mesh.GetType()];	//��ǰ��������

			//Mesh����ѡ�������б�
			UI::DrawDropdownList("Mesh", currentMeshType, meshTypes, 7, [&](int index, const char* value)
			{
				mesh.SetType((Mesh::Type)index);	//������������
			});
			//TODO��if Type == Other then ��Ӱ�ť ���ļ�����Mesh
		});

		//����Material���
		DrawComponent<Material>("Material", object, true, [](Material& material)
		{
			uint32_t count = ShaderLibrary::GetSize();

			const char* shaderNames[2];
			const char* currentShaderName = "";
			int i = 0;
			//������ɫ����
			for (auto& [name, shader] : ShaderLibrary::GetShaders()) {
				if (name == material.GetShader()->GetName()) {
					currentShaderName = name.c_str();	//��ǰ��ɫ����
				}
				shaderNames[i++] = name.c_str();
			}

			//Shader��ɫ��ѡ�������б�
			UI::DrawDropdownList("Shader", currentShaderName, shaderNames, count, [&](int index, const char* value)
			{
				material.SetShader(ShaderLibrary::GetShaders()[value]);	//������ɫ��
			});

			if (material.GetShader()->GetName() == "Standard") {	//��׼��ɫ��
				
				UI::DrawColorEditor4("Color", glm::value_ptr(material.GetColor()));			//Color������ɫ ��ɫ�༭��

				//Albedo Map ��������ͼ ѡ��&Ԥ����ť
				uint32_t albedoTextureID = material.GetAlbedoTextureID();	//Albedo��ͼID
				UI::DrawImageButton("Albedo Map", albedoTextureID, { 100,100 }, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Albedo Texture(*.png;*.jpg)\0*.png;*.jpg\0");		//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						material.SetAlbedoTexture(filepath);	//����Albedo��ͼ
					}
				});

				//Specular Map ��������ͼ ѡ��&Ԥ����ť
				uint32_t specularTextureID = material.GetSpecularTextureID();	//Specular��ͼID
				UI::DrawImageButton("Specular Map", specularTextureID, { 100,100 }, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Specular Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						material.SetSpecularTexture(filepath);	//����Specular��ͼ
					}
				});

				UI::DrawSlider("Shininess", &material.GetShininess_Ref(), 1.0f, 648.0f);					//Shininess����� ������
				UI::DrawDrag("Tiling", glm::value_ptr(material.GetTiling()), 0.01f, UI::ValueType::Float2);	//Tiling����ƽ������ �϶���
				UI::DrawDrag("Offset", glm::value_ptr(material.GetOffset()), 0.01f, UI::ValueType::Float2);	//Offset����ƫ���� �϶���
			}
		});

		//����SpriteRenderer���
		DrawComponent<SpriteRenderer>("Sprite Renderer", object, true, [](SpriteRenderer& spriteRenderer)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.m_Color));
		});
	}
}