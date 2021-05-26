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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shaders.hpp"

Shaders::Shaders(){ std::cout<<"Only for derived classes"<<std::endl; }

Shaders::Shaders(int nVAO, int* nVBO, int* nIBO, int nTextures){
	
	this->VAOs = std::vector<vertexArray>(nVAO, vertexArray());
	std::cout<<"Max of one IBO per VAO"<<std::endl;
	std::cout<<"Min of three textures per VAO"<<std::endl;
	std::cout<<nVAO<<" VAOs created"<<std::endl;
	for(unsigned int i = 0; i < nVAO; i++){
		glGenVertexArrays(1, &VAOs[i].VAO);
		std::cout<<"\t"<<i+1<<") VAO"<<std::endl;
		VAOs[i].VBO = std::vector<GLuint>(nVBO[i], 0);
		glGenBuffers(nVBO[i], &(VAOs[i].VBO.front()));		
		glGenBuffers(std::min(nIBO[i], 1), &VAOs[i].IBO);
		std::cout<<"\t\t"<<nVBO[i]<<" VBOs "<<std::min(nIBO[i], 1)<<" IBOs "<<std::endl;			
	}
	textures = std::vector<GLuint>(std::max(nTextures, 3), 0);
	glGenTextures(std::max(nTextures, 3), &(textures.front()));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	std::cout<<std::max(nTextures, 3)<<" textures attached"<<std::endl;
}

std::string Shaders::LoadCode(char* shaderFile){
	std::string code;
	std::ifstream codeStream(shaderFile);
	if(codeStream.is_open()){
		std::stringstream sstr;
		sstr << codeStream.rdbuf();
		code = sstr.str();
		codeStream.close();
	}
	else{
		printf("Unable to open the vertex shader file");
		return NULL;
	}
	return code;
}

int Shaders::AddShader(GLuint program, char* shaderCode, GLenum shaderType){
	GLuint shader = glCreateShader(shaderType);
	
	GLchar* code[1];
	code[0] = shaderCode;
	GLint codeLen[1];
	codeLen[0] = strlen(shaderCode);
	
	glShaderSource(shader, 1, code, codeLen);
	glCompileShader(shader);
	
	GLint result = 0;
	GLchar eLog[1024] = {0};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	
	if(!result){
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling %d shader: '%s'", shaderType, eLog);
		return -1;
	}
	
	glAttachShader(program, shader);
	return 0;
}

GLuint Shaders::initialize(char* vertex, char* fragment){
	GLuint program = glCreateProgram();
	if(!program){
		printf("Error creating a program");
		return -1;
	}
	
	std::string vertexCode = LoadCode(vertex);
	std::string fragmentCode = LoadCode(fragment);
	if(!vertexCode.empty() && !fragmentCode.empty()){
		if(AddShader(program, &vertexCode[0], GL_VERTEX_SHADER) != 0){
			printf("Error adding the vertex shader");
			return -1;
		}
		if(AddShader(program, &fragmentCode[0], GL_FRAGMENT_SHADER) != 0){
				printf("Error adding the fragment shaders");
				return -1;
		}
	}
	else{
		printf("Shaders could not be loaded");
		return -1;
	}
	
	GLint result = 0;
	GLchar eLog[1024];
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(program, sizeof(eLog), NULL, eLog);
		printf("error linking the shaders: '%s'", eLog);
		return -1;
	}
	
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(program, sizeof(eLog), NULL, eLog);
		printf("error validating the program: '%s'", eLog);
		return -1;
	}
	
	programID = program;
	return 0;
}

int Shaders::loadTexture(GLuint textureIndex, const char* texture){

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
		/*glUseProgram(programID);

		std::string texVar = "texSampler";
		texVar.append(std::to_string(textureIndex));
		setInt(texVar.c_str(), textureIndex);
		glUseProgram(0);*/
		if(textureIndex == 0)
			setInt("material.diffuse", textures[textureIndex]);
		else if(textureIndex == 1)
			setInt("material.specular", textures[textureIndex]);
		else if(textureIndex == 2)
			setInt("material.emission", textures[textureIndex]);			
		return 0;
	} else {
		std::cout<<"Error loading "<<texture<<std::endl;
		return -1;
	}
}


