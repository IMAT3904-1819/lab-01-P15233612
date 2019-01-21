#include "stdafx.h"
#include "CCamera.hpp"

namespace game
{
	CCamera::~CCamera()
	{
		
	}

	void CCamera::setFOV(float fov)
	{
		m_fov = fov;
	}

	std::shared_ptr<ICamera> ICamera::Create()
	{
		return std::shared_ptr<ICamera>(new CCamera);
	}

	void ICamera::Destroy(std::shared_ptr<ICamera> obj)
	{
		obj.reset();
	}
}