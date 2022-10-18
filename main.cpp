#include <iostream>
#include <string>
#include <chrono>

#undef APIENTRY
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util.hpp"
#include "Shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processEvents(GLFWwindow* window);//, float deltatime);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;
const char* WIN_NAME = "Node Game Engine";
unsigned int MAX_FPS = 30;

int main()
{
	// ===================== Initialize Engine =====================
	// 
	// 
	// These are for measuring FPS and loading time
	float time1 = glfwGetTime();
	float time2 = glfwGetTime();

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

		int attributeCount;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributeCount);
		std::cout << "Maximum vertex attributes supported: " << attributeCount << "\n";
	#endif

	//stbi_set_flip_vertically_on_load(true);
	//glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	// ==================== Load Shaders ===================
	GLuint program = glCreateProgram();

	Shader vertexShader = Shader(GL_VERTEX_SHADER);
	Shader fragmentShader = Shader(GL_FRAGMENT_SHADER);

	//glDeleteProgram(vertexShader.getProgram());
	//glDeleteProgram(fragmentShader.getProgram());

	vertexShader.setProgram(program);
	fragmentShader.setProgram(program);

	vertexShader.LoadFromFile("static.vert");
	fragmentShader.LoadFromFile("default.frag");

	vertexShader.Link();
	fragmentShader.Link();


	// ================ Creating game objects ==============

	float vertices[] = {
		// first triangle
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  0.5f, 0.0f,  // top left 
		// second triangle
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};

	// Create and bind vertex buffer and array

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// ===================== Game loop =====================

	#ifdef _DEBUG
		time2 = glfwGetTime();
		float deltatime = time2 - time1;
		time1 = time2;
		std::cout << "Finished loading after " << deltatime << " seconds\n";
		std::cout << "Press [ESC] to quit.\n";
	#endif

	while (!glfwWindowShouldClose(window))
	{
		// FPS Management
		time2 = glfwGetTime();
		deltatime = time2 - time1;
		time1 = time2;
		float FPS = 1.0f / deltatime;

		if (FPS > MAX_FPS)
			sleep(FPS / 1000); // preciseSleep(1.0 / MAX_FPS); 

		// This was using too much of the CPU
		//#ifdef _DEBUG
		//	eraseLines(1);
		//	std::cout << "FPS: " << (int)FPS << "\n";
		//#endif

		// Input and clearing
		processEvents(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render
		fragmentShader.Bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ===================== Close everything up =====================

	vertexShader.~Shader();
	fragmentShader.~Shader();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processEvents(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}