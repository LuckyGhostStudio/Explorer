#include "exppch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Explorer/ImGui/UI.h"

namespace Explorer
{
	extern const std::filesystem::path g_AssetPath = "assets";	//�ʲ�Ŀ¼��ȫ�֣�

	/// <summary>
	/// �ļ�����תΪ�����ʲ�����
	/// </summary>
	/// <param name="fileTypeString">�ļ����ͣ���չ����</param>
	/// <returns>�����ʲ�����</returns>
	AssetType FileTypeToAssetType(const std::string& fileTypeString)
	{
		if (fileTypeString == ".font") return AssetType::FONT;
		else if (fileTypeString == ".png") return AssetType::PNG;
		else if (fileTypeString == ".jpg") return AssetType::JPG;
		else if (fileTypeString == ".txt") return AssetType::TXT;
		else if (fileTypeString == ".mesh") return AssetType::MESH;
		else if (fileTypeString == ".obj") return AssetType::OBJ;
		else if (fileTypeString == ".explor") return AssetType::EXPLOR;
		else if (fileTypeString == ".mat") return AssetType::MAT;
		else if (fileTypeString == ".vert") return AssetType::VERT;
		else if (fileTypeString == ".frag") return AssetType::FRAG;
		else if (fileTypeString == ".skybox") return AssetType::SKYBOX;
		else return AssetType::NONE;
	}

	std::shared_ptr<Texture2D>& ContentBrowserPanel::SetAssetFileIcon(AssetType assetType, const std::filesystem::path& path)
	{
		switch (assetType)
		{
			case AssetType::NONE:	return m_NoneFileIcon;
			case AssetType::FONT:	return m_NoneFileIcon;	//TODO
			case AssetType::PNG:	return m_PngFileIcon;
			case AssetType::JPG:	return m_JpgFileIcon;
			case AssetType::TXT:	return m_TxtFileIcon;
			case AssetType::MESH:	return m_MeshFileIcon;
			case AssetType::OBJ:	return m_ObjFileIcon;
			case AssetType::EXPLOR:	return m_ExplorFileIcon;
			case AssetType::MAT:	return m_NoneFileIcon;	//TODO
			case AssetType::VERT:	return m_VertFileIcon;
			case AssetType::FRAG:	return m_FragFileIcon;
			case AssetType::SKYBOX:	return m_NoneFileIcon;	//TODO
		}

		EXP_ASSERT(false, "Asset Type Not Found!");
		return std::make_shared<Texture2D>(1, 1);
	}

	ContentBrowserPanel::ContentBrowserPanel() :m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/Directory_Icon.png");
		m_DirectoryClosedIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/DirectoryClosed_Icon.png");
		m_DirectoryOpenedIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/DirectoryOpened_Icon.png");

		m_NoneFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/File_Icon.png");
		m_PngFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/PNGFile_Icon.png");
		m_JpgFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/JPGFile_Icon.png");
		m_TxtFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/TXTFile_Icon.png");
		m_ObjFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/OBJFile_Icon.png");
		m_MeshFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/MESHFile_Icon.png");
		m_VertFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/VERTFile_Icon.png");
		m_FragFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/FRAGFile_Icon.png");
		m_ExplorFileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/EXPLORFile_Icon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		//��Ŀ�ļ��������
		ImGui::Begin("Project");

		ImGui::Columns(2, g_AssetPath.string().c_str(), true);	//��Ϊ2�� �ʲ�·��Ϊid ��Ŀ¼���ν���� | �ļ�����ͼ�У�
		ImGui::GetStyle().ColumnsMinSpacing = 150.0f;	//��С�п�
		
		//Ŀ¼���ڵ㣺����
		ImGui::BeginChild("Directory TreeNodes");
		
		DrawDirectoryNode(g_AssetPath);	//�����ʲ�Ŀ¼���

		ImGui::EndChild();

		ImGui::NextColumn();

		//�ļ�����ͼ��壺����
		ImGui::BeginChild("File Thumbnails Panel");
			
		DrawFileThumbnailsPanel();	//�����ļ�����ͼ���

		ImGui::EndChild();

		ImGui::Columns(1);	//��Ϊ1��

