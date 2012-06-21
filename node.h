#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include "datastore.h"
#include "nodeserver.h"

class node : public QObject
{
    Q_OBJECT

public:
    explicit node(QObject *parent=0, QString nodePort="666", dataStore *ds=0, nodeAddresses nodeA=nodeAddresses(), bool d=false );
    
    enum operations {
        NONE,
        REGISTER,
        RECEIVEDB,
        SENDIMAGE,
        SENDBLENDERFILE,
        KEEPALIVE
    };

    bool gotACK(QByteArray b);
    bool sendBlenderFile (frameListType f);

signals:
    void dbComplete();

public slots:
    void connected();
    void disconnected();
    void bytesWritten( qint64 bytes);
    void sendRenderedImage (QString filename);
    void sendQueuePosition (nodeAddresses na, double position);
    void sendKeepAlive();
    void processReadyRead();

private:
    QTcpSocket *socket;
    nodeServer *ns;
    QString np;
    int currentOperation;
    bool debug;
    dataStore *ds;
    bool firstConnection;
    bool waitForACK;
    nodeAddresses serverInfo;
    bool outEmpty;
};

#endif // NODE_H
