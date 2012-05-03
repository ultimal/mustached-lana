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

    enum dataTypes {
        SENDBLENDERFILE,
        SENDIMAGE,
        QUEUEPOSITION
    };

signals:

public slots:

    // Once a file has been selected for rendering send it to all the people that will render it
    void sendBlenderFile(QString filename);

    // Once the image has been rendered send it to the owner
    void sendRenderedImage (QString filename);

    // Find the position in the queue for specified na and send it to them
    void sendQueuePosition (nodeAddresses na);

    // Process Reads
    void processReadyRead();

    // Get DataStore from Server
    void getDataStore();

    // Timer: Send Keep Alive to Server
    void keepAlive();

    // Timer: Send Queue Position to Task
    void sendQueuePosition();

private:
    bool debug;

    // Timer for the keepAlive / Queue Position updates
    QTimer timer;
};

#endif // NODECONNECTION_H