		ImGui::End();	//Project
	}

	void ContentBrowserPanel::DrawDirectoryNode(const std::filesystem::path& directory)
	{
		bool isLeaf = true;	//��ǰ�����Ҷ���
		for (auto& directoryEntry : std::filesystem::directory_iterator(directory)) {
			//����Ŀ¼
			if (directoryEntry.is_directory()) {
				isLeaf = false;
				break;
			}
		}

		std::string filename = directory.filename().string();	//Ŀ¼��
		ImGui::PushID(filename.c_str());

		//���ڵ��־�������ص�
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_AllowItemOverlap;

		ImGuiTreeNodeFlags flags = (m_CurrentDirectory == directory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth | treeNodeFlags;	//ˮƽ���쵽�߿�
		
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->Fonts[1];	//Ĭ�����壺1������

		//Assets��Ŀ¼
		if (directory == g_AssetPath) {
			flags |= ImGuiTreeNodeFlags_DefaultOpen;	//�ʲ���Ŀ¼ Ĭ�ϴ�
			font = io.Fonts->Fonts[0];	//���壺0������
		}
		if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;							//��Ҷ��� û�м�ͷ

		static ImVec4 headerColor = { 0, 0, 0, 0 };	//���ڵ���ɫ

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));	//���ñ߿�ߴ�
		ImGui::PushStyleColor(ImGuiCol_Header, headerColor);

		ImVec2 nodePos = ImGui::GetCursorPos();	//Ŀ¼���λ������

		bool opened = ImGui::TreeNodeEx((void*)filename.c_str(), flags, "");	//Ŀ¼���ڵ㣺���id ����־ �������Ŀ¼����
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		//��ǰ��㱻���
		if (ImGui::IsItemClicked()) {		
			m_CurrentDirectory = directory;	//directory��ѡ��
			headerColor = ImVec4(0.13f, 0.30f, 0.43f, 1.0f);	//����ѡ����ɫ
		}

		uint32_t directoryIconID = opened && !isLeaf ? m_DirectoryOpenedIcon->GetRendererID() : m_DirectoryClosedIcon->GetRendererID();	//Ŀ¼ͼ��ID

		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(nodePos.x + 28, nodePos.y + 2));							//����Iconλ��
		ImGui::Image((void*)directoryIconID, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));	//Ŀ¼ͼ��ͼƬ

		ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(nodePos.x + 52, nodePos.y));	//����Textλ��

		ImGui::PushFont(font);			//���ý���ı�����Ϊ����
		ImGui::Text(filename.c_str());	//Ŀ¼��
		ImGui::PopFont();

		//��ǰ�����Ŀ¼���
		if (opened) {
			//������ǰĿ¼���
			for (auto& directoryEntry : std::filesystem::directory_iterator(directory)) {
				//�ӽ����Ŀ¼
				if (directoryEntry.is_directory()) {
					DrawDirectoryNode(directoryEntry.path());	//����Ŀ¼���
				}
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void ContentBrowserPanel::DrawFileThumbnailsPanel()
	{
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;		//����ͼ��С
		float cellSize = thumbnailSize + padding;	//���Ӵ�С

		float panelWidth = ImGui::GetContentRegionAvail().x;	//�����
		int columnCount = (int)(panelWidth / cellSize);			//������ = ���� / ���Ӵ�С
		if (columnCount < 1) columnCount = 1;
		if (thumbnailSize <= 16.0f) columnCount = 1;	//����<=16 ��Ϊһ��

		ImGui::Columns(columnCount, 0, false);	//����ΪcolumnCount��

		//������ǰĿ¼
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const std::filesystem::path& path = directoryEntry.path();			//��ǰĿ¼�� ��Ŀ¼���ļ�·��
			auto relativePath = std::filesystem::relative(path, g_AssetPath);	//path������ʲ�Ŀ¼��·��

			std::string filenameString = relativePath.filename().string();	//Ŀ¼���ļ�����������չ����
			std::string stemString = relativePath.stem().string();			//Ŀ¼���ļ�������������չ����

			ImGui::PushID(filenameString.c_str());	//������ק��id ����ק���ļ���

			std::shared_ptr<Texture2D> icon;
			//�����ļ����������ļ�ͼ��
			if (directoryEntry.is_directory()) {
				icon = m_DirectoryClosedIcon;	//�ļ��йر�ͼ��
			}
			else {
				const std::string extension = relativePath.extension().string();	//��չ��
				AssetType assetType = FileTypeToAssetType(extension);				//�ʲ�����

				icon = SetAssetFileIcon(assetType, path);	//�����ʲ��ļ�ͼ��
			}

			//�ļ�ͼ�갴ť 
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			//��ʼ��קԴ
			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();	//��ק������·��
				//������ק���� ������ ���� ���ݴ�С�����β���ַ��� ����һ������
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);

				ImGui::EndDragDropSource();
			}
			ImGui::PopStyleColor();

			//�����ͣ�ڰ�ť�� && ������˫��
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				//��Ŀ¼�ļ�
				if (directoryEntry.is_directory()) {
					m_CurrentDirectory /= path.filename();	//�ı䵱ǰĿ¼Ϊ��ǰĿ¼����Ŀ¼����ת������Ŀ¼��
				}
			}

			float textAlignValue = 0.0f;
			if (thumbnailSize <= 16.0f) ImGui::SameLine();	//����ͼ ��Сʱ ͼ�����ļ���һ��
			else {
				textAlignValue = 0.5f;
			}
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, { textAlignValue, textAlignValue });	//�ı����ж���
			ImGui::Selectable(stemString.c_str(), m_CurrentDirectory == path);
			ImGui::PopStyleVar();

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);	//��Ϊһ�У��ļ�����ͼ�У�

		ImGui::Begin("##tool", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		
		UI::DrawSlider("Thumbnail Size", &thumbnailSize, 16, 128);	//����ͼ��С ������ 28
		//ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}