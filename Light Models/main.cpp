#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<Shader.h>
#include<utils.h>
#include<Camera.h>

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
void input_callback(GLFWwindow* window, float deltaTime) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
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
	auto window = openglInit("Light", 800, 600, NULL, mouse_callback);
	float floorModel[] = {
		0.5, 0.5, 1, 1, 1,
		0.5 , -0.5, 1, 1, 0,
		-0.5 , -0.5, 1, 0, 0,
		-0.5, 0.5, 1, 0, 1
	};
	unsigned int floorInd[] = {
		0,1,3,
		1,2,3
	};
	GLuint floorVAO;
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);
	GLuint floorVBO;
	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER ,floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorModel), floorModel, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLuint floorEBO;
	glGenBuffers(1, &floorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorInd), floorInd, GL_STATIC_DRAW);
	glBindVertexArray(0);

	GLuint floorTex = loadMaterial("../asset/texture/wood.png");

	Shader shader("../asset/shader/phone/vert.vert", "../asset/shader/phone/frag.frag");
	shader.use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
	shader.setMat4("model", model);
	shader.setMat4("projection", projection);

	auto lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		//std::cout << camera.Position.x << std::endl;
		auto deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		input_callback(window, deltaTime);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3, 0.3, 0.3, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, floorTex);
		shader.use();
		shader.setInt("texture1", 0);	
		shader.setMat4("view", camera.GetViewMatrix());
		glBindVertexArray(floorVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}