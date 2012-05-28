#include "server.h"
#include "connection.h"
#include <QDebug>

Server::Server(QObject *parent, QString port, dataStore *ds, bool d) :
    QTcpServer(parent)
{
    // Running in debugging mode or not
    debug = d;

    // Get pointer to the dataStore object
    data = ds;

    if (listen(QHostAddress::Any,port.toInt())) {
        if (debug) { qDebug() << "Listening on port 666" << endl; }
    }
}

void Server::incomingConnection(int socketDescriptor) {
    Connection *connection = new Connection(this, data, debug);
    connection->setSocketDescriptor(socketDescriptor);

    // Pass pointer to the dataStore object
    connection->setLocalDS(data);

    if (debug) { qDebug() << "New incoming connection" << endl; }

    emit newConnection(connection);

    // Send this information back to the window
    nodeAddresses n;

    n.ipAddress = connection->peerAddress().toString();
    n.keepAlive = QTime::currentTime();
    n.port = "0";

    emit connectionEstablished(n);
}

