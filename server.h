#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "datastore.h"

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0, QString port="666", dataStore *ds=0, bool d=false);
    void setLocalDS (dataStore *ds);

signals:
    void newConnection(Connection *connection);
    void connectionEstablished (nodeAddresses na);

public slots:
    
protected:
    void incomingConnection(int socketDescriptor);

private:
    dataStore *data;
    bool debug;
};

#endif // SERVER_H
