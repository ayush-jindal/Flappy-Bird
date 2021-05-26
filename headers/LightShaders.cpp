#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "stb_image.h"

#include "LightShaders.hpp"

LightShaders::LightShaders(){};

LightShaders::LightShaders(int nVAO, int* nVBO, int* nIBO, int nTextures){
	
	this->VAOs = std::vector<vertexArray>(nVAO, vertexArray());
	std::cout<<"Max of one IBO per VAO"<<std::endl;
	std::cout<<nVAO<<" VAOs created"<<std::endl;
	for(unsigned int i = 0; i < nVAO; i++){
		std::cout<<"here"<<std::endl;
		glGenVertexArrays(1, &VAOs[i].VAO);
		std::cout<<"\t"<<i+1<<") VAO"<<std::endl;
		VAOs[i].VBO = std::vector<GLuint>(nVBO[i], 0);
		std::cout<<VAOs[i].VBO.size()<<std::endl;
		glGenBuffers(nVBO[i], &(VAOs[i].VBO.front()));
		glGenBuffers(std::min(nIBO[i], 1), &VAOs[i].IBO);
		std::cout<<"\t\t"<<nVBO[i]<<" VBOs "<<std::min(nIBO[i], 1)<<" IBOs"<<std::endl;		
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	textures = std::vector<GLuint>(nTextures, 0);
	glGenTextures(nTextures, &(textures.front()));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	
	std::cout<<"\t\t"<<nTextures<<" textures attached"<<std::endl;
}

int LightShaders::loadTexture(GLuint textureIndex, const char* texture){

	int width, height, nChannels;	
	unsigned char *data = stbi_load(texture, &width, &height, &nChannels, 0);
	if(width!=height)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	
	if(data){
		glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
		if(nChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if(nChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		std::string texVar = "texSampler";
		texVar.append(std::to_string(textureIndex));
		setInt(texVar.c_str(), textures[textureIndex]);
		
		return 0;
	} else {
		std::cout<<"Error loading "<<texture<<std::endl;
		return -1;
	}
}

void LightShaders::draw(GLuint VAOindex){
	vertexArray VA = VAOs[VAOindex];
	glUseProgram(programID);
	glBindVertexArray(VA.VAO);
}

void LightShaders::detail(){
	std::cout<<programID<<std::endl;
	std::cout<<VAOs.size()<<std::endl;
	std::cout<<VAOs[0].VAO<<std::endl;
	std::cout<<VAOs[0].VBO.size()<<std::endl;
	std::cout<<textures.size()<<std::endl;
}

LightShaders::~LightShaders(){}
