#include "frmserver.h"
#include "ui_frmserver.h"

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

    ui->nodeTable->setRowCount(1);
    ui->nodeTable->setColumnCount(3);
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("Test"));
    newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui->nodeTable->setItem(1,1,newItem);

}

void frmServer::appendNode(nodeAddresses node) {
    int row = ui->nodeTable->rowCount() + 1;

    ui->nodeTable->setRowCount(2);

    QTableWidgetItem *nodeIP = new QTableWidgetItem (node.ipAddress);
    QTableWidgetItem *nodeKeepAlive = new QTableWidgetItem (node.keepAlive.toString());
    QTableWidgetItem *state = new QTableWidgetItem (tr("Connected"));

    ui->nodeTable->setItem(2,1, nodeIP);
    ui->nodeTable->setItem(2,2, nodeKeepAlive);
    ui->nodeTable->setItem(2,3, state);
}

frmServer::~frmServer()
{
    delete ui;
}
