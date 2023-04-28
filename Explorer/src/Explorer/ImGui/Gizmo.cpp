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
		ImGuizmo::SetOrthographic(false);						//͸��ͶӰ
		ImGuizmo::SetDrawlist();								//���û����б�
		
		ImGuizmo::SetRect(minBounds.x, minBounds.y, maxBounds.x - minBounds.x, maxBounds.y - minBounds.y);	//���û�������
	}

	void Gizmo::DrawTransformation(Object& operatedObject, const glm::mat4& cameraView, const glm::mat4& cameraProjection)
	{
		//������������� && Gizmo�任�������ʹ���
		if (operatedObject && s_TransformType != TransformationType::None) {
			auto& transformComponent = operatedObject.GetComponent<Transform>();	//Transform���
			glm::mat4 transform = transformComponent.GetTransform();

			bool span = Input::IsKeyPressed(Key::LeftControl);	//Ctrl�̶Ȳ�׽������ʱ�̶�delta�̶�
			//TODO:������ӿ��ֶ�����span��ֵ
			float spanValue = 0.5f;	//ƽ�����ż����0.5m
			//��ת���ֵ��5��
			if (s_TransformType == TransformationType::Rotate) {
				spanValue = 5.0f;
			}

			float spanValues[3] = { spanValue, spanValue, spanValue };	//xyz��̶Ȳ�׽ֵ
			//TODO:������ӱ��� ��������ѡ��
			//���Ʋ���Gizmo�������ͼ���� ���ͶӰ���� Gizmo���� ��������ϵ ѡ������transform �������� �̶Ȳ�׽ֵ
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)s_TransformType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, span ? spanValues : nullptr);

			//Gizmo��ʹ��
			if (ImGuizmo::IsUsing()) {
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transform, position, rotation, scale);	//�ֽ�transform����

				glm::vec3 deltaRotation = rotation - transformComponent.GetRotation();	//��ת����

				transformComponent.GetPosition() = position;		//����λ��
				transformComponent.GetRotation() += deltaRotation;	//������ת���ۼ������������������
				transformComponent.GetScale() = scale;				//��������
			}
		}
	}
}