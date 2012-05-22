#include "frmsettings.h"
#include "ui_frmsettings.h"

frmSettings::frmSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmSettings)
{
    ui->setupUi(this);
}

frmSettings::~frmSettings()
{
    delete ui;
}

// Save the settings and close the window
void frmSettings::saveAndClose() {
    // Save all settings and close
    settings.setValue("Server/IP",ui->txtServerPublicIP);
    settings.setValue("Server/Port",ui->txtServerPort);
    settings.setValue("Node/TempFolder",ui->txtTempFolder);
    settings.setValue("Node/Port", ui->txtNodePort);

    this->hide();
}
