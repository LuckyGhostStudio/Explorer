#pragma once

#include "Explorer.h"

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
		Object m_SquareObject;					//正方形对象
		Object m_CameraObject;					//相机对象

		bool m_ViewportFocused = false;				//视口被聚焦
		bool m_ViewportHovered = false;				//鼠标悬停在视口
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };	//视口大小

		glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_SquareRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_SquareScale = { 1.0f, 1.0f, 1.0f };

		glm::vec2 m_TextureTilingFactor = { 1.0f, 1.0f };	//纹理重复因子

		glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 1.0f };

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
	};
}