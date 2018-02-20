#pragma once

#include <QMainWindow>
#include <QColorDialog>

#include "NodeEditorWindow.h"

class QFileDialog;
class QUndoStack;
class QUndoView;

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

	void		SetStatus		(const QString & str);

private:

	Ui::MainWindow * ui;

	QUndoStack * m_pUndoStack;
	QUndoView * m_pUndoView;

protected:

	NodeEditorWindow * m_pNodeEditorWindow;

public:

	DrawableSurface * m_pDrawable;

	QFileDialog * m_pFileChooser;

private slots:

	//
	// File menu
	void on_actionNew_triggered(void);
	void on_actionOpen_triggered(void);

	void on_actionSave_triggered(void);
	void on_actionSaveAs_triggered(void);

	void on_actionImport_triggered(void);
	void on_actionExport_triggered(void);

	//void on_actionExit_triggered(void);

	//
	// Edit menu
	void on_actionCut_triggered(void);
	void on_actionCopy_triggered(void);
	void on_actionPaste_triggered(void);

	void on_actionSelectAll_triggered(void);
	void on_actionDelete_triggered(void);
	void on_actionFind_triggered(void);
	void on_actionPreferences_triggered(void);

	//
	// View menu
	void on_actionFullscreen_triggered(void);
	void on_actionDrawObjectsAABB_toggled(bool checked);
	void on_actionDrawSceneAABB_toggled(bool checked);

	//
	// Camera menu
	void on_actionResetCamera_triggered(void);

	//
	// Window menu
	void on_actionShowRenderGraphEditor_triggered(void);
	void on_actionShowCommandList_triggered(void);

	//
	// Help menu
	void on_actionHelp_triggered(void);
	void on_actionAbout_triggered(void);

	//
	// Custom
	void onGraphLoaded(const Graph & graph);
	void onGraphSaved(const Graph & graph);

};
