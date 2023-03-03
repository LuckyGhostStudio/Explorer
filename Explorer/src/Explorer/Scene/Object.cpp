#include "exppch.h"
#include "Object.h"

namespace Explorer
{
	Object::Object(entt::entity objectID, Scene* scene, const std::string& name) 
		:m_ObjectID(objectID), m_Scene(scene), m_Name(name)
	{

	}
}