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
    nodeAddresses   node;
    QString         blenderFile;
    QString         imageFile;
    double          queuePosition, frameNumber;
    QTime           keepAlive;
    bool            complete;
};

struct jobListType {
    QString         blenderFile;
    double          totalFrames, completedFrames;
    int             percentComplete;
};

struct taskListType {
    nodeAddresses   node;
    QString         blenderFile;
    double          frameNumber;
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

    // Node: Get next available node from data store
    nodeAddresses nodeGetNext();

    QVector<taskListType> getTaskList()      { return taskList; }
    QVector<jobListType> getJobList()        { return jobList; }
    QVector<frameListType> getJobFrameList() { return jobFrameList; }

    void setMyPort (QString port) { myPort = port; }

    QString getMyPort() { return myPort; }

    double nodeCount() { return na.count(); }

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
    void jobFrameUpdate(frameListType frame);

private:
    QVector<nodeAddresses> na;
    QVector<taskListType> taskList;
    QVector<jobListType> jobList;
    QVector<frameListType> jobFrameList;

    QString myPort;
    double currentNode;
};

#endif // DATASTORE_H
