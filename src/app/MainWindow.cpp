#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutDialog.h"
#include "NodeEditorWindow.h"

#include "DrawableSurface.h"
#include "RendererWrapper.h"
#include "SerializeGraph.h"

#include <QSettings>
#include <QFileDialog>

#include <QSplitter>

#include <QDesktopServices>

extern RendererWrapper g_RendererWrapper;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget * pParent)
: QMainWindow(pParent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//
	// Setup undo/redo
	{
		m_pUndoStack = new QUndoStack(this);

		QIcon iconUndo(QIcon::fromTheme(QStringLiteral("edit-undo")));
		QAction * undoAction = m_pUndoStack->createUndoAction(this, tr("&Undo"));
		undoAction->setIcon(iconUndo);
		undoAction->setShortcuts(QKeySequence::Undo);
		ui->menuEdit->insertAction(ui->actionCut, undoAction);

		QIcon iconRedo(QIcon::fromTheme(QStringLiteral("edit-redo")));
		QAction * redoAction = m_pUndoStack->createRedoAction(this, tr("&Redo"));
		redoAction->setIcon(iconRedo);
		redoAction->setShortcuts(QKeySequence::Redo);
		ui->menuEdit->insertAction(ui->actionCut, redoAction);

		ui->menuEdit->insertSeparator(ui->actionCut);

		m_pUndoView = new QUndoView(m_pUndoStack);
		m_pUndoView->setWindowTitle(tr("Command List"));
		m_pUndoView->setAttribute(Qt::WA_QuitOnClose, false);
	}

	m_pSplitter = new QSplitter(this);
	setCentralWidget(m_pSplitter);
	m_pDrawable = new DrawableSurface(this);
	m_pSplitter->addWidget(m_pDrawable);

	{
		m_pFileChooser = new QFileDialog(this );
		m_pFileChooser->hide();
		m_pFileChooser->setWindowTitle("Import scene ...");
		m_pFileChooser->setAcceptMode(QFileDialog::AcceptOpen);
		m_pFileChooser->setFileMode(QFileDialog::ExistingFile);

		const char ext [] = "*.dae *.blend *.3ds *.ase *.obj *.ifc *.xgl *.zgl *.ply *.dxf *.lwo *.lws *.lxo *.stl *.x *.ac *.ms3d *.cob *.scn " /** Common interchange formats **/
							"*.bvh *.csm " /** Motion Capture Formats **/
							"*.xml *.irrmesh *.irr " /** Motion Capture Formats **/
							"*.mdl *.md2 *.md3 *.pk3 *.mdc *.md5 *.smd *.vta *.m3 *.3d " /** Game file formats **/
							"*.b3d *.q3d *.q3s *.nff *.nff *.off *.raw *.ter *.mdl *.hmp *.ndo "; /** Other file formats **/

		QString filter(ext);
		m_pFileChooser->setNameFilters(filter.split(' '));

		connect(m_pFileChooser, SIGNAL(fileSelected(const QString &)), m_pDrawable, SLOT(importScene(const QString &)));
	}

	{
		m_pNodeEditorWindow = new NodeEditorWindow(this);
		connect(m_pNodeEditorWindow, SIGNAL(graphSaved(const Graph &)), this, SLOT(onGraphSaved(const Graph &)));
		connect(m_pNodeEditorWindow, SIGNAL(graphLoaded(const Graph &)), this, SLOT(onGraphLoaded(const Graph &)));
	}

	{
		m_pAboutDialog = new AboutDialog(this);
	}

	QSettings settings;
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	restoreState(settings.value("mainWindowState").toByteArray());

	if (isFullScreen())
	{
		showMaximized(); // don't want the window in fullscreen mode at launch
	}

	ui->statusbar->showMessage(tr("Ready"), 2000);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
	delete ui;
}

/**
 * @brief MainWindow::init
 */
void MainWindow::init(void)
{
	show();

	m_pNodeEditorWindow->init();
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent * pEvent)
{
	(void)pEvent;

	QSettings settings;
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
}

/**
 * @brief MainWindow::SetStatus
 * @param str
 */
void MainWindow::SetStatus(const QString & str)
{
	ui->statusbar->showMessage(str, 2000);
}

/**
 * @brief MainWindow::on_actionNew_triggered
 */
void MainWindow::on_actionNew_triggered(void)
{
	// TODO
}

/**
 * @brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered(void)
{
	// TODO : format ?
}

/**
 * @brief MainWindow::on_actionSave_triggered
 */
void MainWindow::on_actionSave_triggered(void)
{
	// TODO : format ?
}

