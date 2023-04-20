#pragma once

#include "Explorer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Explorer/Renderer/EditorCamera.h"

namespace Explorer
{
	/// <summary>
	/// 2D渲染测试层
	/// </summary>
	class EditorLayer :public Layer
	{
	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;	//帧缓冲区

		std::shared_ptr<Scene> m_ActiveScene;		//活动场景
		EditorCamera m_EditorCamera;				//编辑器相机

		Object m_MainCamera;						//场景默认相机：主相机
		Object m_Cube;								//场景默认正方体
		Object m_Light;								//场景默认光源

		Object m_PickedObject;						//被鼠标拾取的物体
		bool m_Pickable = true;						//是否可拾取：可重新设置被选中物体

		bool m_ViewportFocused = false;				//视口被聚焦
		bool m_ViewportHovered = false;				//鼠标悬停在视口

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };	//视口大小
		glm::vec2 m_ViewportBounds[2];				//视口边界位置

		enum class SceneState
		{
			Edit = 0,	//编辑
			Play = 1	//运行
		};

		SceneState m_SceneState = SceneState::Edit;	//场景状态
		
		SceneHierarchyPanel m_SceneHierarchyPanel;	//场景Hierarchy面板
		ContentBrowserPanel m_ContentBrowserPanel;	//项目文件目录Project浏览面板

		std::shared_ptr<Texture2D> m_SelectionIcon;		//选择物体 按钮图标

		std::shared_ptr<Texture2D> m_TranslationIcon;	//平移 按钮图标
		std::shared_ptr<Texture2D> m_RotationIcon;		//选中 按钮图标
		std::shared_ptr<Texture2D> m_ScaleIcon;			//缩放 按钮图标

		std::shared_ptr<Texture2D> m_PlayIcon;	//Play按钮图标
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
	private:
		/// <summary>
		/// 按键按下时调用
		/// </summary>
		/// <param name="e">按键按下事件</param>
		/// <returns>处理结果</returns>
		bool OnKeyPressed(KeyPressedEvent& e);

		/// <summary>
		/// 鼠标按钮按下时调用
		/// </summary>
		/// <param name="e">鼠标按钮按下事件</param>
		/// <returns>处理结果</returns>
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		/// <summary>
		/// 创建新场景
		/// </summary>
		void NewScene();

		/// <summary>
		/// 打开场景
		/// </summary>
		void OpenScene();

		/// <summary>
		/// 打开场景（从文件浏览面板打开）
		/// </summary>
		/// <param name="path">场景文件路径</param>
		void OpenScene(const std::filesystem::path& path);

		/// <summary>
		/// 场景另存为
		/// </summary>
		void SaveSceneAs();

		/// <summary>
		/// 导入模型文件
		/// </summary>
		void ImportModelFile();

		/// <summary>
		/// 导入模型文件
		/// </summary>
		/// <param name="path">模型文件路径</param>
		void ImportModelFile(const std::filesystem::path& path);

		/// <summary>
		/// Play按钮按下时调用
		/// </summary>
		void OnScenePlay();

		/// <summary>
		/// Stop按钮按下时调用
		/// </summary>
		void OnSceneStop();

		/// <summary>
		/// 工具栏
		/// </summary>
		void UI_ToolBar();
	};
}