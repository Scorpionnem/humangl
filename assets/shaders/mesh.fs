#version 330 core

in vec3 FragPos;
uniform vec3 color;

out vec4 outColor;

void main()
{
	outColor = vec4(vec3(1), 1.0);	
}
