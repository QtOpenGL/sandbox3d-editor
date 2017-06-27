﻿#include "DrawableSurface.h"

#include "MainWindow.h"

#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFileSystemWatcher>

#include <QGLWidget> // for convertToGLFormat

#include "QOpenGLResourceManager.h"

#include <QDebug>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assert.h>

#include <Misc/Timer.h>

#define ASSIMP_MAT4X4(m) mat4x4(m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4, m.c1, m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4);

#define _min(x, y) ((x < y) ? x : y)
#define _max(x, y) ((x > y) ? x : y)

Rendering * g_pRendering = nullptr; // ugly but needed for now

/**
 * @brief Constructor
 * @param parent
 */
DrawableSurface::DrawableSurface(QWidget *parent)
: QOpenGLWidget(parent)
, m_camera()
, m_vLastPos(0, 0)
, m_vClearColor(0.0f, 0.0f, 0.0f, 0.0f)
, m_vAmbientColor(0.0f, 0.0f, 0.0f, 0.0f)
, m_pSelectedObject(nullptr)
, m_pCurrentTexture(nullptr)
{
	setAutoFillBackground(false);
	setFocusPolicy(Qt::StrongFocus);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
	connect(this, &QOpenGLWidget::resized, this, &DrawableSurface::onResized);
	connect(this, &QOpenGLWidget::frameSwapped, this, &DrawableSurface::onFrameSwapped);
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))

	g_pRendering = &m_renderer;
}

/**
 * @brief Destructor
 */
DrawableSurface::~DrawableSurface(void)
{
	// ...
}

/**
 * @brief DrawableSurface::ResetCamera
 */
void DrawableSurface::ResetCamera(void)
{
	m_camera = Camera();
	update();
}

/**
 * @brief DrawableSurface::setCurrentTexture
 * @param strFinalTextureId
 */
void DrawableSurface::setCurrentTexture(const std::string & strFinalTextureId)
{
	m_pCurrentTexture = m_renderer.GetRenderTexture(strFinalTextureId.c_str());
}

/**
 * @brief initializeGL
 */
void DrawableSurface::initializeGL(void)
{
	QOpenGLResourceManager::instance().init();

	GLint v;
	glGetIntegerv(GL_CONTEXT_FLAGS, &v);
	//assert (v & GL_CONTEXT_FLAG_DEBUG_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glGenQueries(1, &m_query);

	for (int i = 0; i < NB_BUFFER; ++i)
	{
		m_apBuffer[i] = new GPU::Buffer<GL_PIXEL_UNPACK_BUFFER>();
	}

	m_pSpecial = new GPU::Buffer<GL_PIXEL_UNPACK_BUFFER>();

	m_renderer.onReady();

	m_renderer.initQueue("/tmp/render.xml");
}

/**
 * @brief drawable::resizeGL
 * @param w
 * @param h
 */
void DrawableSurface::resizeGL(int w, int h)
{
	m_renderer.onResize(w, h);

	QOpenGLResourceManager & manager = QOpenGLResourceManager::instance();
	manager.onResize(QSize(w, h));

	m_renderer.SetPickBufferFramebuffer(manager.pickBufferFramebufferObject());
}

/**
 * @brief drawable::paintGL
 */
