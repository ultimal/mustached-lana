#include "datastore.h"

dataStore::dataStore(QObject *parent) : QObject(parent)
{

}

void dataStore::nodeAppend(nodeAddresses node) {
    na.append(node);
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

}

// Add Task
void dataStore::taskAppend() {

}

// Remove Task
void dataStore::taskRemove() {

}

// Update Task
void dataStore::taskGetQueuePosition() {

}

// Job: Append
void dataStore::jobAppend() {

}

// Job: Remove
void dataStore::jobRemove() {

}

// Job: Update Job List with Queue Position from remote Node
void dataStore::jobUpdate() {

}

// Job: Next available frame for scheduling
void dataStore::jobGetNextFrame() {

}

