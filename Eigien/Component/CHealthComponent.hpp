#ifndef __CHEALTHCOMPONENT_H__
#define __CHEALTHCOMPONENT_H__

#include "IComponent.h"
#include "glog/logging.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace game
{
	class CHealthComponent : public IComponent
	{
	public:
		CHealthComponent();
		~CHealthComponent();
		void update(float value) override;

		void yaw(float angle) override
		{
			LOG(INFO) << "HealthComponent Not Support";
		}
		void pitch(float angle) override
		{
			LOG(INFO) << "HealthComponent Not Support";
		}
		void roll(float angle) override
		{
			LOG(INFO) << "HealthComponent Not Support";
		}
		void scaleUp(const glm::vec3 &v) override
		{
			LOG(INFO) << "HealthComponent Not Support";
		}
		void scaleUp(float x, float y, float z) override
		{
			LOG(INFO) << "HealthComponent Not Support";
		}
		glm::mat4 getModelMatrix() override
		{
			LOG(INFO) << "HealthComponent Not Support";
			glm::mat4 transMatrix(1.0);
			return transMatrix;
		}
	private:
		void __Init();
		void __update(float value);
		float m_fValue;
	};
}

#endif