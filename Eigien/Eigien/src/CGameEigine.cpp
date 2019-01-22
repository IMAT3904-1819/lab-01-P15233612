#include <iostream>
#include "CGameEigine.h"

#include "glog/logging.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ProtobufIO.hpp"
#include <SOIL.h>

namespace game
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	// Function prototypes
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void Do_Movement();

	// Camera
	Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Moves/alters the camera positions based on user input
	void Do_Movement()
	{
		// Camera controls
		if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (keys[GLFW_KEY_D])
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// Is called whenever a key is pressed/released via GLFW
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera.ProcessMouseScroll(yoffset);
	}

	CGameEigine::CGameEigine()
	{

	}

	CGameEigine::~CGameEigine()
	{
		// cleanup
		glfwTerminate();
	}

	bool CGameEigine::createWindow(const int width, const int height, const std::string& windowName)
	{
		m_nWidth = width;
		m_nHeight = height;

		m_cWindowName = windowName;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		m_pWindow = glfwCreateWindow(width, height, m_cWindowName.c_str(), nullptr, nullptr);
		if (nullptr == m_pWindow)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		// make this new window our current context
		glfwMakeContextCurrent(m_pWindow);
		GLenum res = glewInit();
		if (GLEW_OK != res)
		{
			glfwTerminate();
			LOG(FATAL) << "GLEW Init False";
			return false;
		}
		return true;
	}

	bool CGameEigine::initEngine(const std::string& configFilePath)
	{
		glGenVertexArrays(1, &VAO);
		glGenVertexArrays(1, &lampVAO);
		glGenVertexArrays(1, &sphereVAO);
		glGenBuffers(1, &VBO);
		
		//read prototxt
		int res = ReadProtoFromTextFile(configFilePath, &m_config);
		if (!res)
		{
			LOG(ERROR) << "Read Material Config Prototxt Failed !";
			return false;
		}

		int sObjects = 0;
		for (int i = 0; i < m_config.configs_size(); ++i)
		{
			int type = m_config.configs(i).elementtype();
			if (type == 0)  // object
			{
				std::string filename = m_config.configs(i).filename();
				m_ModelMap.insert(std::make_pair(filename, sObjects));
				std::string path = std::string("./resources/") + filename + std::string("/") + filename + std::string(".obj");
				Model newModel(path);
				m_Models.push_back(newModel);
				sObjects++;
			}
			else // picture
			{
				std::string filename = m_config.configs(i).filename();
				std::string path = std::string("./resources/") + filename;
				int width, height;
				unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
				// Assign texture to ID
				GLuint textureID;
				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
				glBindTexture(GL_TEXTURE_2D, 0);
				SOIL_free_image_data(image);
				m_TextureMap.insert(std::make_pair(filename, textureID));
			}
		}

		// Define the viewport dimensions
		glViewport(0, 0, m_nWidth, m_nHeight);

		glfwSetKeyCallback(m_pWindow, key_callback);
		glfwSetCursorPosCallback(m_pWindow, mouse_callback);
		glfwSetScrollCallback(m_pWindow, scroll_callback);

		// until we receive a message to close the program
		while (!glfwWindowShouldClose(m_pWindow))
		{
			// Set frame time
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Check and call events
			glfwPollEvents();
			Do_Movement();

			__render();

			// Swap the buffers
			glfwSwapBuffers(m_pWindow);
		}
		return true;
	}

	void CGameEigine::renderBackground(const std::string& picture, aiColor4D color)
	{
		__renderBackground(picture, color);
	}

	void CGameEigine::__renderBackground(const std::string& picture, aiColor4D color)
	{
		if (picture == "")
		{
			// Clear the colorbuffer
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			auto ID = m_TextureMap.find(picture);
			if (ID == m_TextureMap.end())
			{
				LOG(WARNING) << "background " << picture << " has been loaded" << "  Auto Render Background";
				// Clear the colorbuffer
				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				return;
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0f, m_nWidth - 1, 0.0f, m_nHeight - 1, -1.0f, 1.0f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_DEPTH_TEST);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, ID->second);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(0, m_nHeight);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(m_nWidth, m_nHeight);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(m_nWidth, 0);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}
	}

	void CGameEigine::drawCube(const aiVector3D& center, const aiVector3D& dSize, aiColor4D color)
	{
		__drawCube(center, dSize, color);
	}

	void CGameEigine::__drawCube(const aiVector3D& center, const aiVector3D& dSize, aiColor4D color)
	{
		float xUp = (center.x + dSize.x / 2) / m_nWidth;
		float xBottom = (center.x - dSize.x / 2) / m_nWidth;
		float yUp = (center.y + dSize.y / 2) / m_nWidth;
		float yBottom = (center.y - dSize.y / 2) / m_nWidth;
		float zUp = (center.z + dSize.z / 2) / m_nHeight;
		float zBottom = (center.z - dSize.z / 2) / m_nHeight;

		vertices[0] = xBottom; vertices[1] = yBottom; vertices[2] = zBottom;
		vertices[3] = xUp; vertices[4] = yBottom; vertices[5] = zBottom;
		vertices[6] = xUp; vertices[7] = yUp; vertices[8] = zBottom;
		vertices[9] = xUp; vertices[10] = yUp; vertices[11] = zBottom;
		vertices[12] = xBottom; vertices[13] = yUp; vertices[14] = zBottom;
		vertices[15] = xBottom; vertices[16] = yBottom; vertices[17] = zBottom;

		vertices[0 + 18] = xBottom; vertices[1 + 18] = yBottom; vertices[2 + 18] = zUp;
		vertices[3 + 18] = xUp; vertices[4 + 18] = yBottom; vertices[5 + 18] = zUp;
		vertices[6 + 18] = xUp; vertices[7 + 18] = yUp; vertices[8 + 18] = zUp;
		vertices[9 + 18] = xUp; vertices[10 + 18] = yUp; vertices[11 + 18] = zUp;
		vertices[12 + 18] = xBottom; vertices[13 + 18] = yUp; vertices[14 + 18] = zUp;
		vertices[15 + 18] = xBottom; vertices[16 + 18] = yBottom; vertices[17 + 18] = zUp;

		vertices[0 + 36] = xBottom; vertices[1 + 36] = yUp; vertices[2 + 36] = zUp;
		vertices[3 + 36] = xBottom; vertices[4 + 36] = yUp; vertices[5 + 36] = zBottom;
		vertices[6 + 36] = xBottom; vertices[7 + 36] = yBottom; vertices[8 + 36] = zBottom;
		vertices[9 + 36] = xBottom; vertices[10 + 36] = yBottom; vertices[11 + 36] = zBottom;
		vertices[12 + 36] = xBottom; vertices[13 + 36] = yBottom; vertices[14 + 36] = zUp;
		vertices[15 + 36] = xBottom; vertices[16 + 36] = yUp; vertices[17 + 36] = zUp;

		vertices[0 + 54] = xUp; vertices[1 + 54] = yUp; vertices[2 + 54] = zUp;
		vertices[3 + 54] = xUp; vertices[4 + 54] = yUp; vertices[5 + 54] = zBottom;
		vertices[6 + 54] = xUp; vertices[7 + 54] = yBottom; vertices[8 + 54] = zBottom;
		vertices[9 + 54] = xUp; vertices[10 + 54] = yBottom; vertices[11 + 54] = zBottom;
		vertices[12 + 54] = xUp; vertices[13 + 54] = yBottom; vertices[14 + 54] = zUp;
		vertices[15 + 54] = xUp; vertices[16 + 54] = yBottom; vertices[17 + 54] = zUp;

		vertices[0 + 72] = xBottom; vertices[1 + 72] = yBottom; vertices[2 + 72] = zBottom;
		vertices[3 + 72] = xUp; vertices[4 + 72] = yBottom; vertices[5 + 72] = zBottom;
		vertices[6 + 72] = xUp; vertices[7 + 72] = yBottom; vertices[8 + 72] = zUp;
		vertices[9 + 72] = xUp; vertices[10 + 72] = yBottom; vertices[11 + 72] = zUp;
		vertices[12 + 72] = xBottom; vertices[13 + 72] = yBottom; vertices[14 + 72] = zUp;
		vertices[15 + 72] = xBottom; vertices[16 + 72] = yBottom; vertices[17 + 72] = zBottom;

		vertices[0 + 90] = xBottom; vertices[1 + 90] = yUp; vertices[2 + 90] = zBottom;
		vertices[3 + 90] = xUp; vertices[4 + 90] = yUp; vertices[5 + 90] = zBottom;
		vertices[6 + 90] = xUp; vertices[7 + 90] = yUp; vertices[8 + 90] = zUp;
		vertices[9 + 90] = xUp; vertices[10 + 90] = yUp; vertices[11 + 90] = zUp;
		vertices[12 + 90] = xBottom; vertices[13 + 90] = yUp; vertices[14 + 90] = zUp;
		vertices[15 + 90] = xBottom; vertices[16 + 90] = yUp; vertices[17 + 90] = zBottom;

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		Shader m_cubeShader("./shaders/cube.vert", "./shaders/cube.frag");
		m_cubeShader.Use();

		glDisable(GL_DEPTH_TEST);

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)m_nWidth / (float)m_nHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 transform = glm::mat4(1.0);
		transform = glm::rotate(transform, float(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		GLfloat colour[4];
		colour[0] = color.r;
		colour[1] = color.g;
		colour[2] = color.b;
		colour[3] = color.a;

		glUniformMatrix4fv(glGetUniformLocation(m_cubeShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
		glUniformMatrix4fv(glGetUniformLocation(m_cubeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(m_cubeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		unsigned int colorLoc = glGetUniformLocation(m_cubeShader.Program, "color");
		glUniform4fv(colorLoc, 1, colour);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

	}

	void CGameEigine::drawSphere(const aiVector3D& dCenter, float radius, aiColor4D color)
	{
		__drawSphere(dCenter, radius, color);
	}

	void CGameEigine::__drawSphere(const aiVector3D& dCenter, float r, aiColor4D color)
	{
		float m_r = r;
		float PI = 3.141592653f;
		int angleSpan = 10;
		float x = (dCenter.x) / m_nWidth;
		float y = (dCenter.y) / m_nHeight;
		float z = (dCenter.z) / m_nHeight;
		for (int vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan) 
		{
			for (int hAngle = 0; hAngle <= 360; hAngle = hAngle + angleSpan) 
			{
				float x0 = r * ::cos(vAngle * PI / 180) * ::cos(hAngle * PI / 180) + x;
				float y0 = r * ::cos(vAngle * PI / 180) * ::sin(hAngle * PI / 180) + y;
				float z0 = r * ::sin(vAngle * PI / 180) + z;

				float x1 = r * ::cos(vAngle * PI / 180) * ::cos((hAngle + angleSpan) * PI / 180) + x;
				float y1 = r * ::cos(vAngle * PI / 180) * ::sin((hAngle + angleSpan) * PI / 180) + y;
				float z1 = r * ::sin(vAngle * PI / 180) + z;

				float x2 = r * ::cos((vAngle + angleSpan) * PI / 180) * ::cos((hAngle + angleSpan) * PI / 180) + x;
				float y2 = r * ::cos((vAngle + angleSpan) * PI / 180) * ::sin((hAngle + angleSpan) * PI / 180) + y;
				float z2 = r * ::sin((vAngle + angleSpan) * PI / 180) + z;

				float x3 = r * ::cos((vAngle + angleSpan) * PI / 180) * ::cos(hAngle * PI / 180) + x;
				float y3 = r * ::cos((vAngle + angleSpan) * PI / 180) * ::sin(hAngle * PI / 180) + y;
				float z3 = r * ::sin((vAngle + angleSpan) * PI / 180) + z;

				m_points.push_back(x1);
				m_points.push_back(y1);
				m_points.push_back(z1);

				m_points.push_back(x3);
				m_points.push_back(y3);
				m_points.push_back(z3);

				m_points.push_back(x0);
				m_points.push_back(y0);
				m_points.push_back(z0);

				m_points.push_back(x1);
				m_points.push_back(y1);
				m_points.push_back(z1);

				m_points.push_back(x2);
				m_points.push_back(y2);
				m_points.push_back(z2);

				m_points.push_back(x3);
				m_points.push_back(y3);
				m_points.push_back(z3);
			}
		}

		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_points.size(), m_points.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		Shader sphereShader("./shaders/sphere.vert", "./shaders/sphere.frag");
		sphereShader.Use();

		glDisable(GL_DEPTH_TEST);

		GLfloat colour[4];
		colour[0] = color.r;
		colour[1] = color.g;
		colour[2] = color.b;
		colour[3] = color.a;

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)m_nWidth / (float)m_nHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 trans = glm::translate(model, glm::vec3(-0.1f, 0.25f, 0.0f));
		glm::mat4 scale = glm::rotate(trans, float(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		glUniform1f(glGetUniformLocation(sphereShader.Program, "uR"), m_r);
		glUniformMatrix4fv(glGetUniformLocation(sphereShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(sphereShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(sphereShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(scale));

		unsigned int colorLoc = glGetUniformLocation(sphereShader.Program, "color");
		glUniform4fv(colorLoc, 1, colour);

		glBindVertexArray(sphereVAO);
		glDrawArrays(GL_TRIANGLES, 0, m_points.size() / 3);
		glBindVertexArray(0);

	}

	void CGameEigine::drawObject(const std::string& objectName, const aiVector3D& dCenter)
	{
		__drawObject(objectName, dCenter);
	}

	void CGameEigine::__drawObject(const std::string& objectName, const aiVector3D& dCenter)
	{
		auto index = m_ModelMap.find(objectName);
		if (index == m_ModelMap.end())
		{
			LOG(WARNING) << "not object name " << objectName << " has been loaded";
			return;
		}
		// Setup some OpenGL options
		glEnable(GL_DEPTH_TEST);

		// 点光源坐标
		glm::vec3 pointLightPositions[] = { glm::vec3(0.7f,  0.2f,  0.5f), glm::vec3(-1.0f,  0.2f, -0.5f) };
		// Setup and compile our shaders
		std::string vertName = std::string("./shaders/") + objectName + std::string(".vert");
		std::string fragName = std::string("./shaders/") + objectName + std::string(".frag");
		Shader shader(vertName.c_str(), fragName.c_str());

		Model ourModel = m_Models[index->second];
		shader.Use();

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)m_nWidth / (float)m_nHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Set the lighting uniforms
		glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Point light 1
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].linear"), 0.009);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].quadratic"), 0.0032);
		// Point light 2
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].linear"), 0.009);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].quadratic"), 0.0032);

		// Draw the loaded model
		float x = (dCenter.x) / m_nWidth;
		float y = (dCenter.y) / m_nHeight;
		float z = (dCenter.z) / m_nHeight;
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 trans = glm::translate(model, glm::vec3(x, y - 1.75, z));
		glm::mat4 scale = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(scale));
		ourModel.Draw(shader);

		// Draw the lamps
		Shader lampShader("./shaders/lamp.vert", "./shaders/lamp.frag");
		lampShader.Use();
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(lampVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		model = glm::mat4(1.0);
		trans = glm::translate(model, glm::vec3(0.7f, 0.2f, 0.5f));
		scale = glm::scale(trans, glm::vec3(0.3f, 0.3f, 0.3f)); // Downscale lamp object (a bit too large)

		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(scale));

		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void CGameEigine::__render()
	{
		//__renderBackground(std::string("bck.jpg"), aiColor4D(0.15f, 0.25f, 0.35f, 1.0f));
		__renderBackground(std::string(""), aiColor4D(0.15f, 0.25f, 0.35f, 1.0f));
		__drawCube(aiVector3D(0, 0, 500), aiVector3D(300, 300, 300), aiColor4D(0.0f, 1.0f, 0.0f, 0.5f));
		__drawSphere(aiVector3D(-500, -500, 0), 0.1f, aiColor4D(0.0f, 1.0f, 0.0f, 0.5f));
		__drawObject(std::string("nanosuit"), aiVector3D(0,0,0));
	}

	std::shared_ptr<IGameEigine> IGameEigine::GetInstance()
	{
		static std::shared_ptr<IGameEigine> pObj = std::shared_ptr<IGameEigine>(new CGameEigine);
		return pObj;
	}
}


