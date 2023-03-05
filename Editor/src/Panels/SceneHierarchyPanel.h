#pragma once

#include "Explorer/Scene/Scene.h"
#include "Explorer/Core/Core.h"
#include "Explorer/Core/Log.h"
#include "Explorer/Scene/Object.h"

namespace Explorer
{
	/// <summary>
	/// 场景对象层级面板
	/// </summary>
	class SceneHierarchyPanel
	{
	private:
		std::shared_ptr<Scene> m_Scene;	//面板显示的场景
		Object m_SelectionObject;		//被选择中实体

		/// <summary>
		/// 绘制物体结点
		/// </summary>
		/// <param name="Object">物体</param>
		void DrawObjectNode(Object Object);
	public:
		SceneHierarchyPanel() = default;

		/// <summary>
		/// 场景对象层级面板
		/// </summary>
		/// <param name="context">场景</param>
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// 设置场景参数
		/// </summary>
		/// <param name="context">场景</param>
		void SetScene(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// 渲染ImGui
		/// </summary>
		void OnImGuiRender();
	};
}