#include <memory>

#include "IComponent.h"

namespace game
{
	class ComponentFactory
	{
	public:
		static std::shared_ptr<IComponent> Create(ComponentType Type);
	};
}