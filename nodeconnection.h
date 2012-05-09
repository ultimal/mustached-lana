#ifndef NODECONNECTION_H
#define NODECONNECTION_H

#include <QTcpSocket>
#include <QtNetwork>
#include <QDebug>
#include "datastore.h"
#include <QFile>
#include <QDir>

QDataStream &operator <<(QDataStream &stream, const nodeAddresses &myclass);
QDataStream &operator >>(QDataStream &stream, nodeAddresses &myclass);

class nodeConnection : public QTcpSocket
{
    Q_OBJECT

public:
    explicit nodeConnection(QObject *parent = 0, dataStore *ds, bool debug=false);

    enum getOperations {
        OP_NONE,
        OP_GETIMAGE,
        OP_GETBLENDERFILE,
        OP_GETQUEUEPOSITION
    };

    enum getBlenderFileProgress {
        BF_NONE,
        BF_SOURCEIP,
        BF_SOURCEPORT,
        BF_BLENDERFILENAME,
        BF_FRAMENUMBER,
        BF_BLENDERFILE
    };

    enum getImageFileOperationProgress {
        IMG_NONE,
        IMG_SOURCEIP,
        IMG_SOURCEPORT,
        IMG_IMAGEFILENAME,
        IMG_FRAMENUMBER,
        IMG_BLENDERFILENAME,
        IMG_IMAGEFILE
    };

signals:

public slots:

    // Find the position in the queue for specified na and send it to them
    void getQueuePosition (nodeAddresses na);

    // Process Reads
    void processReadyRead();

private:
    bool debug;

    // Timer for the keepAlive / Queue Position updates
    QTimer timer;

    int currentOperation;
    int getBlenderFileOperation;
    int getImageFileOperation;

    QByteArray block;

    dataStore *d;
    taskListType task;
    frameListType img;
};

#endif // NODECONNECTION_H
