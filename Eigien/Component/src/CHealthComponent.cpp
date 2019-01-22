#include "stdafx.h"
#include "CHealthComponent.hpp"
#include "glog/logging.h"

namespace game
{
	CHealthComponent::CHealthComponent()
	{
		__Init();
	}

	CHealthComponent::~CHealthComponent()
	{
		LOG(INFO) << "HealthComponent Release";
	}

	void CHealthComponent::update(float value)
	{
		__update(value);
	}

	void CHealthComponent::__Init()
	{
		m_fValue = 0;
		LOG(INFO) << "HealthComponent Create";
	}

	void CHealthComponent::__update(float value)
	{
		m_fValue += value;
	}
}

