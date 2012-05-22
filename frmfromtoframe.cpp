#include "frmfromtoframe.h"
#include "ui_frmfromtoframe.h"

frmFromToFrame::frmFromToFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmFromToFrame)
{
    ui->setupUi(this);

    QWidget::setWindowFlags(Qt::Popup);

    connect (ui->txtFrom, SIGNAL(textChanged(QString)),this,SLOT(setFrom(QString)));
    connect (ui->txtTo, SIGNAL(textChanged(QString)),this,SLOT(setTo(QString)));
    connect (ui->btnOk,SIGNAL(clicked()),this,SLOT(okClicked()));
}

frmFromToFrame::~frmFromToFrame()
{
    delete ui;
}

void frmFromToFrame::okClicked() {
    emit sendFromFrame(from);
    emit sendToFrame(to);
    this->close();
}
