#include "RendererWrapper.h"

// -------------------------------------------------------------
// TODO : remove this
#define HAVE_OPENGL 1
#define HAVE_VULKAN 0
#define HAVE_GL_GLCOREARB_H 1
#include "Rendering/Rendering.h"
// -------------------------------------------------------------

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSIMP_MAT4X4(m) mat4x4(m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4, m.c1, m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4);

#define _min(x, y) ((x < y) ? x : y)
#define _max(x, y) ((x > y) ? x : y)

// -------------------------------------------------------------

#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>

#include <QGLWidget> // for convertToGLFormat

// -------------------------------------------------------------

struct SubMeshDefinition
{
	unsigned int triangle_count; // GL_TRIANGLES
	unsigned int index_offset; // GL_UNSIGNED_INT
	unsigned int base_vertex;
	SubMesh::Material material;
	GLuint m_NormalMapId;
	vec3 m_vMin;
	vec3 m_vMax;
};

static GLuint loadTexture(const char * szFilename);
static void loadAllMaterials(const aiScene * scene);
static void addMeshRecursive(const aiNode * nd, const mat4x4 & parentTransformation, const std::vector<SubMesh *> & preloaded, GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer, const std::vector<SubMeshDefinition> & offsets, const std::vector<Mesh::VertexSpec> & specs, std::vector<Mesh*> & apMeshes, Scene & scene);

std::vector<Mesh*> g_apMeshes;

#ifndef RENDERER_LIB
#	error "RENDERER_LIB must be defined"
#endif // RENDERER_LIB

/**
 * @brief RendererWrapper::RendererWrapper
 */
RendererWrapper::RendererWrapper(void) : m_rendererLibrary(RENDERER_LIB)
{
	m_pScene = new Scene();

	//
	// Load renderer library
	if (!m_rendererLibrary.load())
	{
		return;
	}

	renderer_onLoad_function OnLoad = (renderer_onLoad_function)m_rendererLibrary.resolve("renderer_onLoad");

	if (OnLoad == nullptr)
	{
		m_rendererLibrary.unload();
		return;
	}

	if (!OnLoad())
	{
		m_rendererLibrary.unload();
		return;
	}

	m_pOnInit = (renderer_onInit_function)m_rendererLibrary.resolve("renderer_onInit");
	m_pOnRelease = (renderer_onRelease_function)m_rendererLibrary.resolve("renderer_onRelease");

	m_pOnReady = (renderer_onReady_function)m_rendererLibrary.resolve("renderer_onReady");
	m_pOnResize = (renderer_onResize_function)m_rendererLibrary.resolve("renderer_onResize");
	m_pOnUpdate = (renderer_onUpdate_function)m_rendererLibrary.resolve("renderer_onUpdate");

	m_pInitQueue = (renderer_initQueue_function)m_rendererLibrary.resolve("renderer_initQueue");
	m_pReleaseQueue = (renderer_releaseQueue_function)m_rendererLibrary.resolve("renderer_releaseQueue");

	m_pGetRenderTexture = (renderer_getRenderTexture_function)m_rendererLibrary.resolve("renderer_getRenderTexture");
}

/**
 * @brief RendererWrapper::~RendererWrapper
 */
RendererWrapper::~RendererWrapper(void)
{
	m_rendererLibrary.unload();
	delete m_pScene;
}

/**
 * @brief RendererWrapper::init
 * @return
 */
bool RendererWrapper::init(void)
{
	if (m_pOnInit)
	{
		return(m_pOnInit(*m_pScene));
	}

	return(false);
}

/**
 * @brief RendererWrapper::release
 * @return
 */
bool RendererWrapper::release(void)
{
	if (m_pOnRelease)
	{
		return(m_pOnRelease());
	}

	return(false);
}

/**
 * @brief RendererWrapper::onReady
 */
void RendererWrapper::onReady(void)
{
	if (m_pOnReady)
	{
		m_pOnReady();
	}
}

/**
 * @brief RendererWrapper::onResize
 * @param width
 * @param height
 */
void RendererWrapper::onResize(unsigned int width, unsigned int height)
{
	if (m_pOnResize)
	{
		m_pOnResize(width, height);
	}
}

/**
 * @brief RendererWrapper::onUpdate
 * @param mView
 * @param clearColor
 */
void RendererWrapper::onUpdate(const mat4x4 & mView)
{
	if (m_pOnUpdate)
	{
		m_pOnUpdate(mView);
	}
}

/**
 * @brief RendererWrapper::initQueue
 * @param szFilename
 */
void RendererWrapper::initQueue(const char * szFilename)
{
	if (m_pInitQueue)
	{
		m_pInitQueue(szFilename);
	}
}

/**
 * @brief RendererWrapper::releaseQueue
 */
