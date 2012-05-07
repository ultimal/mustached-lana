#ifndef FRMFROMTOFRAME_H
#define FRMFROMTOFRAME_H

#include <QWidget>

namespace Ui {
    class frmFromToFrame;
}

class frmFromToFrame : public QWidget
{
    Q_OBJECT

public:
    explicit frmFromToFrame(QWidget *parent = 0);
    ~frmFromToFrame();

    double fromFrame() { return from; }
    double toFrame() { return to; }

signals:
    void sendFromFrame(double fromFrame);
    void sendToFrame(double toFrame);

public slots:
    void setFrom(QString fromFrame) { from = fromFrame; }
    void setTo(QString toFrame) { to = toFrame; }
    void okClicked();

private:
    Ui::frmFromToFrame *ui;
    double from, to;
};

#endif // FRMFROMTOFRAME_H
