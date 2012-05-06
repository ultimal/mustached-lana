#include "frmserver.h"
#include "ui_frmserver.h"

frmServer::frmServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmServer)
{
    ui->setupUi(this);
}

frmServer::~frmServer()
{
    delete ui;
}
