#include "exppch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

#include "Explorer/ImGui/UI.h"

namespace Explorer
{
	extern const std::filesystem::path g_AssetPath = "assets";	//�ʲ�Ŀ¼��ȫ�֣�

	ContentBrowserPanel::ContentBrowserPanel() :m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/Directory_Icon.png");
		m_FileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/File_Icon.png");
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

		ImGuiTreeNodeFlags flags = (m_CurrentDirectory == directory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;	//ˮƽ���쵽�߿�
		
		if (directory == g_AssetPath) flags |= ImGuiTreeNodeFlags_DefaultOpen;	//���ʲ���Ŀ¼ Ĭ�ϴ�
		if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;							//��Ҷ��� û�м�ͷ

		//TODO:���Ŀ¼ͼ��
		bool opened = ImGui::TreeNodeEx((void*)filename.c_str(), flags, filename.c_str());	//Ŀ¼���ڵ㣺���id ����־ �������Ŀ¼����
		
		if (ImGui::IsItemClicked()) {		//��ǰ��㱻���
			m_CurrentDirectory = directory;	//directory��ѡ��
		}

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

			std::string filenameString = relativePath.filename().string();	//Ŀ¼���ļ���

			ImGui::PushID(filenameString.c_str());	//������ק��id ����ק���ļ���

			std::shared_ptr<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;	//�ļ�ͼ��

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
			ImGui::Selectable(filenameString.c_str(), m_CurrentDirectory == path);
			ImGui::PopStyleVar();
			//ImGui::TextWrapped(filenameString.c_str());	//�ļ���

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