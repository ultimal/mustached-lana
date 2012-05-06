#include "frmnode.h"
#include "ui_frmnode.h"
#include <QFileDialog>

frmNode::frmNode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmNode)
{
    ui->setupUi(this);
    connect (ui->btnScheduledJobs,SIGNAL(clicked()),this,SLOT(openFile()));

}

frmNode::~frmNode()
{
    delete ui;
}

void frmNode::openFile() {
    // Show the open file dialog
    blenderFilename = QFileDialog::getOpenFileName(this, tr("Open Blender File"),"", tr("Blender Files (*.blender)"));
}
