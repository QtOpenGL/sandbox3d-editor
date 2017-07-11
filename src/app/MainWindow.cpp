#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DrawableSurface.h"
#include "RendererWrapper.h"
#include "SerializeGraph.h"

#include <QSettings>
#include <QDockWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QFileSystemWatcher>

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

	m_pDrawable = new DrawableSurface(this);
	setCentralWidget(m_pDrawable);

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

	QSettings settings;
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	restoreState(settings.value("mainWindowState").toByteArray());

	ui->statusbar->showMessage(tr("Ready"), 2000);
	ui->statusbar->addPermanentWidget(ui->cpu_time_text);
	ui->statusbar->addPermanentWidget(ui->gpu_time_text);
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
 * @brief MainWindow::SetRenderTime
 * @param t
 */
void MainWindow::SetRenderTime(double cpu_time, double gpu_time)
{
	QString str1 = "CPU : "+ QString::number(cpu_time, 'f', 6 ) +" ms";
	ui->cpu_time_text->setText(str1);

	if (cpu_time < 4.0)
	{
		ui->cpu_time_text->setStyleSheet("* {color : black; }");
	}
	else
	{
		ui->cpu_time_text->setStyleSheet("* {color : red; }");
	}

	QString str2 = "GPU : "+ QString::number(gpu_time, 'f', 6 ) +" ms";
	ui->gpu_time_text->setText(str2);

	if (gpu_time < 16.0)
	{
		ui->gpu_time_text->setStyleSheet("* {color : black; }");
	}
	else
	{
		ui->gpu_time_text->setStyleSheet("* {color : red; }");
	}
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
 * @brief MainWindow::on_actionFullscreen_toggled
 * @param checked
 */
void MainWindow::on_actionFullscreen_toggled(bool checked)
{
	if (checked)
	{
		showFullScreen();
	}
	else
	{
		showNormal();
	}
}

/**
 * @brief MainWindow::on_actionImport_triggered
 */
void MainWindow::on_actionImport_triggered()
{
	m_pFileChooser->show();
}

/**
 * @brief MainWindow::on_actionNode_Editor_triggered
 */
void MainWindow::on_actionNode_Editor_triggered()
{
	m_pNodeEditorWindow->show();
}

/**
 * @brief MainWindow::on_actionClear_color_clicked
 */
void MainWindow::on_actionResetCamera_triggered()
{
	static_cast<DrawableSurface*>(m_pDrawable)->ResetCamera();
}

/**
 * @brief MainWindow::onGraphLoaded
 */
void MainWindow::onGraphLoaded(const Graph & graph)
{
	std::string strFinalTextureId;
	SerializeGraph(graph, m_pNodeEditorWindow->m_aNodeDescriptors, strFinalTextureId);
	g_RendererWrapper.initQueue("/tmp/render.xml");
	m_pDrawable->setCurrentTexture(strFinalTextureId);
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
	m_pDrawable->setCurrentTexture(strFinalTextureId);
	m_pDrawable->update();
}
