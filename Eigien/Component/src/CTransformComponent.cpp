#include "stdafx.h"

#include "CTransformComponent.hpp"
#include "glog/logging.h"

namespace game
{
	CTransformComponent::~CTransformComponent()
	{
		LOG(INFO) << "TransformComponent Release";
	}

	void CTransformComponent::update(float value)
	{
		LOG(INFO) << "TransformComponent does not support update";
	}
}

