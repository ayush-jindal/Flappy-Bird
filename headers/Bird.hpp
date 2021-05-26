#ifndef BIRD_HPP
#define BIRD_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LightShaders.hpp"

class Bird{
private:
	float jumpHeight;
	glm::vec3 birdPos;
	float g;
	float bird_half_width;
	float pipe_half_width;
	float pipe_height;
	LightShaders bird;
	bool collisionCheck(std::vector<float> xpositions, std::vector<float>heights, std::vector<float>heightsUp);
public:
	Bird();
	void initialize();
	Bird(glm::vec3 birdPos, float jumpHeight, float g, float pipeWidth, float pipeHeight, float birdFactor);
	void draw();
	void setMVP(glm::mat4 MVP);
	void jump();
	bool gameLoop(std::vector<float> xpositions, std::vector<float>heights, std::vector<float>heightsUp);
	
	~Bird();
};

#endif
