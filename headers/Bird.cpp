#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include "Bird.hpp"

Bird::Bird(){}

Bird::Bird(glm::vec3 birdPos, float jumpHeight, float g, float pipeWidth, float pipeHeight, float birdFactor){
	this->jumpHeight = jumpHeight;
	this->birdPos = birdPos;
	this->g = g;
	this->bird_half_width = float(pipeWidth/birdFactor);
	this->pipe_half_width = pipeWidth/2.0f;
	this->pipe_height = pipeHeight;
	this->bird = LightShaders();
}

void Bird::initialize(){
	int VBO[] = {2};
	int IBO[] = {0};
	bird = LightShaders(1, VBO, IBO, 1);
	bird.initialize((char*)"vertex.shader", (char*)"fragment.shader");
	bird.loadTexture(0, "bird.png");
	
	std::vector<float> bird_coord = {
		-bird_half_width, bird_half_width, 0.0f, 0.0f, 1.0f,
		bird_half_width, bird_half_width, 0.0f, 1.0f, 1.0f,
		bird_half_width, -bird_half_width, 0.0f, 1.0f, 0.0f,
		bird_half_width, -bird_half_width, 0.0f, 1.0f, 0.0f,
		-bird_half_width, -bird_half_width, 0.0f, 0.0f, 0.0f,
		-bird_half_width, bird_half_width, 0.0f, 0.0f, 1.0f
	};
	
	bird.bindVBO(0, 0, 3, GL_FALSE, 5, 0, bird_coord);
	bird.bindVBO(0, 1, 2, GL_FALSE, 5, 3, bird_coord);
}

void Bird::draw(){
	bird.draw(0);
	bird.enableTexture(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	bird.stopDraw();
}

void Bird::setMVP(glm::mat4 MVP){ bird.setMat4("MVP", MVP); }
void Bird::jump(){ birdPos[1] += jumpHeight; }

bool Bird::gameLoop(std::vector<float> xpositions, std::vector<float>heights, std::vector<float>heightsUp){
	
	bool check = collisionCheck(xpositions, heights, heightsUp);

	if(!check){
	static float lastCalled = glfwGetTime();
	float present = glfwGetTime();
	birdPos[1] -= 0.5f*g*(present - lastCalled)*(present - lastCalled);
	glm::mat4 birdModel(1.0f);
	birdModel = glm::translate(birdModel, birdPos);
	lastCalled = present;

	setMVP(birdModel);
	}
	
	return check;
}

bool Bird::collisionCheck(std::vector<float> xpositions, std::vector<float>heights, std::vector<float>heightsUp){
	int x = 0;
	for(; x<xpositions.size(); x++){
		if(birdPos[1] + bird_half_width >= 1.0 - pipe_height*heightsUp[x] || birdPos[1] - bird_half_width <= pipe_height*heights[x] - 1.0)
			if(xpositions[x] + pipe_half_width >= birdPos[0] - bird_half_width && xpositions[x] + pipe_half_width <= birdPos[0] + bird_half_width)
				{
					std::cout<<"in first cond"<<std::endl;
					return true;
				}// the bird is leaving the pipe
			else if(xpositions[x] + pipe_half_width >= birdPos[0] + bird_half_width && xpositions[x] - pipe_half_width <= birdPos[0] - bird_half_width){
				
					std::cout<<"in second cond"<<std::endl;
					return true;
				}// the bird is on the pipe
			else if(xpositions[x] - pipe_half_width <= birdPos[0] + bird_half_width && xpositions[x] - pipe_half_width >= birdPos[0] - bird_half_width){
				
					std::cout<<"in third cond"<<std::endl;
					return true;
				}// the bird is entering the pipe
	}
	//std::cout<<"******************************************"<<std::endl;
	return false;	
}

Bird::~Bird(){};
