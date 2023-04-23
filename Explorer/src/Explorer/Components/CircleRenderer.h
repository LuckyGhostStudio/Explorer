#pragma once

#include "Component.h"
#include "Circle.h"

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// Ô²ÐÎäÖÈ¾Æ÷×é¼þ
	/// </summary>
	class CircleRenderer :public Component
	{
	private:
		Circle m_Circle;
	public:
		CircleRenderer() = default;
		CircleRenderer(const CircleRenderer&) = default;
		CircleRenderer(const Circle& circle) :m_Circle(circle) {};
	
		virtual void SetName() override { m_Name = "Circle Renderer"; }
		virtual const std::string& GetName() override { SetName(); return m_Name; }
		//TOTO ICON
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("Resources/Icons/Components/CircleRenderer_Icon.png"); }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		Circle& GetCircle() { return m_Circle; }
		void SetCircle(const Circle& circle) { m_Circle = circle; }
	};
}