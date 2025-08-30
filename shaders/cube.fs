#version 330 core

in vec3 FragPos;

out vec4 outColor;

void main()
{
	outColor = vec4(vec3(FragPos), 1.0);	
}
