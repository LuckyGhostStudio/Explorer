#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer2D.h"

#include "Components.h"

namespace Explorer
{


	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();	//创建实体
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