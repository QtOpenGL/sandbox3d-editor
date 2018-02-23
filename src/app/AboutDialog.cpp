#include "AboutDialog.h"
#include "ui_AboutDialog.h"

/**
 * @brief Constructor
 * @param parent
 */
AboutDialog::AboutDialog(QWidget * pParent)
: QDialog(pParent)
, ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
}

/**
 * @brief Destructor
 */
AboutDialog::~AboutDialog(void)
{
	delete ui;
}
