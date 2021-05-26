#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "headers/LightShaders.hpp"
#include "headers/Window.hpp"
#include "headers/Bird.hpp"

float HEIGHT = 0.67f;
float X_OFFSET = -0.00125;
int BIRD_FACTOR = 3;
int N_PIPES = 10;
float WIDTH = (float)1.0/N_PIPES;
float DIST_BW_PIPES = (float)1.0/(N_PIPES + 1);
glm::vec3 birdPos(-0.25f, 0.0f, 0.0f);
float jumpHeight = 0.25;
float g = 250;
// this variable so as to keep some buffer between the bird's jump and upper pipe level
float heightFactor = 1.5;
float x = heightFactor * jumpHeight;
//distance between up and lower pipe has to be atleast equal to the height of bird's jump
float VERT_DIST = jumpHeight;
int TOTAL_VERT = 2;
int TOTAL_HOR = 2;

float calcScale(){
	srand(time(0));
	float random = (float)rand()/RAND_MAX;
	if((int)random%2 == 0)
			return random * (TOTAL_VERT - 2*x)/(2*HEIGHT);
	else
		return random * (TOTAL_VERT - 2*x)/(2*HEIGHT) + x/HEIGHT;
}

float calcInverseScale(float scale1){
	float scale2 = -1;
	srand(time(0));
	float random = (float)rand()/RAND_MAX;
	if(4*x < TOTAL_VERT){
		if(scale1 < x/HEIGHT)
			scale2 = random * ((TOTAL_VERT - 2*x)/HEIGHT - 2*scale1) + scale1 + x/HEIGHT;
		else if(scale1 < (TOTAL_VERT - 2*x)/(2*HEIGHT)){
			if((int)random%2 == 0)
				scale2 = random * ((TOTAL_VERT - 2*x)/HEIGHT - 2*scale1) + scale1 + x/HEIGHT;
			else
				scale2 = random * (scale1 - x/HEIGHT);
		}
		else if(scale1 < TOTAL_VERT/(2*HEIGHT))
			scale2 = random * (scale1 - x/HEIGHT);
		else if(scale1 < (TOTAL_VERT-x)/x)
			scale2 = random * ((TOTAL_VERT - x)/HEIGHT - scale1);
		else
			std::cout<<"Invalid value of scale2"<<std::endl;
	} else{
		std::cout<<"insdie loop2"<<std::endl;
		if(scale1 < (TOTAL_VERT - 2*x)/(2*HEIGHT))
			scale2 = random * ((TOTAL_VERT - 2*x)/HEIGHT - 2*scale1) + scale1 + x/HEIGHT;
		else if(scale1 > x/HEIGHT && scale1 < TOTAL_VERT/(2*HEIGHT))
			scale2 = random * (scale1 - x/HEIGHT);
		else if(scale1 < (TOTAL_VERT-x)/x)
			scale2 = random * ((TOTAL_VERT - x)/HEIGHT - scale1);
		else
			std::cout<<"Invalid value of scale2"<<std::endl;
	}
	
	return scale2;
}

int main(){

	Window theWindow = Window("Flappy Bird", 800, 600);
	Bird bird2(birdPos, jumpHeight, g, WIDTH, HEIGHT, BIRD_FACTOR);
	theWindow.initialize(bird2);

	GLFWwindow* window = theWindow.getWindow();
	
	int VBO[] = {2};
	int IBO[] = {0};
	LightShaders background(1, VBO, IBO, 1);
	LightShaders pipes(1, VBO, IBO, 1);

	background.initialize((char*)"vertex.shader", (char*)"fragment.shader");
	pipes.initialize((char*)"vertex.shader", (char*)"fragment.shader");

	pipes.loadTexture(0, "pipe.png");
	background.loadTexture(0, "bg.jpeg");
	
	
	std::vector<float> bg = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	
	std::vector<float> pipe = {
		float(-WIDTH/2.0), HEIGHT, 0.0f, 0.0f, 1.0f,
		float(WIDTH/2.0), HEIGHT, 0.0f, 1.0f, 1.0f,
		float(WIDTH/2.0), 0.0f, 0.0f, 1.0f, 0.0f,
		float(WIDTH/2.0), 0.0f, 0.0f, 1.0f, 0.0f,
		float(-WIDTH/2.0), 0.0f, 0.0f, 0.0f, 0.0f,
		float(-WIDTH/2.0), HEIGHT, 0.0f, 0.0f, 1.0f
	};
	
	background.bindVBO(0, 0, 3, GL_FALSE, 5, 0, bg);
	background.bindVBO(0, 1, 2, GL_FALSE, 5, 3, bg);
	
	pipes.bindVBO(0, 0, 3, GL_FALSE, 5, 0, pipe);
	pipes.bindVBO(0, 1, 2, GL_FALSE, 5, 3, pipe);
	
	glm::mat4 VP(1.0f);
	background.setMat4("MVP", VP);
	
	// pos of the centre x of the pipe
	std::vector<float> xpositions(N_PIPES, 0);
	for(int i = 1; i<N_PIPES; i++){
		float newPos = xpositions[i-1] + DIST_BW_PIPES + WIDTH;
		xpositions[i] = newPos;
	}
	float lastPos = xpositions.back();
	
	std::vector<float> heights(N_PIPES, 0);
	std::vector<float> heightsUp(N_PIPES, 0);

	for(int i = 0; i<N_PIPES; i++){
		srand(time(0));
		heights[i] = calcScale();
		heightsUp[i] = calcInverseScale(heights[i]);
	}		
	bool exit = false;
	while(!glfwWindowShouldClose(window) && !exit){
		glfwPollEvents();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		exit = theWindow.gameLoop(xpositions, heights, heightsUp);
		
		// modelling the pipes
		for(int i = 0; i<N_PIPES; i++){
			glm::mat4 model(1.0f), modelRev(1.0f);	
			
			model = glm::translate(model, glm::vec3(xpositions[i], -1, 0));
			modelRev = glm::translate(modelRev, glm::vec3(xpositions[i], 1, 0));		
			model = glm::scale(model, glm::vec3(1, heights[i], 1));
			modelRev = glm::scale(modelRev, glm::vec3(1, heightsUp[i], 1));
			modelRev = glm::rotate(modelRev, glm::radians(180.0f), glm::vec3(0, 0, 1));
			
			xpositions[i] += X_OFFSET;

			pipes.draw(0);
			pipes.setMat4("MVP", VP*model);
			pipes.enableTexture(0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pipes.stopDraw();
			
			// drawing reversed pipes
			pipes.draw(0);
			pipes.setMat4("MVP", VP*modelRev);
			pipes.enableTexture(0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pipes.stopDraw();
			
			if(xpositions[i] + WIDTH/2.0 <= -1.0){
				xpositions.erase(xpositions.begin() + i);
				heights.erase(heights.begin() + i);
				heightsUp.erase(heightsUp.begin() + i);
				xpositions.push_back(lastPos);
				heights.push_back(calcScale());
				heightsUp.push_back(calcInverseScale(heights.back()));
			}
		}
						
		background.draw(0);
		background.enableTexture(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		background.stopDraw();

		glfwSwapBuffers(window);
	}
}
