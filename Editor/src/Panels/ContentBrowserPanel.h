#pragma once

#include "Explorer/Renderer/Texture.h"
#include "Explorer/Utils/GeneralUtils.h"

#include <filesystem>

namespace Explorer
{
	/// <summary>
	/// �ʲ��ļ�����
	/// </summary>
	enum class AssetType
	{
		NONE = 0,	//δ֪����

		FONT = 1,	//����	TODO:��ʵ��
		//Texture
		PNG = 2,
		JPG = 3,

		TXT = 4,	//�ı�

		//Model
		MESH = 5,	//����
		OBJ = 6,

		EXPLOR = 7,	//����

		MAT = 8,	//����	TODO:��ʵ��

		//Shader TODO:������Ϊ�Զ���Shader�ʲ�
		VERT = 9,
		FRAG = 10,

		SKYBOX = 11	//��պ�	TODO:��ʵ��
	};

	/// <summary>
	/// ��Ŀ�ļ�Ŀ¼������
	/// </summary>
	class ContentBrowserPanel
	{
	private:
		std::filesystem::path m_CurrentDirectory;		//��ǰĿ¼

		std::shared_ptr<Texture2D> m_DirectoryClosedIcon;	//�ļ��йر�ͼ��
		std::shared_ptr<Texture2D> m_DirectoryOpenedIcon;	//�ļ��д�ͼ��

		std::shared_ptr<Texture2D> m_NoneFileIcon;		//δ֪�ļ�ͼ��
		std::shared_ptr<Texture2D> m_FontFileIcon;		//�����ļ�ͼ��
		std::shared_ptr<Texture2D> m_PngFileIcon;		//png�ļ�ͼ��
		std::shared_ptr<Texture2D> m_JpgFileIcon;		//jpj�ļ�ͼ��
		std::shared_ptr<Texture2D> m_TxtFileIcon;		//�ı��ļ�ͼ��
		std::shared_ptr<Texture2D> m_MeshFileIcon;		//Mesh�ļ�ͼ��
		std::shared_ptr<Texture2D> m_ObjFileIcon;		//Obj�ļ�ͼ��
		std::shared_ptr<Texture2D> m_ExplorFileIcon;	//�����ļ�ͼ��
		std::shared_ptr<Texture2D> m_MatFileIcon;		//�����ļ�ͼ��	TODO
		std::shared_ptr<Texture2D> m_VertFileIcon;		//������ɫ���ļ�ͼ��
		std::shared_ptr<Texture2D> m_FragFileIcon;		//ƬԪ��ɫ���ļ�ͼ��
		std::shared_ptr<Texture2D> m_SkyboxFileIcon;	//��պ��ļ�ͼ��	TODO

		/// <summary>
		/// ����Ŀ¼���ڵ�
		/// </summary>
		/// <param name="directory">Ŀ¼</param>
		void DrawDirectoryNode(const std::filesystem::path& directory);

		/// <summary>
		/// ��������ͼ���
		/// </summary>
		void DrawFileThumbnailsPanel();

		/// <summary>
		/// �����ʲ��ļ�ͼ��
		/// </summary>
		/// <param name="assetType">�ʲ�����</param>
		/// <param name="path">�ʲ��ļ�·��</param>
		/// <returns>�ʲ�ͼ��</returns>
		std::shared_ptr<Texture2D>& SetAssetFileIcon(AssetType assetType, const std::filesystem::path& path);
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

				std::string suffixname = GeneralUtils::GetFileType(filepath.filename().string());	//�ļ���׺��

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