#pragma once

#include "Explorer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Explorer/Renderer/EditorCamera.h"

namespace Explorer
{
	/// <summary>
	/// 2D渲染测试层
	/// </summary>
	class EditorLayer :public Layer
	{
	private:
		//Temp
		std::shared_ptr<VertexArray> m_SquareVA;			//正方形顶点数组
		std::shared_ptr<Texture2D> m_CheckerboardTexture;	//棋盘纹理
		std::shared_ptr<Framebuffer> m_Framebuffer;			//帧缓冲区

		std::shared_ptr<Scene> m_ActiveScene;	//活动场景
		EditorCamera m_EditorCamera;			//编辑器相机

		Object m_Camera;						//场景默认相机
		Object m_Cube;							//场景默认正方体
		//TODO:添加场景默认Light

		bool m_ViewportFocused = false;				//视口被聚焦
		bool m_ViewportHovered = false;				//鼠标悬停在视口
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };	//视口大小

		int m_GizmoType = -1;	//Gizmo操作类型 -1无 0平移 1旋转 2缩放

		SceneHierarchyPanel m_HierarchyPanel;	//场景Hierarchy面板

		float fps = 0.0f;

		//性能测试结果
		struct ProfileResult
		{
			const char* Name;	//测试程序段名
			float Time;			//运行时间
		};

		std::vector<ProfileResult> m_ProfileResults;	//性能测试结果集合
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		/// <summary>
		/// 将该层添加到层栈时调用
		/// </summary>
		virtual void OnAttach() override;

		/// <summary>
		/// 将该层从层栈移除时调用
		/// </summary>
		virtual void OnDetach() override;

		/// <summary>
		/// 该层更新时调用：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		virtual void OnUpdate(DeltaTime dt) override;

		/// <summary>
		/// 渲染ImGui
		/// </summary>
		virtual void OnImGuiRender() override;

		/// <summary>
		/// 事件函数：接收事件
		/// </summary>
		/// <param name="event">事件</param>
		virtual void OnEvent(Event& event) override;

		/// <summary>
		/// 创建新场景
		/// </summary>
		void NewScene();

		/// <summary>
		/// 打开场景
		/// </summary>
		void OpenScene();

		/// <summary>
		/// 场景另存为
		/// </summary>
		void SaveSceneAs();
	private:
		/// <summary>
		/// 按键按下时调用
		/// </summary>
		/// <param name="e">按键按下事件</param>
		/// <returns>处理结果</returns>
		bool OnKeyPressed(KeyPressedEvent& e);
	};
}