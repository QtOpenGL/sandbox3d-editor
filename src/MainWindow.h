#pragma once

#include <QMainWindow>

#include "NodeEditorWindow.h"
#include "EnvSettingsWidget.h"

class QDockWidget;
class QFileDialog;
class QFileSystemWatcher;

class DrawableSurface;

class Graph;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit	MainWindow		(QWidget * parent = 0);
	virtual		~MainWindow		(void);

	void		init			(void);

	void		closeEvent		(QCloseEvent * event);

	void		SetRenderTime	(double cpu_time, double gpu_time);
	void		SetStatus		(const QString & str);

private:

	Ui::MainWindow * ui;

protected:

	NodeEditorWindow * m_pNodeEditorWindow;
	EnvSettingsWidget * m_pEnvSettingsWidget;

public:

	DrawableSurface * m_pDrawable;

	QColorDialog * m_pClearColorChooser;

	QFileDialog * m_pFileChooser;

	QFileSystemWatcher * m_pFileWatcher;

private slots:

	void on_actionFullscreen_toggled(bool checked);

	void on_actionImport_triggered();

	void on_actionClear_color_triggered();
	void on_actionNode_Editor_triggered();

	void on_actionResetCamera_triggered();

	void onGraphLoaded(const Graph & graph);
	void onGraphSaved(const Graph & graph);

};
