#pragma once

#include "Explorer/Renderer/Texture.h"
#include "Explorer/Utils/GeneralUtils.h"

#include <filesystem>

namespace Explorer
{
	/// <summary>
	/// 资产文件类型
	/// </summary>
	enum class AssetType
	{
		NONE = 0,	//未知类型

		FONT = 1,	//字体	TODO:待实现
		//Texture
		PNG = 2,
		JPG = 3,

		TXT = 4,	//文本

		//Model
		MESH = 5,	//网格
		OBJ = 6,

		EXPLOR = 7,	//场景

		MAT = 8,	//材质	TODO:待实现

		//Shader TODO:待更改为自定义Shader资产
		VERT = 9,
		FRAG = 10,

		SKYBOX = 11	//天空盒	TODO:待实现
	};

	/// <summary>
	/// 项目文件目录浏览面板
	/// </summary>
	class ContentBrowserPanel
	{
	private:
		std::filesystem::path m_CurrentDirectory;		//当前目录

		std::shared_ptr<Texture2D> m_DirectoryClosedIcon;	//文件夹关闭图标
		std::shared_ptr<Texture2D> m_DirectoryOpenedIcon;	//文件夹打开图标

		std::shared_ptr<Texture2D> m_NoneFileIcon;		//未知文件图标
		std::shared_ptr<Texture2D> m_FontFileIcon;		//字体文件图标
		std::shared_ptr<Texture2D> m_PngFileIcon;		//png文件图标
		std::shared_ptr<Texture2D> m_JpgFileIcon;		//jpj文件图标
		std::shared_ptr<Texture2D> m_TxtFileIcon;		//文本文件图标
		std::shared_ptr<Texture2D> m_MeshFileIcon;		//Mesh文件图标
		std::shared_ptr<Texture2D> m_ObjFileIcon;		//Obj文件图标
		std::shared_ptr<Texture2D> m_ExplorFileIcon;	//场景文件图标
		std::shared_ptr<Texture2D> m_MatFileIcon;		//材质文件图标	TODO
		std::shared_ptr<Texture2D> m_VertFileIcon;		//顶点着色器文件图标
		std::shared_ptr<Texture2D> m_FragFileIcon;		//片元着色器文件图标
		std::shared_ptr<Texture2D> m_SkyboxFileIcon;	//天空盒文件图标	TODO

		/// <summary>
		/// 绘制目录树节点
		/// </summary>
		/// <param name="directory">目录</param>
		void DrawDirectoryNode(const std::filesystem::path& directory);

		/// <summary>
		/// 绘制缩略图面板
		/// </summary>
		void DrawFileThumbnailsPanel();

		/// <summary>
		/// 设置资产文件图标
		/// </summary>
		/// <param name="assetType">资产类型</param>
		/// <param name="path">资产文件路径</param>
		/// <returns>资产图标</returns>
		std::shared_ptr<Texture2D>& SetAssetFileIcon(AssetType assetType, const std::filesystem::path& path);
	public:
		ContentBrowserPanel();

		/// <summary>
		/// 渲染ImGui
		/// </summary>
		void OnImGuiRender();

		/// <summary>
		/// 拖放源数据到目标
		/// </summary>
		/// <typeparam name="Func">拖放后执行的函数</typeparam>
		/// <param name="fileType">所需文件类型列表</param>
		/// <param name="function">拖放后执行的函数</param>
		template<typename Func>
		static void DragDropToTarget(const std::vector<std::string>& fileTypes, Func function);
	};
	
	template<typename Func>
	static inline void ContentBrowserPanel::DragDropToTarget(const std::vector<std::string>& fileTypes, Func function)
	{
		//开始拖放目标：将从拖拽源复制的数据拖放到目标
		if (ImGui::BeginDragDropTarget()) {
			//接受文件浏览面板的拖放数据 
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;	//负载数据
				std::filesystem::path filepath = std::filesystem::path(g_AssetPath) / path;	//文件路径

				std::string suffixname = GeneralUtils::GetFileType(filepath.filename().string());	//文件后缀名

				bool findType = false;
				for (auto& fileType : fileTypes) {
					//找到后缀名匹配的文件类型
					if (suffixname == fileType) {
						findType = true;
						function(filepath);	//处理文件（文件名）
						break;
					}
				}

				if (!findType) {
					EXP_ERROR("File type {0} not found!", suffixname);	//文件类型不匹配
				}
			}

			ImGui::EndDragDropTarget();
		}
	}
}