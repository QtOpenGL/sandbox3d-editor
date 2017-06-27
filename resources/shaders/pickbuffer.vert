#version 330 core

layout(location = 0)in vec3 inPosition;
layout(location = 5)in uint inColor;

layout(std140)uniform CameraBlock { mat4x4 View;mat4x4 Projection;mat4x4 ViewProjection;mat4x4 InverseView;mat4x4 InverseProjection;mat4x4 InverseViewProjection;float aspectRatio;float tanHalfFOV;};

uniform mat4x4 Model;

flat out uint color;

void main(void)
{
	color = inColor;
	gl_Position = vec4(inPosition, 1.0) * Model * ViewProjection;
}
