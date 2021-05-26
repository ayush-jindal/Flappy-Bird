#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Bird.hpp"
//class Bird;

class Window{
private:
	GLFWwindow *window;
	const char* title;
	int width;
	int height;
	Bird bird;
	
public:
	Window(const char* title, int width, int height);
	int initialize(Bird bird);
	void getDimensions(int &width, int &height);
	GLFWwindow* getWindow();
	void shouldClose();
	bool gameLoop(std::vector<float> xpositions, std::vector<float>heights, std::vector<float>heightsUp);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	~Window();
};

#endif