void RendererWrapper::releaseQueue(void)
{
	if (m_pReleaseQueue)
	{
		m_pReleaseQueue();
	}
}

/**
 * @brief RendererWrapper::GetRenderTexture
 * @param name
 * @return
 */
GLuint RendererWrapper::GetRenderTexture(const char * name) const
{
	if (m_pGetRenderTexture)
	{
		return(m_pGetRenderTexture(name));
	}

	return(0);
}

/**
 * @brief RendererWrapper::getScene
 * @return
 */
Scene & RendererWrapper::getScene(void)
{
	return(*m_pScene);
}

/**
 * @brief RendererWrapper::getScene
 * @return
 */
const Scene & RendererWrapper::getScene(void) const
{
	return(*m_pScene);
}

/**
 * @brief RendererWrapper::importToScene
 * @param szFilename
 * @return
 */
bool RendererWrapper::importToScene(const char * szFilename)
{
	std::string filepath(szFilename);

	Assimp::Importer importer;

	const aiScene * scene = importer.ReadFile(filepath.data(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		return(false);
	}

	loadAllMaterials(scene);

	std::vector<SubMesh*> meshes;
	std::vector<SubMeshDefinition> offsets;

	uint NumVertices = 0;
	uint NumIndices = 0;

	// Count the number of vertices and indices
	for (int i = 0 ; i < scene->mNumMeshes ; i++)
	{
		NumVertices += scene->mMeshes[i]->mNumVertices;
		NumIndices  += scene->mMeshes[i]->mNumFaces;
	}

	// Reserve GPU memory for the vertex attributes and indices
	GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer = new GPU::Buffer<GL_ARRAY_BUFFER>();
	GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer = new GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER>();

	GPU::realloc(*vertexBuffer, NumVertices * sizeof(SubMesh::VertexSimple));
	GPU::realloc(*indexBuffer, NumIndices * 3 * sizeof(unsigned int));

	void * pVertexGPU = GPU::mmap(*vertexBuffer, GL_WRITE_ONLY);
	void * pIndexGPU = GPU::mmap(*indexBuffer, GL_WRITE_ONLY);

	std::vector<Mesh::VertexSpec> specs;

	Mesh::VertexSpec SPEC_POS;
	SPEC_POS.index			= 0;
	SPEC_POS.size			= 3;
	SPEC_POS.type			= GL_FLOAT;
	SPEC_POS.normalized		= GL_FALSE;
	SPEC_POS.stride			= sizeof(SubMesh::VertexSimple);
	SPEC_POS.pointer		= BUFFER_OFFSET(0);

	Mesh::VertexSpec SPEC_UV;
	SPEC_UV.index			= 2;
	SPEC_UV.size			= 2;
	SPEC_UV.type			= GL_FLOAT;
	SPEC_UV.normalized		= GL_FALSE;
	SPEC_UV.stride			= sizeof(SubMesh::VertexSimple);
	SPEC_UV.pointer			= BUFFER_OFFSET(sizeof(float)*3);

	Mesh::VertexSpec SPEC_NORMAL;
	SPEC_NORMAL.index		= 1;
	SPEC_NORMAL.size		= 3;
	SPEC_NORMAL.type		= GL_FLOAT;
	SPEC_NORMAL.normalized	= GL_FALSE;
	SPEC_NORMAL.stride		= sizeof(SubMesh::VertexSimple);
	SPEC_NORMAL.pointer		= BUFFER_OFFSET(sizeof(float)*5);

	Mesh::VertexSpec SPEC_TANGENT;
	SPEC_TANGENT.index		= 3;
	SPEC_TANGENT.size		= 3;
	SPEC_TANGENT.type		= GL_FLOAT;
	SPEC_TANGENT.normalized	= GL_FALSE;
	SPEC_TANGENT.stride		= sizeof(SubMesh::VertexSimple);
	SPEC_TANGENT.pointer	= BUFFER_OFFSET(sizeof(float)*8);

	specs.push_back(SPEC_POS);
	specs.push_back(SPEC_UV);
	specs.push_back(SPEC_NORMAL);
	specs.push_back(SPEC_TANGENT);

	//unsigned int vertex_offset = 0;
	unsigned int index_offset = 0;
	unsigned int base_vertex = 0;

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh * mesh = scene->mMeshes[i];

		std::vector<SubMesh::VertexSimple> vertices;
		std::vector<unsigned int> triangles;

		vertices.reserve(mesh->mNumVertices);
		triangles.reserve(mesh->mNumFaces*3);

		vec3 min(1e10f, 1e10f, 1e10f);
		vec3 max(-1e10f, -1e10f, -1e10f);

		// Populate the vertex attribute vectors
		for (int j = 0 ; j < mesh->mNumVertices ; ++j)
		{
			min.x = _min(min.x, mesh->mVertices[j].x);
			min.y = _min(min.y, mesh->mVertices[j].y);
			min.z = _min(min.z, mesh->mVertices[j].z);

			max.x = _max(max.x, mesh->mVertices[j].x);
			max.y = _max(max.y, mesh->mVertices[j].y);
			max.z = _max(max.z, mesh->mVertices[j].z);

			SubMesh::VertexSimple vertex;

			vertex.position.x = mesh->mVertices[j].x;
			vertex.position.y = mesh->mVertices[j].y;
			vertex.position.z = mesh->mVertices[j].z;

			vertex.normal.x = mesh->mNormals[j].x;
			vertex.normal.y = mesh->mNormals[j].y;
			vertex.normal.z = mesh->mNormals[j].z;

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.tangent.x = mesh->mTangents[j].x;
				vertex.tangent.y = mesh->mTangents[j].y;
				vertex.tangent.z = mesh->mTangents[j].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.uv.x = mesh->mTextureCoords[0][j].x;
				vertex.uv.y = mesh->mTextureCoords[0][j].y;
			}
			else
			{
				vertex.uv.x = 0.5f;
				vertex.uv.y = 0.5f;
			}

			vertices.push_back(vertex);
		}

		// Populate the index buffer
		for (int j = 0 ; j < mesh->mNumFaces ; ++j)
		{
			const aiFace & Face = mesh->mFaces[j];
			triangles.push_back(Face.mIndices[0]);
			triangles.push_back(Face.mIndices[1]);
			triangles.push_back(Face.mIndices[2]);
		}

		memcpy(pVertexGPU, (void *)vertices.data(), vertices.size() * sizeof(SubMesh::VertexSimple));
		memcpy(pIndexGPU, (void *)triangles.data(), triangles.size() * sizeof(unsigned int));

		pVertexGPU = (void*)(((char*)pVertexGPU) + vertices.size() * sizeof(SubMesh::VertexSimple));
		pIndexGPU = (void*)(((char*)pIndexGPU) + triangles.size() * sizeof(unsigned int));

		SubMeshDefinition definition;
		definition.triangle_count = triangles.size();
		definition.index_offset = index_offset;
		definition.base_vertex = base_vertex;

		definition.m_vMin = min;
		definition.m_vMax = max;

		definition.m_NormalMapId = 0;

		if (mesh->HasTangentsAndBitangents())
		{
			aiString str;
			scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &str);
			std::string texture_name(str.C_Str());

			if (g_Textures.find(texture_name) != g_Textures.end())
			{
				definition.m_NormalMapId = g_Textures[texture_name];
			}
		}

		{
			aiString str;
			scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			std::string texture_name(str.C_Str());

			if (g_Textures.find(texture_name) != g_Textures.end())
			{
				definition.material.m_diffuse = g_Textures[texture_name];
			}
			else
			{
				aiColor3D color (0.f, 0.f, 0.f);
				scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);

				GPU::Texture<GL_TEXTURE_2D> * pTexture = new GPU::Texture<GL_TEXTURE_2D>(); // FIXME

				pTexture->init<GL_RGBA8>(1, 1);

				glBindTexture(GL_TEXTURE_2D, pTexture->GetObject());

				unsigned char texels [] =
				{
					(unsigned char)(color.r * 255),
					(unsigned char)(color.g * 255),
					(unsigned char)(color.b * 255),
					(unsigned char)255
				};

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, texels);

				glBindTexture(GL_TEXTURE_2D, 0);

				definition.material.m_diffuse = pTexture->GetObject();
			}
		}

		{
			aiString str;
			scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_SPECULAR, 0, &str);
			std::string texture_name(str.C_Str());

			if (g_Textures.find(texture_name) != g_Textures.end())
			{
				definition.material.m_specular = g_Textures[texture_name];
			}
			else
			{
				aiColor3D color (0.f, 0.f, 0.f);
				scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);

				GPU::Texture<GL_TEXTURE_2D> * pTexture = new GPU::Texture<GL_TEXTURE_2D>(); // FIXME

				pTexture->init<GL_RGBA8>(1, 1);

				glBindTexture(GL_TEXTURE_2D, pTexture->GetObject());

				unsigned char texels [] =
				{
					(unsigned char)(color.r * 255),
					(unsigned char)(color.g * 255),
					(unsigned char)(color.b * 255),
					(unsigned char)255
				};

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, texels);

				glBindTexture(GL_TEXTURE_2D, 0);

				definition.material.m_specular = pTexture->GetObject();
			}
		}

		{
			definition.material.shininess = 1.0;
			scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_SHININESS, definition.material.shininess);
		}

		offsets.push_back(definition);

