#version 330 core

flat in uint color;

out uint id;

void main(void)
{
	id = color;
}
