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
 * @brief QOpenGLResourceManager::QOpenGLResourceManager
 */
QOpenGLResourceManager::QOpenGLResourceManager(void) : m_vertexBufferQuad(QOpenGLBuffer::VertexBuffer)
{
	// ...
}

/**
 * @brief QOpenGLResourceManager::~QOpenGLResourceManager
 */
QOpenGLResourceManager::~QOpenGLResourceManager(void)
{
	// ...
}

/**
 * @brief QOpenGLResourceManager::init
 * @return
 */
bool QOpenGLResourceManager::init(void)
{
	static const float points [] =
	{
		-1.0f, -1.0f, /* | */ 0.0f, 0.0f,
		 1.0f, -1.0f, /* | */ 1.0f, 0.0f,
		-1.0f,  1.0f, /* | */ 0.0f, 1.0f,
		 1.0f,  1.0f, /* | */ 1.0f, 1.0f,
	};

	// Create Shader
	m_pShaderQuad = new QOpenGLShaderProgram();
	bool bCompiledVS = m_pShaderQuad->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
	Q_ASSERT(bCompiledVS);
    bool bCompiledFS = m_pShaderQuad->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
	Q_ASSERT(bCompiledFS);
    bool bLinked = m_pShaderQuad->link();
	Q_ASSERT(bLinked);
	m_pShaderQuad->bind();

	// Create Vertex Buffer
	m_vertexBufferQuad.create();
    m_vertexBufferQuad.bind();
    m_vertexBufferQuad.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBufferQuad.allocate(points, sizeof(points));

	// Create Vertex Array Object
    m_vertexArrayQuad.create();
    m_vertexArrayQuad.bind();

	// Setup Shader
	m_pShaderQuad->enableAttributeArray(0);
	m_pShaderQuad->enableAttributeArray(1);
	m_pShaderQuad->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(float));
	m_pShaderQuad->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 2, 4 * sizeof(float));

	// Unbind everything
	m_vertexArrayQuad.release();
	m_vertexBufferQuad.release();
	m_pShaderQuad->release();

	return(true);
}

/**
 * @brief QOpenGLResourceManager::release
 */
void QOpenGLResourceManager::release(void)
{
	m_vertexArrayQuad.destroy();
	m_vertexBufferQuad.destroy();
	delete m_pShaderQuad;
	m_pShaderQuad = nullptr;
}

/**
 * @brief QOpenGLResourceManager::bindQuadResources
 */
void QOpenGLResourceManager::bindQuadResources(void)
{
	char *version = (char*)glGetString(GL_VERSION);

	m_pShaderQuad->bind();

	//m_pShaderQuad->setUniformValue("size", size);
	//m_pShaderQuad->setUniformValue("transform", transform);

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