void DrawableSurface::paintGL(void)
{
	const mat4x4 & matView = m_camera.getViewMatrix();

	GLuint elapsed_time = 0;
	Timer t;

	glBeginQuery(GL_TIME_ELAPSED, m_query);

	t.Start();
	{
		m_renderer.onUpdate(matView, m_vClearColor);
	}
	t.Stop();

	glEndQuery(GL_TIME_ELAPSED);

	GLint done = 0;

	while (!done)
	{
		glGetQueryObjectiv(m_query, GL_QUERY_RESULT_AVAILABLE, &done);
	}

	glGetQueryObjectuiv(m_query, GL_QUERY_RESULT, &elapsed_time);

	static_cast<MainWindow*>(parent())->SetRenderTime(t.getElapsedTimeInMs(), elapsed_time / 1000000.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	QOpenGLResourceManager & manager = QOpenGLResourceManager::instance();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, manager.pickBufferFramebufferObject());

	{
		unsigned int i = 0;

		for (const Object & object : m_renderer.m_scene.getObjects())
		{
			object.mesh->bind();

//			SetUniform(m_pickBufferPipeline.m_uShaderObject, "Model", object.transformation);

			glVertexAttribI1ui(5, i);

			for (SubMesh * m : object.getDrawCommands())
			{
				m->drawGL();
			}

			object.mesh->unbind();

			++i;
		}
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFramebufferObject());

	if (nullptr != m_pCurrentTexture)
	{
		manager.bindQuadResources();

		glBindTexture(GL_TEXTURE_2D, m_pCurrentTexture->GetObject());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		manager.unbindQuadResources();
	}

	if (nullptr != m_pSelectedObject)
	{
		Mesh::BoundingBox bbox = m_pSelectedObject->mesh->m_BoundingBox;

		mat4x4 mMVP = (m_renderer.m_matProjection * (matView * m_pSelectedObject->transformation));

		QVector3D BBoxMin(bbox.min.x, bbox.min.y, bbox.min.z);
		QVector3D BBoxMax(bbox.max.x, bbox.max.y, bbox.max.z);

		manager.bindBboxResources(QMatrix4x4((float*)&mMVP), BBoxMin, BBoxMax);

		glDrawArrays(GL_POINTS, 0, 1);

		manager.unbindBboxResources();
	}
}

/**
 * @brief DrawableSurface::onFrameSwapped
 */
void DrawableSurface::onFrameSwapped(void)
{
	//update();
}

/**
 * @brief DrawableSurface::onResized
 */
void DrawableSurface::onResized(void)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
	m_renderer.SetDefaultFramebuffer(defaultFramebufferObject());
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
}

/**
 * @brief drawable::mousePressEvent
 * @param event
 */
void DrawableSurface::mousePressEvent(QMouseEvent * event)
{
	ivec2 pos(event->x(), event->y());
	m_vLastPos = pos;
	m_bMoved = false;
	update();
}

/**
 * @brief drawable::mouseMoveEvent
 * @param event
 */
void DrawableSurface::mouseMoveEvent(QMouseEvent * event)
{
	ivec2 pos(event->x(), event->y());

	//if (event->button() & Qt::LeftButton)
	{
		ivec2 diff = pos - m_vLastPos;

		if (event->modifiers() & Qt::ControlModifier)
		{
			// Translate X/Y
			vec2 translation(diff.x / 100.0f, diff.y / -100.0f);
			m_camera.TranslateXY(translation);
		}
		else
		{
			vec2 rotation(diff.x / -500.0f, diff.y / -500.0f);
			m_camera.Rotate(rotation);
		}
	}

	m_vLastPos = pos;
	m_bMoved = true;

	update();
}

/**
 * @brief drawable::mouseReleaseEvent
 * @param event
 */
void DrawableSurface::mouseReleaseEvent(QMouseEvent * event)
{
	ivec2 pos(event->x(), event->y());

	if (!m_bMoved)
	{
		m_pSelectedObject = m_renderer.getObjectAtPos(pos);
	}

	update();
}

/**
 * @brief wheelEvent
 * @param event
 */
void DrawableSurface::wheelEvent(QWheelEvent * event)
{
	float translation = event->delta() / -100.0f;
	m_camera.TranslateZ(translation);
	update();
}

/**
 * @brief DrawableSurface::keyPressEvent
 * @param event
 */
void DrawableSurface::keyPressEvent(QKeyEvent *event)
{

}

/**
 * @brief DrawableSurface::keyReleaseEvent
 * @param event
 */
void DrawableSurface::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete)
	{
		if (nullptr != m_pSelectedObject)
		{
			m_renderer.m_scene.remove(*m_pSelectedObject);
			m_pSelectedObject = nullptr;
			update();
		}
	}
}

#define USE_PBO 1

/**
 * @brief DrawableSurface::loadSprites
 */
