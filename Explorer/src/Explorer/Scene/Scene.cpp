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
	/// 复制组件：复制源注册表中Component类型的所有组件到目的注册表
	/// </summary>
	/// <typeparam name="Component">组件类型</typeparam>
	/// <param name="dst">目的注册表</param>
	/// <param name="src">源注册表</param>
	/// <param name="objectMap">UUID-物体 Map</param>
	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity> objectMap)
	{
		auto view = src.view<Component>();	//源注册表 有Component类型组件的所有实体

		for (auto object : view) {
			UUID uuid = src.get<ID>(object).GetID();		//源uuid
			EXP_CORE_ASSERT(objectMap.find(uuid) != objectMap.end(), "UUID Not Found in Map!");
			entt::entity dstObjectID = objectMap.at(uuid);	//对应uuid的目的物体ID

			auto& component = src.get<Component>(object);	//待复制组件
			dst.emplace_or_replace<Component>(dstObjectID, component);	//添加或替换 目的注册表 的dstObjectID的component组件
		}
	}

	/// <summary>
	/// 复制组件：复制源物体组件到目的物体
	/// </summary>
	/// <typeparam name="Component">组件类型</typeparam>
	/// <param name="dst">目的物体</param>
	/// <param name="src">源物体</param>
	template<typename Component>
	static void CopyComponent(Object dst, Object src)
	{
		if (src.HasComponent<Component>()) dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	}

	void Scene::CopyObject(Object object)
	{
		std::string name = object.GetName();
		Object newObject = CreateEmptyObject(name, object.GetEnable());	//创建空物体

		//复制组件 新物体<-源物体
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
		std::shared_ptr<Scene> newScene = std::make_shared<Scene>(scene->GetName());	//创建新场景
		
		//视口大小
		newScene->m_ViewportHeight = scene->m_ViewportHeight;
		newScene->m_ViewportWidth = scene->m_ViewportWidth;

		newScene->m_Environment = scene->m_Environment;		//环境
		newScene->m_Gravity = scene->m_Gravity;				//重力加速度

		auto& srcSceneRegistry = scene->m_Registry;		//源场景注册表
		auto& dstSceneRegistry = newScene->m_Registry;	//目的场景注册表

		std::unordered_map<UUID, entt::entity> objectMap;	//UUID-object Map

		//在新场景中创建物体
		auto idView = srcSceneRegistry.view<ID>();		//源场景 有ID组件的所有实体
		for (auto object : idView) {
			UUID uuid = srcSceneRegistry.get<ID>(object).GetID();					//物体UUID
			const auto& name = srcSceneRegistry.get<Self>(object).GetObjectName();	//物体名
			bool enable = srcSceneRegistry.get<Self>(object).GetObjectEnable();		//物体启用状态
			
			objectMap[uuid] = newScene->CreateEmptyObject(uuid, name, enable);	//新场景内创建空物体 添加 uuid-新物体 到Map
		}

		//复制组件 目的注册<-表源注册表
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

	Object Scene::CreateCircleObject(const std::string& name)
	{
		Object circle = CreateEmptyObject(name);	//创建circle

		circle.AddComponent<CircleRenderer>();

		return circle;
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

	Object Scene::CreateSphereObjectToRayTracing(const std::string& name)
	{
		Object sphere = CreateEmptyObject(name);	//创建Sphere

		RayTracing::PBRMaterial material;
		material.Albedo = { 0.8, 0.8, 0.8 };
		material.Roughness = 1.0f;
		m_RayTracingScene.Materials.push_back(material);	//添加材质

		RayTracing::Sphere s;
		s.Position = sphere.GetComponent<Transform>().GetPosition();
		s.MaterialIndex = m_RayTracingScene.Materials.size() - 1;
		m_RayTracingScene.Spheres.push_back(s);		//添加球体

		m_RayTracingScene.SphereObjects.push_back(sphere);	//添加物体

		return sphere;
	}

	void Scene::DestroyObject(Object object)
	{
		m_Registry.destroy(object);	//从注册表移除物体
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ m_Gravity.x, m_Gravity.y });	//创建物理世界

		auto view = m_Registry.view<Rigidbody2D>();	//有Rigidbody2D的所有物体
		for (auto obj : view) {
			Object object = { obj, this };
			//object启用
			if (object.GetEnable()) {
				auto& transform = object.GetComponent<Transform>();
				auto& rigidbody2d = object.GetComponent<Rigidbody2D>();

				b2BodyDef bodyDef;	//box2d刚体定义
				bodyDef.type = RigidbodyTypeToB2BodyType(rigidbody2d.GetBodyType());		//刚体类型
				bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y);	//初始位置 = transform位置
				float angle = transform.GetRotation().z;
				bodyDef.angle = glm::radians(transform.GetRotation().z);					//初始旋转 弧度 z轴

				b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);		//创建刚体
				body->SetFixedRotation(rigidbody2d.GetFreezeRotation());	//旋转冻结状态
				rigidbody2d.SetRuntimeBody(body);							//设置运行时刚体

				//BoxCollider2D
				if (object.HasComponent<BoxCollider2D>()) {
					auto& boxCollider2D = object.GetComponent<BoxCollider2D>();	//Box2D碰撞体
					//BoxCollider2D组件启用
					if (boxCollider2D.GetEnable()) {
						b2PolygonShape boxShape;	//Box形状
						//Box大小
						float sizeX = transform.GetScale().x * boxCollider2D.GetSize().x * 0.5f;
						float sizeY = transform.GetScale().y * boxCollider2D.GetSize().y * 0.5f;
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

				//CircleCollider2D
				if (object.HasComponent<CircleCollider2D>()) {
					auto& circleCollider2D = object.GetComponent<CircleCollider2D>();	//Box2D碰撞体
					//CircleCollider2D组件启用
					if (circleCollider2D.GetEnable()) {
						b2CircleShape circleShape;	//Circle形状
						circleShape.m_p.Set(circleCollider2D.GetOffset().x, circleCollider2D.GetOffset().y);	//碰撞体位置 偏移量
						circleShape.m_radius = glm::max(transform.GetScale().x, transform.GetScale().y) * circleCollider2D.GetRadius();	//半径 随缩放值改变

						b2FixtureDef fixtureDef;			//碰撞体定义
						fixtureDef.shape = &circleShape;												//形状
						fixtureDef.density = circleCollider2D.GetDensity();								//密度
						fixtureDef.friction = circleCollider2D.GetFriction();							//摩擦力
						fixtureDef.restitution = circleCollider2D.GetRestitution();						//恢复系数
						fixtureDef.restitutionThreshold = circleCollider2D.GetRestitutionThreshold();	//恢复阈值
						body->CreateFixture(&fixtureDef);	//创建碰撞体
					}
				}
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

		//光线追踪渲染 TODO
		if (Renderer::s_Type == Renderer::Type::Raytracing) {
			RayTracing::Renderer::BeginScene(camera);
			RayTracing::Renderer::Render(m_RayTracingScene);
			return;
		}

		//-----------------Mesh---------------------------
		Renderer3D::BeginScene(m_Environment, camera, lightObjects);	//开始渲染场景
		auto meshes = m_Registry.view<Transform, Mesh, Material>();		//返回有Transform Mesh Material的所有物体

		for (auto object : meshes) {
			Object obj = Object{ object, this };
			//object启用时渲染此Obj
			if (obj.GetEnable()) {
				auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

				Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//绘制网格
			}
		}
		Renderer3D::EndScene(m_Environment, camera);	//结束渲染场景

		//=================2D Renderer======================
		Renderer3D::BeginScene(camera);									//开始渲染场景
		{
			//-----------------Sprite--------------------------
			auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//返回有Transform SpriteRenderer的所有物体 TODO 待添加Material

			for (auto object : sprites) {
				Object obj = Object{ object, this };
				//object启用时渲染此Obj
				if (obj.GetEnable()) {
					auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

					Renderer3D::DrawSprite(transform, sprite, (int)object);	//绘制Sprite
				}
			}

			//-----------------Circle--------------------------
			auto circles = m_Registry.view<Transform, CircleRenderer>();	//返回有Transform CircleRenderer的所有物体

			for (auto object : circles) {
				Object obj = Object{ object, this };
				//object启用时渲染此Obj
				if (obj.GetEnable()) {
					auto [transform, circle] = circles.get<Transform, CircleRenderer>(object);

					Renderer3D::DrawCircle(transform, circle, (int)object);	//绘制Circle
				}
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
				//object启用时渲染此Obj
				if (object.GetEnable()) {
					auto& transform = object.GetComponent<Transform>();		//Transform组件
					auto& rigidbody2d = object.GetComponent<Rigidbody2D>();	//Rigidbody2D组件

					b2Body* body = (b2Body*)rigidbody2d.GetRuntimeBody();	//运行时刚体
					//根据刚体数据更新Transform数据
					const auto& position = body->GetPosition();

					transform.GetPosition().x = position.x;
					transform.GetPosition().y = position.y;
					transform.GetRotation().z = glm::degrees(body->GetAngle());
				}
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
			//光线追踪渲染TODO
			if (Renderer::s_Type == Renderer::Type::Raytracing) {
				RayTracing::Renderer::BeginScene(*mainCamera, *cameraTransform, { m_ViewportWidth, m_ViewportWidth });
				RayTracing::Renderer::Render(m_RayTracingScene);
				return;
			}

			//-----------------Mesh---------------------------
			Renderer3D::BeginScene(m_Environment, *mainCamera, *cameraTransform, lightObjects);	//开始渲染场景
			
			auto meshes = m_Registry.view<Transform, Mesh, Material>();	//返回有Transform Mesh Material的所有物体

			for (auto object : meshes) {
				Object obj = Object{ object, this };
				//object启用时渲染此Obj
				if (obj.GetEnable()) {
					auto [transform, mesh, material] = meshes.get<Transform, Mesh, Material>(object);

					Renderer3D::DrawMesh(transform, mesh, material, (int)object);	//绘制网格
				}
			}

			Renderer3D::EndScene(m_Environment, *mainCamera, *cameraTransform);	//结束渲染场景
			
			//=====================2D Renderer===========================
			Renderer3D::BeginScene(*mainCamera, *cameraTransform);			//开始渲染场景
			{
				//-----------------Sprite--------------------------
				auto sprites = m_Registry.view<Transform, SpriteRenderer>();	//返回有Transform SpriteRenderer的所有物体 TODO 待添加Material
				for (auto object : sprites) {
					Object obj = Object{ object, this };
					//object启用时渲染此Obj
					if (obj.GetEnable()) {
						auto [transform, sprite] = sprites.get<Transform, SpriteRenderer>(object);

						Renderer3D::DrawSprite(transform, sprite, (int)object);	//绘制Sprite
					}
				}

				//-----------------Circle--------------------------
				auto circles = m_Registry.view<Transform, CircleRenderer>();	//返回有Transform CircleRenderer的所有物体
				for (auto object : circles) {
					Object obj = Object{ object, this };
					//object启用时渲染此Obj
					if (obj.GetEnable()) {
						auto [transform, circle] = circles.get<Transform, CircleRenderer>(object);

						Renderer3D::DrawCircle(transform, circle, (int)object);	//绘制Circle
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