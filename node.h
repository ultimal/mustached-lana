#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include "datastore.h"
#include "nodeserver.h"

QDataStream &operator <<(QDataStream &stream, const nodeAddresses &myclass);
QDataStream &operator >>(QDataStream &stream, nodeAddresses &myclass);

class node : public QObject
{
    Q_OBJECT
public:
    explicit node(QObject *parent=0, QString nodePort="666", dataStore *ds=0, nodeAddresses nodeA=nodeAddresses(), bool d=false );

    enum sendOperations {
        NONE,
        SENDDB,
        SENDIMAGE,
        SENDBLENDERFILE,
        SENDKEEPALIVE,
        SENDQUEUEPOSITION
    };

public slots:
    void connected();
    void disconnected();
    void bytesWritten( qint64 bytes);

    // Once a file has been selected for rendering send it to all the people that will render it
    void sendBlenderFile(QString filename);

    // Once the image has been rendered send it to the owner
    void sendRenderedImage (QString filename);

    // Find the position in the queue for specified na and send it to them
    void sendQueuePosition (nodeAddresses na);

    // Process Reads
    void processReadyRead();

    // Timer: Send Keep Alive to Server
    void sendKeepAlive();

private:

    QTcpSocket *socket;

    nodeServer *ns;

    QString np;

    int currentOperation;

    bool debug;

    dataStore *ds;

    bool firstConnection;
};

#endif // NODE_H
