#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"

Window::Window(const char* title, int width, int height){
	this->title = title;
	this->width = width;
	this->height = height;
	this->window = NULL;
	this->bird = Bird();
}

int Window::initialize(Bird bird){
	if(!glfwInit()){
		std::cout<<"GLFW init failed"<<std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if(!window){
		std::cout<<"GLFW Window creation failed"<<std::endl;
		glfwTerminate();		
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cout<<"GLEW init failed"<<std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
	
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	this->bird = bird;
	this->bird.initialize();
	return 0;
}

GLFWwindow* Window::getWindow(){ return window; }

void Window::getDimensions(int &width, int &height){
	width = this->width;
	height = this->height;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if(action == GLFW_PRESS){
		if(key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE)
		    theWindow->shouldClose();
		else if(key == GLFW_KEY_SPACE)
			theWindow->bird.jump();
	}
}

bool Window::gameLoop(std::vector<float> xpositions, std::vector<float>heights, std::vector<float>heightsUp){
	bird.draw();
	return bird.gameLoop(xpositions, heights, heightsUp);
}

void Window::shouldClose(){ glfwSetWindowShouldClose(window, GL_TRUE); }
Window::~Window(){}
