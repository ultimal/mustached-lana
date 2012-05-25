#include "frmserver.h"
#include "ui_frmserver.h"
#include <QMessageBox>

frmServer::frmServer(QWidget *parent, QString ip, QString port, bool debug) :
    QWidget(parent), ui(new Ui::frmServer)
{
    ui->setupUi(this);

    data = new dataStore(this);
    srv = new Server(this,port,data,debug);

    connect (srv,SIGNAL(connectionEstablished(nodeAddresses)),this,SLOT(appendNode(nodeAddresses)));

    // Set the ipAddress to the address the server is listening on

    // Set the port to the port the server is running on
    ui->txtPort->setText(port);
    ui->txtIpAddress->setText(ip);

    ui->nodeTable->setColumnCount(3);
    QStringList columnHeadings;

    columnHeadings << "Node IP" << "Keep alive" << "Status";
    ui->nodeTable->setHorizontalHeaderLabels(columnHeadings);

}

void frmServer::appendNode(nodeAddresses node) {

    // Since the table is zero indexed, rowCount is always 1 more
    ui->nodeTable->setRowCount(ui->nodeTable->rowCount()+1);

    // Since rowCount is zero indexed subtracting 1 corrects it
    int row = ui->nodeTable->rowCount() - 1;

    QTableWidgetItem *nodeIP = new QTableWidgetItem (node.ipAddress);
    QTableWidgetItem *nodeKeepAlive = new QTableWidgetItem (node.keepAlive.toString());
    QTableWidgetItem *state = new QTableWidgetItem (tr("Connected"));

    ui->nodeTable->setItem(row,0, nodeIP);
    ui->nodeTable->setItem(row,1, nodeKeepAlive);
    ui->nodeTable->setItem(row,2, state);
}

frmServer::~frmServer()
{
    delete ui;
}
