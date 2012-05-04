#ifndef DATASTORE_H
#define DATASTORE_H

#include <QObject>
#include <QVector>
#include <QTime>

struct nodeAddresses {
    QString ipAddress, port;
    QTime keepAlive;
};

struct frameListType {
    nodeAddresses node;
    QString blenderFile;
    double queuePosition, frameNumber;
};

struct jobListType {
    nodeAddresses node;
    QString blenderFile;
    double totalFrames, completedFrames;
    int percentComplete;
};

struct taskListType {
    nodeAddresses node;
    QString blenderFile;
    double frameNumber;
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

    // Get Task Queue Position at remote Node
    double taskGetQueuePosition(taskListType task);

    // Job: Next available frame for scheduling
    frameListType jobGetNextFrame();

signals:
    
public slots:

    // Used by connection to append a new Node to the data store
    void nodeAppend(nodeAddresses node);

    // Remove dead nodes
    void removeDeadNodes();

    // Used by node manager to remove dead nodes from the data store
    void nodeRemove(nodeAddresses node);

    // Update nodes with keep alive data
    void nodeUpdate(nodeAddresses node);

    // Add Task
    void taskAppend(taskListType task);

    // Remove Task
    void taskRemove(taskListType task);

    // Job: Append
    void jobAppend(jobListType job);

    // Remove completed frame from list
    void jobFrameRemove(frameListType frame);

    // Job: Remove
    void jobRemove(jobListType job);

    // Job: Update Job List with Queue Position from remote Node
    void jobUpdate(frameListType frame);

private:
    QVector<nodeAddresses> na;
    QVector<taskListType> taskList;
    QVector<jobListType> jobList;
    QVector<frameListType> jobFrameList;

};

#endif // DATASTORE_H