void DrawableSurface::loadSprites()
{
	QDir dir("data/sprites");

	const char ext [] =	"*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm";

	QString filter(ext);
	dir.setNameFilters(filter.split(' '));

	QStringList list = dir.entryList(QDir::Files);

	printf("Using technique %d \n", USE_PBO);

	double totalTime = 0.0f;

#if USE_PBO == 1

	const int totalBufferSize = 2048*2048*4*sizeof(char);

	for (int i = 0; i < NB_BUFFER; ++i)
	{
		GPU::realloc(*(m_apBuffer[i]), totalBufferSize, GL_STREAM_DRAW);
	}

	int buf = 0;
	int offset = 0;
#endif

	for (QString & filename : list)
	{
#if USE_PBO == 0 || USE_PBO == 1
		QImage img(dir.filePath(filename));
		QImage tex = QGLWidget::convertToGLFormat(img);

		Timer t;

		t.Start();
		{
			GPU::Texture<GL_TEXTURE_2D> * pTexture = new GPU::Texture<GL_TEXTURE_2D>();

			glBindTexture(GL_TEXTURE_2D, pTexture->GetObject());

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

#if USE_PBO == 0
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
#elif USE_PBO == 1
			size_t size = (tex.width() * tex.height() * 4 * sizeof(char));

			if (size > totalBufferSize)
			{
				GPU::realloc(*m_pSpecial, size, GL_STREAM_DRAW);

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pSpecial->GetObject());

				void * ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size, GL_MAP_WRITE_BIT);
				memcpy(ptr, tex.bits(), size);
				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

				offset = size;

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

				continue;
			}

			int nextOffset = offset + size;
			if (nextOffset > totalBufferSize)
			{
				buf = (buf + 1) % NB_BUFFER;

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_apBuffer[buf]->GetObject());

				void * ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size, GL_MAP_WRITE_BIT);
				memcpy(ptr, tex.bits(), size);
				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

				offset = size;

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
			}
			else
			{
				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_apBuffer[buf]->GetObject());

				void * ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
				memcpy(ptr, tex.bits(), size);
				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, BUFFER_OFFSET(offset));

				offset = nextOffset % totalBufferSize;

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			if (0 == offset)
			{
				++buf;
				GPU::realloc(*(m_apBuffer[buf]), totalBufferSize, GL_STREAM_DRAW);
			}
#endif
		}
		t.Stop();

		double elapsed = t.getElapsedTimeInMs();
		totalTime += elapsed;
		printf("CPU time : %f \n", elapsed);
#elif USE_PBO == 2
		loadTexture(dir.filePath(filename));
#endif
	}

	printf("Total Time elapsed : %f \n", totalTime);

#if 0
	Timer t;

	t.Start();
	glFinish();
	t.Stop();

	printf("Finish CPU time : %f \n", t.getElapsedTimeInMs());
#endif
}

/**
 * @brief DrawableSurface::loadAllMaterials
 * @param scene
 * @param dir
 */
void DrawableSurface::loadAllMaterials(const aiScene * scene)
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
				QString texture_filename(str.C_Str());

				GPU::Texture<GL_TEXTURE_2D> * pTexture = loadTexture(texture_filename);

				if (nullptr != pTexture)
				{
					g_Textures.insert(std::pair<std::string, GPU::Texture<GL_TEXTURE_2D> *>(texture_filename.toStdString(), pTexture));
				}
			}
		}
	}
}

/**
 * @brief DrawableSurface::addMeshRecursive
 * @param nd
 * @param parentTransformation
 * @param preloaded
 */
