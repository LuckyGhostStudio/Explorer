#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer2D.h"
#include "Explorer/Renderer/Renderer3D.h"

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
		Object object = { m_Registry.create(), this };	//��������
		object.AddComponent<Transform>();				//���Transform�����Ĭ�������
		Name& objName = object.AddComponent<Name>();	//���Name�����Ĭ�������
		objName.m_Name = name;

		return object;
	}

	void Scene::DestroyEntity(Object object)
	{
		m_Registry.destroy(object);	//��ע����Ƴ�����
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

		auto view = m_Registry.view<Transform, Camera>();	//������Transform��Camera������ʵ��

		for (auto entity : view) {
			auto [transform, camera] = view.get<Transform, Camera>(entity);

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

			Renderer3D::BeginScene(*mainCamera, *cameraTransform);	//��ʼ��Ⱦ����
			for (auto entity : group) {
				auto [transform, sprite] = group.get<Transform, SpriteRenderer>(entity);

				//Renderer2D::DrawQuad(transform, sprite.m_Color);
				Renderer3D::DrawMesh(transform);	//��������
			}
			Renderer3D::EndScene();			//������Ⱦ����
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

	template<typename T>
	void Scene::OnComponentAdded(Object object, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<Transform>(Object object, Transform& component)
	{

	}

	/// <summary>
	/// Camera��������ʱ
	/// </summary>
	/// <param name="object">����</param>
	/// <param name="component">Camera���</param>
	template<>
	void Scene::OnComponentAdded<Camera>(Object object, Camera& component)
	{
		component.SetViewportSize(m_ViewportWidth, m_ViewportHeight);	//�����ӿ�
	}

	template<>
	void Scene::OnComponentAdded<SpriteRenderer>(Object object, SpriteRenderer& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Name>(Object object, Name& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScript>(Object object, NativeScript& component)
	{

	}
}