//		vertex_offset += vertices.size() * sizeof(SubMesh::VertexSimple);
		index_offset += triangles.size() * sizeof(unsigned int);
		base_vertex += vertices.size();
	}

	GPU::munmap(*vertexBuffer);
	GPU::munmap(*indexBuffer);

	const mat4x4 identity (1.0f);

	addMeshRecursive(scene->mRootNode, identity, meshes, vertexBuffer, indexBuffer, offsets, specs, g_apMeshes, *m_pScene);

	return(false);
}

// -------------------------------------------------------------

/**
 * @brief loadTexture
 * @param dir
 * @param filename
 * @return
 */
static GLuint loadTexture(const char * szFilename)
{
	QImage img;

	if (QDir::isAbsolutePath(szFilename))
	{
		img = QImage(szFilename);
	}
	else
	{
		img = QImage(QDir("data/meshes").filePath(szFilename));
	}

	QImage tex = QGLWidget::convertToGLFormat(img);

	size_t size = (tex.width() * tex.height() * 4 * sizeof(char));

	if (size > 0)
	{
		GLuint textureId = 0;

		glGenTextures(1, &textureId); // FIXME !!!!

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		return(textureId);
	}

	return(0);
}

/**
 * @brief loadAllMaterials
 * @param scene
 * @param dir
 */
