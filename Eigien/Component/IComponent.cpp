#include "stdafx.h"

#include "IComponent.h"
#include "ComponentFactory.h"

namespace game
{
	std::shared_ptr<IComponent> IComponent::Create(ComponentType Type)
	{
		return ComponentFactory::Create(Type);
	}
	void IComponent::Destroy(std::shared_ptr<IComponent> obj)
	{
		obj.reset();
	}
}