#pragma once

#include "Component.h"
#include "Explorer/Renderer/Texture.h"

#include "Sprite.h"

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// Í¼Æ¬äÖÈ¾Æ÷×é¼þ
	/// </summary>
	class SpriteRenderer :public Component
	{
	private:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Sprite m_Sprite;
	public:
		SpriteRenderer();

		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4& color) :m_Color(color) {}
		SpriteRenderer(const glm::vec4& color, const Sprite& sprite) :m_Color(color), m_Sprite(sprite) {}

		virtual void SetName() override { m_Name = "Sprite Renderer"; }
		virtual const std::string& GetName() override { SetName(); return m_Name; }

		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("Resources/Icons/Components/SpriteRenderer_Icon.png"); }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		glm::vec4& GetColor() { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		Sprite& GetSprite() { return m_Sprite; }
		void SetSprite(const Sprite& sprite) { m_Sprite = sprite; }
	};

}