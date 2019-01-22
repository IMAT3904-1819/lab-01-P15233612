#ifndef __CTRANSFORMCOMPONENT_H__
#define __CTRANSFORMCOMPONENT_H__
#include "IComponent.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace game
{
	class CTransformComponent : public IComponent
	{

	public:
		CTransformComponent() : m_position(0), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
		CTransformComponent(const glm::vec3 &pos, const glm::quat &orient) : m_position(pos), m_orientation(orient), m_scale(1.0f){}
		~CTransformComponent();
		void update(float value) override;
		
		glm::mat4 getModelMatrix() override
		{
			glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
			glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
			return transMatrix * rotMatrix * scaleMatrix;
		}

		void translate(const glm::vec3 &v)
		{
			m_position += v;
		}

		void translate(float x, float y, float z)
		{
			m_position += glm::vec3(x, y, z);
		}

		void rotate(float angle, const glm::vec3 &axis)
		{
			m_orientation *= glm::angleAxis(angle, axis * m_orientation);
		}

		void rotate(float angle, float x, float y, float z)
		{
			m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation);
		}

		void scaleUp(const glm::vec3 &v) override
		{
			m_scale += v;
		}

		void scaleUp(float x, float y, float z) override
		{
			m_scale += glm::vec3(x, y, z);
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

	private:
		float m_fValue;
		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::quat m_orientation;
	};
}

#endif