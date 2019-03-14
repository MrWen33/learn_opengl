/*
	用于测试各种光照模型的程序
	数字键切换光照模型
	WSAD移动, ESC退出
	数字键对应的模型:[
		1: Phone
		2: Blinn-Phone
	]
*/
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<functional>
#include<Shader.h>
#include<utils.h>
#include<Camera.h>

Camera camera(glm::vec3(0.0f, 3.0f, 1.0f));
void input_callback(GLFWwindow* window, float deltaTime, std::function<void(int)> switcher) {
	
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
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		switcher(0);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		switcher(1);
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
		//Position	//TexCoord	//Normal
		5,   0,  5,		1, 1,	0,1,0,
		5,   0, -5,		1, 0,	0,1,0,
		-5,  0, -5,		0, 0,	0,1,0,
		-5,  0,  5,		0, 1,	0,1,0	
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
	unsigned int stride = 8*sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLuint floorEBO;
	glGenBuffers(1, &floorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorInd), floorInd, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
	
	GLuint floorTex = loadMaterial("../asset/texture/wood.png");

	Shader shaders[] = {
		Shader("../asset/shader/phone/vert.vert", "../asset/shader/phone/frag.frag"),
		Shader("../asset/shader/blinn-phone/vert.vert", "../asset/shader/blinn-phone/frag.frag")
	};

	float boxModel[] = {
		-1,1,-1,
		-1,1,1,
		1,1,1,
		1,1,-1,
		-1,-1,-1,
		-1,-1,1,
		1,-1,1,
		1,-1,-1
	};
	GLuint boxInd[] = {
		1,0,3,1,3,2, //上
		1,5,0,0,5,4, //左
		0,4,7,0,7,3, //前
		3,7,6,3,6,2, //右
		2,6,5,2,5,1, //后
		4,5,7,7,5,6  //下
	};
	glm::vec3 lightPos(0, 2, 0);
	GLuint boxVAO, boxVBO, boxEBO;
	glGenVertexArrays(1, &boxVAO);
	glBindVertexArray(boxVAO);
	glGenBuffers(1, &boxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxModel), boxModel, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &boxEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxInd), boxInd, GL_STATIC_DRAW);
	glBindVertexArray(0);
	Shader lightSourceShader("../asset/shader/lightSource/vert.vert", "../asset/shader/lightSource/frag.frag");
	lightSourceShader.use();
	glm::mat4 lightPosMat;
	lightPosMat = glm::translate(lightPosMat, lightPos);
	lightPosMat = glm::scale(lightPosMat, glm::vec3(0.1, 0.1, 0.1));
	lightSourceShader.setMat4("model", lightPosMat);
	lightSourceShader.setMat4("projection", projection);

	Shader* shader = &shaders[0];

	auto lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		//std::cout << camera.Position.x << std::endl;
		auto deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		input_callback(window, deltaTime, [&](int ind) {shader = &shaders[ind]; });

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3, 0.3, 0.3, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, floorTex);
		shader->use();
		shader->setMat4("model", model);
		shader->setMat4("projection", projection);
		shader->setInt("texture1", 0);	
		shader->setMat4("view", camera.GetViewMatrix());
		shader->setVec3("camPos", camera.Position);
		shader->setVec3("lightPos", lightPos);
		glBindVertexArray(floorVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		lightSourceShader.use();
		lightSourceShader.setMat4("view", camera.GetViewMatrix());
		glBindVertexArray(boxVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}