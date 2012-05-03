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
    explicit node(QObject *parent = 0,  QString nodePort = "666", nodeAddresses nodeA, bool d = false);
    
    enum operations {
        NONE,
        SENDDB,
        SENDIMAGE,
        SENDBLENDERFILE
    };

public slots:
    void connected();
    void disconnected();
    void bytesWritten( qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    nodeServer *ns;
    QString np;
    int currentOperation;
    bool debug;
};

#endif // NODE_H
