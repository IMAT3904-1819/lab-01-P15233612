/**
* @file			:	ICamera.h
* @author		:
* @date			:	2018.12.10
* @version		:	v0.1
* @description	:	this is the interface file of camera
**/

#ifndef __ICAMERA_H__
#define __ICAMERA_H__

#include <memory>

#define GLM_ENABLE_EXPERIMENTAL

#include "glm\fwd.hpp"

namespace game
{
	class ICamera
	{
	public:
		virtual ~ICamera() {}
		/*! @brief set a camera with a float value for fov.
		*
		*  This function set up a camera
		*
		*  @param[value] float value.
		*/
		virtual void setFOV(float fov) = 0;

		/*! @brief set camera a look at point
		*
		*  This function set up camera look at some place
		*
		*  @param[value] a point replaced by (x,y,z).
		*/
		virtual void lookAt(float x, float y, float z) = 0;

		/*! @brief get a camera's view point matrix
		*
		*  This function get a camera's view point matrix
		*
		*  @param none
		*/
		virtual glm::mat4 getViewMatrix() = 0;

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

		/*! @brief create a object of Camera.
		*
		*  This function create a camera.
		*
		*  @param none
		*/
		static std::shared_ptr<ICamera> Create();

		/*! @brief  delete a camera.
		*
		*  This function delete a camera object by parameter obj
		*
		*  @param[Pointer] obj.
		*/
		void Destroy(std::shared_ptr<ICamera> obj);
	};
}

#endif // !__ICAMERA_H__
