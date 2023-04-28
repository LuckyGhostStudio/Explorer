#pragma once

#include <glm/glm.hpp>

#include "Explorer/Scene/Object.h"

namespace Explorer
{
	class Gizmo
	{
	public:
		//TODO:添加 世界坐标 本地坐标 类型
		//TODO:添加绘制物体Gizmo图标
		//TODO:添加绘制世界坐标系网格

		/// <summary>
		/// 变换操作类型
		/// </summary>
		enum class TransformationType
		{
			None = -1,
			Translate = 0,	//平移
			Rotate = 1,		//旋转
			Scale = 2		//缩放
		};

		static bool s_Show;							//是否显示Gizmo
		static TransformationType s_TransformType;	//Gizmo变换操作类型

		/// <summary>
		/// 初始化：设置Gizmo绘制环境参数
		/// </summary>
		/// <param name="minBounds">绘制区域最小边界：左上角</param>
		/// <param name="maxBounds">绘制区域最大边界：右下角</param>
		static void Init(glm::vec2 minBounds, glm::vec2 maxBounds);

		/// <summary>
		/// 绘制Transform Gizmos
		/// </summary>
		/// <param name="operatedObject">被操作对象</param>
		/// <param name="cameraView">相机视图矩阵</param>
		/// <param name="cameraProjection">相机投影矩阵</param>
		static void DrawTransformation(Object& operatedObject, const glm::mat4& cameraView, const glm::mat4& cameraProjection);
	};
}