#pragma once

#include <vulkan/vulkan.h>

#if HAVE_OPENGL
#	define E(NAME,GL,VK) NAME = GL
#endif // HAVE_OPENGL

#if HAVE_VULKAN
#define E(NAME,GL,VK) NAME = VK
#endif // HAVE_VULKAN

namespace RHI
{

enum PrimitiveType
{
	E(PRIMITIVE_POINT_LIST, GL_POINTS, VK_PRIMITIVE_TOPOLOGY_POINT_LIST),
	E(PRIMITIVE_LINE_LIST, GL_LINES, VK_PRIMITIVE_TOPOLOGY_LINE_LIST),
	E(PRIMITIVE_LINE_STRIP, GL_LINE_STRIP, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP),
	E(PRIMITIVE_TRIANGLE_LIST, GL_TRIANGLES, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
	E(PRIMITIVE_TRIANGLE_STRIP, GL_TRIANGLE_STRIP, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP),
	E(PRIMITIVE_TRIANGLE_FAN, GL_TRIANGLE_FAN, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN),
	E(PRIMITIVE_LINE_LIST_WITH_ADJACENCY, GL_LINES_ADJACENCY, VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY),
	E(PRIMITIVE_LINE_STRIP_WITH_ADJACENCY, GL_LINE_STRIP_ADJACENCY, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY),
	E(PRIMITIVE_TRIANGLE_LIST_WITH_ADJACENCY, GL_TRIANGLES_ADJACENCY, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY),
	E(PRIMITIVE_TRIANGLE_STRIP_WITH_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY),
	E(PRIMITIVE_PATCH_LIST, GL_PATCHES,	VK_PRIMITIVE_TOPOLOGY_PATCH_LIST),
};

enum CompareOp
{
	E(COMPARE_OP_NEVER, GL_NEVER, VK_COMPARE_OP_NEVER),
	E(COMPARE_OP_LESS, GL_LESS, VK_COMPARE_OP_LESS),
	E(COMPARE_OP_EQUAL, GL_EQUAL, VK_COMPARE_OP_EQUAL),
	E(COMPARE_OP_LESS_OR_EQUAL, GL_LEQUAL, VK_COMPARE_OP_LESS_OR_EQUAL),
	E(COMPARE_OP_GREATER, GL_GREATER, VK_COMPARE_OP_GREATER),
	E(COMPARE_OP_NOT_EQUAL, GL_NOTEQUAL, VK_COMPARE_OP_NOT_EQUAL),
	E(COMPARE_OP_GREATER_OR_EQUAL, GL_GEQUAL, VK_COMPARE_OP_GREATER_OR_EQUAL),
	E(COMPARE_OP_ALWAYS, GL_ALWAYS, VK_COMPARE_OP_ALWAYS),
};

enum ColorMask
{
	COLOR_MASK_R = 1,
	COLOR_MASK_G = 2,
	COLOR_MASK_B = 4,
	COLOR_MASK_A = 8
};

enum BlendFactor
{
	E(BLEND_FACTOR_ZERO, GL_ZERO, VK_BLEND_FACTOR_ZERO),
	E(BLEND_FACTOR_ONE, GL_ONE, VK_BLEND_FACTOR_ONE),
	E(BLEND_FACTOR_SRC_COLOR, GL_SRC_COLOR, VK_BLEND_FACTOR_SRC_COLOR),
	E(BLEND_FACTOR_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR),
	E(BLEND_FACTOR_DST_COLOR, GL_DST_COLOR, VK_BLEND_FACTOR_DST_COLOR),
	E(BLEND_FACTOR_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR),
	E(BLEND_FACTOR_SRC_ALPHA, GL_SRC_ALPHA, VK_BLEND_FACTOR_SRC_ALPHA),
	E(BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
	E(BLEND_FACTOR_DST_ALPHA, GL_DST_ALPHA, VK_BLEND_FACTOR_DST_ALPHA),
	E(BLEND_FACTOR_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA),
	/*
	E(BLEND_FACTOR_CONSTANT_COLOR = 10,          VK_BLEND_FACTOR_CONSTANT_COLOR),
	E(BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR = 11,VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR),
	E(BLEND_FACTOR_CONSTANT_ALPHA = 12,          VK_BLEND_FACTOR_CONSTANT_ALPHA),
	E(BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA = 13,VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA),
	E(BLEND_FACTOR_SRC_ALPHA_SATURATE = 14,      VK_BLEND_FACTOR_SRC_ALPHA_SATURATE),
	E(BLEND_FACTOR_SRC1_COLOR = 15,              VK_BLEND_FACTOR_SRC1_COLOR),
	E(BLEND_FACTOR_ONE_MINUS_SRC1_COLOR = 16,    VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR),
	E(BLEND_FACTOR_SRC1_ALPHA = 17,              VK_BLEND_FACTOR_SRC1_ALPHA),
	E(BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA = 18,    VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA),
	*/
};

enum BlendOp
{
	E(BLEND_OP_ADD, GL_FUNC_ADD, VK_BLEND_OP_ADD),
	E(BLEND_OP_SUBTRACT, GL_FUNC_SUBTRACT, VK_BLEND_OP_SUBTRACT),
	E(BLEND_OP_REVERSE_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, VK_BLEND_OP_REVERSE_SUBTRACT),
	E(BLEND_OP_MIN, GL_MIN, VK_BLEND_OP_MIN),
	E(BLEND_OP_MAX, GL_MAX, VK_BLEND_OP_MAX),
};

}
