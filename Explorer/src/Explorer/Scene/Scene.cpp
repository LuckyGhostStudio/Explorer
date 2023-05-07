#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer.h"
#include "Explorer/Renderer/Renderer3D.h"
#include "Explorer/Renderer/RendererRayTracing.h"
#include "Explorer/Components/NativeScript.h"
#include "Explorer/Components/Components.h"
#include "Explorer/Components/SpriteRenderer.h"
#include "Explorer/Components/CircleRenderer.h"
#include "Explorer/Components/Rigidbody/Rigidbody2D.h"
#include "Explorer/Components/Rigidbody/BoxCollider2D.h"
#include "Explorer/Components/Rigidbody/CircleCollider2D.h"

#include "Object.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

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

	/// <summary>
	/// �������������Դע�����Component���͵����������Ŀ��ע���
	/// </summary>
	/// <typeparam name="Component">�������</typeparam>
	/// <param name="dst">Ŀ��ע���</param>
	/// <param name="src">Դע���</param>
	/// <param name="objectMap">UUID-���� Map</param>
	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity> objectMap)
	{
		auto view = src.view<Component>();	//Դע��� ��Component�������������ʵ��

		for (auto object : view) {
			UUID uuid = src.get<ID>(object).GetID();		//Դuuid
			EXP_CORE_ASSERT(objectMap.find(uuid) != objectMap.end(), "UUID Not Found in Map!");
			entt::entity dstObjectID = objectMap.at(uuid);	//��Ӧuuid��Ŀ������ID

			auto& component = src.get<Component>(object);	//���������
			dst.emplace_or_replace<Component>(dstObjectID, component);	//��ӻ��滻 Ŀ��ע��� ��dstObjectID��component���
		}
	}

	/// <summary>
	/// �������������Դ���������Ŀ������
	/// </summary>
	/// <typeparam name="Component">�������</typeparam>
	/// <param name="dst">Ŀ������</param>
	/// <param name="src">Դ����</param>
	template<typename Component>
	static void CopyComponent(Object dst, Object src)
	{
		if (src.HasComponent<Component>()) dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	}

	void Scene::CopyObject(Object object)
	{
		std::string name = object.GetName();
		Object newObject = CreateEmptyObject(name, object.GetEnable());	//����������

		//������� ������<-Դ����
		CopyComponent<Transform>(newObject, object);
		CopyComponent<Camera>(newObject, object);
		CopyComponent<Light>(newObject, object);
		CopyComponent<Mesh>(newObject, object);
		CopyComponent<Material>(newObject, object);
		CopyComponent<SpriteRenderer>(newObject, object);
		CopyComponent<CircleRenderer>(newObject, object);
		CopyComponent<NativeScript>(newObject, object);
		CopyComponent<Rigidbody2D>(newObject, object);
		CopyComponent<BoxCollider2D>(newObject, object);
		CopyComponent<CircleCollider2D>(newObject, object);
	}

	std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<Scene> newScene = std::make_shared<Scene>(scene->GetName());	//�����³���
		
		//�ӿڴ�С
		newScene->m_ViewportHeight = scene->m_ViewportHeight;
		newScene->m_ViewportWidth = scene->m_ViewportWidth;

		newScene->m_Environment = scene->m_Environment;		//����
		newScene->m_Gravity = scene->m_Gravity;				//�������ٶ�

		auto& srcSceneRegistry = scene->m_Registry;		//Դ����ע���
		auto& dstSceneRegistry = newScene->m_Registry;	//Ŀ�ĳ���ע���

		std::unordered_map<UUID, entt::entity> objectMap;	//UUID-object Map

		//���³����д�������
		auto idView = srcSceneRegistry.view<ID>();		//Դ���� ��ID���������ʵ��
		for (auto object : idView) {
			UUID uuid = srcSceneRegistry.get<ID>(object).GetID();					//����UUID
			const auto& name = srcSceneRegistry.get<Self>(object).GetObjectName();	//������
			bool enable = srcSceneRegistry.get<Self>(object).GetObjectEnable();		//��������״̬
			
			objectMap[uuid] = newScene->CreateEmptyObject(uuid, name, enable);	//�³����ڴ��������� ��� uuid-������ ��Map
		}

		//������� Ŀ��ע��<-��Դע���
		CopyComponent<Transform>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<Camera>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<Light>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<Mesh>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<Material>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<SpriteRenderer>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<CircleRenderer>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<NativeScript>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<Rigidbody2D>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<BoxCollider2D>(dstSceneRegistry, srcSceneRegistry, objectMap);
		CopyComponent<CircleCollider2D>(dstSceneRegistry, srcSceneRegistry, objectMap);

		return newScene;
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

	Object Scene::CreateCircleObject(const std::string& name)
	{
		Object circle = CreateEmptyObject(name);	//����circle

		circle.AddComponent<CircleRenderer>();

		return circle;
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

	Object Scene::CreateSphereObjectToRayTracing(const std::string& name)
	{
		Object sphere = CreateEmptyObject(name);	//����Sphere

		RayTracing::PBRMaterial material;
		material.Albedo = { 0.8, 0.8, 0.8 };
		material.Roughness = 1.0f;
		m_RayTracingScene.Materials.push_back(material);	//��Ӳ���

		RayTracing::Sphere s;
		s.Position = sphere.GetComponent<Transform>().GetPosition();
		s.MaterialIndex = m_RayTracingScene.Materials.size() - 1;
		m_RayTracingScene.Spheres.push_back(s);		//�������

		m_RayTracingScene.SphereObjects.push_back(sphere);	//�������

		return sphere;
	}

	void Scene::DestroyObject(Object object)
	{
		m_Registry.destroy(object);	//��ע����Ƴ�����
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ m_Gravity.x, m_Gravity.y });	//������������

		auto view = m_Registry.view<Rigidbody2D>();	//��Rigidbody2D����������
		for (auto obj : view) {
			Object object = { obj, this };
			//object����
			if (object.GetEnable()) {
				auto& transform = object.GetComponent<Transform>();
				auto& rigidbody2d = object.GetComponent<Rigidbody2D>();

				b2BodyDef bodyDef;	//box2d���嶨��
				bodyDef.type = RigidbodyTypeToB2BodyType(rigidbody2d.GetBodyType());		//��������
				bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y);	//��ʼλ�� = transformλ��
				float angle = transform.GetRotation().z;
				bodyDef.angle = glm::radians(transform.GetRotation().z);					//��ʼ��ת ���� z��

				b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);		//��������
				body->SetFixedRotation(rigidbody2d.GetFreezeRotation());	//��ת����״̬
				rigidbody2d.SetRuntimeBody(body);							//��������ʱ����

				//BoxCollider2D
				if (object.HasComponent<BoxCollider2D>()) {
					auto& boxCollider2D = object.GetComponent<BoxCollider2D>();	//Box2D��ײ��
					//BoxCollider2D�������
					if (boxCollider2D.GetEnable()) {
						b2PolygonShape boxShape;	//Box��״
						//Box��С
						float sizeX = transform.GetScale().x * boxCollider2D.GetSize().x * 0.5f;
						float sizeY = transform.GetScale().y * boxCollider2D.GetSize().y * 0.5f;
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

				//CircleCollider2D
				if (object.HasComponent<CircleCollider2D>()) {
					auto& circleCollider2D = object.GetComponent<CircleCollider2D>();	//Box2D��ײ��
					//CircleCollider2D�������
					if (circleCollider2D.GetEnable()) {
						b2CircleShape circleShape;	//Circle��״
						circleShape.m_p.Set(circleCollider2D.GetOffset().x, circleCollider2D.GetOffset().y);	//��ײ��λ�� ƫ����
						circleShape.m_radius = glm::max(transform.GetScale().x, transform.GetScale().y) * circleCollider2D.GetRadius();	//�뾶 ������ֵ�ı�

						b2FixtureDef fixtureDef;			//��ײ�嶨��
						fixtureDef.shape = &circleShape;												//��״
						fixtureDef.density = circleCollider2D.GetDensity();								//�ܶ�
						fixtureDef.friction = circleCollider2D.GetFriction();							//Ħ����
						fixtureDef.restitution = circleCollider2D.GetRestitution();						//�ָ�ϵ��
						fixtureDef.restitutionThreshold = circleCollider2D.GetRestitutionThreshold();	//�ָ���ֵ
						body->CreateFixture(&fixtureDef);	//������ײ��
					}
				}
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

		//����׷����Ⱦ TODO
		if (Renderer::s_Type == Renderer::Type::Raytracing) {
			RayTracing::Renderer::BeginScene(camera);
			RayTracing::Renderer::Render(m_RayTracingScene);
			return;
		}

		//-----------------Mesh---------------------------
		Renderer3D::BeginScene(m_Environment, camera, lightObjects);	//��ʼ��Ⱦ����
		auto meshes = m_Registry.view<Transform, Mesh, Material>();		//������Transform Mesh Material����������

		for (auto object : meshes) {
			Object obj = Object{ object, this };
			//object����ʱ��Ⱦ��Obj
			if (obj.GetEnable()) {
				auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

				Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//��������
			}
		}
		Renderer3D::EndScene(m_Environment, camera);	//������Ⱦ����

		//=================2D Renderer======================
		Renderer3D::BeginScene(camera);									//��ʼ��Ⱦ����
		{
			//-----------------Sprite--------------------------
			auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//������Transform SpriteRenderer���������� TODO �����Material

			for (auto object : sprites) {
				Object obj = Object{ object, this };
				//object����ʱ��Ⱦ��Obj
				if (obj.GetEnable()) {
					auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

					Renderer3D::DrawSprite(transform, sprite, (int)object);	//����Sprite
				}
			}

			//-----------------Circle--------------------------
			auto circles = m_Registry.view<Transform, CircleRenderer>();	//������Transform CircleRenderer����������

			for (auto object : circles) {
				Object obj = Object{ object, this };
				//object����ʱ��Ⱦ��Obj
				if (obj.GetEnable()) {
					auto [transform, circle] = circles.get<Transform, CircleRenderer>(object);

					Renderer3D::DrawCircle(transform, circle, (int)object);	//����Circle
				}
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
				//object����ʱ��Ⱦ��Obj
				if (object.GetEnable()) {
					auto& transform = object.GetComponent<Transform>();		//Transform���
					auto& rigidbody2d = object.GetComponent<Rigidbody2D>();	//Rigidbody2D���

					b2Body* body = (b2Body*)rigidbody2d.GetRuntimeBody();	//����ʱ����
					//���ݸ������ݸ���Transform����
					const auto& position = body->GetPosition();

					transform.GetPosition().x = position.x;
					transform.GetPosition().y = position.y;
					transform.GetRotation().z = glm::degrees(body->GetAngle());
				}
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
			//����׷����ȾTODO
			if (Renderer::s_Type == Renderer::Type::Raytracing) {
				RayTracing::Renderer::BeginScene(*mainCamera, *cameraTransform, { m_ViewportWidth, m_ViewportWidth });
				RayTracing::Renderer::Render(m_RayTracingScene);
				return;
			}

			//-----------------Mesh---------------------------
			Renderer3D::BeginScene(m_Environment, *mainCamera, *cameraTransform, lightObjects);	//��ʼ��Ⱦ����
			
			auto meshes = m_Registry.view<Transform, Mesh, Material>();	//������Transform Mesh Material����������

			for (auto object : meshes) {
				Object obj = Object{ object, this };
				//object����ʱ��Ⱦ��Obj
				if (obj.GetEnable()) {
					auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

					Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//��������
				}
			}

			Renderer3D::EndScene(m_Environment, *mainCamera, *cameraTransform);	//������Ⱦ����
			
			//=====================2D Renderer===========================
			Renderer3D::BeginScene(*mainCamera, *cameraTransform);			//��ʼ��Ⱦ����
			{
				//-----------------Sprite--------------------------
				auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//������Transform SpriteRenderer���������� TODO �����Material
				for (auto object : sprites) {
					Object obj = Object{ object, this };
					//object����ʱ��Ⱦ��Obj
					if (obj.GetEnable()) {
						auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

						Renderer3D::DrawSprite(transform, sprite, (int)object);	//����Sprite
					}
				}

				//-----------------Circle--------------------------
				auto circles = m_Registry.view<Transform, CircleRenderer>();	//������Transform CircleRenderer����������
				for (auto object : circles) {
					Object obj = Object{ object, this };
					//object����ʱ��Ⱦ��Obj
					if (obj.GetEnable()) {
						auto [transform, circle] = circles.get<Transform, CircleRenderer>(object);

						Renderer3D::DrawCircle(transform, circle, (int)object);	//����Circle
					}
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
	void Scene::OnComponentAdded<ID>(Object object, ID& id)
	{

	}

	template<>
	void Scene::OnComponentAdded<Self>(Object object, Self& self)
	{

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
	void Scene::OnComponentAdded<CircleRenderer>(Object object, CircleRenderer& circleRenderer)
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
	void Scene::OnComponentAdded<CircleCollider2D>(Object object, CircleCollider2D& circleCollider2D)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScript>(Object object, NativeScript& script)
	{

	}
}