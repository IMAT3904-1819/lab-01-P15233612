/**
* @file			:	IComponent.h
* @author		:
* @date			:	2018.12.10
* @version		:	v0.1
* @description	:	this is the interface file of component
**/

#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__

#include <memory>
#include "glm\fwd.hpp"

#define GLM_ENABLE_EXPERIMENTAL

namespace game
{
	/**
	* @brief a enum class to define how many components factory can generate
	*   This enum could be added by any extra Component Type but the factory should also be changed
	**/
	enum ComponentType
	{
		Health,		// a kind of health component
		Transform	// a kind of tansformation component
	};

	/*
	* @brief virtual interface class of game component
	*s*/
	class IComponent
	{
	public:
		virtual ~IComponent() {}
		/*! @brief update the component with a float value.
		*
		*  This function update the value of the component.
		*
		*  @param[value] float value.
		*/
		virtual void update(float value) = 0;

		/*! @brief do yaw.
		*
		*  This function set up a camera with yaw.
		*
		*  @param[value] float angle.
		*/
		virtual void yaw(float angle) = 0;

		/*! @brief do pitch.
		*
		*  This function set up a camera with pitch.
		*
		*  @param[value] float angle.
		*/
		virtual void pitch(float angle) = 0;

		/*! @brief do roll.
		*
		*  This function set up a camera with roll.
		*
		*  @param[value] float angle.
		*/
		virtual void roll(float angle) = 0;

		virtual void scaleUp(const glm::vec3 &v) = 0;

		virtual void scaleUp(float x, float y, float z) = 0;

		/*! @brief get a tansformation matrix
		*
		*  This function gets a tansformation matrix
		*
		*  @param none
		*/
		virtual glm::mat4 getModelMatrix() = 0;

		/*! @brief factory to create a object of defined ComponentType.
		*
		*  This function create a component according to the defined ComponentType
		*
		*  @param[ComponentType] Type.
		*/
		static std::shared_ptr<IComponent> Create(ComponentType Type);

		/*! @brief factory to delete a object.
		*
		*  This function delete a component object by parameter obj
		*
		*  @param[Pointer] obj.
		*/
		static void Destroy(std::shared_ptr<IComponent> obj);
	};
}

#endif