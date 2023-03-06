#include "window.h"

Window::Window(char* name, int width, int height)
{
	this -> name = name;
	this -> width = width;
	this -> height = height;
	this->yaw = 0;
	this->pitch = 0;
	this->lastX = 0;
	this->lastY = 0;
	this->mouseMoved = false;
	init();

	for (int i = 0; i < MAX_KEYBOARD; i++)
	{
		this->keys[i] = false;
	}

	for (int i = 0; i < MAX_MOUSE; i++)
	{
		this->mouseButtons[i] = false;
	}
}

Window::~Window()
{
	glfwTerminate();
}

void Window::init()
{
	if (!glfwInit())
	{
		std::cout << "Error initializing glfw!" << std::endl;
	}
	else
	{
		std::cout << "Successfully initializing glfw!" << std::endl;
	}

	window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	mouseEnabled = true;

	//callbacks for user input
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing glew!" << std::endl;
	}
	else
	{
		std::cout << "Successfully initializing glew!" << std::endl;
	}

	std::cout << "Open GL " << glGetString(GL_VERSION) << std::endl;
}

void Window::update()
{
	glfwPollEvents();
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSwapBuffers(window);
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* Window::getWindow()
{
	return window;
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

void Window::setKey(int key, bool ok)
{
	this -> keys[key] = ok;
}

void Window::setMouseButton(int button, bool ok)
{
	this->mouseButtons[button] = ok;
}

void Window::setMousePos(double xpos, double ypos)
{
	this->xpos = xpos;
	this->ypos = ypos;
}

void Window::getMousePos(double &xpos, double &ypos)
{
	xpos = this->xpos;
	ypos = this->ypos;
}

//Handling key pressed
bool Window::isPressed(int key)
{
	return keys[key];
}

//Handling mouse buttons pressed
bool Window::isMousePressed(int button)
{
	return mouseButtons[button];
}

//Handling keyboard actions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* wind = (Window*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {			//Callback for Mouse toggle
		wind->setMouseEnabled(!wind->getMouseEnabled());
	}

	if (action != GLFW_RELEASE)
		wind->setKey(key, true);
	else
		wind->setKey(key, false);
}

//Handling mouse actions
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* wind = (Window*)glfwGetWindowUserPointer(window);

	if (action != GLFW_RELEASE)
		wind->setMouseButton(button, true);
	else
		wind->setMouseButton(button, false);
}

//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* wind = (Window*)glfwGetWindowUserPointer(window);
	if (wind->getMouseEnabled()) {
		return;
	}

	wind->setMouseMoved(true);

	if (wind->firstMouse)
	{
		wind->setLastX(xpos);
		wind->setLastY(ypos);
		wind->firstMouse = false;
	}

	float xoffset = xpos - wind->getLastX();
	float yoffset = wind->getLastY() - ypos;
	wind->setLastX(xpos);
	wind->setLastY(ypos);

	float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	wind->setYaw(xoffset);
	wind->setPitch(yoffset);

	if (wind->getPitch() > 89.0f)
		wind->setPitch(89.0f);
	if (wind->getPitch() < -89.0f)
		wind->setPitch(-89.0f);

	/*glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);*/
}


void Window::setMouseEnabled(bool newMouseEnabled) {
	if (newMouseEnabled) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		firstMouse = true;
	}
	mouseEnabled = newMouseEnabled;
}