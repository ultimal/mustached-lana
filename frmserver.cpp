#include "frmserver.h"
#include "ui_frmserver.h"

frmServer::frmServer(QWidget *parent, QString port, bool debug) :
    QWidget(parent), ui(new Ui::frmServer)
{
    ui->setupUi(this);

    data = new dataStore(this);
    srv = new Server(this,port,data,debug);
}

void frmServer::appendNode(nodeAddresses node) {
    ui->txtNodeList->append(node.ipAddress);
}

frmServer::~frmServer()
{
    delete ui;
}
