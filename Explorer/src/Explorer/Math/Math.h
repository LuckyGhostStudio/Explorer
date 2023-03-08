#pragma once

#include <glm/glm.hpp>

namespace Explorer::Math 
{
	/// <summary>
	/// 分解Transform矩阵
	/// </summary>
	/// <param name="transform">Transform矩阵</param>
	/// <param name="outPosition">输出位置向量</param>
	/// <param name="outRotation">输出旋转向量</param>
	/// <param name="outScale">输出缩放向量</param>
	/// <returns>结果</returns>
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outPosition, glm::vec3& outRotation, glm::vec3& outScale);
}