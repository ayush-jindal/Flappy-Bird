#ifndef SHADER_HPP
#define SHADER_HPP
// to avoid linking conflicts in case of this file included by several files

class Shaders{
protected:
	struct vertexArray{
		GLuint VAO;
		std::vector<GLuint> VBO;		
		GLuint IBO;
	};
protected:
	GLuint programID;
	std::string LoadCode(char* shaderFile);
	int AddShader(GLuint program, char* shaderCode, GLenum shaderType);
	std::vector<vertexArray> VAOs;
	std::vector<GLuint> textures;
public:
	Shaders();
	Shaders(int nVAO, int* nVBO, int* nIBO, int nTextures);
	GLuint initialize(char* vertex, char* fragment);
	void draw(GLuint VAOindex);
	void enableTexture(GLuint textureIndex);
	void stopDraw();
	void bindVBO(GLuint VAOindex, GLuint VBOindex, GLuint size, GLboolean normalized, GLsizei stride, GLsizei offset, std::vector<float>& data) const;
	void bindIBO(GLuint VAOindex, std::vector<unsigned int>& data) const;
	int loadTexture(GLuint textureIndex, const char* texture);
	void setBool(const char* location, bool val);
	void setInt(const char* location, int val);
	void setFloat(const char* location, float val);
	void setVec2(const char* location, glm::vec2 val);
	void setVec3(const char* location, glm::vec3 val);
	void setVec4(const char* location, glm::vec4 val);
	void setMat2(const char* location, glm::mat2 val);
	void setMat3(const char* location, glm::mat3 val);
	void setMat4(const char* location, glm::mat4 val);
	void setStruct(const char* location, int intVal, std::vector<glm::vec3> vec3Val, std::vector<const char*> charVal, std::vector<float> floatVal);
	~Shaders();
};

#endif
