#include "stdafx.h"

#include <memory>
#include "ComponentFactory.h"
#include "CHealthComponent.hpp"
#include "CTransformComponent.hpp"

namespace game
{
	std::shared_ptr<IComponent> ComponentFactory::Create(ComponentType Type)
	{
		switch (Type)
		{
		case game::Health:
			return std::shared_ptr<IComponent>(new CHealthComponent);
			break;
		case game::Transform:
			return std::shared_ptr<IComponent>(new CTransformComponent);
			break;
		default:
			return std::shared_ptr<IComponent>();
		}
	}
}