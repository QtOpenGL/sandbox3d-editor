#include "QOpenGLResourceManager.h"

QOpenGLResourceManager QOpenGLResourceManager::instance_;

/**
 * @brief QOpenGLResourceManager::instance
 * @return
 */
QOpenGLResourceManager & QOpenGLResourceManager::instance(void)
{
	return(instance_);
}

/**
 * @brief Constructor
 */
QOpenGLResourceManager::QOpenGLResourceManager(void) : m_sharedVertexBuffer(QOpenGLBuffer::VertexBuffer)
{
	// ...
}

/**
 * @brief Destructor
 */
QOpenGLResourceManager::~QOpenGLResourceManager(void)
{
	// ...
}

/**
 * @brief Initialize resources
 * @return
 */
bool QOpenGLResourceManager::init(void)
{
	initShaders();

	static const float points [] =
	{
		// quad
		-1.0f, -1.0f, /* | */ 0.0f, 0.0f,
		 1.0f, -1.0f, /* | */ 1.0f, 0.0f,
		-1.0f,  1.0f, /* | */ 0.0f, 1.0f,
		 1.0f,  1.0f, /* | */ 1.0f, 1.0f,
		// point (bbox)
		0.0f, 0.0f,
	};

	//
	// Create Shared Vertex Buffer
	m_sharedVertexBuffer.create();
    m_sharedVertexBuffer.bind();
    m_sharedVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_sharedVertexBuffer.allocate(points, sizeof(points));

	//
	// Create VAO
	m_vertexArrayQuad.create();
	m_vertexArrayBbox.create();

	//
	// Setup Quad Inputs
	{
		m_pShaderQuad->bind();
		m_vertexArrayQuad.bind();

		m_pShaderQuad->enableAttributeArray(0);
		m_pShaderQuad->enableAttributeArray(1);
		m_pShaderQuad->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(float));
		m_pShaderQuad->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 2, 4 * sizeof(float));

		m_vertexArrayQuad.release();
		m_pShaderQuad->release();
	}

	//
	// Setup Bounding Box Inputs
	{
		m_pShaderBbox->bind();
		m_vertexArrayBbox.bind();

		m_pShaderBbox->enableAttributeArray(0);
		m_pShaderBbox->setAttributeBuffer(0, GL_FLOAT, 16 * sizeof(float), 2, 0);

		m_vertexArrayBbox.release();
		m_pShaderBbox->release();
	}

	m_sharedVertexBuffer.release();

	return(true);
}

/**
 * @brief Release resources
 */
void QOpenGLResourceManager::release(void)
{
	m_vertexArrayBbox.destroy();
	m_vertexArrayQuad.destroy();

	m_sharedVertexBuffer.destroy();

	delete m_pShaderQuad;
	m_pShaderQuad = nullptr;

	delete m_pShaderBbox;
	m_pShaderBbox = nullptr;
}

/**
 * @brief QOpenGLResourceManager::initShaders
 */
void QOpenGLResourceManager::initShaders(void)
{
	Q_INIT_RESOURCE(shaders);

	//
	// Create Bounding Box Shader
	{
		m_pShaderBbox = new QOpenGLShaderProgram();
		bool bCompiledVS = m_pShaderBbox->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/bbox.vert");
		Q_ASSERT(bCompiledVS);
		bool bCompiledGS = m_pShaderBbox->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/bbox.geom");
		Q_ASSERT(bCompiledGS);
		bool bCompiledFS = m_pShaderBbox->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/bbox.frag");
		Q_ASSERT(bCompiledFS);
		bool bLinked = m_pShaderBbox->link();
		Q_ASSERT(bLinked);
	}

	//
	// Create Quad Shader
	{
		m_pShaderQuad = new QOpenGLShaderProgram();
		bool bCompiledVS = m_pShaderQuad->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
		Q_ASSERT(bCompiledVS);
		bool bCompiledFS = m_pShaderQuad->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
		Q_ASSERT(bCompiledFS);
		bool bLinked = m_pShaderQuad->link();
		Q_ASSERT(bLinked);
	}
}

/**
 * @brief QOpenGLResourceManager::bindQuadResources
 */
void QOpenGLResourceManager::bindQuadResources(void)
{
	m_pShaderQuad->bind();

	m_vertexArrayQuad.bind();
}

/**
 * @brief QOpenGLResourceManager::unbindQuadResources
 */
void QOpenGLResourceManager::unbindQuadResources(void)
{
	m_vertexArrayQuad.release();

	m_pShaderQuad->release();
}

/**
 * @brief QOpenGLResourceManager::bindBboxResources
 */
void QOpenGLResourceManager::bindBboxResources(const QMatrix4x4 & ModelViewProjection, const QVector3D & BBoxMin, const QVector3D & BBoxMax)
{
	m_pShaderBbox->bind();

	m_pShaderBbox->setUniformValue("ModelViewProjection", ModelViewProjection);
	m_pShaderBbox->setUniformValue("BBoxMin", BBoxMin);
	m_pShaderBbox->setUniformValue("BBoxMax", BBoxMax);

	m_vertexArrayBbox.bind();
}

/**
 * @brief QOpenGLResourceManager::unbindBboxResources
 */
void QOpenGLResourceManager::unbindBboxResources(void)
{
	m_vertexArrayBbox.release();

	m_pShaderBbox->release();
}
