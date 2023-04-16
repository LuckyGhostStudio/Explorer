#pragma once

#include "Explorer/Renderer/Texture.h"

#include <filesystem>

namespace Explorer
{
	/// <summary>
	/// 项目文件目录浏览面板
	/// </summary>
	class ContentBrowserPanel
	{
	private:
		std::filesystem::path m_CurrentDirectory;	//当前目录

		std::shared_ptr<Texture2D> m_DirectoryIcon;	//目录图标
		std::shared_ptr<Texture2D> m_FileIcon;		//文件图标

		/// <summary>
		/// 绘制目录树节点
		/// </summary>
		/// <param name="directory">目录</param>
		void DrawDirectoryNode(const std::filesystem::path& directory);

		/// <summary>
		/// 绘制缩略图面板
		/// </summary>
		void DrawFileThumbnailsPanel();
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

				const std::string filename = filepath.filename().string();	//文件名
				uint32_t dotIndex = filename.find_last_of('.');
				std::string suffixname = filename.substr(dotIndex, filename.length() - dotIndex);	//文件后缀名

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