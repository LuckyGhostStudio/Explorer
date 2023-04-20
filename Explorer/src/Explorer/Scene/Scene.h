#pragma once

#include "entt.hpp"

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Renderer/EditorCamera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Environment.h"

namespace Explorer
{
	class Object;

	/// <summary>
	/// ����
	/// </summary>
	class Scene
	{
	private:
		friend class Object;	//��Ԫ��Object
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		entt::registry m_Registry;	//ʵ��ע���ʵ��id���ϣ�unsigned int���ϣ�
		std::string m_Name;			//������

		uint32_t m_ViewportWidth = 1280;	//�����ӿڿ�
		uint32_t m_ViewportHeight = 720;	//�����ӿڸ�

		Environment m_Environment;	//��������
	private:
		/// <summary>
		/// object���T���ʱ����
		/// </summary>
		/// <typeparam name="T">�������</typeparam>
		/// <param name="object">����</param>
		/// <param name="component">���</param>
		template<typename T>
		void OnComponentAdded(Object object, T& component);
	public:
		Scene(const std::string& name = "New Scene");
		~Scene();

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		Environment& GetEnvironment() { return m_Environment; }
		void SetEnvironment(const Environment& environment) { m_Environment = environment; }

		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="enable">��������״̬</param>
		/// <returns>����</returns>
		Object CreateEmptyObject(const std::string& name = "Object", bool enable = true);
		
		/// <summary>
		/// ����ͼƬ����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="enable">��������״̬</param>
		/// <returns>����</returns>
		Object CreateSpriteObject(const std::string& name = "Sprite", bool enable = true);

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="type">������������</param>
		/// <returns>��������</returns>
		Object CreateMeshObject(const std::string& name = "Object", const Mesh::Type type = Mesh::Type::None);

		/// <summary>
		/// ����Camera
		/// </summary>
		/// <param name="name">�����</param>
		/// <param name="primary">�Ƿ��������</param>
		/// <returns>���</returns>
		Object CreateCameraObject(const std::string& name = "Camera", bool primary = false);

		/// <summary>
		/// ����Light
		/// </summary>
		/// <param name="type">��Դ����</param>
		/// <param name="name">��Դ��</param>
		/// <returns>��Դ</returns>
		Object CreateLightObject(Light::Type type = Light::Type::Point, const std::string& name = "Light");

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="object">����</param>
		void DestroyObject(Object object);

		/// <summary>
		/// �༭�����£�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		/// <param name="camera">�༭�����</param>
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);

		/// <summary>
		/// ����ʱ���£�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// �����ӿڴ�С���ӿڸı�ʱ����
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		void OnViewportResize(uint32_t width, uint32_t height);

		/// <summary>
		/// ���������
		/// </summary>
		/// <returns>�����</returns>
		Object GetPrimaryCameraObject();
	};
}