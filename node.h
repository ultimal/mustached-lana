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

public slots:
    void connected();
    void disconnected();
    void bytesWritten( qint64 bytes);
    void processReadyRead();

    // Assign Tasks and send file
    void scheduleBlenderFile (QString filename);

    // Send Blender File
    void sendBlenderFile(QString filename, nodeAddresses node);
    void sendRenderedImage (QString filename);
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
