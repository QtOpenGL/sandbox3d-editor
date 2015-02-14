#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	vec3 position;
	vec2 texCoord;
	vec4 lightCoord;
	float depth_inView;
};

uniform mat4x4 ViewProjection;
uniform mat4x4 View;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = TEXCOORD0)	in vec2 inTexCoord;

out VS_OUTPUT vsOut;

void main(void)
{
	vec4 worldPos = vec4(inPosition, 1.0) * Model;
	vec4 viewPos = worldPos * View;

	vsOut.position = worldPos.xyz;
	vsOut.texCoord = inTexCoord;
	vsOut.depth_inView = abs(viewPos.z / viewPos.w);

	vec4 pos = worldPos * ViewProjection;
	vsOut.lightCoord = pos;
	gl_Position = pos;
}
