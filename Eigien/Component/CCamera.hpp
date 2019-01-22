#ifndef __CCAMERA_HPP__
#define __CCAMERA_HPP__

#include <memory>
#include "ICamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace game
{
	class CCamera : public ICamera
	{
	public:
		~CCamera();
		void setFOV(float fov) override;
		void lookAt(float x, float y, float z) override
		{
			glm::vec3 target(x, y, z);
			__lookAt(target);
		}
		glm::mat4 getViewMatrix() override
		{
			return glm::translate(glm::mat4_cast(m_orientation), m_position);
		}
		void yaw(float angle) override
		{
			rotate(angle, 0.0f, 1.0f, 0.0f);
		}
		void pitch(float angle) override
		{
			rotate(angle, 1.0f, 0.0f, 0.0f);
		}
		void roll(float angle) override
		{
			rotate(angle, 0.0f, 0.0f, 1.0f);
		}

		glm::vec3 m_position;
		glm::quat m_orientation;
		CCamera() : m_position(0), m_orientation(1, 0, 0, 0), m_fov(45) {}
		CCamera(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_fov(45) {}
		CCamera(const glm::vec3& pos, const glm::quat& orient) : m_position(pos),m_orientation(orient), m_fov(45) {}
		void __lookAt(const glm::vec3& target)
		{
			m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0))));
		}
		void translate(const glm::vec3 &v) { m_position += v * m_orientation; }
		void translate(float x, float y, float z)
		{
			m_position += glm::vec3(x, y, z) * m_orientation;
		}
		void rotate(float angle, const glm::vec3 &axis)
		{
			m_orientation *= glm::angleAxis(angle, axis * m_orientation);
		}
		void rotate(float angle, float x, float y, float z)
		{
			m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation);
		}

	private:
		float m_fov;
	};
}

#endif // !__ICAMERA_H__
