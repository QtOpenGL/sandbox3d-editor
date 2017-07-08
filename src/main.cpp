#include <QApplication>

#include "RendererWrapper.h"

#include <QGLFormat>
#include <QSplashScreen>

#include "MainWindow.h"
#include "Remotery.h"

RendererWrapper g_RendererWrapper;

bool setDefaultFormatOpenGL(void)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	return(true);
#else
	return(false);
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
}

int main(int argc, char ** argv)
{
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);

	QApplication app(argc, argv);

	bool version = setDefaultFormatOpenGL();
	if (!version)
	{
		return(-1);
	}

	Remotery* rmt;
    rmt_CreateGlobalInstance(&rmt);

	g_RendererWrapper.init();

	MainWindow w;
	w.init();

	int r = app.exec();

	g_RendererWrapper.release();

    rmt_DestroyGlobalInstance(rmt);

	return(r);
}
