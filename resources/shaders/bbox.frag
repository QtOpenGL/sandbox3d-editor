#version 330 core

uniform vec3 color = vec3(1.0, 1.0, 1.0);

out vec3 outColor;

void main()
{
	outColor = color;
}
