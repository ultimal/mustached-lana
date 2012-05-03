#ifndef DATASTORE_H
#define DATASTORE_H

#include <QObject>
#include <QVector>

struct nodeAddresses {
    QString ipAddress, port;
    int keepAlive;
};

struct frameListType {
    nodeAddresses node;
    QString blenderFile;
    double queuePosition, frameNumber;
};

struct jobListType {
    QString blenderFile;
    double totalFrames, completedFrames;
    int percentComplete;
};

struct taskListType {
    QString blenderFile,
    double totalFrames, completed
};

class dataStore : public QObject
{
    Q_OBJECT

public:
    explicit dataStore(QObject *parent = 0);

    // Server uses this routine to send out the data store to nodes
    QVector<nodeAddresses> getNodeList() { return na; }

    // Recreate the data store from what the server sends
    void restoreDB(QByteArray block);

signals:
    
public slots:
    // Used by connection to append a new Node to the data store
    void appendNode(nodeAddresses node);

    // Used by node manager to remove dead nodes from the data store
    void removeNode(nodeAddresses node);

    // Update nodes with keep alive data
    void updateNode(nodeAddresses node);

    // Add Task
    void taskAppend();

    // Remove Task
    void taskRemove();

    // Update Task
    void taskGetQueuePosition();

    // Job: Append
    void jobAppend();

    // Job: Remove
    void jobRemove();

    // Job: Update Job List with Queue Position from remote Node
    void jobUpdate();

    // Job: Next available frame for scheduling
    void jobGetNextFrame();



private:
    QVector<nodeAddresses> na;

};

#endif // DATASTORE_H
