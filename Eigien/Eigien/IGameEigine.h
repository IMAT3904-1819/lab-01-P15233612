/**
* @file			:	IGameEigine.h
* @author		:
* @date			:	2018.12.13
* @version		:	v0.1
* @description	:	this is the interface file of Game Eigine
**/

#ifndef __IGAMEEIGINE_H__
#define __IGAMEEIGINE_H__

#include <memory>
#include <string>
#include "assimp\color4.h"
#include "assimp\vector2.h"
#include "assimp\vector3.h"

namespace game
{
	class IGameEigine
	{
	public:
		/**
		*	@brief this function will create a window with defined width height and name
		*	@param bckgrnFilePath; width, height, wndname
		*   @return void
		*/
		virtual bool createWindow(const int width, const int height, const std::string& windowName) = 0;

		/**
		*	@brief this function will init the eigine
		*	@param path to the config file
		*   @return bool
		*/
		virtual bool initEngine(const std::string& configFilePath) = 0;

		/**
		*	@brief this function will picture from  <obejctName> as game background. If objectName is empty. It render with color
		*	@param objectName; color
		*   @return void
		*/
		virtual void renderBackground(const std::string& picture, aiColor4D color) = 0;

		/**
		*	@brief this function add a drawCube command into commandlist with center and dSize
		*	@param center and size
		*   @return void
		*/
		virtual void drawCube(const aiVector3D& center, const aiVector3D& dSize, aiColor4D color) = 0;

		/**
		*	@brief this function add a drawSphere command into commandlist with center and radius
		*	@param center and radius
		*   @return void
		*/
		virtual void drawSphere(const aiVector3D& dCenter, float radius, aiColor4D color) = 0;

		/**
		*	@brief this function add a drawObject command into commandlist with center
		*	@param center and radius
		*   @return void
		*/
		virtual void drawObject(const std::string& objectName, const aiVector3D& dCenter) = 0;

		virtual ~IGameEigine() {}

		/**
		*	@brief this function will return a eigine object and this object is singleton
		*	@param none
		*   @return shared pointer points to this eigine
		*/
		static std::shared_ptr<IGameEigine> GetInstance();
	};
}

#endif // !__IGAMEEIGINE_H__



