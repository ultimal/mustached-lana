#include "frmnode.h"
#include "ui_frmnode.h"
#include <QFileDialog>

frmNode::frmNode(QWidget *parent,
                 QString ip,
                 QString nodePort,
                 nodeAddresses serverAddress,
                 bool d) :  QWidget(parent),  ui(new Ui::frmNode)
{
    // Setup all objects required for this app to run
    ui->setupUi(this);

    toFrom = new frmFromToFrame(this);

    ds = new dataStore(this);

    // Connect to server and get dataStore
    n = new node(this, nodePort, ds, serverAddress , true);

    ds->setMyPort(nodePort);

    // Setup all connections required to send and receive information
    connect (ui->btnScheduledJobs,SIGNAL(clicked()),this,SLOT(openFile()));
    connect (toFrom,SIGNAL(sendFromFrame(double)),this,SLOT(setFromFrame(double)));
    connect (toFrom,SIGNAL(sendToFrame(double)),this,SLOT(setToFrame(double)));
    connect (n,SIGNAL(dbComplete()),this,SLOT(updateNodeCount()));

    // Prepare the TableWidget
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Job #") << tr("Filename") << tr("Progress"));
    ui->tableWidget->setColumnWidth(1,300);

}

frmNode::~frmNode()
{
    delete ui;
}

void frmNode::updateNodeCount() {
    ui->txtNodes->setText(ds->nodeCount());
}

void frmNode::scheduleJob() {
    jobListType j;
    j.blenderFile = blenderFilename;
    j.totalFrames = to - from;
    j.percentComplete = 0;
    j.completedFrames = 0;

    ds->jobAppend(j);

    // Get next available node and schedule job
    double i=0;
    while (i<=j.totalFrames) {
        frameListType frame;
        frame.blenderFile = blenderFilename;
        frame.frameNumber = i;
        frame.node = ds->nodeGetNext();
        frame.queuePosition = 0;

        // Tell the node socket to connect to the node and send the blenderfile
        while (!n->sendBlenderFile(frame)) {
            // If unsuccessfull reschedule the frame to a different node
            frame.node = ds->nodeGetNext();
        }

        i++;
    }
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
    scheduleJob();

}
