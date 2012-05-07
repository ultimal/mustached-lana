#include "frmnode.h"
#include "ui_frmnode.h"
#include <QFileDialog>

frmNode::frmNode(QWidget *parent, QString nodePort="666", dataStore *ds=0, nodeAddresses serverAddress=nodeAddresses(), bool d=false ) :  QWidget(parent),  ui(new Ui::frmNode)
{
    // Setup all objects required for this app run
    ui->setupUi(this);
    n = new node(this);
    toFrom = new frmFromToFrame(this);
    ds = new dataStore(this);

    n = new node(this, "666", ds, serverAddress , true)

    // Setup all connections required to send and receive information
    connect (ui->btnScheduledJobs,SIGNAL(clicked()),this,SLOT(openFile()));
    connect (toFrom,SIGNAL(sendFromFrame(double)),this,SLOT(setFromFrame(double)));
    connect (toFrom,SIGNAL(sendToFrame(double)),this,SLOT(setToFrame(double)));
}

frmNode::~frmNode()
{
    delete ui;
}

void frmNode::scheduleJob() {
    jobListType j;
    j.blenderFile = blenderFilename;
    j.totalFrames = to - from;
    j.percentComplete = 0;
    j.completedFrames = 0;

    ds->jobAppend(j);

    // Get next available node and schedule job
    double i;
    while (i<=j.totalFrames) {
        frameListType frame;
        frame.blenderFile = blenderFilename;
        frame.frameNumber = i;
        frame.node = ds->nodeGetNext();
        frame.queuePosition = 0;

        // Tell the node socket to connect to the node and send the blenderfile
        while (!n->sendBlenderFile(frame.blenderFile,frame.node)) {
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


}
