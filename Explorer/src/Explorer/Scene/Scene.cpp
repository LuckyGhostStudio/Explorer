#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer3D.h"

#include "Explorer/Components/Components.h"

#include "Object.h"

namespace Explorer
{
	Scene::Scene(const std::string& name) :m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	Object Scene::CreateEmptyObject(const std::string& name)
	{
		Object object = { m_Registry.create(), this };	//����������
		object.AddComponent<Name>(name);				//���Name�����Ĭ�������
		object.AddComponent<Transform>();				//���Transform�����Ĭ�������

		return object;
	}

	Object Scene::CreateCameraObject(const std::string& name)
	{
		Object camera = { m_Registry.create(), this };		//����Camera

		glm::vec3 position = { 3.7f, 2.5f, 3.5f };			//��ʼλ��
		glm::vec3 rotation = { 30.5f, -53.0f, 175.5f };		//��ʼ��ת

		camera.AddComponent<Name>(name);					//���Name���
		camera.AddComponent<Transform>(position, rotation);	//���Transform���
		camera.AddComponent<Camera>();						//���Camera���

		return camera;
	}

	Object Scene::CreateLightObject(Light::Type type, const std::string& name)
	{
		std::string tempName = name;
		glm::vec3 position = { 0.0f, 1.5f, 0.0f };			//��ʼλ��
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };			//��ʼ��ת

		switch (type)
		{
			case Light::Type::Directional:	//ƽ�й�
				tempName = "Directional Light";
				rotation = { 37.3f, 107.0f, 97.0f };	//���ó�ʼ��תֵ
				break;
			case Light::Type::Point:		//���Դ
				tempName = "Point Light";
				position = { 2.0f, 2.95f, -0.85f };		//���ó�ʼλ��
				break;
			case Light::Type::Spot:			//�۹�Դ
				tempName = "Spot Light";
				rotation = { -90.0f, 0.0f, 0.0f };		//���ó�ʼ��תֵ
				break;
		}
		 
		Object lightObj = { m_Registry.create(), this };		//����Light

		lightObj.AddComponent<Name>(tempName);					//���Name���
		lightObj.AddComponent<Transform>(position, rotation);	//���Transform���
		lightObj.AddComponent<Light>(type);						//���Light�����type���͹�Դ

		return lightObj;
	}

	void Scene::DestroyObject(Object object)
	{
		m_Registry.destroy(object);	//��ע����Ƴ�����
	}

	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		auto lights = m_Registry.view<Light>();	//����ӵ��Light���������
		
		std::vector<Object> lightObjects;		//��������Light����
		lightObjects.reserve(lights.size());	//Ԥ���ռ�

		//������������Light����
		for (auto object : lights) {
			lightObjects.push_back(Object{ object, this });	//��ӵ�Light�б�
		}

		Renderer3D::BeginScene(camera, lightObjects);	//��ʼ��Ⱦ����

		auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);	//������Transform��SpriteRenderer����������

		for (auto object : group) {
			auto [transform, sprite] = group.get<Transform, SpriteRenderer>(object);

			Renderer3D::DrawMesh(transform, (int)object);	//��������
		}
		Renderer3D::EndScene();			//������Ⱦ����
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		//��������ӵ�нű�����Ķ��󣬵���each�ڵĺ���
		m_Registry.view<NativeScript>().each([=](auto object, NativeScript& script)
		{
			//�ű�δʵ����
			if (!script.Instance) {
				script.Instance = script.InstantiateScript();		//ʵ�����ű�
				script.Instance->m_Object = Object{ object, this };	//���ýű������Ķ���

				script.Instance->OnCreate();	//���ýű���OnCreate����
			}
			script.Instance->OnUpdate(dt);		//���ýű���OnOpdate����
		});

		Camera* mainCamera = nullptr;	//�����
		Transform* cameraTransform = nullptr;

		auto view = m_Registry.view<Transform, Camera>();	//������Transform��Camera����������

		for (auto object : view) {
			auto [transform, camera] = view.get<Transform, Camera>(object);

			//�ҵ������
			if (camera.m_Primary) {
				mainCamera = &camera;
				cameraTransform = &transform;
				break;
			}
		}

		//���������
		if (mainCamera) {
			auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);	//������Transform��SpriteRenderer����������

			Renderer3D::BeginScene(*mainCamera, *cameraTransform);	//��ʼ��Ⱦ����
			for (auto object : group) {
				auto [transform, sprite] = group.get<Transform, SpriteRenderer>(object);

				//Renderer2D::DrawQuad(transform, sprite.m_Color);
				//Renderer3D::DrawMesh(transform);	//��������
			}
			Renderer3D::EndScene();			//������Ⱦ����
		}
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<Camera>();	//������Camera���������

		for (auto object : view) {
			auto& camera = view.get<Camera>(object);	//���Camera���
			camera.SetViewportSize(width, height);		//�����ӿڴ�С
		}
	}

	Object Scene::GetPrimaryCameraObject()
	{
		auto view = m_Registry.view<Camera>();	//������Camera���������
		for (auto object : view) {
			const auto& camera = view.get<Camera>(object);	//Camera���
			//�����
			if (camera.m_Primary) {
				return Object{ object, this };	//���ʵ��
			}
		}

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Object object, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<Transform>(Object object, Transform& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Light>(Object object, Light& light)
	{
		
	}

	/// <summary>
	/// Camera��������ʱ
	/// </summary>
	/// <param name="object">����</param>
	/// <param name="component">Camera���</param>
	template<>
	void Scene::OnComponentAdded<Camera>(Object object, Camera& component)
	{
		component.SetViewportSize(m_ViewportWidth, m_ViewportHeight);	//�����ӿ�
	}

	template<>
	void Scene::OnComponentAdded<SpriteRenderer>(Object object, SpriteRenderer& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Name>(Object object, Name& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScript>(Object object, NativeScript& component)
	{

	}
}