static void loadAllMaterials(const aiScene * scene)
{
	aiTextureType aSupportedTextureTypes [] = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS };

	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial * material = scene->mMaterials[i];

		for (aiTextureType type : aSupportedTextureTypes)
		{
			aiString str;

			material->GetTexture(type, 0, &str);

			if (g_Textures.find(str.C_Str()) == g_Textures.end())
			{
				GLuint textureId = loadTexture(str.C_Str());

				if (0 != textureId)
				{
					g_Textures.insert(std::pair<std::string, GLuint>(str.C_Str(), textureId));
				}
			}
		}
	}
}

/**
 * @brief addMeshRecursive
 * @param nd
 * @param parentTransformation
 * @param preloaded
 */
static void addMeshRecursive(const aiNode * nd, const mat4x4 & parentTransformation, const std::vector<SubMesh *> & preloaded, GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer, const std::vector<SubMeshDefinition> & offsets, const std::vector<Mesh::VertexSpec> & specs, std::vector<Mesh*> & apMeshes, Scene & scene)
{
	mat4x4 transformation = parentTransformation * ASSIMP_MAT4X4(nd->mTransformation);

	Mesh * m = new Mesh(vertexBuffer, specs, indexBuffer);
	apMeshes.push_back(m);

	for (int i = 0; i < nd->mNumMeshes; ++i)
	{
		const SubMeshDefinition & offset = offsets[nd->mMeshes[i]];
		SubMesh * subMesh = m->AddSubMesh(offset.triangle_count, GL_TRIANGLES, offset.index_offset, GL_UNSIGNED_INT, offset.base_vertex);
		subMesh->m_material = offset.material;
		subMesh->m_NormalMapId = offset.m_NormalMapId;

		m->m_BoundingBox.min.x = _min(m->m_BoundingBox.min.x, offset.m_vMin.x);
		m->m_BoundingBox.min.y = _min(m->m_BoundingBox.min.y, offset.m_vMin.y);
		m->m_BoundingBox.min.z = _min(m->m_BoundingBox.min.z, offset.m_vMin.z);

		m->m_BoundingBox.max.x = _max(m->m_BoundingBox.max.x, offset.m_vMax.x);
		m->m_BoundingBox.max.y = _max(m->m_BoundingBox.max.y, offset.m_vMax.y);
		m->m_BoundingBox.max.z = _max(m->m_BoundingBox.max.z, offset.m_vMax.z);
	}

	if (nd->mNumMeshes > 0)
	{
		m->m_BoundingSphere.center.x = (m->m_BoundingBox.max.x - m->m_BoundingBox.min.x) * 0.5f;
		m->m_BoundingSphere.center.y = (m->m_BoundingBox.max.y - m->m_BoundingBox.min.y) * 0.5f;
		m->m_BoundingSphere.center.z = (m->m_BoundingBox.max.z - m->m_BoundingBox.min.z) * 0.5f;

		m->m_BoundingSphere.radius = _max(distance(m->m_BoundingSphere.center, m->m_BoundingBox.min), distance(m->m_BoundingSphere.center, m->m_BoundingBox.max));
	}

	Object instance(m);

	instance.transformation = transformation;

	scene.insert(instance);

	for (int i = 0; i < nd->mNumChildren; ++i)
	{
		addMeshRecursive(nd->mChildren[i], transformation, preloaded, vertexBuffer, indexBuffer, offsets, specs, apMeshes, scene);
	}
}
