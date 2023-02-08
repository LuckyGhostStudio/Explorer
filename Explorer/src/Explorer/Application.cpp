#include "Application.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Log.h"

namespace Explorer
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);

		if (e.IsInCategory(EventCategoryApplication)) {
			EXP_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput)) {
			EXP_TRACE(e);
		}

		while (true);
	}
}