void DrawableSurface::addMeshRecursive(const aiNode * nd, const mat4x4 & parentTransformation, const std::vector<SubMesh *> & preloaded, GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer, const std::vector<SubMeshDefinition> & offsets, const std::vector<Mesh::VertexSpec> & specs)
{
	mat4x4 transformation = parentTransformation * ASSIMP_MAT4X4(nd->mTransformation);

	Mesh * m = new Mesh(vertexBuffer, specs, indexBuffer);
	m_apMeshes.push_back(m);

	for (int i = 0; i < nd->mNumMeshes; ++i)
	{
		const SubMeshDefinition & offset = offsets[nd->mMeshes[i]];
		SubMesh * subMesh = m->AddSubMesh(offset.triangle_count, GL_TRIANGLES, offset.index_offset, GL_UNSIGNED_INT, offset.base_vertex);
		subMesh->m_material = offset.material;
		subMesh->m_pNormalMap = offset.m_pNormalMap;

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

	m_renderer.m_scene.insert(instance);

	for (int i = 0; i < nd->mNumChildren; ++i)
	{
		addMeshRecursive(nd->mChildren[i], transformation, preloaded, vertexBuffer, indexBuffer, offsets, specs);
	}
}

/**
 * @brief DrawableSurface::loadTexture
 * @param dir
 * @param filename
 * @return
 */
GPU::Texture<GL_TEXTURE_2D> * DrawableSurface::loadTexture(const QString & filename)
{
	QImage img;

	if (QDir::isAbsolutePath(filename))
	{
		img = QImage(filename);
	}
	else
	{
		img = QImage(QDir("data/meshes").filePath(filename));
	}

	QImage tex = QGLWidget::convertToGLFormat(img);

	size_t size = (tex.width() * tex.height() * 4 * sizeof(char));

	if (size > 0)
	{
		GPU::Texture<GL_TEXTURE_2D> * pTexture = new GPU::Texture<GL_TEXTURE_2D>();

		pTexture->init<GL_RGBA8>(tex.width(), tex.height());

		glBindTexture(GL_TEXTURE_2D, pTexture->GetObject());

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

		glBindTexture(GL_TEXTURE_2D, 0);

		return(pTexture);
	}

	return(nullptr);
}

/**
 * @brief DrawableSurface::importScene
 * @param filename
 */
void DrawableSurface::importScene(const QString & filename)
{
	makeCurrent();

	std::string filepath = filename.toStdString();

	Assimp::Importer importer;

	const aiScene * scene = importer.ReadFile(filepath.data(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		return;
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

		definition.m_pNormalMap = nullptr;

		if (mesh->HasTangentsAndBitangents())
		{
			aiString str;
			scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &str);
			std::string texture_name(str.C_Str());

			if (g_Textures.find(texture_name) != g_Textures.end())
			{
				definition.m_pNormalMap = g_Textures[texture_name];
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

				definition.material.m_diffuse = new GPU::Texture<GL_TEXTURE_2D>();

				definition.material.m_diffuse->init<GL_RGBA8>(1, 1);

				glBindTexture(GL_TEXTURE_2D, definition.material.m_diffuse->GetObject());

				unsigned char texels [] =
				{
					(unsigned char)(color.r * 255),
					(unsigned char)(color.g * 255),
					(unsigned char)(color.b * 255),
					(unsigned char)255
				};

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, texels);

				glBindTexture(GL_TEXTURE_2D, 0);
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

				definition.material.m_specular = new GPU::Texture<GL_TEXTURE_2D>();

				definition.material.m_specular->init<GL_RGBA8>(1, 1);

				glBindTexture(GL_TEXTURE_2D, definition.material.m_specular->GetObject());

				unsigned char texels [] =
				{
					(unsigned char)(color.r * 255),
					(unsigned char)(color.g * 255),
					(unsigned char)(color.b * 255),
					(unsigned char)255
				};

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, texels);

				glBindTexture(GL_TEXTURE_2D, 0);
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

	addMeshRecursive(scene->mRootNode, identity, meshes, vertexBuffer, indexBuffer, offsets, specs);
}

/**
 * @brief DrawableSurface::setClearColor
 */
void DrawableSurface::setClearColor(const QColor & color)
{
	m_vClearColor = vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	update();
}

/**
 * @brief DrawableSurface::setAmbientColor
 * @param color
 */
void DrawableSurface::setAmbientColor(const QColor & color)
{
	m_vAmbientColor = vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	update();
}
