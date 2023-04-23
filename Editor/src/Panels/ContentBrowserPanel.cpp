#include "exppch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Explorer/ImGui/UI.h"

namespace Explorer
{
	extern const std::filesystem::path g_AssetPath = "assets";	//资产目录（全局）

	/// <summary>
	/// 文件类型转为内置资产类型
	/// </summary>
	/// <param name="fileTypeString">文件类型（扩展名）</param>
	/// <returns>内置资产类型</returns>
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

		//树节点标志：允许重叠
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_AllowItemOverlap;

		ImGuiTreeNodeFlags flags = (m_CurrentDirectory == directory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth | treeNodeFlags;	//水平延伸到边框
		
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->Fonts[1];	//默认字体：1号字体

		//Assets根目录
		if (directory == g_AssetPath) {
			flags |= ImGuiTreeNodeFlags_DefaultOpen;	//资产根目录 默认打开
			font = io.Fonts->Fonts[0];	//粗体：0号字体
		}
		if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;							//是叶结点 没有箭头

		static ImVec4 headerColor = { 0, 0, 0, 0 };	//树节点颜色

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));	//设置边框尺寸
		ImGui::PushStyleColor(ImGuiCol_Header, headerColor);

		ImVec2 nodePos = ImGui::GetCursorPos();	//目录结点位置坐标

		bool opened = ImGui::TreeNodeEx((void*)filename.c_str(), flags, "");	//目录树节点：结点id 结点标志 结点名（目录名）
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		//当前结点被点击
		if (ImGui::IsItemClicked()) {		
			m_CurrentDirectory = directory;	//directory被选中
			headerColor = ImVec4(0.13f, 0.30f, 0.43f, 1.0f);	//设置选中颜色
		}

		uint32_t directoryIconID = opened && !isLeaf ? m_DirectoryOpenedIcon->GetRendererID() : m_DirectoryClosedIcon->GetRendererID();	//目录图标ID

		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(nodePos.x + 28, nodePos.y + 2));							//设置Icon位置
		ImGui::Image((void*)directoryIconID, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));	//目录图标图片

		ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(nodePos.x + 52, nodePos.y));	//设置Text位置

		ImGui::PushFont(font);			//设置结点文本字体为粗体
		ImGui::Text(filename.c_str());	//目录名
		ImGui::PopFont();

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

			std::string filenameString = relativePath.filename().string();	//目录或文件名（包含扩展名）
			std::string stemString = relativePath.stem().string();			//目录或文件名（不包含扩展名）

			ImGui::PushID(filenameString.c_str());	//设置拖拽项id ：拖拽项文件名

			std::shared_ptr<Texture2D> icon;
			//根据文件类型设置文件图标
			if (directoryEntry.is_directory()) {
				icon = m_DirectoryClosedIcon;	//文件夹关闭图标
			}
			else {
				const std::string extension = relativePath.extension().string();	//扩展名
				AssetType assetType = FileTypeToAssetType(extension);				//资产类型

				icon = SetAssetFileIcon(assetType, path);	//设置资产文件图标
			}

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
			ImGui::Selectable(stemString.c_str(), m_CurrentDirectory == path);
			ImGui::PopStyleVar();

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