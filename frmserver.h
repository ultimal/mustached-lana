#ifndef FRMSERVER_H
#define FRMSERVER_H

#include <QWidget>

namespace Ui {
    class frmServer;
}

class frmServer : public QWidget
{
    Q_OBJECT

public:
    explicit frmServer(QWidget *parent = 0);
    ~frmServer();

private:
    Ui::frmServer *ui;
};

#endif // FRMSERVER_H
