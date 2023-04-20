#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer2D.h"
#include "Explorer/Renderer/Renderer3D.h"
#include "Explorer/Components/Components.h"
#include "Explorer/Components/SpriteRenderer.h"

#include "Object.h"

namespace Explorer
{
	Scene::Scene(const std::string& name) :m_Name(name)
	{
		
	}

	Scene::~Scene()
	{

	}

	Object Scene::CreateEmptyObject(const std::string& name, bool enable)
	{
		Object object = { m_Registry.create(), this };	//����������
		object.AddComponent<Self>(name, enable);		//���Self�����Ĭ�������
		object.AddComponent<Transform>();				//���Transform�����Ĭ�������

		return object;
	}

	Object Scene::CreateSpriteObject(const std::string& name, bool enable)
	{
		Object sprite = { m_Registry.create(), this };	//����Sprite
		
		sprite.AddComponent<Self>(name);
		sprite.AddComponent<Transform>();

		sprite.AddComponent<SpriteRenderer>();

		return sprite;
	}

	Object Scene::CreateMeshObject(const std::string& name, const Mesh::Type type)
	{
		Object cube = { m_Registry.create(), this };	//����Cube

		cube.AddComponent<Self>(name);
		cube.AddComponent<Transform>();

		cube.AddComponent<Mesh>(type);	//���Mesh�����type��������
		cube.AddComponent<Material>();	//���Material���

		return cube;
	}

	Object Scene::CreateCameraObject(const std::string& name, bool primary)
	{
		Object camera = { m_Registry.create(), this };		//����Camera

		glm::vec3 position = { 2.734f, 1.796f, 3.294f };	//��ʼλ��
		glm::vec3 rotation = { -20.796f, 40.670f, 0.0f };	//��ʼ��ת

		camera.AddComponent<Self>(name);					//���Self���
		camera.AddComponent<Transform>(position, rotation);	//���Transform���
		camera.AddComponent<Camera>().SetPrimary(primary);	//���Camera�����Ĭ�ϲ��������
		//TODO:���Ƴ����������ֻ��һ��
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
				position = { 2.0f, 2.95f, -1.5f };		//���ó�ʼλ��
				break;
			case Light::Type::Spot:			//�۹�Դ
				tempName = "Spot Light";
				rotation = { -90.0f, 0.0f, 0.0f };		//���ó�ʼ��תֵ
				break;
		}
		 
		Object lightObj = { m_Registry.create(), this };		//����Light

		lightObj.AddComponent<Self>(tempName);					//���Self���
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

		//-----------------Mesh---------------------------
		Renderer3D::BeginScene(m_Environment, camera, lightObjects);	//��ʼ��Ⱦ����
		auto meshes = m_Registry.view<Transform, Mesh, Material>();		//������Transform Mesh Material����������

		for (auto object : meshes) {
			Object obj = Object{ object, this };
			//object����ʱ��Ⱦ��Obj
			if (obj.GetComponent<Self>().GetObjectEnable()) {
				auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

				Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//��������
			}
		}
		Renderer3D::EndScene(m_Environment, camera);	//������Ⱦ����

		//-----------------Sprite--------------------------
		Renderer3D::BeginScene(camera);									//��ʼ��Ⱦ����
		auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//������Transform SpriteRenderer���������� TODO �����Material
		
		for (auto object : sprites) {
			Object obj = Object{ object, this };
			//object����ʱ��Ⱦ��Obj
			if (obj.GetComponent<Self>().GetObjectEnable()) {
				auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

				Renderer3D::DrawSprite(transform, sprite, (int)object);	//����Sprite
			}
		}
		Renderer3D::EndScene();
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

		auto lights = m_Registry.view<Light>();	//����ӵ��Light���������
		std::vector<Object> lightObjects;		//��������Light����
		lightObjects.reserve(lights.size());	//Ԥ���ռ�

		//������������Light����
		for (auto object : lights) {
			lightObjects.push_back(Object{ object, this });	//��ӵ�Light�б�
		}

		Camera* mainCamera = nullptr;	//�����
		Transform* cameraTransform = nullptr;

		auto view = m_Registry.view<Transform, Camera>();	//������Transform��Camera����������

		for (auto object : view) {
			auto [transform, camera] = view.get<Transform, Camera>(object);

			//�ҵ������
			if (camera.IsPrimary()) {
				mainCamera = &camera;
				cameraTransform = &transform;
				break;
			}
		}

		//��������� && �����������
		if (mainCamera && mainCamera->GetEnable()) {
			//-----------------Mesh---------------------------
			Renderer3D::BeginScene(m_Environment, *mainCamera, *cameraTransform, lightObjects);	//��ʼ��Ⱦ����
			
			auto meshes = m_Registry.view<Transform, Mesh, Material>();	//������Transform Mesh Material����������

			for (auto object : meshes) {
				Object obj = Object{ object, this };
				//object����ʱ��Ⱦ��Obj
				if (obj.GetComponent<Self>().GetObjectEnable()) {
					auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

					Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//��������
				}
			}

			Renderer3D::EndScene(m_Environment, *mainCamera, *cameraTransform);	//������Ⱦ����

			//-----------------Sprite--------------------------
			Renderer3D::BeginScene(*mainCamera, *cameraTransform);			//��ʼ��Ⱦ����
			auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//������Transform SpriteRenderer���������� TODO �����Material

			for (auto object : sprites) {
				Object obj = Object{ object, this };
				//object����ʱ��Ⱦ��Obj
				if (obj.GetComponent<Self>().GetObjectEnable()) {
					auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

					Renderer3D::DrawSprite(transform, sprite, (int)object);	//����Sprite
				}
			}
			Renderer3D::EndScene();
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
			auto& camera = view.get<Camera>(object);	//Camera���
			//�����
			if (camera.IsPrimary()) {
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
	void Scene::OnComponentAdded<Transform>(Object object, Transform& transform)
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
	void Scene::OnComponentAdded<Camera>(Object object, Camera& camera)
	{
		camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);	//�����ӿ�
	}

	template<>
	void Scene::OnComponentAdded<Mesh>(Object object, Mesh& mesh)
	{

	}

	template<>
	void Scene::OnComponentAdded<Material>(Object object, Material& material)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRenderer>(Object object, SpriteRenderer& spriteRenderer)
	{

	}

	template<>
	void Scene::OnComponentAdded<Self>(Object object, Self& name)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScript>(Object object, NativeScript& script)
	{

	}
}