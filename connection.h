#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QtNetwork>
#include <QDebug>
#include "datastore.h"

QDataStream &operator <<(QDataStream &stream, const nodeAddresses &myclass);
QDataStream &operator >>(QDataStream &stream, nodeAddresses &myclass);

class Connection : public QTcpSocket {

    Q_OBJECT
public:

    enum dataType {                     // What type of data was sent
        REGISTER,                       // Register with server
        PING,                           // Ping the server
        SENDDB,                         // Server is sending the DB
        IMAGE,                          // Node sending rendered image
        BLENDERFILE,                    // Node sending blender file to be rendered
        QUEUEPOSITION,                  // Job Queue Position
        NONE,                           // No operation
        KEEPALIVE                       // Keep alive request
    };

    explicit Connection(QObject *parent = 0, dataStore *dataS = 0, bool d = false);
    void setLocalDS (dataStore *data) { ds = data; }

signals:
    void keepAlive(nodeAddresses node);

public slots:
    void processReadyRead();            // This is where we process the readyRead() signal
    void sendDB();                      // Send the DB of registered nodes out to new node

private:
    dataStore *ds;
    nodeAddresses newNode;
    int currentOperation;
    bool debug;
};

#endif // CONNECTION_H
