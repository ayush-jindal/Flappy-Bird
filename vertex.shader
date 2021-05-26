#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 MVP;

out vec2 vTexCoord;

void main(){
	gl_Position = MVP * vec4(pos, 1.0);
	vTexCoord = texCoord;
}
