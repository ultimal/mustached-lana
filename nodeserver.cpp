#include "nodeserver.h"

nodeServer::nodeServer(QObject *parent, QString port, dataStore *ds, bool d) :  QTcpServer(parent)
{
    debug = d;

    // Get pointer to the dataStore object
    setLocalDS (ds);

    if (listen(QHostAddress::Any,port.toInt())) {
        if (debug) { qDebug() << "Listening on port 666" << endl; }
    }
}

void nodeServer::incomingConnection(int socketDescriptor) {
    Connection *connection = new Connection(this);
    connection->setSocketDescriptor(socketDescriptor);

    // Pass pointer to the dataStore object
    connection->setLocalDS(data);

    qDebug() << "New incoming connection" << endl;

    emit newConnection(connection);
}

void nodeServer::setLocalDS (dataStore *ds) {
    data = ds;
}
