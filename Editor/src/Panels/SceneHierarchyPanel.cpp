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
#include "Explorer/Components/SpriteRenderer.h"
#include "Explorer/Components/CircleRenderer.h"
#include "Explorer/Components/Rigidbody/Rigidbody2D.h"
#include "Explorer/Components/Rigidbody/BoxCollider2D.h"
#include "Explorer/Components/Rigidbody/CircleCollider2D.h"

#include "Explorer/Utils/PlatformUtils.h"
#include "Explorer/Utils/ModelImporter.h"

#include "Explorer/ImGui/UI.h"

#include "ContentBrowserPanel.h"

namespace Explorer
{
	extern const std::filesystem::path g_AssetPath;	//�ʲ�Ŀ¼��ȫ�֣�

	//TODO:��������д��ͻ
	//static std::shared_ptr<Texture2D> s_SettingsIcon = std::make_shared<Texture2D>("assets/textures/defaults/Icons/Buttons/ComponentSettings_Icon.png");

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

		//��������
		if (m_Scene) {
			//Scene�������ڵ�
			UI::DrawTreeNode<Scene>(m_Scene->GetName(), true, [&](float lineHeight)
			{
				//������������ʵ�壬������each�ڵĺ���
				m_Scene->m_Registry.each([&](auto objectID)
					{
						Object object{ objectID, m_Scene.get() };
						DrawObjectNode(object);		//����������
					});
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

				//���˵��2D����
				if (ImGui::BeginMenu("2D Object"))
				{
					if (ImGui::MenuItem("Sprite")) {
						object = m_Scene->CreateSpriteObject();	//����Sprite
					}
					if (ImGui::MenuItem("Circle")) {
						object = m_Scene->CreateCircleObject();	//����Circle
					}

					ImGui::EndMenu();
				}

				//���˵��3D����
				if (ImGui::BeginMenu("3D Object"))
				{
					if (ImGui::MenuItem("Cube")) {		//�Ӳ˵������Cube
						object = m_Scene->CreateMeshObject("Cube", Mesh::Type::Cube);
					}
					if (ImGui::MenuItem("Plane")) {		//�Ӳ˵������Plane
						object = m_Scene->CreateMeshObject("Plane", Mesh::Type::Plane);
					}
					if (ImGui::MenuItem("Cone")) {		//�Ӳ˵������Cone
						object = m_Scene->CreateMeshObject("Cone", Mesh::Type::Cone);
					}
					if (ImGui::MenuItem("Cylinder")) {	//�Ӳ˵������Cylinder
						object = m_Scene->CreateMeshObject("Cylinder", Mesh::Type::Cylinder);
					}
					if (ImGui::MenuItem("Sphere")) {	//�Ӳ˵������Sphere
						object = m_Scene->CreateMeshObject("Sphere", Mesh::Type::Sphere);
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

				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Right(+x)��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Right);	//����RightԤ����ͼ
				});

				//Left(-x)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Left [-X]", leftTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Left[-X] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Left);	//����LeftԤ����ͼ
					}
				});

				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Left(-x)��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Left);	//����LeftԤ����ͼ
				});

				//Up(+y)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Up [+Y]", upTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Up[+Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Up);	//����UpԤ����ͼ
					}
				});

				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Up(+y)��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Up);	//����UpԤ����ͼ
				});

				//Down(-y)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Down [-Y]", downTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Down[-Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Down);	//����DownԤ����ͼ
					}
				});

				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Down(-y)��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Down);	//����DownԤ����ͼ
				});

				//Front(+z)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Front [+Z]", frontTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Front[+Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Front);	//����FrontԤ����ͼ
					}
				});

				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Front(+z)��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Front);	//����FrontԤ����ͼ
				});

				//Back(-z)��պ���ͼ ѡ��&Ԥ����ť
				UI::DrawImageButton("Back [-Z]", backTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Back[-Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Back);	//����BackԤ����ͼ
					}
				});

				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Back(-z)��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Back);	//����BackԤ����ͼ
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
		auto& name = object.GetComponent<Self>().GetObjectName();	//������
		
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
			//TODO:���չ������ �����ӽڵ�
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
	/// <param name="entity">ʵ��</param>
	/// <param name="uiFunction">������ܺ���</param>
	template<typename T, typename UIFunction>
	static void DrawComponent(Object object, UIFunction uiFunction)
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

			//������ð�ťͼ��ID
			//uint32_t settingsIconID = s_SettingsIcon->GetRendererID();
			ImGui::SetCursorPos(ImVec2(nodePos.x + ImGui::GetWindowContentRegionWidth() - lineHeight, nodePos.y));	//����λ��
			//ImGui::Image((void*)settingsIconID, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {	//������ð�ť TODO:����Image
				ImGui::OpenPopup("ComponentSettings");	//�򿪵�����
			}

			//�Ƴ����
			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings")) {	//��Ⱦ������
				bool disableRemove = false;	//�����Ƴ�
				//Transform �� Material�����Ƴ�
				if (component.GetName() == "Transform" || component.GetName() == "Material") {
					disableRemove = true;
				}

				//�Ƴ�����˵���
				if (ImGui::MenuItem("Remove Component","",false, !disableRemove)) {
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
		//Self���
		if (object.HasComponent<Self>()) {
			auto& self = object.GetComponent<Self>();	//Self���

			ImGui::Checkbox("##Enable Object", &self.GetObjectEnable_Ref());	//�Ƿ����ø�Object ��ѡ��
			ImGui::SameLine();

			float panelWidth = ImGui::GetWindowContentRegionWidth();	//�����
			ImGui::PushItemWidth(panelWidth - 150);

			auto& name = self.GetObjectName();	//������

			char buffer[256];								//���������buffer
			memset(buffer, 0, sizeof(buffer));				//��buffer����
			strcpy_s(buffer, sizeof(buffer), name.c_str());	//buffer = name

			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];	//���壺0������

			ImGui::PushFont(boldFont);	//���ý���ı�����Ϊ����
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {	//�����������������ݸı�ʱ
				name = std::string(buffer);								//����name��Ϊbuffer
			}
			ImGui::PopFont();

			ImGui::PopItemWidth();
		}

		ImGui::SameLine();
		//��������ť
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");	//�򿪵����� ������
		}
		
		//TODO:��������⣬�������������������������������
		if (ImGui::BeginPopup("AddComponent")) {	//��Ⱦ������
			//Camera���������
			if (!m_SelectionObject.HasComponent<Camera>()) {
				//���Camera���
				if (ImGui::MenuItem("Camera")) {
					m_SelectionObject.AddComponent<Camera>();
					ImGui::CloseCurrentPopup();
				}
			}
			//Light���������
			if (!m_SelectionObject.HasComponent<Light>()) {
				//���Light���
				if (ImGui::MenuItem("Light")) {
					m_SelectionObject.AddComponent<Light>();
					ImGui::CloseCurrentPopup();
				}
			}
			//Mesh���������
			if (!m_SelectionObject.HasComponent<Mesh>()) {
				//���Mesh���
				if (ImGui::MenuItem("Mesh")) {
					m_SelectionObject.AddComponent<Mesh>(Mesh::Type::None);
					ImGui::CloseCurrentPopup();
				}
			}
			//Material���������
			if (!m_SelectionObject.HasComponent<Material>()) {
				//���Material���
				if (ImGui::MenuItem("Material")) {
					m_SelectionObject.AddComponent<Material>();
					ImGui::CloseCurrentPopup();
				}
			}
			//SpriteRenderer���������
			if (!m_SelectionObject.HasComponent<SpriteRenderer>()) {
				//���SpriteRenderer���
				if (ImGui::MenuItem("Sprite Renderer")) {
					m_SelectionObject.AddComponent<SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}
			}
			//CircleRenderer���������
			if (!m_SelectionObject.HasComponent<CircleRenderer>()) {
				//���CircleRenderer���
				if (ImGui::MenuItem("Circle Renderer")) {
					m_SelectionObject.AddComponent<CircleRenderer>();
					ImGui::CloseCurrentPopup();
				}
			}
			//Rigidbody2D���������
			if (!m_SelectionObject.HasComponent<Rigidbody2D>()) {
				//���Rigidbody2D���
				if (ImGui::MenuItem("Rigidbody 2D")) {
					m_SelectionObject.AddComponent<Rigidbody2D>();
					ImGui::CloseCurrentPopup();
				}
			}
			//BoxCollider2D���������
			if (!m_SelectionObject.HasComponent<BoxCollider2D>()) {
				//���BoxCollider2D���
				if (ImGui::MenuItem("Box Collider 2D")) {
					m_SelectionObject.AddComponent<BoxCollider2D>();
					ImGui::CloseCurrentPopup();
				}
			}
			//CircleCollider2D���������
			if (!m_SelectionObject.HasComponent<CircleCollider2D>()) {
				//���CircleCollider2D���
				if (ImGui::MenuItem("Circle Collider 2D")) {
					m_SelectionObject.AddComponent<CircleCollider2D>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();	//�ָ���

		//����Transform���
		DrawComponent<Transform>(object, [](Transform& transform)
		{
			DrawVec3Control("Position", transform.GetPosition());	//λ��
			DrawVec3Control("Rotation", transform.GetRotation());	//��ת
			DrawVec3Control("Scale", transform.GetScale(), 1.0f);	//���ţ�Ĭ��ֵ1.0f
		});

		//����Light���
		DrawComponent<Light>(object, [](Light& light)
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
		DrawComponent<Camera>(object, [](Camera& camera)
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
		DrawComponent<Mesh>(object, [](Mesh& mesh)
		{
			//Mesh����ȥ��'(Mesh)'
			std::string meshName = mesh.GetName() != "[None] (Mesh)" ? mesh.GetName().substr(0, mesh.GetName().find(' ')) : "None (Mesh)";

			//Meshѡ��&��ʾ��ť
			UI::DrawButton("Mesh", meshName, [&]()
			{
				std::string filepath = FileDialogs::OpenFile("Wavefront(*.obj)\0*.obj\0");	//���ļ��Ի��� .obj�ļ�
				//·����Ϊ��
				if (!filepath.empty()) {
					ModelImporter::Load(filepath);		//�����ļ�

					auto& m = ModelImporter::GetMesh();	//Mesh

					m.SetType(Mesh::Type::Other);								//Mesh����
					m.SetName(std::filesystem::path(filepath).stem().string());	//Mesh��

					mesh = m;
				}
			});

			//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Mesh�ļ���
			ContentBrowserPanel::DragDropToTarget({ ".obj", ".mesh"}, [&](const std::filesystem::path& filepath)
			{
				ModelImporter::Load(filepath.string());	//�����ļ�

				auto& m = ModelImporter::GetMesh();		//Mesh
				
				m.SetType(Mesh::Type::Other);			//Mesh����
				m.SetName(filepath.stem().string());	//Mesh��

				mesh = m;
			});
		});

		//����Material���
		DrawComponent<Material>(object, [](Material& material)
		{
			uint32_t count = ShaderLibrary::GetSize();

			const char* shaderNames[5]; //TODO����shader��
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
				
				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨��������ͼ��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".png", ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					material.SetAlbedoTexture(filepath.string());	//����Albedo��ͼ
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

				//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨�߹���ͼ��	���ļ�·��
				ContentBrowserPanel::DragDropToTarget({ ".png", ".jpg"}, [&](const std::filesystem::path& filepath)
				{
					material.SetSpecularTexture(filepath.string());	//����Albedo��ͼ
				});

				UI::DrawSlider("Shininess", &material.GetShininess_Ref(), 1.0f, 648.0f);					//Shininess����� ������
				UI::DrawDrag("Tiling", glm::value_ptr(material.GetTiling()), 0.01f, UI::ValueType::Float2);	//Tiling����ƽ������ �϶���
				UI::DrawDrag("Offset", glm::value_ptr(material.GetOffset()), 0.01f, UI::ValueType::Float2);	//Offset����ƫ���� �϶���
			}
		});

		//����SpriteRenderer���
		DrawComponent<SpriteRenderer>(object, [](SpriteRenderer& spriteRenderer)
		{
			UI::DrawColorEditor4("Color", glm::value_ptr(spriteRenderer.GetColor()));	//Color Sprite��ɫ ��ɫ�༭��

			//Sprite��ͼ ѡ��&Ԥ����ť
			uint32_t spriteID = spriteRenderer.GetSprite().GetTexture()->GetRendererID();
			UI::DrawImageButton("Sprite", spriteID, { 100,100 }, [&]()
			{
				std::string filepath = FileDialogs::OpenFile("Sprite Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//���ļ��Ի���.png|.jpg
				if (!filepath.empty()) {
					spriteRenderer.GetSprite().SetTexture(filepath);	//����Sprite��Texture
				}
			});

			//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨Sprite��	���ļ�·��
			ContentBrowserPanel::DragDropToTarget({ ".png", ".jpg" }, [&](const std::filesystem::path& filepath)
			{
				spriteRenderer.GetSprite().SetTexture(filepath.string());	//����Sprite��Texture
			});
		});

		//����CircleRenderer���
		DrawComponent<CircleRenderer>(object, [](CircleRenderer& circleRenderer)
		{
			Circle& circle = circleRenderer.GetCircle();

			UI::DrawColorEditor4("Color", glm::value_ptr(circle.GetColor()));								//Color��ɫ ��ɫ�༭��
			UI::DrawDrag("Thickness", &circle.GetThickness_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//Thickness��� vec1�϶���
			UI::DrawDrag("Fade", &circle.GetFade_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);			//Fadeģ���� vec1�϶���
		});

		//����Rigidbody2D���
		DrawComponent<Rigidbody2D>(object, [](Rigidbody2D& rigidbody2D)
		{
			const char* bodyTypes[] = { "Dynamic", "Static", "Kinematic" };				//�������ͣ���̬ ��̬ ����ѧ 
			const char* currentBodyType = bodyTypes[(int)rigidbody2D.GetBodyType()];	//��ǰ��������

			//��������ѡ�������б�
			UI::DrawDropdownList("Body Type", currentBodyType, bodyTypes, 3, [&](int index, const char* value)
			{
				rigidbody2D.SetBodyType((Rigidbody2D::BodyType)index);	//���ø�������
			});

			UI::DrawCheckBox("Freeze Rotation", &rigidbody2D.GetFreezeRotation_Ref());	//�Ƿ񶳽���תZ�� ��ѡ��
		});

		//����BoxCollider2D���
		DrawComponent<BoxCollider2D>(object, [](BoxCollider2D& boxCollider2D)
		{
			UI::DrawCheckBox("Render Box", &boxCollider2D.GetRenderBox_Ref());	//�Ƿ���ȾBox
			UI::DrawDrag("Offset", glm::value_ptr(boxCollider2D.GetOffset()), 0.01f, UI::ValueType::Float2);	//Offsetƫ���� vec2�϶���
			UI::DrawDrag("Size", glm::value_ptr(boxCollider2D.GetSize()), 0.01f, UI::ValueType::Float2, 0.0f);	//Size��С vec2�϶���
			//TODO������ֵ
			UI::DrawDrag("Density", &boxCollider2D.GetDensity_Ref());													//Density�ܶ� vec1�϶���
			UI::DrawDrag("Friction", &boxCollider2D.GetFriction_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);		//FrictionĦ���� vec1�϶���
			UI::DrawDrag("Restitution", &boxCollider2D.GetRestitution_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//Restitution�ָ�ϵ�� vec1�϶���
			UI::DrawDrag("Restitution Threshold", &boxCollider2D.GetRestitutionThreshold_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//RestitutionThreshold�ָ���ֵ vec1�϶���
		});
		
		//����CircleCollider2D���
		DrawComponent<CircleCollider2D>(object, [](CircleCollider2D& circleCollider2D)
		{
			UI::DrawCheckBox("Render Circle", &circleCollider2D.GetRenderCircle_Ref());	//�Ƿ���ȾCircle
			UI::DrawDrag("Offset", glm::value_ptr(circleCollider2D.GetOffset()), 0.01f, UI::ValueType::Float2);	//Offsetƫ���� vec2�϶���
			UI::DrawDrag("Radius", &circleCollider2D.GetRadius_Ref(), 0.01f, UI::ValueType::Float, 0.0f);		//Radius�뾶 vec1�϶���
			//TODO������ֵ
			UI::DrawDrag("Density", &circleCollider2D.GetDensity_Ref());													//Density�ܶ� vec1�϶���
			UI::DrawDrag("Friction", &circleCollider2D.GetFriction_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);			//FrictionĦ���� vec1�϶���
			UI::DrawDrag("Restitution", &circleCollider2D.GetRestitution_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//Restitution�ָ�ϵ�� vec1�϶���
			UI::DrawDrag("Restitution Threshold", &circleCollider2D.GetRestitutionThreshold_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//RestitutionThreshold�ָ���ֵ vec1�϶���
		});
	}
}