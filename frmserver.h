#ifndef FRMSERVER_H
#define FRMSERVER_H

#include <QWidget>
#include "datastore.h"
#include "server.h"

namespace Ui {
    class frmServer;
}

class frmServer : public QWidget
{
    Q_OBJECT

public:
    explicit frmServer(QWidget *parent = 0, , QString port="666", bool debug=false);
    ~frmServer();

public slots:
    void appendNode(nodeAddresses node);

private:
    Ui::frmServer *ui;
    server *srv;
    dataStore *data;

};

#endif // FRMSERVER_H
