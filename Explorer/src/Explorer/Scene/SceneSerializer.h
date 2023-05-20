#pragma once

#include "Scene.h"

namespace Explorer
{
	/// <summary>
	/// �������л���
	/// </summary>
	class SceneSerializer
	{
	private:
		std::shared_ptr<Scene> m_Scene;	//����
	public:
		/// <summary>
		/// �������л���
		/// </summary>
		/// <param name="scene">����</param>
		SceneSerializer(const std::shared_ptr<Scene> scene);

		/// <summary>
		/// ���л����������л�Ϊ�ı��ļ�
		/// </summary>
		/// <param name="filepath">�ļ����·��</param>
		void Serialize(const std::string& filepath);

		/// <summary>
		/// �����л��������ı��ļ������л�
		/// </summary>
		/// <param name="filepath">�ļ�·��</param>
		/// <returns>���</returns>
		bool Deserialize(const std::string& filepath);
	};
}