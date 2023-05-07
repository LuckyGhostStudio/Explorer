#pragma once

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Renderer/EditorCamera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Environment.h"

#include "entt.hpp"

#include "Explorer/Core/UUID.h"

#include "Explorer/Renderer/RendererRayTracing.h"

#include <glm/glm.hpp>

class b2World;

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

		b2World* m_PhysicsWorld = nullptr;		//��������
		glm::vec2 m_Gravity = { 0.0f, -9.8f };	//�������ٶ�
	public:
		RayTracing::Scene m_RayTracingScene;	//TODO ���Ƴ�
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

		glm::vec2& GetGravity() { return m_Gravity; }
		void SetGravity(const glm::vec2& gravity) { m_Gravity = gravity; }

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="object">Դ����</param>
		void CopyObject(Object object);

		/// <summary>
		/// ���Ƴ���
		/// </summary>
		/// <param name="scene">�����Ƴ���</param>
		/// <returns>������</returns>
		static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> scene);

		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="enable">��������״̬</param>
		/// <returns>����</returns>
		Object CreateEmptyObject(const std::string& name = "Object", bool enable = true);

		/// <summary>
		/// ���������壺ʹ������UUID
		/// </summary>
		/// <param name="uuid">ID</param>
		/// <param name="name">������</param>
		/// <param name="enable">��������״̬</param>
		/// <returns>����</returns>
		Object CreateEmptyObject(UUID uuid, const std::string& name = "Object", bool enable = true);
		
		/// <summary>
		/// ����ͼƬ����
		/// </summary>
		/// <param name="name">������</param>
		/// <returns>����</returns>
		Object CreateSpriteObject(const std::string& name = "Sprite");

		/// <summary>
		/// ����Circle
		/// </summary>
		/// <param name="name">������</param>
		/// <returns>����</returns>
		Object CreateCircleObject(const std::string& name = "Circle");

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
		/// ��������׷�ٲ�������
		/// </summary>
		/// <param name="name">������</param>
		/// <returns>����</returns>
		Object CreateSphereObjectToRayTracing(const std::string& name = "Sphere");

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="object">����</param>
		void DestroyObject(Object object);

		/// <summary>
		/// ��ʼ����ʱ���ã����Playʱ����ʼ����ģ�⣩
		/// </summary>
		void OnRuntimeStart();

		/// <summary>
		/// ֹͣ����ʱ���ã�ֹͣPlayʱ��ֹͣ����ģ�⣩
		/// </summary>
		void OnRuntimeStop();

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

		/// <summary>
		/// ����ӵ��Components�������������
		/// </summary>
		/// <typeparam name="...Components">��������б�</typeparam>
		/// <returns>�����б�</returns>
		template<typename... Components>
		auto GetAllObjectsWith()
		{
			return m_Registry.view<Components...>();
		}
	};
}