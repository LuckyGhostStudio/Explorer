#include "exppch.h"
#include "Explorer/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Explorer/Core/Application.h"

namespace Explorer
{
	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().GetWindow().GetNativeWindow());	//�Ի��򸸴���
		ofn.lpstrFile = szFile;			//�ļ�·��
		ofn.nMaxFile = sizeof(szFile);	//����ļ���С
		ofn.lpstrFilter = filter;		//�ļ�������
		ofn.nFilterIndex = 1;			//Ĭ�Ϲ�������
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;	//·������|�ļ�����|û�и���Ŀ¼
		if (GetOpenFileNameA(&ofn) == TRUE)	//���ļ�������
		{
			return ofn.lpstrFile;	//�ļ�·��
		}
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)	//�����ļ�������
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
}