/**
 * @brief MainWindow::on_actionSaveAs_triggered
 */
void MainWindow::on_actionSaveAs_triggered(void)
{
	// TODO : format ?
}

/**
 * @brief MainWindow::on_actionImport_triggered
 */
void MainWindow::on_actionImport_triggered(void)
{
	m_pFileChooser->show();
}

/**
 * @brief MainWindow::on_actionExport_triggered
 */
void MainWindow::on_actionExport_triggered(void)
{
	// FIXME : is this really needed ?
}

/**
 * @brief MainWindow::on_actionCut_triggered
 */
void MainWindow::on_actionCut_triggered(void)
{
	// TODO : format ?
}

/**
 * @brief MainWindow::on_actionCopy_triggered
 */
void MainWindow::on_actionCopy_triggered(void)
{
	// TODO : format ?
}

/**
 * @brief MainWindow::on_actionPaste_triggered
 */
void MainWindow::on_actionPaste_triggered(void)
{
	// TODO : format ?
}

/**
 * @brief MainWindow::on_actionSelectAll_triggered
 */
void MainWindow::on_actionSelectAll_triggered(void)
{
	// TODO : implement selection first
}

/**
 * @brief MainWindow::on_actionDelete_triggered
 */
void MainWindow::on_actionDelete_triggered(void)
{
	// TODO : implement selection first
}

/**
 * @brief MainWindow::on_actionFind_triggered
 */
void MainWindow::on_actionFind_triggered(void)
{
	// TODO : show find window
}

/**
 * @brief MainWindow::on_actionPreferences_triggered
 */
void MainWindow::on_actionPreferences_triggered(void)
{
	// TODO : show preferences window
}

/**
 * @brief MainWindow::on_actionFullscreen_triggered
 * @param checked
 */
void MainWindow::on_actionFullscreen_triggered(void)
{
	if (!isFullScreen())
	{
		ui->menubar->hide();
		ui->statusbar->hide();
		showFullScreen();
	}
	else
	{
		showNormal();
		ui->menubar->show();
		ui->statusbar->show();
	}
}

/**
 * @brief MainWindow::on_actionDrawObjectsAABB_toggled
 * @param checked
 */
void MainWindow::on_actionDrawObjectsAABB_toggled(bool checked)
{
	static_cast<DrawableSurface*>(m_pDrawable)->DrawObjectsAABB(checked);
}

/**
 * @brief MainWindow::on_actionDrawSceneAABB_toggled
 * @param checked
 */
void MainWindow::on_actionDrawSceneAABB_toggled(bool checked)
{
	static_cast<DrawableSurface*>(m_pDrawable)->DrawSceneAABB(checked);
}

/**
 * @brief MainWindow::on_actionClear_color_clicked
 */
void MainWindow::on_actionResetCamera_triggered()
{
	static_cast<DrawableSurface*>(m_pDrawable)->ResetCamera();
}

/**
 * @brief MainWindow::on_actionShowRenderGraphEditor_triggered
 */
void MainWindow::on_actionShowRenderGraphEditor_triggered()
{
	m_pNodeEditorWindow->show();
}

/**
 * @brief MainWindow::on_actionShowCommandList_triggered
 */
void MainWindow::on_actionShowCommandList_triggered()
{
	m_pUndoView->show();
}

/**
 * @brief MainWindow::on_actionHelp_triggered
 */
void MainWindow::on_actionHelp_triggered()
{
	QDesktopServices::openUrl(QUrl("https://github.com/magestik/sandbox3d-editor"));
}

/**
 * @brief MainWindow::on_actionAbout_triggered
 */
void MainWindow::on_actionAbout_triggered()
{
	m_pAboutDialog->show();
}

/**
 * @brief MainWindow::onGraphLoaded
 */
void MainWindow::onGraphLoaded(const Graph & graph)
{
	std::string strFinalTextureId;
	SerializeGraph(graph, m_pNodeEditorWindow->m_aNodeDescriptors, strFinalTextureId);
	g_RendererWrapper.initQueue("/tmp/render.xml");

	m_pDrawable->setCurrentTexture(QString(strFinalTextureId.data()));
	m_pDrawable->update();
}

/**
 * @brief MainWindow::onGraphSaved
 */
void MainWindow::onGraphSaved(const Graph & graph)
{
	std::string strFinalTextureId;
	SerializeGraph(graph, m_pNodeEditorWindow->m_aNodeDescriptors, strFinalTextureId);
	g_RendererWrapper.initQueue("/tmp/render.xml");

	m_pDrawable->setCurrentTexture(QString(strFinalTextureId.data()));
	m_pDrawable->update();
}
