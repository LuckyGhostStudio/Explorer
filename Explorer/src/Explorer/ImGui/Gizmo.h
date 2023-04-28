#pragma once

#include <glm/glm.hpp>

#include "Explorer/Scene/Object.h"

namespace Explorer
{
	class Gizmo
	{
	public:
		//TODO:��� �������� �������� ����
		//TODO:��ӻ�������Gizmoͼ��
		//TODO:��ӻ�����������ϵ����

		/// <summary>
		/// �任��������
		/// </summary>
		enum class TransformationType
		{
			None = -1,
			Translate = 0,	//ƽ��
			Rotate = 1,		//��ת
			Scale = 2		//����
		};

		static bool s_Show;							//�Ƿ���ʾGizmo
		static TransformationType s_TransformType;	//Gizmo�任��������

		/// <summary>
		/// ��ʼ��������Gizmo���ƻ�������
		/// </summary>
		/// <param name="minBounds">����������С�߽磺���Ͻ�</param>
		/// <param name="maxBounds">�����������߽磺���½�</param>
		static void Init(glm::vec2 minBounds, glm::vec2 maxBounds);

		/// <summary>
		/// ����Transform Gizmos
		/// </summary>
		/// <param name="operatedObject">����������</param>
		/// <param name="cameraView">�����ͼ����</param>
		/// <param name="cameraProjection">���ͶӰ����</param>
		static void DrawTransformation(Object& operatedObject, const glm::mat4& cameraView, const glm::mat4& cameraProjection);
	};
}