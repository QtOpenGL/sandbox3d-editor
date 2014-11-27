#version 330

#define USE_PCF 1

uniform mat4x4 DepthTransformation;

uniform sampler2D lightSampler;

uniform sampler2DShadow shadowMap;

in vec3 outPosition;
in vec2 outTexCoord;
in vec4 outLightCoord;

out vec3 outColor;

vec3 ambient = vec3(0.3, 0.0, 0.0);

float getShadow(vec3 position)
{
	vec4 shadowCoord = vec4(position, 1.0) * DepthTransformation;
	shadowCoord.xyz = (shadowCoord.xyz/shadowCoord.w) * vec3(0.5, 0.5, 0.5) + vec3(0.5, 0.5, 0.5);

	#if USE_PCF
		float factor = 0.0;

		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(-1, -1));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(-1, 0));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(-1, 1));

		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(0, -1));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(0, 0));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(0, 1));

		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(1, -1));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(1, 0));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(1, 1));

		return (0.5 + (factor / 18.0));
	#else
		return (0.5 + texture(shadowMap, shadowCoord.xyz) * 0.5);
	#endif
}

void main(void)
{
	vec3 diffuse = vec3(0.5, 0.5, 0.5);

	vec2 fragcoord = outLightCoord.xy/outLightCoord.w * 0.5 + 0.5;
	vec3 light_precalculated = texture(lightSampler, fragcoord).rgb;

	float shadow_factor = getShadow(outPosition);

	outColor = diffuse * shadow_factor * light_precalculated + ambient;
}

