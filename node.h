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
        SENDDB,
        SENDIMAGE,
        SENDBLENDERFILE,
        KEEPALIVE
    };

signals:
    void dbComplete();

public slots:
    void connected();
    void disconnected();
    void bytesWritten( qint64 bytes);
    bool sendBlenderFile (frameListType f);
    void sendRenderedImage (QString filename);
    void sendKeepAlive();
    void processReadyRead();
    void sendQueuePosition(nodeAddresses na, double position);

private:
    QTcpSocket *socket;
    nodeServer *ns;
    QString np;
    int currentOperation;
    bool debug;
    dataStore *ds;
    nodeAddresses serverInfo;
    bool firstConnection;
};

#endif // NODE_H
