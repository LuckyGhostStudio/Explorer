#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Explorer
{
	/// <summary>
	/// Explorer�༭��
	/// </summary>
	class ExplorerEditor :public Application
	{
	public:
		ExplorerEditor() :Application("Explorer Editor")
		{
			PushLayer(new EditorLayer());	//��ӱ༭����
		}

		~ExplorerEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new ExplorerEditor();
	}
}