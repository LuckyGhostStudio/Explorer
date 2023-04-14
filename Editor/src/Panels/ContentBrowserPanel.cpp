#include "exppch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

#include "Explorer/ImGui/UI.h"

namespace Explorer
{
	extern const std::filesystem::path g_AssetPath = "assets";	//资产目录（全局）

	ContentBrowserPanel::ContentBrowserPanel() :m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/Directory_Icon.png");
		m_FileIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/File_Icon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		//项目文件浏览窗口
		ImGui::Begin("Project");

		ImGui::Columns(2, g_AssetPath.string().c_str(), true);	//设为2列 资产路径为id （目录树形结点列 | 文件缩略图列）
		ImGui::GetStyle().ColumnsMinSpacing = 150.0f;	//最小列宽
		
		//目录树节点：左列
		ImGui::BeginChild("Directory TreeNodes");
		
		DrawDirectoryNode(g_AssetPath);	//绘制资产目录结点

		ImGui::EndChild();

		ImGui::NextColumn();

		//文件缩略图面板：右列
		ImGui::BeginChild("File Thumbnails Panel");
			
		DrawFileThumbnailsPanel();	//绘制文件缩略图面板

		ImGui::EndChild();

		ImGui::Columns(1);	//设为1列

		ImGui::End();	//Project
	}

	void ContentBrowserPanel::DrawDirectoryNode(const std::filesystem::path& directory)
	{
		bool isLeaf = true;	//当前结点是叶结点
		for (auto& directoryEntry : std::filesystem::directory_iterator(directory)) {
			//是子目录
			if (directoryEntry.is_directory()) {
				isLeaf = false;
				break;
			}
		}

		std::string filename = directory.filename().string();	//目录名
		ImGui::PushID(filename.c_str());

		ImGuiTreeNodeFlags flags = (m_CurrentDirectory == directory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;	//水平延伸到边框
		
		if (directory == g_AssetPath) flags |= ImGuiTreeNodeFlags_DefaultOpen;	//是资产根目录 默认打开
		if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;							//是叶结点 没有箭头

		//TODO:添加目录图标
		bool opened = ImGui::TreeNodeEx((void*)filename.c_str(), flags, filename.c_str());	//目录树节点：结点id 结点标志 结点名（目录名）
		
		if (ImGui::IsItemClicked()) {		//当前结点被点击
			m_CurrentDirectory = directory;	//directory被选中
		}

		//当前结点是目录结点
		if (opened) {
			//迭代当前目录结点
			for (auto& directoryEntry : std::filesystem::directory_iterator(directory)) {
				//子结点是目录
				if (directoryEntry.is_directory()) {
					DrawDirectoryNode(directoryEntry.path());	//绘制目录结点
				}
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void ContentBrowserPanel::DrawFileThumbnailsPanel()
	{
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;		//缩略图大小
		float cellSize = thumbnailSize + padding;	//格子大小

		float panelWidth = ImGui::GetContentRegionAvail().x;	//面板宽度
		int columnCount = (int)(panelWidth / cellSize);			//列数量 = 面板宽 / 格子大小
		if (columnCount < 1) columnCount = 1;
		if (thumbnailSize <= 16.0f) columnCount = 1;	//缩略<=16 设为一列

		ImGui::Columns(columnCount, 0, false);	//设置为columnCount列

		//迭代当前目录
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const std::filesystem::path& path = directoryEntry.path();			//当前目录的 子目录或文件路径
			auto relativePath = std::filesystem::relative(path, g_AssetPath);	//path相对于资产目录的路径

			std::string filenameString = relativePath.filename().string();	//目录或文件名

			ImGui::PushID(filenameString.c_str());	//设置拖拽项id ：拖拽项文件名

			std::shared_ptr<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;	//文件图标

			//文件图标按钮
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			//开始拖拽源
			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();	//拖拽项的相对路径
				//设置拖拽负载 负载名 数据 数据大小（算结尾空字符） 复制一次数据
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);

				ImGui::EndDragDropSource();
			}
			ImGui::PopStyleColor();

			//鼠标悬停在按钮上 && 鼠标左键双击
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				//是目录文件
				if (directoryEntry.is_directory()) {
					m_CurrentDirectory /= path.filename();	//改变当前目录为当前目录的子目录（跳转到该子目录）
				}
			}

			float textAlignValue = 0.0f;
			if (thumbnailSize <= 16.0f) ImGui::SameLine();	//缩略图 最小时 图标与文件名一行
			else {
				textAlignValue = 0.5f;
			}
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, { textAlignValue, textAlignValue });	//文本居中对齐
			ImGui::Selectable(filenameString.c_str(), m_CurrentDirectory == path);
			ImGui::PopStyleVar();
			//ImGui::TextWrapped(filenameString.c_str());	//文件名

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);	//设为一列（文件缩略图列）

		ImGui::Begin("##tool", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		
		UI::DrawSlider("Thumbnail Size", &thumbnailSize, 16, 128);	//缩略图大小 滑动条 28
		//ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}