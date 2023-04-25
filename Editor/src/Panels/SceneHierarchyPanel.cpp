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
	extern const std::filesystem::path g_AssetPath;	//资产目录（全局）

	//TODO:纹理创建读写冲突
	//static std::shared_ptr<Texture2D> s_SettingsIcon = std::make_shared<Texture2D>("assets/textures/defaults/Icons/Buttons/ComponentSettings_Icon.png");

	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
	{
		SetScene(scene);
	}
	
	void SceneHierarchyPanel::SetScene(const std::shared_ptr<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectionObject = {};	//置空旧场景被选中物体
	}
	
	void SceneHierarchyPanel::OnImGuiRender()
	{
		//Hierarchy面板
		ImGui::Begin("Hierarchy");

		//场景存在
		if (m_Scene) {
			//Scene场景根节点
			UI::DrawTreeNode<Scene>(m_Scene->GetName(), true, [&](float lineHeight)
			{
				//遍历场景所有实体，并调用each内的函数
				m_Scene->m_Registry.each([&](auto objectID)
					{
						Object object{ objectID, m_Scene.get() };
						DrawObjectNode(object);		//绘制物体结点
					});
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {	//鼠标悬停在该窗口 && 点击鼠标 （点击空白位置）
				m_SelectionObject = {};	//取消选中：置空选中物体
			}

			//创建物体
			if (ImGui::BeginPopupContextWindow(0, 1, false)) {	//右键点击窗口白区域弹出菜单：- 右键 不在物体项上
				Object object;

				if (ImGui::MenuItem("Create Empty")) {		//菜单项：创建空物体
					object = m_Scene->CreateEmptyObject("Object");	//创建空物体
				}

				//父菜单项：2D物体
				if (ImGui::BeginMenu("2D Object"))
				{
					if (ImGui::MenuItem("Sprite")) {
						object = m_Scene->CreateSpriteObject();	//创建Sprite
					}
					if (ImGui::MenuItem("Circle")) {
						object = m_Scene->CreateCircleObject();	//创建Circle
					}

					ImGui::EndMenu();
				}

				//父菜单项：3D物体
				if (ImGui::BeginMenu("3D Object"))
				{
					if (ImGui::MenuItem("Cube")) {		//子菜单项：创建Cube
						object = m_Scene->CreateMeshObject("Cube", Mesh::Type::Cube);
					}
					if (ImGui::MenuItem("Plane")) {		//子菜单项：创建Plane
						object = m_Scene->CreateMeshObject("Plane", Mesh::Type::Plane);
					}
					if (ImGui::MenuItem("Cone")) {		//子菜单项：创建Cone
						object = m_Scene->CreateMeshObject("Cone", Mesh::Type::Cone);
					}
					if (ImGui::MenuItem("Cylinder")) {	//子菜单项：创建Cylinder
						object = m_Scene->CreateMeshObject("Cylinder", Mesh::Type::Cylinder);
					}
					if (ImGui::MenuItem("Sphere")) {	//子菜单项：创建Sphere
						object = m_Scene->CreateMeshObject("Sphere", Mesh::Type::Sphere);
					}
					ImGui::EndMenu();
				}

				//父菜单项：光源
				if (ImGui::BeginMenu("Light"))
				{
					if (ImGui::MenuItem("Directional Light")) {		//子菜单项：创建平行光源
						object = m_Scene->CreateLightObject(Light::Type::Directional);
					}
					if (ImGui::MenuItem("Point Light")) {			//子菜单项：创建点光源
						object = m_Scene->CreateLightObject(Light::Type::Point);
					}
					if (ImGui::MenuItem("Spot Light")) {			//子菜单项：创建聚光源
						object = m_Scene->CreateLightObject(Light::Type::Spot);
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Camera")) {		//菜单项：创建相机
					object = m_Scene->CreateCameraObject();
				}

				m_SelectionObject = object;	//选中物体设为新创建物体

				ImGui::EndPopup();	//结束弹出菜单
			}
		}

		ImGui::End();	//Hierarchy

		//属性面板
		ImGui::Begin("Properties");
		if (m_SelectionObject) {				//被选中的物体存在
			DrawComponents(m_SelectionObject);	//绘制组件
		}
		ImGui::End();	//Properties

		//环境参数设置面板
		ImGui::Begin("Environment");

		Environment& environment = m_Scene->GetEnvironment();	//场景环境
		Skybox& skybox = environment.GetSkybox();	//天空盒

		UI::DrawCheckBox("Enable Skybox", &environment.GetSkyboxEnable_Ref());	//Enable Skybox是否启用天空盒 勾选框

		//Skybox天空盒设置 树节点
		UI::DrawTreeNode<Environment>(skybox.GetShader()->GetName() + "(Skybox)", true, [&](float lineHeight)
		{
			UI::DrawColorEditor3("Tint Color", glm::value_ptr(skybox.GetTintColor()));					//Tint Color色调 颜色编辑器
			UI::DrawSlider("Expose", &skybox.GetExpose_Ref(), 0.0f, 8.0f);								//Expose曝光度 滑动条
			UI::DrawSlider("Rotation", &skybox.GetRotation_Ref(), 0.0f, 360.0f, UI::ValueType::Angle);	//Rotation旋转值z 滑动条

			uint32_t rightTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Right]->GetRendererID();	//Right预览贴图ID
			uint32_t leftTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Left]->GetRendererID();	//Left预览贴图ID
			uint32_t upTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Up]->GetRendererID();		//Up预览贴图ID
			uint32_t downTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Down]->GetRendererID();	//Down预览贴图ID
			uint32_t frontTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Front]->GetRendererID();	//Front预览贴图ID
			uint32_t backTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Back]->GetRendererID();	//Back预览贴图ID

			//Cubemap贴图设置 树节点
			UI::DrawTreeNode<Cubemap>("Cubemap Settings", false, [&](float lineHeight)
			{
				glm::vec2 textureButtonSize = { 50, 50 };

				//Right(+x)天空盒贴图 选择&预览按钮
				UI::DrawImageButton("Right [+X]", rightTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Right[+X] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Right);	//设置Right预览贴图
					}
				});

				//将从拖拽源（Project面板）复制的数据拖放到目标（Right(+x)）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Right);	//设置Right预览贴图
				});

				//Left(-x)天空盒贴图 选择&预览按钮
				UI::DrawImageButton("Left [-X]", leftTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Left[-X] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Left);	//设置Left预览贴图
					}
				});

				//将从拖拽源（Project面板）复制的数据拖放到目标（Left(-x)）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Left);	//设置Left预览贴图
				});

				//Up(+y)天空盒贴图 选择&预览按钮
				UI::DrawImageButton("Up [+Y]", upTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Up[+Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Up);	//设置Up预览贴图
					}
				});

				//将从拖拽源（Project面板）复制的数据拖放到目标（Up(+y)）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Up);	//设置Up预览贴图
				});

				//Down(-y)天空盒贴图 选择&预览按钮
				UI::DrawImageButton("Down [-Y]", downTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Down[-Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Down);	//设置Down预览贴图
					}
				});

				//将从拖拽源（Project面板）复制的数据拖放到目标（Down(-y)）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Down);	//设置Down预览贴图
				});

				//Front(+z)天空盒贴图 选择&预览按钮
				UI::DrawImageButton("Front [+Z]", frontTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Front[+Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Front);	//设置Front预览贴图
					}
				});

				//将从拖拽源（Project面板）复制的数据拖放到目标（Front(+z)）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Front);	//设置Front预览贴图
				});

				//Back(-z)天空盒贴图 选择&预览按钮
				UI::DrawImageButton("Back [-Z]", backTextureID, textureButtonSize, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Back[-Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Back);	//设置Back预览贴图
					}
				});

				//将从拖拽源（Project面板）复制的数据拖放到目标（Back(-z)）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Back);	//设置Back预览贴图
				});
			});

			//Cubemap预览图 树节点
			UI::DrawTreeNode<Cubemap>("Cubemap Preview", false, [&](float lineHeight)
			{
				ImVec2 size = ImVec2(90, 90);	//Cubemap单张大小
				int positionX = 35;				//Cubemap X位置

				//Cubemap预览图
				ImGui::SetCursorPosX(positionX);
				ImGui::BeginChild("Cubemap Preview", ImVec2(size.x * 4 + 40, size.y * 3 + 40), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				//Up [+Y]
				ImGui::SetCursorPosX(size.x);	//设置游标X位置
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

		//Environment Lighting环境光照设置 树节点
		UI::DrawTreeNode<Environment>("Environment Lighting", true, [&](float lineHeight)
		{
			const char* sourceTypes[] = { "Skybox","Color" };							//环境光源类型
			const char* currentSource = sourceTypes[(int)environment.GetSourceType()];	//当前环境光源类型

			//Source环境光源类型 选择下拉列表
			UI::DrawDropdownList("Source", currentSource, sourceTypes, 2, [&](int index, const char* value)
			{
				environment.SetSourceType((Environment::SourceType)index);	//设置环境光源类型
			});

			//天空盒未启用
			if (!environment.GetSkyboxEnable()) {
				UI::DrawColorEditor3("Ambient Color", glm::value_ptr(environment.GetAmbientColor()));	//AmbientColor环境光颜色 颜色编辑器
			}
			else {
				//环境光源 天空盒（不叠加光源为颜色的环境光）
				if (environment.GetSourceType() == Environment::SourceType::Skybox) {
					UI::DrawSlider("Intensity Multiplier", &environment.GetIntensityMultiplier_Ref(), 0.0f, 8.0f, UI::ValueType::Float);	//光强度倍数 滑动条
				}
				//环境光源为 颜色
				if (environment.GetSourceType() == Environment::SourceType::Color) {
					UI::DrawColorEditor3("Ambient Color", glm::value_ptr(environment.GetAmbientColor()));	//AmbientColor环境光颜色 颜色编辑器
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
		auto& name = object.GetComponent<Self>().GetObjectName();	//物体名
		
		//树结点标志（绘制的节点是否被选中 ？被选中的标志 ：0 | 单击箭头时打开）
		ImGuiTreeNodeFlags flags = (m_SelectionObject == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;	//水平延伸到边框
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());	//树节点：结点id 结点标志 结点名（实体名）
	
		if (ImGui::IsItemClicked()) {		//树结点被点击
			m_SelectionObject = object;		//Object被选中
		}

		//删除物体
		bool objectDeleted = false;	//物体是否已删除
		if (ImGui::BeginPopupContextItem()) {		//右键点击该物体结点
			if (ImGui::MenuItem("Delete")) {		//菜单项：删除物体
				objectDeleted = true;				//物体标记为已删除：渲染结束后面的UI 再删除该物体
			}
			ImGui::EndPopup();
		}

		if (opened) {			//树结点已打开
			//TODO:结点展开内容 物体子节点
			ImGui::TreePop();	//展开结点
		}

		if (objectDeleted) {
			m_Scene->DestroyObject(object);		//删除物体
			if (m_SelectionObject == object) {	//删除的物体为已选中物体
				m_SelectionObject = {};			//清空已选中物体
			}
		}
	}

	/// <summary>
	/// 绘制Vector3控件
	/// </summary>
	/// <param name="label">标签</param>
	/// <param name="values">值</param>
	/// <param name="resetValue">重置值</param>
	/// <param name="columnWidth">每列宽度</param>
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];	//粗体：0号字体

		ImGui::PushID(label.c_str());	//设置控件ID

		float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度

		//标签列
		ImGui::Columns(2, 0, false);	//设置为 两列 id 边界0
		ImGui::SetColumnWidth(0, columnWidth);	//设置0号列宽
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, panelWidth - columnWidth - 105);	//设置3个列宽
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 });	//Var Item空格

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;	//行高 = 字体大小 + 边框.y * 2
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };	//按钮大小

		//X分量UI
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });	//Var Item空格
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));		//按钮颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f));	//鼠标悬停在按钮时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));	//按钮按下颜色
		ImGui::PushFont(boldFont);					//设置粗体
		if (ImGui::Button("X", buttonSize)) {		//X按钮按下
			values.x = resetValue;					//重置x分量
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);	//按钮颜色
		ImGui::SameLine();							//在同一行
		ImGui::PopStyleVar();											//Var Item空格

		ImGui::DragFloat("##X", &values.x, 0.01f);	//X分量列 ##不显示标签 拖动精度0.01
		ImGui::PopItemWidth();						//推出第一个列宽
		ImGui::SameLine();
		

		//Y分量UI
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });	//Var Item空格
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));			//按钮颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));	//鼠标悬停在按钮时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));	//按钮按下颜色
		ImGui::PushFont(boldFont);					//设置粗体
		if (ImGui::Button("Y", buttonSize)) {		//Y按钮按下
			values.y = resetValue;					//重置Y分量
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);	//按钮颜色
		ImGui::SameLine();							//在同一行
		ImGui::PopStyleVar();											//Var Item空格

		ImGui::DragFloat("##Y", &values.y, 0.01f);	//Y分量列 ##不显示标签 拖动精度0.01
		ImGui::PopItemWidth();						//推出第一个列宽
		ImGui::SameLine();

		//Z分量UI
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });	//Var Item空格
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));		//按钮颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));	//鼠标悬停在按钮时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f));	//按钮按下颜色
		ImGui::PushFont(boldFont);					//设置粗体
		if (ImGui::Button("Z", buttonSize)) {		//Z按钮按下
			values.z = resetValue;					//重置Z分量
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);	//按钮颜色
		ImGui::SameLine();							//在同一行
		ImGui::PopStyleVar();											//Var Item空格

		ImGui::DragFloat("##Z", &values.z, 0.01f);	//Z分量列 ##不显示标签 拖动精度0.01
		ImGui::PopItemWidth();						//推出第一个列宽

		ImGui::PopStyleVar();

		ImGui::Columns(1);	//设置为1列

		ImGui::PopID();	//弹出控件标签
	}

	/// <summary>
	/// 绘制组件
	/// </summary>
	/// <typeparam name="T">组件类型</typeparam>
	/// <typeparam name="UIFunction">组件功能函数类型</typeparam>
	/// <param name="entity">实体</param>
	/// <param name="uiFunction">组件功能函数</param>
	template<typename T, typename UIFunction>
	static void DrawComponent(Object object, UIFunction uiFunction)
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

			//组件设置按钮图标ID
			//uint32_t settingsIconID = s_SettingsIcon->GetRendererID();
			ImGui::SetCursorPos(ImVec2(nodePos.x + ImGui::GetWindowContentRegionWidth() - lineHeight, nodePos.y));	//设置位置
			//ImGui::Image((void*)settingsIconID, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {	//组件设置按钮 TODO:设置Image
				ImGui::OpenPopup("ComponentSettings");	//打开弹出框
			}

			//移除组件
			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings")) {	//渲染弹出框
				bool disableRemove = false;	//禁用移除
				//Transform 和 Material不能移除
				if (component.GetName() == "Transform" || component.GetName() == "Material") {
					disableRemove = true;
				}

				//移除组件菜单项
				if (ImGui::MenuItem("Remove Component","",false, !disableRemove)) {
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

			ImGui::Separator();	//分隔符
		}
	}

	void SceneHierarchyPanel::DrawComponents(Object object)
	{
		//Self组件
		if (object.HasComponent<Self>()) {
			auto& self = object.GetComponent<Self>();	//Self组件

			ImGui::Checkbox("##Enable Object", &self.GetObjectEnable_Ref());	//是否启用该Object 勾选框
			ImGui::SameLine();

			float panelWidth = ImGui::GetWindowContentRegionWidth();	//面板宽度
			ImGui::PushItemWidth(panelWidth - 150);

			auto& name = self.GetObjectName();	//物体名

			char buffer[256];								//输入框内容buffer
			memset(buffer, 0, sizeof(buffer));				//将buffer置零
			strcpy_s(buffer, sizeof(buffer), name.c_str());	//buffer = name

			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];	//粗体：0号字体

			ImGui::PushFont(boldFont);	//设置结点文本字体为粗体
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {	//创建输入框，输入框内容改变时
				name = std::string(buffer);								//物体name设为buffer
			}
			ImGui::PopFont();

			ImGui::PopItemWidth();
		}

		ImGui::SameLine();
		//添加组件按钮
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");	//打开弹出框 添加组件
		}
		
		//TODO:设置组件库，保存已有组件，从组件库查找组件并添加
		if (ImGui::BeginPopup("AddComponent")) {	//渲染弹出框
			//Camera组件不存在
			if (!m_SelectionObject.HasComponent<Camera>()) {
				//添加Camera组件
				if (ImGui::MenuItem("Camera")) {
					m_SelectionObject.AddComponent<Camera>();
					ImGui::CloseCurrentPopup();
				}
			}
			//Light组件不存在
			if (!m_SelectionObject.HasComponent<Light>()) {
				//添加Light组件
				if (ImGui::MenuItem("Light")) {
					m_SelectionObject.AddComponent<Light>();
					ImGui::CloseCurrentPopup();
				}
			}
			//Mesh组件不存在
			if (!m_SelectionObject.HasComponent<Mesh>()) {
				//添加Mesh组件
				if (ImGui::MenuItem("Mesh")) {
					m_SelectionObject.AddComponent<Mesh>(Mesh::Type::None);
					ImGui::CloseCurrentPopup();
				}
			}
			//Material组件不存在
			if (!m_SelectionObject.HasComponent<Material>()) {
				//添加Material组件
				if (ImGui::MenuItem("Material")) {
					m_SelectionObject.AddComponent<Material>();
					ImGui::CloseCurrentPopup();
				}
			}
			//SpriteRenderer组件不存在
			if (!m_SelectionObject.HasComponent<SpriteRenderer>()) {
				//添加SpriteRenderer组件
				if (ImGui::MenuItem("Sprite Renderer")) {
					m_SelectionObject.AddComponent<SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}
			}
			//CircleRenderer组件不存在
			if (!m_SelectionObject.HasComponent<CircleRenderer>()) {
				//添加CircleRenderer组件
				if (ImGui::MenuItem("Circle Renderer")) {
					m_SelectionObject.AddComponent<CircleRenderer>();
					ImGui::CloseCurrentPopup();
				}
			}
			//Rigidbody2D组件不存在
			if (!m_SelectionObject.HasComponent<Rigidbody2D>()) {
				//添加Rigidbody2D组件
				if (ImGui::MenuItem("Rigidbody 2D")) {
					m_SelectionObject.AddComponent<Rigidbody2D>();
					ImGui::CloseCurrentPopup();
				}
			}
			//BoxCollider2D组件不存在
			if (!m_SelectionObject.HasComponent<BoxCollider2D>()) {
				//添加BoxCollider2D组件
				if (ImGui::MenuItem("Box Collider 2D")) {
					m_SelectionObject.AddComponent<BoxCollider2D>();
					ImGui::CloseCurrentPopup();
				}
			}
			//CircleCollider2D组件不存在
			if (!m_SelectionObject.HasComponent<CircleCollider2D>()) {
				//添加CircleCollider2D组件
				if (ImGui::MenuItem("Circle Collider 2D")) {
					m_SelectionObject.AddComponent<CircleCollider2D>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();	//分隔符

		//绘制Transform组件
		DrawComponent<Transform>(object, [](Transform& transform)
		{
			DrawVec3Control("Position", transform.GetPosition());	//位置
			DrawVec3Control("Rotation", transform.GetRotation());	//旋转
			DrawVec3Control("Scale", transform.GetScale(), 1.0f);	//缩放：默认值1.0f
		});

		//绘制Light组件
		DrawComponent<Light>(object, [](Light& light)
		{
			const char* types[] = { "Directional", "Point", "Spot"};	//光源类型：透视 正交 
			const char* currentType = types[(int)light.GetType()];		//当前光源类型

			//Type光源类型选择下拉列表
			UI::DrawDropdownList("Type", currentType, types, 3, [&](int index, const char* value)
			{
				light.SetType((Light::Type)index);	//设置光源类型
			});

			//点光源 | 聚光源
			if (light.GetType() == Light::Type::Point || light.GetType() == Light::Type::Spot) {
				UI::DrawDrag("Range", &light.GetRange_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1000.0f);	//Range光照半径 拖动条
			}

			//聚光源
			if (light.GetType() == Light::Type::Spot) {
				UI::DrawSlider("Spot Angle", &light.GetSpotOuterAngle_Ref(), 1.0f, 179.0f, UI::ValueType::Angle);	//SpotAngle外张角 滑动条
				light.SetSpotInnerAngle();	//设置内张角 = Outer - Outer / 12
			}

			UI::DrawColorEditor3("Color", glm::value_ptr(light.GetColor()));									//Color灯光颜色 颜色编辑器
			UI::DrawDrag("Intensity", &light.GetIntensity_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1000.0f);	//Intensity光照强度 拖动条
			UI::DrawCheckBox("Cast Shadow", &light.GetCastShadow_Ref());										//CastShadow是否投射阴影 勾选框
		});

		//绘制Camera组件
		DrawComponent<Camera>(object, [](Camera& camera)
		{
			UI::DrawCheckBox("Main Camera", &camera.IsPrimary_Ref());	//MainCamera是否是主相机 勾选框

			const char* clearFlags[] = { "Color", "Skybox" };						//清屏标志：颜色 天空盒 
			const char* currentClearFlag = clearFlags[(int)camera.GetClearFlag()];	//当前清屏标志

			//Clear Flags清屏标志选择下拉列表
			UI::DrawDropdownList("Clear Flags", currentClearFlag, clearFlags, 2, [&](int index, const char* value)
			{
				camera.SetClearFlag((Camera::ClearFlag)index);	//设置相机清屏标志
			});

			UI::DrawColorEditor4("Background", glm::value_ptr(camera.GetBackgroundColor()));	//Background背景颜色 颜色编辑器

			const char* projectionTypes[] = { "Perspective", "Orthographic" };						//投影类型：透视 正交 
			const char* currentProjectionType = projectionTypes[(int)camera.GetProjectionType()];	//当前投影类型

			//Projection投影类型选择下拉列表
			UI::DrawDropdownList("Projection", currentProjectionType, projectionTypes, 2, [&](int index, const char* value)
			{
				camera.SetProjectionType((Camera::ProjectionType)index);	//设置相机投影类型
			});

			//透视投影
			if (camera.GetProjectionType() == Camera::ProjectionType::Perspective) {	
				UI::DrawSlider("Vertical FOV", &camera.GetFOV_Ref(), 1.0f, 179.0f, UI::ValueType::Angle);	//FOV垂直张角 滑动条
			}

			//正交投影
			if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {	
				UI::DrawDrag("Size", &camera.GetSize_Ref());	//Size尺寸 拖动条
			}

			UI::DrawDrag("Near", &camera.GetNearClip_Ref(), 0.01f, UI::ValueType::Float, 0.01f, camera.GetFarClip() - 0.01f);	//Near近裁剪平面 拖动条
			UI::DrawDrag("Far", &camera.GetFarClip_Ref(), 0.01f, UI::ValueType::Float, camera.GetNearClip() + 0.01f, 1000.0f);	//Far远裁剪平面 拖动条
		});

		//绘制Mesh组件
		DrawComponent<Mesh>(object, [](Mesh& mesh)
		{
			//Mesh名：去掉'(Mesh)'
			std::string meshName = mesh.GetName() != "[None] (Mesh)" ? mesh.GetName().substr(0, mesh.GetName().find(' ')) : "None (Mesh)";

			//Mesh选择&显示按钮
			UI::DrawButton("Mesh", meshName, [&]()
			{
				std::string filepath = FileDialogs::OpenFile("Wavefront(*.obj)\0*.obj\0");	//打开文件对话框 .obj文件
				//路径不为空
				if (!filepath.empty()) {
					ModelImporter::Load(filepath);		//加载文件

					auto& m = ModelImporter::GetMesh();	//Mesh

					m.SetType(Mesh::Type::Other);								//Mesh类型
					m.SetName(std::filesystem::path(filepath).stem().string());	//Mesh名

					mesh = m;
				}
			});

			//将从拖拽源（Project面板）复制的数据拖放到目标（Mesh文件）
			ContentBrowserPanel::DragDropToTarget({ ".obj", ".mesh"}, [&](const std::filesystem::path& filepath)
			{
				ModelImporter::Load(filepath.string());	//加载文件

				auto& m = ModelImporter::GetMesh();		//Mesh
				
				m.SetType(Mesh::Type::Other);			//Mesh类型
				m.SetName(filepath.stem().string());	//Mesh名

				mesh = m;
			});
		});

		//绘制Material组件
		DrawComponent<Material>(object, [](Material& material)
		{
			uint32_t count = ShaderLibrary::GetSize();

			const char* shaderNames[5]; //TODO更新shader数
			const char* currentShaderName = "";
			int i = 0;
			//遍历着色器库
			for (auto& [name, shader] : ShaderLibrary::GetShaders()) {
				if (name == material.GetShader()->GetName()) {
					currentShaderName = name.c_str();	//当前着色器名
				}
				shaderNames[i++] = name.c_str();
			}

			//Shader着色器选择下拉列表
			UI::DrawDropdownList("Shader", currentShaderName, shaderNames, count, [&](int index, const char* value)
			{
				material.SetShader(ShaderLibrary::GetShaders()[value]);	//设置着色器
			});

			if (material.GetShader()->GetName() == "Standard") {	//标准着色器
				
				UI::DrawColorEditor4("Color", glm::value_ptr(material.GetColor()));			//Color材质颜色 颜色编辑器

				//Albedo Map 反照率贴图 选择&预览按钮
				uint32_t albedoTextureID = material.GetAlbedoTextureID();	//Albedo贴图ID
				UI::DrawImageButton("Albedo Map", albedoTextureID, { 100,100 }, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Albedo Texture(*.png;*.jpg)\0*.png;*.jpg\0");		//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						material.SetAlbedoTexture(filepath);	//设置Albedo贴图
					}
				});
				
				//将从拖拽源（Project面板）复制的数据拖放到目标（反照率贴图）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".png", ".jpg" }, [&](const std::filesystem::path& filepath)
				{
					material.SetAlbedoTexture(filepath.string());	//设置Albedo贴图
				});

				//Specular Map 反照率贴图 选择&预览按钮
				uint32_t specularTextureID = material.GetSpecularTextureID();	//Specular贴图ID
				UI::DrawImageButton("Specular Map", specularTextureID, { 100,100 }, [&]()
				{
					std::string filepath = FileDialogs::OpenFile("Specular Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
					if (!filepath.empty()) {
						material.SetSpecularTexture(filepath);	//设置Specular贴图
					}
				});

				//将从拖拽源（Project面板）复制的数据拖放到目标（高光贴图）	：文件路径
				ContentBrowserPanel::DragDropToTarget({ ".png", ".jpg"}, [&](const std::filesystem::path& filepath)
				{
					material.SetSpecularTexture(filepath.string());	//设置Albedo贴图
				});

				UI::DrawSlider("Shininess", &material.GetShininess_Ref(), 1.0f, 648.0f);					//Shininess反光度 滑动条
				UI::DrawDrag("Tiling", glm::value_ptr(material.GetTiling()), 0.01f, UI::ValueType::Float2);	//Tiling纹理平铺因子 拖动条
				UI::DrawDrag("Offset", glm::value_ptr(material.GetOffset()), 0.01f, UI::ValueType::Float2);	//Offset纹理偏移量 拖动条
			}
		});

		//绘制SpriteRenderer组件
		DrawComponent<SpriteRenderer>(object, [](SpriteRenderer& spriteRenderer)
		{
			UI::DrawColorEditor4("Color", glm::value_ptr(spriteRenderer.GetColor()));	//Color Sprite颜色 颜色编辑器

			//Sprite贴图 选择&预览按钮
			uint32_t spriteID = spriteRenderer.GetSprite().GetTexture()->GetRendererID();
			UI::DrawImageButton("Sprite", spriteID, { 100,100 }, [&]()
			{
				std::string filepath = FileDialogs::OpenFile("Sprite Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
				if (!filepath.empty()) {
					spriteRenderer.GetSprite().SetTexture(filepath);	//设置Sprite的Texture
				}
			});

			//将从拖拽源（Project面板）复制的数据拖放到目标（Sprite）	：文件路径
			ContentBrowserPanel::DragDropToTarget({ ".png", ".jpg" }, [&](const std::filesystem::path& filepath)
			{
				spriteRenderer.GetSprite().SetTexture(filepath.string());	//设置Sprite的Texture
			});
		});

		//绘制CircleRenderer组件
		DrawComponent<CircleRenderer>(object, [](CircleRenderer& circleRenderer)
		{
			Circle& circle = circleRenderer.GetCircle();

			UI::DrawColorEditor4("Color", glm::value_ptr(circle.GetColor()));								//Color颜色 颜色编辑器
			UI::DrawDrag("Thickness", &circle.GetThickness_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//Thickness厚度 vec1拖动条
			UI::DrawDrag("Fade", &circle.GetFade_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);			//Fade模糊度 vec1拖动条
		});

		//绘制Rigidbody2D组件
		DrawComponent<Rigidbody2D>(object, [](Rigidbody2D& rigidbody2D)
		{
			const char* bodyTypes[] = { "Dynamic", "Static", "Kinematic" };				//刚体类型：动态 静态 动力学 
			const char* currentBodyType = bodyTypes[(int)rigidbody2D.GetBodyType()];	//当前刚体类型

			//刚体类型选择下拉列表
			UI::DrawDropdownList("Body Type", currentBodyType, bodyTypes, 3, [&](int index, const char* value)
			{
				rigidbody2D.SetBodyType((Rigidbody2D::BodyType)index);	//设置刚体类型
			});

			UI::DrawCheckBox("Freeze Rotation", &rigidbody2D.GetFreezeRotation_Ref());	//是否冻结旋转Z轴 勾选框
		});

		//绘制BoxCollider2D组件
		DrawComponent<BoxCollider2D>(object, [](BoxCollider2D& boxCollider2D)
		{
			UI::DrawCheckBox("Render Box", &boxCollider2D.GetRenderBox_Ref());	//是否渲染Box
			UI::DrawDrag("Offset", glm::value_ptr(boxCollider2D.GetOffset()), 0.01f, UI::ValueType::Float2);	//Offset偏移量 vec2拖动条
			UI::DrawDrag("Size", glm::value_ptr(boxCollider2D.GetSize()), 0.01f, UI::ValueType::Float2, 0.0f);	//Size大小 vec2拖动条
			//TODO设置最值
			UI::DrawDrag("Density", &boxCollider2D.GetDensity_Ref());													//Density密度 vec1拖动条
			UI::DrawDrag("Friction", &boxCollider2D.GetFriction_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);		//Friction摩擦力 vec1拖动条
			UI::DrawDrag("Restitution", &boxCollider2D.GetRestitution_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//Restitution恢复系数 vec1拖动条
			UI::DrawDrag("Restitution Threshold", &boxCollider2D.GetRestitutionThreshold_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//RestitutionThreshold恢复阈值 vec1拖动条
		});
		
		//绘制CircleCollider2D组件
		DrawComponent<CircleCollider2D>(object, [](CircleCollider2D& circleCollider2D)
		{
			UI::DrawCheckBox("Render Circle", &circleCollider2D.GetRenderCircle_Ref());	//是否渲染Circle
			UI::DrawDrag("Offset", glm::value_ptr(circleCollider2D.GetOffset()), 0.01f, UI::ValueType::Float2);	//Offset偏移量 vec2拖动条
			UI::DrawDrag("Radius", &circleCollider2D.GetRadius_Ref(), 0.01f, UI::ValueType::Float, 0.0f);		//Radius半径 vec1拖动条
			//TODO设置最值
			UI::DrawDrag("Density", &circleCollider2D.GetDensity_Ref());													//Density密度 vec1拖动条
			UI::DrawDrag("Friction", &circleCollider2D.GetFriction_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);			//Friction摩擦力 vec1拖动条
			UI::DrawDrag("Restitution", &circleCollider2D.GetRestitution_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//Restitution恢复系数 vec1拖动条
			UI::DrawDrag("Restitution Threshold", &circleCollider2D.GetRestitutionThreshold_Ref(), 0.01f, UI::ValueType::Float, 0.0f, 1.0f);	//RestitutionThreshold恢复阈值 vec1拖动条
		});
	}
}