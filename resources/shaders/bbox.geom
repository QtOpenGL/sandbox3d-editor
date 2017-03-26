#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 18) out;

uniform mat4x4 ModelViewProjection;

uniform vec3 BBoxMin;
uniform vec3 BBoxMax;

void main()
{
	vec4 vertices [8];

	// front
	vertices[0] = vec4(BBoxMin.x, BBoxMin.y, BBoxMin.z, 1.0) * ModelViewProjection;
	vertices[1] = vec4(BBoxMax.x, BBoxMin.y, BBoxMin.z, 1.0) * ModelViewProjection;
	vertices[2] = vec4(BBoxMin.x, BBoxMax.y, BBoxMin.z, 1.0) * ModelViewProjection;
	vertices[3] = vec4(BBoxMax.x, BBoxMax.y, BBoxMin.z, 1.0) * ModelViewProjection;
	// back
	vertices[4] = vec4(BBoxMin.x, BBoxMin.y, BBoxMax.z, 1.0) * ModelViewProjection;
	vertices[5] = vec4(BBoxMax.x, BBoxMin.y, BBoxMax.z, 1.0) * ModelViewProjection;
	vertices[6] = vec4(BBoxMin.x, BBoxMax.y, BBoxMax.z, 1.0) * ModelViewProjection;
	vertices[7] = vec4(BBoxMax.x, BBoxMax.y, BBoxMax.z, 1.0) * ModelViewProjection;

	// ---------

	gl_Position = vec4(vertices[0]);
	EmitVertex();
	gl_Position = vec4(vertices[1]);
	EmitVertex();
	gl_Position = vec4(vertices[3]);
	EmitVertex();
	gl_Position = vec4(vertices[2]);
	EmitVertex();
	gl_Position = vec4(vertices[0]);
	EmitVertex();

	EndPrimitive();

	// ---------

	gl_Position = vec4(vertices[4]);
	EmitVertex();
	gl_Position = vec4(vertices[5]);
	EmitVertex();
	gl_Position = vec4(vertices[7]);
	EmitVertex();
	gl_Position = vec4(vertices[6]);
	EmitVertex();
	gl_Position = vec4(vertices[4]);
	EmitVertex();

	EndPrimitive();

	// ---------

	gl_Position = vec4(vertices[0]);
	EmitVertex();
	gl_Position = vec4(vertices[4]);
	EmitVertex();

	EndPrimitive();

	// ---------

	gl_Position = vec4(vertices[1]);
	EmitVertex();
	gl_Position = vec4(vertices[5]);
	EmitVertex();

	EndPrimitive();

	// ---------

	gl_Position = vec4(vertices[2]);
	EmitVertex();
	gl_Position = vec4(vertices[6]);
	EmitVertex();

	EndPrimitive();

	// ---------

	gl_Position = vec4(vertices[3]);
	EmitVertex();
	gl_Position = vec4(vertices[7]);
	EmitVertex();

	EndPrimitive();

}
