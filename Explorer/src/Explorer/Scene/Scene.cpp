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
		Object object = { m_Registry.create(), this, name };	//����ʵ��
		object.AddComponent<Transform>();						//���Transform�����Ĭ�������

		return object;
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		Camera* mainCamera = nullptr;	//�����
		Transform* cameraTransform = nullptr;

		auto view = m_Registry.view<Transform, Camera>();	//������Transform��Camera������ʵ��

		for (auto entity : view) {
			auto& [transform, camera] = view.get<Transform, Camera>(entity);

			//�ҵ������
			if (camera.m_Primary) {
				mainCamera = &camera;
				cameraTransform = &transform;
				break;
			}
		}

		//���������
		if (mainCamera) {
			auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);	//������Transform��SpriteRenderer������ʵ��

			Renderer2D::BeginScene(*mainCamera, *cameraTransform);	//��ʼ��Ⱦ����
			for (auto entity : group) {
				auto& [transform, sprite] = group.get<Transform, SpriteRenderer>(entity);

				Renderer2D::DrawQuad(transform.m_Position, transform.m_Rotation.z, transform.m_Scale, sprite.m_Color);
			}
			Renderer2D::EndScene();			//������Ⱦ����
		}
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<Camera>();	//������Camera�����ʵ��

		for (auto entity : view) {
			auto& camera = view.get<Camera>(entity);	//���Camera���
			camera.SetViewportSize(width, height);		//�����ӿڴ�С
		}
	}
}