#pragma once

#include "Explorer/Renderer/Texture.h"

#include <filesystem>

namespace Explorer
{
	/// <summary>
	/// ��Ŀ�ļ�Ŀ¼������
	/// </summary>
	class ContentBrowserPanel
	{
	private:
		std::filesystem::path m_CurrentDirectory;	//��ǰĿ¼

		std::shared_ptr<Texture2D> m_DirectoryIcon;	//Ŀ¼ͼ��
		std::shared_ptr<Texture2D> m_FileIcon;		//�ļ�ͼ��

		/// <summary>
		/// ����Ŀ¼���ڵ�
		/// </summary>
		/// <param name="directory">Ŀ¼</param>
		void DrawDirectoryNode(const std::filesystem::path& directory);

		/// <summary>
		/// ��������ͼ���
		/// </summary>
		void DrawFileThumbnailsPanel();
	public:
		ContentBrowserPanel();

		/// <summary>
		/// ��ȾImGui
		/// </summary>
		void OnImGuiRender();

		/// <summary>
		/// �Ϸ�Դ���ݵ�Ŀ��
		/// </summary>
		/// <typeparam name="Func">�Ϸź�ִ�еĺ���</typeparam>
		/// <param name="fileType">�����ļ������б�</param>
		/// <param name="function">�Ϸź�ִ�еĺ���</param>
		template<typename Func>
		static void DragDropToTarget(const std::vector<std::string>& fileTypes, Func function);
	};
	
	template<typename Func>
	static inline void ContentBrowserPanel::DragDropToTarget(const std::vector<std::string>& fileTypes, Func function)
	{
		//��ʼ�Ϸ�Ŀ�꣺������קԴ���Ƶ������Ϸŵ�Ŀ��
		if (ImGui::BeginDragDropTarget()) {
			//�����ļ���������Ϸ����� 
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;	//��������
				std::filesystem::path filepath = std::filesystem::path(g_AssetPath) / path;	//�ļ�·��

				const std::string filename = filepath.filename().string();	//�ļ���
				uint32_t dotIndex = filename.find_last_of('.');
				std::string suffixname = filename.substr(dotIndex, filename.length() - dotIndex);	//�ļ���׺��

				bool findType = false;
				for (auto& fileType : fileTypes) {
					//�ҵ���׺��ƥ����ļ�����
					if (suffixname == fileType) {
						findType = true;
						function(filepath);	//�����ļ����ļ�����
						break;
					}
				}

				if (!findType) {
					EXP_ERROR("File type {0} not found!", suffixname);	//�ļ����Ͳ�ƥ��
				}
			}

			ImGui::EndDragDropTarget();
		}
	}
}