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

int main() {
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GLFWwindow* window = openglInit(
		"LearnOGL",
		SCR_WIDTH, SCR_HEIGHT,
		framebuffer_size_callback,
		mouse_callback);

	Model obj("objs/nanosuit/nanosuit.obj");
	Shader shader("shaderfile/shader.vs", "shaderfile/shader.fs");
	shader.use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", projection);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model);
	shader.setVec3("light.pos", glm::vec3(2));
	shader.setVec3("light.diffuse", glm::vec3(1));
	shader.setVec3("light.specular", glm::vec3(2));
	shader.setVec3("light.ambient", glm::vec3(0.3));
	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) 
	{
		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		//Input handle
		process_input(window);
		//Render func
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		shader.setVec3("camPos", camera.Position);
		obj.Draw(shader);

		//Swap buffer and handle events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}