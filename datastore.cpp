#include "datastore.h"

dataStore::dataStore(QObject *parent) : QObject(parent)
{

}

nodeAddresses dataStore::nodeGetNext() {
    nodeAddresses node;
    node.ipAddress = na[currentNode].ipAddress;
    node.port = na[currentNode].port;
    node.keepAlive = na[currentNode].keepAlive;

    // Start from zero again if we reach the end of the list
    if (na.count() > currentNode) { currentNode++; } else { currentNode = 0; }

    return node;
}

void dataStore::nodeAppend(nodeAddresses node) {
    na.append(node);
}

void dataStore::removeDeadNodes() {
    // Run through the dataStore looking for node that have not checked in
    // Within the last 30 minutes
    for (double i = 0; i<=na.count(); i++) {
        QTime keepAliveTime = na.at(i).keepAlive;
        int timeDifference = keepAliveTime.secsTo(QTime::currentTime());

        if (timeDifference > 1800) {

        }
    }
}

void dataStore::nodeRemove(nodeAddresses node) {
    int i = 0;

    // Improve this so that it is done internally by Qt
    while (!i>na.count()) {

        if ((na.at(i).ipAddress == node.ipAddress) && (na.at(i).port == node.port)) {
            na.remove(i);
        }

        i++;
    }
}

// Update nodes with keep alive data
void dataStore::nodeUpdate(nodeAddresses node) {
    for (int i=0; i<na.count();i++) {
        if ((na[i].ipAddress == node.ipAddress) && (na[i].port == node.port)) {
            // Add keepAlive data to the data Store
            na[i].keepAlive = QTime::currentTime();
        }
    }
}

// Add Task
void dataStore::taskAppend(taskListType task) {
    taskList.append(task);
}

// Remove Task
void dataStore::taskRemove(taskListType task) {
    for(double i=0; i<=taskList.count(); i++) {
        if ((taskList.at(i).node.ipAddress == task.node.ipAddress) &&
            (taskList.at(i).node.port == task.node.port) &&
            (taskList.at(i).blenderFile == task.blenderFile) &&
            (taskList.at(i).frameNumber == task.frameNumber)){
            // If the correct frame is found in the task list
            taskList.remove(i);
            return;
        }
    }
}

// Update Task
double dataStore::taskGetQueuePosition(taskListType task) {
    for (double i=0; i<=taskList.count(); i++) {
        if ((taskList.at(i).node.ipAddress == task.node.ipAddress) &&
            (taskList.at(i).node.port == task.node.port) &&
            (taskList.at(i).blenderFile == task.blenderFile) &&
            (taskList.at(i).frameNumber == task.frameNumber)){
            // If the correct frame is found in the task list
        return i;
        }
    }
}

// Job: Append
void dataStore::jobAppend(jobListType job) {
    jobList.append(job);
}

// Job: Frame Remove
void dataStore::jobFrameRemove(frameListType frame) {
    // Remove job frame from list when rendered image received
    for (double i=0; i<=jobFrameList.count(); i++) {
        if ((jobFrameList.at(i).blenderFile == frame.blenderFile) &&
            (jobFrameList.at(i).frameNumber == frame.frameNumber) &&
            (jobFrameList.at(i).node.ipAddress == frame.node.ipAddress) &&
            (jobFrameList.at(i).node.port == frame.node.port)) {
            jobFrameList.remove(i);
            return;
        }
    }
}

// Job: Remove
void dataStore::jobRemove(jobListType job) {
    for (double i=0; i<=jobList.count(); i++) {
        if ((jobList.at(i).blenderFile == job.blenderFile) &&
            (jobList.at(i).totalFrames== job.totalFrames) &&
            (jobList.at(i).node.ipAddress == job.node.ipAddress) &&
            (jobList.at(i).node.port == job.node.port)) {
            jobList.remove(i);
            return;
        }
    }
}

// Job: Update Job List with Queue Position from remote Node
void dataStore::jobFrameUpdate(frameListType frame) {
    for (double i=0; i<=jobFrameList.count(); i++) {
        if ((jobFrameList.at(i).blenderFile == frame.blenderFile) &&
            (jobFrameList.at(i).frameNumber == frame.frameNumber) &&
            (jobFrameList.at(i).node.ipAddress == frame.node.ipAddress) &&
            (jobFrameList.at(i).node.port == frame.node.port)) {
            jobFrameList[i].queuePosition = frame.queuePosition;
            jobFrameList[i].keepAlive = QTime::currentTime();
            return;
        }
    }
}

// Job: Next available frame for scheduling
frameListType dataStore::jobGetNextFrame() {
    for (double i=0; i<=jobFrameList.count(); i++) {
        if (jobFrameList.at(i).node.ipAddress == "") {
            // If there is no node assigned to a frame return that frame
            frameListType frame = jobFrameList.at(i);
            return frame;
        }
    }
}
