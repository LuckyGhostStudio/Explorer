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

	Object Scene::CreateCubeObject(const std::string& name)
	{
		Object cube = { m_Registry.create(), this };	//����Cube

		cube.AddComponent<Name>(name);
		cube.AddComponent<Transform>();

		//�����嶥��
		std::vector<Vertex> vertices =
		{
			// ---------λ��---------	----------��ɫ----------		---------����--------	---��������---  ID objID
			{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 0 x+
			{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 1 y-
			{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 2 z+
				 													  										 
			{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f },	{ 0.0f, 1.0f }, 1, 0 },	// B 3
			{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 0.0f, 1.0f }, 1, 0 },	// B 4
			{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 0.0f, 1.0f }, 1, 0 },	// B 5
				 													  										
			{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f },	{ 1.0f, 1.0f }, 2, 0 },	// C 6
			{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 1.0f }, 2, 0 },	// C 7
			{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 1.0f, 1.0f }, 2, 0 },	// C 8
				 													  										
			{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, 3, 0 },	// D 9
			{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f }, 3, 0 },	// D 10
			{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f }, 3, 0 },	// D 11
																											
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 0.0f, 0.0f }, 4, 0 },	// E 12
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 0.0f, 0.0f }, 4, 0 },	// E 13
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f },	{ 0.0f, 0.0f }, 4, 0 },	// E 14
																											
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 0.0f, 1.0f }, 5, 0 },	// F 15
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 0.0f, 1.0f }, 5, 0 },	// F 16
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 0.0f, 1.0f }, 5, 0 },	// F 17
																											
			{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 1.0f, 1.0f }, 6, 0 },	// G 18
			{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 1.0f }, 6, 0 },	// G 19
			{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 1.0f, 1.0f }, 6, 0 },	// G 20
																											
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 1.0f, 0.0f }, 7, 0 },	// H 21
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 0.0f }, 7, 0 },	// H 22
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f },	{ 1.0f, 0.0f }, 7, 0 },	// H 23
		};
		 
		//��������
		std::vector<uint32_t> indices =
		{
			0, 3, 6,	// A B C x+
			6, 9, 0,	// C D A x+
			18, 15, 12,	// G F E x-
			18, 21, 12,	// G H E x-
			22, 7, 19,	// H C G y+
			7, 10, 22,	// C D H y+
			13, 16, 4,	// E F B y-
			4, 1, 13,	// B A E y-
			23, 14, 2,	// H E A z+
			2, 11, 23,	// A D H z+
			20, 5, 17,	// G B F z-
			5, 20, 8,	// B G C z-
		};

		Mesh mesh({ SubMesh(vertices, indices) }, Mesh::Type::Cube);	//����Cube����

		cube.AddComponent<Mesh>(mesh);			//���Mesh���

		return cube;
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

		auto meshes = m_Registry.view<Transform, Mesh>();
		//auto meshes = m_Registry.group<Transform>(entt::get<Mesh>);	//������Transform��Mesh����������

		for (auto object : meshes) {
			auto [transform, mesh] = meshes.get<Transform, Mesh>(object);

			Renderer3D::DrawMesh(transform, mesh, (int)object);	//��������
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
	void Scene::OnComponentAdded<SpriteRenderer>(Object object, SpriteRenderer& spriteRenderer)
	{

	}

	template<>
	void Scene::OnComponentAdded<Name>(Object object, Name& name)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScript>(Object object, NativeScript& script)
	{

	}
}