#include <iostream>
#include <glew.h>
#include <glfw3.h>

#pragma once

#define MAX_KEYBOARD 512
#define MAX_MOUSE 8

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

class Window
{
	private:
		char* name;
		int width, height;
		GLFWwindow* window;

		bool keys[MAX_KEYBOARD];
		bool mouseButtons[MAX_MOUSE];
		double xpos;
		double ypos;
		double yaw;
		double pitch;
		double lastX;
		double lastY;
		bool mouseMoved;
		bool mouseEnabled;
	
	public:
		Window(char* name, int width, int height);
		~Window();
		GLFWwindow* getWindow();

		void init();
		void update();
		void clear();

		void setKey(int key, bool ok);
		void setMouseButton(int button, bool ok);
		void setMousePos(double xpos, double ypos);
		void getMousePos(double &xpos, double &ypos);
		bool isPressed(int key);
		bool isMousePressed(int button);
		
		bool firstMouse = true;

		int getWidth();
		int getHeight();
		inline double getYaw() { return yaw; }
		inline double getPitch() { return pitch; }
		inline double getLastX() { return lastX; }
		inline double getLastY() { return lastY; }

		inline bool getMouseMoved() { return mouseMoved; }
		inline bool getMouseEnabled() { return mouseEnabled; }


		inline void setYaw(double newYaw) { yaw = newYaw; }
		inline void setPitch(double newPitch) { pitch = newPitch; }
		inline void setLastX(double newLastX) { lastX = newLastX; }
		inline void setLastY(double newLastY) { lastY = newLastY; }

		inline void setMouseMoved(bool newMouseMoved) { mouseMoved = newMouseMoved; }
		void setMouseEnabled(bool newMouseEnabled);
};