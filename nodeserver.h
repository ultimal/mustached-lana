#ifndef NODESERVER_H
#define NODESERVER_H

#include <QTcpServer>
#include "datastore.h"
#include "connection.h"

class nodeServer : public QTcpServer
{
    Q_OBJECT
public:
    nodeServer(QObject *parent = 0, QString port="666", dataStore *ds=0, bool d=false);
    void setLocalDS (dataStore *ds);

signals:
    void newConnection(Connection *connection);

public slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    dataStore *data;
    bool debug;
};

#endif // NODESERVER_H
