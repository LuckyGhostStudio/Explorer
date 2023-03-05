#include "exppch.h"
#include "Object.h"

namespace Explorer
{
	Object::Object(entt::entity objectID, Scene* scene) 
		:m_ObjectID(objectID), m_Scene(scene)
	{

	}
}