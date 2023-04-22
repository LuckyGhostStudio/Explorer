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
		Object object = { m_Registry.create(), this };	//创建空物体
		object.AddComponent<ID>(uuid);					//添加ID组件（默认组件）
		object.AddComponent<Self>(name, enable);		//添加Self组件（默认组件）
		object.AddComponent<Transform>();				//添加Transform组件（默认组件）

		return object;
	}

	Object Scene::CreateSpriteObject(const std::string& name)
	{
		Object sprite = CreateEmptyObject(name);	//创建Sprite

		sprite.AddComponent<SpriteRenderer>();

		return sprite;
	}

	Object Scene::CreateMeshObject(const std::string& name, const Mesh::Type type)
	{
		Object cube = CreateEmptyObject(name);	//创建Cube

		cube.AddComponent<Mesh>(type);			//添加Mesh组件（type类型网格）
		cube.AddComponent<Material>();			//添加Material组件

		return cube;
	}

	Object Scene::CreateCameraObject(const std::string& name, bool primary)
	{
		Object camera = { m_Registry.create(), this };		//创建Camera

		glm::vec3 position = { 2.734f, 1.796f, 3.294f };	//初始位置
		glm::vec3 rotation = { -20.796f, 40.670f, 0.0f };	//初始旋转

		camera.AddComponent<ID>();							//添加ID组件
		camera.AddComponent<Self>(name);					//添加Self组件
		camera.AddComponent<Transform>(position, rotation);	//添加Transform组件
		camera.AddComponent<Camera>().SetPrimary(primary);	//添加Camera组件：默认不是主相机
		//TODO:限制场景内主相机只有一个
		return camera;
	}

	Object Scene::CreateLightObject(Light::Type type, const std::string& name)
	{
		std::string tempName = name;
		glm::vec3 position = { 0.0f, 1.5f, 0.0f };			//初始位置
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };			//初始旋转

		switch (type)
		{
			case Light::Type::Directional:	//平行光
				tempName = "Directional Light";
				rotation = { 37.3f, 107.0f, 97.0f };	//设置初始旋转值
				break;
			case Light::Type::Point:		//点光源
				tempName = "Point Light";
				position = { 2.0f, 2.95f, -1.5f };		//设置初始位置
				break;
			case Light::Type::Spot:			//聚光源
				tempName = "Spot Light";
				rotation = { -90.0f, 0.0f, 0.0f };		//设置初始旋转值
				break;
		}
		 
		Object light = { m_Registry.create(), this };		//创建Light

		light.AddComponent<ID>();							//添加ID组件
		light.AddComponent<Self>(tempName);					//添加Self组件
		light.AddComponent<Transform>(position, rotation);	//添加Transform组件
		light.AddComponent<Light>(type);					//添加Light组件：type类型光源

		return light;
	}

	void Scene::DestroyObject(Object object)
	{
		m_Registry.destroy(object);	//从注册表移除物体
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });	//创建物理世界 重力方向向量

		auto view = m_Registry.view<Rigidbody2D>();	//有Rigidbody2D的所有物体
		for (auto obj : view) {
			Object object = { obj, this };

			auto& transform = object.GetComponent<Transform>();
			auto& rigidbody2d = object.GetComponent<Rigidbody2D>();

			b2BodyDef bodyDef;	//box2d刚体定义
			bodyDef.type = RigidbodyTypeToB2BodyType(rigidbody2d.GetBodyType());		//刚体类型
			bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y);	//初始位置 = transform位置
			bodyDef.angle = transform.GetRotation().z;									//初始旋转角度 z轴

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);		//创建刚体
			body->SetFixedRotation(rigidbody2d.GetFreezeRotation());	//旋转冻结状态
			rigidbody2d.SetRuntimeBody(body);							//设置运行时刚体

			if (object.HasComponent<BoxCollider2D>()) {
				auto& boxCollider2D = object.GetComponent<BoxCollider2D>();	//Box2D碰撞体

				b2PolygonShape boxShape;	//Box形状
				//Box大小
				float sizeX = boxCollider2D.GetSize().x * 0.5f * transform.GetScale().x;
				float sizeY = boxCollider2D.GetSize().y * 0.5f * transform.GetScale().y;
				boxShape.SetAsBox(sizeX, sizeY);	//设置为Box

				b2FixtureDef fixtureDef;			//碰撞体定义
				fixtureDef.shape = &boxShape;												//形状
				fixtureDef.density = boxCollider2D.GetDensity();							//密度
				fixtureDef.friction = boxCollider2D.GetFriction();							//摩擦力
				fixtureDef.restitution = boxCollider2D.GetRestitution();					//恢复系数
				fixtureDef.restitutionThreshold = boxCollider2D.GetRestitutionThreshold();	//恢复阈值
				body->CreateFixture(&fixtureDef);	//创建碰撞体
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;		//销毁物理世界
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		auto lights = m_Registry.view<Light>();	//所有拥有Light组件的物体
		
		std::vector<Object> lightObjects;		//场景所有Light对象
		lightObjects.reserve(lights.size());	//预留空间

		//遍历场景所有Light对象
		for (auto object : lights) {
			lightObjects.push_back(Object{ object, this });	//添加到Light列表
		}

		//-----------------Mesh---------------------------
		Renderer3D::BeginScene(m_Environment, camera, lightObjects);	//开始渲染场景
		auto meshes = m_Registry.view<Transform, Mesh, Material>();		//返回有Transform Mesh Material的所有物体

		for (auto object : meshes) {
			Object obj = Object{ object, this };
			//object启用时渲染此Obj
			if (obj.GetComponent<Self>().GetObjectEnable()) {
				auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

				Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//绘制网格
			}
		}
		Renderer3D::EndScene(m_Environment, camera);	//结束渲染场景

		//-----------------Sprite--------------------------
		Renderer3D::BeginScene(camera);									//开始渲染场景
		auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//返回有Transform SpriteRenderer的所有物体 TODO 待添加Material
		
		for (auto object : sprites) {
			Object obj = Object{ object, this };
			//object启用时渲染此Obj
			if (obj.GetComponent<Self>().GetObjectEnable()) {
				auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

				Renderer3D::DrawSprite(transform, sprite, (int)object);	//绘制Sprite
			}
		}
		Renderer3D::EndScene();
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		//遍历所有拥有脚本组件的对象，调用each内的函数
		m_Registry.view<NativeScript>().each([=](auto object, NativeScript& script)
		{
			//脚本未实例化
			if (!script.Instance) {
				script.Instance = script.InstantiateScript();		//实例化脚本
				script.Instance->m_Object = Object{ object, this };	//设置脚本所属的对象

				script.Instance->OnCreate();	//调用脚本的OnCreate函数
			}
			script.Instance->OnUpdate(dt);		//调用脚本的OnOpdate函数
		});

		//-------------Physics----------------------
		{
			const uint32_t velocityIterations = 6;	//速度迭代次数
			const uint32_t positionIterations = 2;	//位置迭代次数
			m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);	//设置时间步 速度和位置迭代次数

			auto view = m_Registry.view<Rigidbody2D>();	//有Rigidbody2D的所有物体
			for (auto obj : view) {
				Object object = { obj, this };

				auto& transform = object.GetComponent<Transform>();		//Transform组件
				auto& rigidbody2d = object.GetComponent<Rigidbody2D>();	//Rigidbody2D组件

				b2Body* body = (b2Body*)rigidbody2d.GetRuntimeBody();	//运行时刚体
				//根据刚体数据更新Transform数据
				const auto& position = body->GetPosition();
				//transform.SetRotation({ transform.GetRotation().x, transform.GetRotation().y, body->GetAngle() });
				//transform.SetPosition({ position.x, position.y, transform.GetPosition().z });

				//EXP_CORE_TRACE("Transform Rotation z: {0}, Body Rotation z: {1}", transform.GetRotation().z, body->GetAngle());

				transform.GetPosition().x = position.x;
				transform.GetPosition().y = position.y;
				transform.GetRotation().z = body->GetAngle();

				//TODO 旋转有bug
			}
		}

		//-------------Renderer---------------------
		auto lights = m_Registry.view<Light>();	//所有拥有Light组件的物体
		std::vector<Object> lightObjects;		//场景所有Light对象
		lightObjects.reserve(lights.size());	//预留空间

		//遍历场景所有Light对象
		for (auto object : lights) {
			lightObjects.push_back(Object{ object, this });	//添加到Light列表
		}

		Camera* mainCamera = nullptr;	//主相机
		Transform* cameraTransform = nullptr;

		auto view = m_Registry.view<Transform, Camera>();	//返回有Transform和Camera的所有物体

		for (auto object : view) {
			auto [transform, camera] = view.get<Transform, Camera>(object);

			//找到主相机
			if (camera.IsPrimary()) {
				mainCamera = &camera;
				cameraTransform = &transform;
				break;
			}
		}

		//主相机存在 && 主相机已启用
		if (mainCamera && mainCamera->GetEnable()) {
			//-----------------Mesh---------------------------
			Renderer3D::BeginScene(m_Environment, *mainCamera, *cameraTransform, lightObjects);	//开始渲染场景
			
			auto meshes = m_Registry.view<Transform, Mesh, Material>();	//返回有Transform Mesh Material的所有物体

			for (auto object : meshes) {
				Object obj = Object{ object, this };
				//object启用时渲染此Obj
				if (obj.GetComponent<Self>().GetObjectEnable()) {
					auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

					Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//绘制网格
				}
			}

			Renderer3D::EndScene(m_Environment, *mainCamera, *cameraTransform);	//结束渲染场景

			//-----------------Sprite--------------------------
			Renderer3D::BeginScene(*mainCamera, *cameraTransform);			//开始渲染场景
			auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//返回有Transform SpriteRenderer的所有物体 TODO 待添加Material

			for (auto object : sprites) {
				Object obj = Object{ object, this };
				//object启用时渲染此Obj
				if (obj.GetComponent<Self>().GetObjectEnable()) {
					auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

					Renderer3D::DrawSprite(transform, sprite, (int)object);	//绘制Sprite
				}
			}
			Renderer3D::EndScene();
		}
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<Camera>();	//所有有Camera组件的物体

		for (auto object : view) {
			auto& camera = view.get<Camera>(object);	//获得Camera组件
			camera.SetViewportSize(width, height);		//设置视口大小
		}
	}

	Object Scene::GetPrimaryCameraObject()
	{
		auto view = m_Registry.view<Camera>();	//返回有Camera组件的物体
		for (auto object : view) {
			auto& camera = view.get<Camera>(object);	//Camera组件
			//主相机
			if (camera.IsPrimary()) {
				return Object{ object, this };	//相机实体
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
	/// Camera组件被添加时
	/// </summary>
	/// <param name="object">物体</param>
	/// <param name="component">Camera组件</param>
	template<>
	void Scene::OnComponentAdded<Camera>(Object object, Camera& camera)
	{
		camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);	//设置视口
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