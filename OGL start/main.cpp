#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include<string>
#include"GLExceptions.h"
#include"Shader.h"
#include"stb_image.h"
#include"ImgLoader.h"
#include"utils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include"Camera.h"
using std::string;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.f;

GLFWwindow* openglInit(string name, int wid, int height, GLFWframebuffersizefun kbd_callback, GLFWcursorposfun mouse_callback) {
	/*
	Init opengl Window.
	*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, name.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return NULL;
	}

	glfwSetFramebufferSizeCallback(window, kbd_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

float lastX = 0, lastY = 0;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

float quad[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

int indices[] = {
	1, 3, 2,
	1, 0, 3
};


float box[] = {
	1.0f, 1.0f, 1.0f,
};

int boxInd[] = {
	1
};

int main() {
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GLFWwindow* window = openglInit(
		"LearnOGL",
		SCR_WIDTH, SCR_HEIGHT,
		framebuffer_size_callback,
		mouse_callback);

	unsigned int scrVAO;
	glGenVertexArrays(1, &scrVAO);
	glBindVertexArray(scrVAO);
	unsigned int scrVBO;
	glGenBuffers(1, &scrVBO);
	glBindBuffer(GL_ARRAY_BUFFER, scrVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	unsigned int scrEBO;
	glGenBuffers(1, &scrEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scrEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Model obj("objs/nanosuit/nanosuit.obj");
	Shader shader("shaderfile/shader.vs", "shaderfile/shader.fs");
	Shader screenShader("shaderfile/screenShader.vs", "shaderfile/screenShader.fs");
	Shader floorShader("shaderfile/floorShader.vs", "shaderfile/floorShader.fs");
	Shader shadowMapShader("shaderfile/shadowMapShader.vs", "shaderfile/emptyShader.fs");
	Shader showShadowMap("shaderfile/screenShader.vs", "shaderfile/showShadowMap.fs");
	//FLOOR
	float floorVertexs[] = {
		10.0, 0.0, 10.0,
		10.0, 0.0, -10.0,
		-10.0,0.0,-10.0,
		-10.0,0.0,10.0
	};
	GLuint floorInd[] = {
		0,1,3,
		1,2,3
	};
	GLuint floorVAO;
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);
	GLuint floorVBO;
	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertexs), floorVertexs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLuint floorEBO;
	glGenBuffers(1, &floorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorInd), floorInd, GL_STATIC_DRAW);
	glBindVertexArray(0);
	//FLOOR end

	//ShadowMap
	const GLuint SHADOW_HEIGHT = 1024, SHADOW_WIDTH = 1024;
	GLuint shaderMapFBO;
	glGenFramebuffers(1, &shaderMapFBO);
	GLuint shaderMapTex;
	glGenTextures(1, &shaderMapTex);
	glBindTexture(GL_TEXTURE_2D, shaderMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shaderMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shaderMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//ShadowMap end
	
	glm::vec3 lightPos(3);
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 0.1f, 100.0f);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0), glm::vec3(0,1,0));
	glm::mat4 lightSpaceMat = lightProjection * lightView;
	
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	{
		shadowMapShader.use();
		shadowMapShader.setMat4("model", model);
		shadowMapShader.setMat4("lightSpaceMat", lightSpaceMat);
		//shadowMapShader.setMat4()
	}
	{
		floorShader.use();
		floorShader.setMat4("projection", projection);
		floorShader.setMat4("model", model);
		floorShader.setVec3("color", glm::vec3(1, 1, 1));
		floorShader.setMat4("lightSpaceMat", lightSpaceMat);
	}
	{
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("model", model);
		shader.setVec3("light.pos", lightPos);
		shader.setVec3("light.diffuse", glm::vec3(1));
		shader.setVec3("light.specular", glm::vec3(2));
		shader.setVec3("light.ambient", glm::vec3(0.3));
	}
	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) 
	{
		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		//Input handle
		process_input(window);
		//Render func
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glm::mat4 view = camera.GetViewMatrix();
		//Generate shadow map
		shadowMapShader.use();
		
		//shadowMapShader.setMat4("view", view);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shaderMapFBO);
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_FRONT);
			glBindVertexArray(floorVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			obj.Draw(shadowMapShader);
			glCullFace(GL_BACK);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Render scene
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		floorShader.use();
		floorShader.setMat4("view", view);
		floorShader.setInt("depthMap", 0);
		glBindTexture(GL_TEXTURE_2D, shaderMapTex);
		glBindVertexArray(floorVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		shader.use();
		shader.setMat4("view", view);
		shader.setVec3("camPos", camera.Position);
		obj.Draw(shader);
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		
		screenShader.use();
		screenShader.setInt("screenTex", 0);
		glBindVertexArray(scrVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// //ÏÔÊ¾Éî¶ÈÍ¼ 
		//showShadowMap.use();
		//showShadowMap.setInt("depthMap", 0);
		//glBindVertexArray(scrVAO);
		//glDisable(GL_DEPTH_TEST);
		//glBindTexture(GL_TEXTURE_2D, shaderMapTex);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//Swap buffer and handle events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}