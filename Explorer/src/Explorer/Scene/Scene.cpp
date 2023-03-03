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

	Object Scene::CreateEntity(const std::string& name)
	{
		Object object = { m_Registry.create(), this, name };	//创建实体
		object.AddComponent<Transform>();						//添加Transform组件（默认组件）

		return object;
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);

		for (auto entity : group) {
			auto& [transform, sprite] = group.get<Transform, SpriteRenderer>(entity);

			Renderer2D::DrawQuad(transform.m_Position, transform.m_Rotation.z, transform.m_Scale, sprite.m_Color);
		}
	}
}