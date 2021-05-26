#version 330

in vec2 vTexCoord;

uniform sampler2D texSampler0;

out vec4 fColor;

void main(){
	fColor = texture(texSampler0, vTexCoord);
}
