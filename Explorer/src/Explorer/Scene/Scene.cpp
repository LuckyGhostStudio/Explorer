#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer3D.h"
#include "Explorer/Components/NativeScript.h"
#include "Explorer/Components/Components.h"
#include "Explorer/Components/SpriteRenderer.h"
#include "Explorer/Components/Rigidbody/Rigidbody2D.h"
#include "Explorer/Components/Rigidbody/BoxCollider2D.h"

#include "Object.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Explorer
{
	static b2BodyType RigidbodyTypeToB2BodyType(Rigidbody2D::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2D::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
			case Rigidbody2D::BodyType::Static: return b2BodyType::b2_staticBody;
			case Rigidbody2D::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
		}

		EXP_CORE_ASSERT(false, "BodyType Not Found!");

		return b2_staticBody;
	}

	Scene::Scene(const std::string& name) :m_Name(name)
	{
		
	}

	Scene::~Scene()
	{

	}

	Object Scene::CreateEmptyObject(const std::string& name, bool enable)
	{
		return CreateEmptyObject(UUID(), name, enable);
	}

	Object Scene::CreateEmptyObject(UUID uuid, const std::string& name, bool enable)
	{
		Object object = { m_Registry.create(), this };	//����������
		object.AddComponent<ID>(uuid);					//���ID�����Ĭ�������
		object.AddComponent<Self>(name, enable);		//���Self�����Ĭ�������
		object.AddComponent<Transform>();				//���Transform�����Ĭ�������

		return object;
	}

	Object Scene::CreateSpriteObject(const std::string& name)
	{
		Object sprite = CreateEmptyObject(name);	//����Sprite

		sprite.AddComponent<SpriteRenderer>();

		return sprite;
	}

	Object Scene::CreateMeshObject(const std::string& name, const Mesh::Type type)
	{
		Object cube = CreateEmptyObject(name);	//����Cube

		cube.AddComponent<Mesh>(type);			//���Mesh�����type��������
		cube.AddComponent<Material>();			//���Material���

		return cube;
	}

	Object Scene::CreateCameraObject(const std::string& name, bool primary)
	{
		Object camera = { m_Registry.create(), this };		//����Camera

		glm::vec3 position = { 2.734f, 1.796f, 3.294f };	//��ʼλ��
		glm::vec3 rotation = { -20.796f, 40.670f, 0.0f };	//��ʼ��ת

		camera.AddComponent<ID>();							//���ID���
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
		 
		Object light = { m_Registry.create(), this };		//����Light

		light.AddComponent<ID>();							//���ID���
		light.AddComponent<Self>(tempName);					//���Self���
		light.AddComponent<Transform>(position, rotation);	//���Transform���
		light.AddComponent<Light>(type);					//���Light�����type���͹�Դ

		return light;
	}

	void Scene::DestroyObject(Object object)
	{
		m_Registry.destroy(object);	//��ע����Ƴ�����
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });	//������������ ������������

		auto view = m_Registry.view<Rigidbody2D>();	//��Rigidbody2D����������
		for (auto obj : view) {
			Object object = { obj, this };

			auto& transform = object.GetComponent<Transform>();
			auto& rigidbody2d = object.GetComponent<Rigidbody2D>();

			b2BodyDef bodyDef;	//box2d���嶨��
			bodyDef.type = RigidbodyTypeToB2BodyType(rigidbody2d.GetBodyType());		//��������
			bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y);	//��ʼλ�� = transformλ��
			bodyDef.angle = transform.GetRotation().z;									//��ʼ��ת�Ƕ� z��

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);		//��������
			body->SetFixedRotation(rigidbody2d.GetFreezeRotation());	//��ת����״̬
			rigidbody2d.SetRuntimeBody(body);							//��������ʱ����

			if (object.HasComponent<BoxCollider2D>()) {
				auto& boxCollider2D = object.GetComponent<BoxCollider2D>();	//Box2D��ײ��

				b2PolygonShape boxShape;	//Box��״
				//Box��С
				float sizeX = boxCollider2D.GetSize().x * 0.5f * transform.GetScale().x;
				float sizeY = boxCollider2D.GetSize().y * 0.5f * transform.GetScale().y;
				boxShape.SetAsBox(sizeX, sizeY);	//����ΪBox

				b2FixtureDef fixtureDef;			//��ײ�嶨��
				fixtureDef.shape = &boxShape;												//��״
				fixtureDef.density = boxCollider2D.GetDensity();							//�ܶ�
				fixtureDef.friction = boxCollider2D.GetFriction();							//Ħ����
				fixtureDef.restitution = boxCollider2D.GetRestitution();					//�ָ�ϵ��
				fixtureDef.restitutionThreshold = boxCollider2D.GetRestitutionThreshold();	//�ָ���ֵ
				body->CreateFixture(&fixtureDef);	//������ײ��
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;		//������������
		m_PhysicsWorld = nullptr;
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

		//-------------Physics----------------------
		{
			const uint32_t velocityIterations = 6;	//�ٶȵ�������
			const uint32_t positionIterations = 2;	//λ�õ�������
			m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);	//����ʱ�䲽 �ٶȺ�λ�õ�������

			auto view = m_Registry.view<Rigidbody2D>();	//��Rigidbody2D����������
			for (auto obj : view) {
				Object object = { obj, this };

				auto& transform = object.GetComponent<Transform>();		//Transform���
				auto& rigidbody2d = object.GetComponent<Rigidbody2D>();	//Rigidbody2D���

				b2Body* body = (b2Body*)rigidbody2d.GetRuntimeBody();	//����ʱ����
				//���ݸ������ݸ���Transform����
				const auto& position = body->GetPosition();
				//transform.SetRotation({ transform.GetRotation().x, transform.GetRotation().y, body->GetAngle() });
				//transform.SetPosition({ position.x, position.y, transform.GetPosition().z });

				//EXP_CORE_TRACE("Transform Rotation z: {0}, Body Rotation z: {1}", transform.GetRotation().z, body->GetAngle());

				transform.GetPosition().x = position.x;
				transform.GetPosition().y = position.y;
				transform.GetRotation().z = body->GetAngle();

				//TODO ��ת��bug
			}
		}

		//-------------Renderer---------------------
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
	void Scene::OnComponentAdded<Rigidbody2D>(Object object, Rigidbody2D& rigidbody2D)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2D>(Object object, BoxCollider2D& boxCollider2D)
	{

	}

	template<>
	void Scene::OnComponentAdded<ID>(Object object, ID& id)
	{

	}

	template<>
	void Scene::OnComponentAdded<Self>(Object object, Self& self)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScript>(Object object, NativeScript& script)
	{

	}
}