void Shaders::bindVBO(GLuint VAOindex, GLuint VBOindex, GLuint size, GLboolean normalized, GLsizei stride, GLsizei offset, std::vector<float>& data) const {
	vertexArray VA = VAOs[VAOindex];
	glBindVertexArray(VA.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VA.VBO[VBOindex]);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(VBOindex, size, GL_FLOAT, normalized, stride*sizeof(float), (void*)(offset*sizeof(float)));
	glEnableVertexAttribArray(VBOindex);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Shaders::bindIBO(GLuint VAOindex, std::vector<unsigned int>& data) const {
	vertexArray VA = VAOs[VAOindex];
	glBindVertexArray(VA.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VA.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(unsigned int), &data.front(), GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Shaders::draw(GLuint VAOindex){
	vertexArray VA = VAOs[VAOindex];
	glUseProgram(programID);
	glBindVertexArray(VA.VAO);
	
	// first for diffuse, then specular and last for emissive
	enableTexture(0);
	enableTexture(1);
	enableTexture(2);	
}

void Shaders::enableTexture(GLuint textureIndex){	
	glActiveTexture(GL_TEXTURE0 + textures[textureIndex]);
	glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
}

void Shaders::stopDraw(){
	glBindVertexArray(0);
	glUseProgram(0);
}

void Shaders::setBool(const char* location, bool val){
	setInt(location, int(val));
	/*int value[1] = {-100};
	std::cout<<val<<std::endl;
	std::cout<<int(val)<<std::endl;
		std::cout<<glGetUniformLocation(programID, location)<<std::endl;
		glGetUniformiv(programID, glGetUniformLocation(programID, location), value);
		std::cout<<value[0]<<std::endl;*/
}
void Shaders::setInt(const char* location, int val){
	glProgramUniform1i(programID, glGetUniformLocation(programID, location), val);
}
void Shaders::setFloat(const char* location, float val){
	glProgramUniform1f(programID, glGetUniformLocation(programID, location), val);
}
void Shaders::setVec2(const char* location, glm::vec2 val){
	glProgramUniform2fv(programID, glGetUniformLocation(programID, location), 1, glm::value_ptr(val));
};
void Shaders::setVec3(const char* location, glm::vec3 val){
	glProgramUniform3fv(programID, glGetUniformLocation(programID, location), 1, glm::value_ptr(val));
}
void Shaders::setVec4(const char* location, glm::vec4 val){
	glProgramUniform4fv(programID, glGetUniformLocation(programID, location), 1, glm::value_ptr(val));
}
void Shaders::setMat2(const char* location, glm::mat2 val){
	glProgramUniformMatrix2fv(programID, glGetUniformLocation(programID, location), 1, GL_FALSE, glm::value_ptr(val));
}
void Shaders::setMat3(const char* location, glm::mat3 val){
	glProgramUniformMatrix3fv(programID, glGetUniformLocation(programID, location), 1, GL_FALSE, glm::value_ptr(val));
}
void Shaders::setMat4(const char* location, glm::mat4 val){
	glProgramUniformMatrix4fv(programID, glGetUniformLocation(programID, location), 1, GL_FALSE, glm::value_ptr(val));
}

void Shaders::setStruct(const char* location, int intVal, std::vector<glm::vec3> vec3Val, std::vector<const char*> charVal, std::vector<float> floatVal){
	std::vector<const char*> structs{"material", "dirLight", "spotLight", "pointLight"};
	if(strcmp(location, structs[0]) == 0){
		loadTexture(0, charVal[0]);
		loadTexture(1, charVal[1]);
		loadTexture(2, charVal[2]);
		// can change this to int if other ints also present
		setInt("material.shininess", intVal);
	} else if(strcmp(location, structs[1]) == 0){
		setVec3("dirLight.direction", vec3Val[0]);
		setVec3("dirLight.ambient", vec3Val[1]);
		setVec3("dirLight.diffuse", vec3Val[2]);
		setVec3("dirLight.specular", vec3Val[3]);
	} else if(strcmp(location, structs[2]) == 0){
		setVec3("spotLight.position", vec3Val[0]);
		setVec3("spotLight.ambient", vec3Val[1]);
		setVec3("spotLight.diffuse", vec3Val[2]);
		setVec3("spotLight.specular", vec3Val[3]);
		setVec3("spotLight.direction", vec3Val[4]);
		setFloat("spotLight.constant", floatVal[0]);
		setFloat("spotLight.linear", floatVal[1]);
		setFloat("spotLight.quadratic", floatVal[2]);
		setFloat("spotLight.cutOff", floatVal[3]);
		setFloat("spotLight.outerCutOff", floatVal[4]);
		/*float value[1] = {-100};
		std::cout<<glGetUniformLocation(programID, "spotLight.outerCutOff")<<std::endl;
		glGetUniformfv(programID, glGetUniformLocation(programID, "spotLight.outerCutOff"), value);
		std::cout<<value[0]<<std::endl;*/
	} else if(strcmp(location, structs[3]) == 0){
		std::string lightVar = "pointLight[" + std::to_string(intVal) + "]";
		setVec3((lightVar + ".position").c_str(), vec3Val[0]);
		setVec3((lightVar + ".ambient").c_str(), vec3Val[1]);
		setVec3((lightVar + ".diffuse").c_str(), vec3Val[2]);
		setVec3((lightVar + ".specular").c_str(), vec3Val[3]);
		setFloat((lightVar + ".constant").c_str(), floatVal[0]);
		setFloat((lightVar + ".linear").c_str(), floatVal[1]);
		setFloat((lightVar + ".quadratic").c_str(), floatVal[2]);
	}
}

Shaders::~Shaders(){}
