#ifndef NODECONNECTION_H
#define NODECONNECTION_H

#include <QTcpSocket>
#include <QtNetwork>
#include <QDebug>
#include "datastore.h"

QDataStream &operator <<(QDataStream &stream, const nodeAddresses &myclass);
QDataStream &operator >>(QDataStream &stream, nodeAddresses &myclass);

class nodeConnection : public QTcpSocket
{
    Q_OBJECT

public:
    explicit nodeConnection(QObject *parent = 0, bool debug=false);

    enum getOperations {
        NONE,
        GETIMAGE,
        GETBLENDERFILE,
        GETQUEUEPOSITION
    };

signals:

public slots:

    // Find the position in the queue for specified na and send it to them
    void getQueuePosition (nodeAddresses na);

    // Process Reads
    void processReadyRead();

    // Get BlenderFile / Read the "block" and stream it to the file
    void getBlenderFile();

    // Get ImageFile / Read the "block" and stream it to the file
    void getImageFile();

private:
    bool debug;

    // Timer for the keepAlive / Queue Position updates
    QTimer timer;

    int currentOperation;

    QByteArray block;

};

#endif // NODECONNECTION_H
