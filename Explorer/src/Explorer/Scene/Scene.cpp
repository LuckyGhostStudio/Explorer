#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer2D.h"

#include "Components.h"

#include "Object.h"

namespace Explorer
{
	Scene::Scene(const std::string& name) :m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	Object Scene::CreateObject(const std::string& name)
	{
		Object object = { m_Registry.create(), this };	//创建物体
		object.AddComponent<Transform>();				//添加Transform组件（默认组件）
		Name& objName = object.AddComponent<Name>();	//添加Name组件（默认组件）
		objName.m_Name = name;

		return object;
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

		Camera* mainCamera = nullptr;	//主相机
		Transform* cameraTransform = nullptr;

		auto view = m_Registry.view<Transform, Camera>();	//返回有Transform和Camera的所有实体

		for (auto entity : view) {
			auto [transform, camera] = view.get<Transform, Camera>(entity);

			//找到主相机
			if (camera.m_Primary) {
				mainCamera = &camera;
				cameraTransform = &transform;
				break;
			}
		}

		//主相机存在
		if (mainCamera) {
			auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);	//返回有Transform和SpriteRenderer的所有实体

			Renderer2D::BeginScene(*mainCamera, *cameraTransform);	//开始渲染场景
			for (auto entity : group) {
				auto [transform, sprite] = group.get<Transform, SpriteRenderer>(entity);

				Renderer2D::DrawQuad(transform.m_Position, transform.m_Rotation.z, transform.m_Scale, sprite.m_Color);
			}
			Renderer2D::EndScene();			//结束渲染场景
		}
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<Camera>();	//所有有Camera组件的实体

		for (auto entity : view) {
			auto& camera = view.get<Camera>(entity);	//获得Camera组件
			camera.SetViewportSize(width, height);		//设置视口大小
		}
	}
}