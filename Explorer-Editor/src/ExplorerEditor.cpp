#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Explorer
{
	/// <summary>
	/// Explorer±à¼­Æ÷
	/// </summary>
	class ExplorerEditor :public Application
	{
	public:
		ExplorerEditor() :Application("Explorer Editor")
		{
			PushLayer(new EditorLayer());	//Ìí¼Ó±à¼­Æ÷²ã
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