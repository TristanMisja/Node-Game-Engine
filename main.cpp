#include <iostream>
#include <string>
#include <chrono>

#undef APIENTRY
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processEvents(GLFWwindow* window, float deltatime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;
const char* WIN_NAME = "Node Game Engine";
unsigned int MAX_FPS = 30;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = 1;
float lastMouseX = (float)WIN_WIDTH / 2.0f;
float lastMouseY = (float)WIN_HEIGHT / 2.0f;

int main()
{
	// ===================== Initialize Engine =====================


	float time1 = glfwGetTime(); // These are for measuring FPS 
	float time2 = glfwGetTime(); // and loading time

	std::cout.sync_with_stdio(false); // This is to speed up std::cout

	// Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // This is required so that GLFW works on Apple devices
	#endif
	#ifdef _DEBUG
		std::cout << "Initialized GLFW\n";
	#endif

	// Create a window and OpenGL context
	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_NAME, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "ERROR: Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	#ifdef _DEBUG
		std::cout << "Created game window\n";
	#endif

	// Initialize glad and OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "ERROR: Could not initialize GLAD\n";
		return -1;
	}

	#ifdef _DEBUG
		std::cout << "Initialized GLAD\n";
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
		std::cout << "GPU Vendor: " << glGetString(GL_VENDOR) << "\n";

		int attributeCount;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributeCount);
		std::cout << "Maximum vertex attributes supported: " << attributeCount << "\n";
	#endif

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// ==================== Load Shaders ===================

	GLuint program = glCreateProgram();

	Shader vertexShader = Shader(GL_VERTEX_SHADER);
	Shader fragmentShader = Shader(GL_FRAGMENT_SHADER);

	//glDeleteProgram(vertexShader.getProgram());
	//glDeleteProgram(fragmentShader.getProgram());

	vertexShader.setProgram(program);
	fragmentShader.setProgram(program);

	vertexShader.LoadFromFile("shaders/static.vert");
	fragmentShader.LoadFromFile("shaders/default.frag");

	vertexShader.Link();
	fragmentShader.Link();


	// ================ Creating game objects ==============

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Texture tex = Texture("assets/container.jpg");
	fragmentShader.Bind();
	fragmentShader.setInt("Texture", 0);

	#ifdef _WIREFRAME
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	#endif

	// ===================== Game loop =====================

	#ifdef _DEBUG
		time2 = glfwGetTime();
		float deltatime = time2 - time1;
		time1 = time2;
		std::cout << "Finished loading after " << deltatime << " seconds\n";
		std::cout << "Press [ESC] to quit.\n";
	#else
		float deltatime;
	#endif

	while (!glfwWindowShouldClose(window))
	{
		// FPS Management
		time2 = glfwGetTime();
		deltatime = time2 - time1;
		time1 = time2;
		float FPS = 1.0f / deltatime;

		if (FPS > MAX_FPS)
			preciseSleep(1.0 / MAX_FPS); //sleep(FPS / 1000); Change to this if performance gets too low. Should probably be configurable

		// This was using too much of the CPU
		#ifdef _DEBUG
			eraseLines(1);
			std::cout << "FPS: " << (int)FPS << "\n";
		#endif

		// Input and clearing
		processEvents(window, deltatime);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create necessary matrices
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.5f, 2.9f, 0.8f)); 
		glm::mat4 projection = glm::perspective(glm::radians(camera.FOV), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		fragmentShader.setMat4("projection", projection);
		glm::mat4 view = camera.GetViewMatrix();
		fragmentShader.setMat4("view", view);
		fragmentShader.setMat4("model", model);

		// Render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ===================== Close everything up =====================

	vertexShader.~Shader();
	fragmentShader.~Shader();

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processEvents(GLFWwindow* window, float deltatime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.position += camera.speed * camera.front * deltatime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.position -= camera.speed * camera.front * deltatime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * camera.speed * deltatime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * camera.speed * deltatime;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.position.y += camera.speed * deltatime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.position.y -= camera.speed * deltatime;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastMouseX;
	float yoffset = lastMouseY - ypos; // This is reversed because y-coordinates go from bottom to top
	lastMouseX = xpos;
	lastMouseY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}