#include <QApplication>

#include "RendererWrapper.h"

#include <QGLFormat>
#include <QSplashScreen>

#include "MainWindow.h"

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

	if (argc > 1)
	{
		g_RendererWrapper.loadPlugins(argv[1]);
	}

	g_RendererWrapper.init();

	MainWindow w;
	w.init();

	int r = app.exec();

	g_RendererWrapper.release();

	return(r);
}
