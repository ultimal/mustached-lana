#include "frmnode.h"
#include "ui_frmnode.h"
#include <QFileDialog>

frmNode::frmNode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmNode)
{
    // Setup all objects required for this app run
    ui->setupUi(this);
    n = new node(this);
    toFrom = new frmFromToFrame(this);
    ds = new dataStore(this);
    ns = new nodeServer(this);

    // Setup all connections required to send and receive information
    connect (ui->btnScheduledJobs,SIGNAL(clicked()),this,SLOT(openFile()));
    connect (toFrom,SIGNAL(sendFromFrame(double)),this,SLOT(setFromFrame(double)));
    connect (toFrom,SIGNAL(sendToFrame(double)),this,SLOT(setToFrame(double)));
}

frmNode::~frmNode()
{
    delete ui;
}

void frmNode::openFile() {
    // Show the open file dialog
    blenderFilename = QFileDialog::getOpenFileName(
                this,
                tr("Open Blender File"),
                "",
                tr("Blender Files (*.blend)"));

    // Get Frame count
    toFrom->show();

    // Schedule frames using Data Store


}
