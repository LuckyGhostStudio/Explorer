#include "exppch.h"
#include "Gizmo.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "Explorer/Core/Input.h"
#include "Explorer/Components/Transform.h"
#include "Explorer/Math/Math.h"

namespace Explorer
{
	bool s_Show = true;
	Gizmo::TransformationType Gizmo::s_TransformType = Gizmo::TransformationType::None;
	
	void Gizmo::Init(glm::vec2 minBounds, glm::vec2 maxBounds)
	{
		ImGuizmo::SetOrthographic(false);						//透视投影
		ImGuizmo::SetDrawlist();								//设置绘制列表
		
		ImGuizmo::SetRect(minBounds.x, minBounds.y, maxBounds.x - minBounds.x, maxBounds.y - minBounds.y);	//设置绘制区域
	}

	void Gizmo::DrawTransformation(Object& operatedObject, const glm::mat4& cameraView, const glm::mat4& cameraProjection)
	{
		//被操作物体存在 && Gizmo变换操作类型存在
		if (operatedObject && s_TransformType != TransformationType::None) {
			auto& transformComponent = operatedObject.GetComponent<Transform>();	//Transform组件
			glm::mat4 transform = transformComponent.GetTransform();

			bool span = Input::IsKeyPressed(Key::LeftControl);	//Ctrl刻度捕捉：操作时固定delta刻度
			//TODO:后期添加可手动设置span数值
			float spanValue = 0.5f;	//平移缩放间隔：0.5m
			//旋转间隔值：5度
			if (s_TransformType == TransformationType::Rotate) {
				spanValue = 5.0f;
			}

			float spanValues[3] = { spanValue, spanValue, spanValue };	//xyz轴刻度捕捉值
			//TODO:后期添加本地 世界坐标选择
			//绘制操作Gizmo：相机视图矩阵 相机投影矩阵 Gizmo类型 本地坐标系 选中物体transform 增量矩阵 刻度捕捉值
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)s_TransformType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, span ? spanValues : nullptr);

			//Gizmo被使用
			if (ImGuizmo::IsUsing()) {
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transform, position, rotation, scale);	//分解transform矩阵

				glm::vec3 deltaRotation = rotation - transformComponent.GetRotation();	//旋转增量

				transformComponent.GetPosition() = position;		//更新位置
				transformComponent.GetRotation() += deltaRotation;	//更新旋转：累加增量，避免万向节锁
				transformComponent.GetScale() = scale;				//更新缩放
			}
		}
	}
}