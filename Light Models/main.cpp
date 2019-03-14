#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<Shader.h>

void input_callback(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* openglInit(std::string name, int wid, int height, GLFWframebuffersizefun kbd_callback, GLFWcursorposfun mouse_callback) {
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

int main() {
	auto window = openglInit("Light", 800, 600, NULL, NULL);
	float floorModel[] = {
		-1, 0, 1,
		1, 0, 1,
		1,0,-1,
		-1,0,-1
	};
	unsigned int floorInd[] = {
		0,3,1,
		1,3,2
	};
	GLuint floorVAO;
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);
	GLuint floorVBO;
	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER ,floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorModel), floorModel, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLuint floorEBO;
	glGenBuffers(1, &floorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorInd), floorInd, GL_STATIC_DRAW);
	glBindVertexArray(0);



	while (!glfwWindowShouldClose(window)) {
		input_callback(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}