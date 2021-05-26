#ifndef SHADER_LIGHT_HPP
#define SHADER_LIGHT_HPP
// to avoid linking conflicts in case of this file included by several files
#include "shaders.hpp"

class LightShaders: public Shaders{

public:
	LightShaders();
	LightShaders(int nVAO, int* nVBO, int* nIBO, int nTextures);
	void draw(GLuint VAOindex);
	int loadTexture(GLuint textureIndex, const char* texture);
	void detail();
	~LightShaders();
};

#endif
