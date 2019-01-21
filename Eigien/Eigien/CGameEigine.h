#ifndef __CGAMEEIGINE_H__
#define __CGAMEEIGINE_H__

#define GLEW_STATIC

#include <map>

#include "IGameEigine.h"
#include <GL\glew.h>
#include "GLFW\glfw3.h"

#include "Camera.h"
#include "Shader.h"
#include "CModel.h"
#include "GameObject.pb.h"

namespace game
{
	class CGameEigine : public IGameEigine
	{
	public:
		bool createWindow(const int width, const int height, const std::string& windowName) override;
		bool initEngine(const std::string& configFilePath) override;
		void renderBackground(const std::string& picture, aiColor4D color) override;
		virtual void drawCube(const aiVector3D& center, const aiVector3D& dSize, aiColor4D color) override;
		virtual void drawSphere(const aiVector3D& dCenter, float radius, aiColor4D color) override;
		virtual void drawObject(const std::string& objectName, const aiVector3D& dCenter) override;
		~CGameEigine();
		CGameEigine();

	private:
		void __render();
		void __renderBackground(const std::string& picture, aiColor4D color);
		void __drawCube(const aiVector3D& center, const aiVector3D& dSize, aiColor4D color);
		void __drawSphere(const aiVector3D& dCenter, float radius, aiColor4D color);
		void __drawObject(const std::string& objectName, const aiVector3D& dCenter);
		int m_nWidth;
		int m_nHeight;
		std::string m_cWindowName;
		GLFWwindow* m_pWindow;
		unsigned int VBO, VAO;
		GLuint lampVAO;
		GLuint sphereVAO;

		std::map<std::string, int> m_ModelMap;
		std::vector<Model> m_Models;

		GameConfig m_config;
		std::vector<GLfloat> m_points;

		std::map<std::string, GLuint> m_TextureMap;
	};
}

#endif // !__CGAMEEIGINE_H__


