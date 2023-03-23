#pragma once

#include "Explorer/Scene/ScriptableObject.h"

namespace Explorer
{
	/// <summary>
	/// �������
	/// </summary>
	class Name
	{
	public:
		std::string m_Name;

		Name() = default;
		Name(const Name&) = default;
		Name(const std::string& name) :m_Name(name) {}
	};

	/// <summary>
	/// ͼƬ��Ⱦ�����
	/// </summary>
	class SpriteRenderer
	{
	public:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4& color) :m_Color(color) {}
	};

	/// <summary>
	/// ���ؽű����
	/// </summary>
	class NativeScript
	{
	public:
		ScriptableObject* Instance = nullptr;		//�ɽű���ʵ��ʵ�������ڵ��ýű��ڵĺ���

		ScriptableObject* (*InstantiateScript)();	//ʵ�����ű�����������ʵ����ScriptableEntity�ű�
		void (*DestroyScript)(NativeScript*);		//���ٽű���������������ScriptableEntity�ű���ʵ��

		/// <summary>
		/// ��T���͵Ľű����ű����
		/// </summary>
		/// <typeparam name="T">�ű�����</typeparam>
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableObject*>(new T()); };	//ʵ�����ű���
			DestroyScript = [](NativeScript* script) { delete script->Instance; script->Instance = nullptr; };	//���ٽű����ʵ��
		}
	};
}