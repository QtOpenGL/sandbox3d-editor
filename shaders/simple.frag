#version 330 core

in vec2 vertex_uv;
out vec3 outColor;

uniform sampler2D texSampler;

void main()
{
	outColor = texture(texSampler, vertex_uv).rgb;
}
