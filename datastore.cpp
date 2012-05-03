#include "datastore.h"

dataStore::dataStore(QObject *parent) : QObject(parent)
{

}

void dataStore::appendNode(nodeAddresses node) {
    na.append(node);
}

void dataStore::removeNode(nodeAddresses node) {
    int i = 0;

    // Improve this so that it is done internally by Qt
    while (!i>na.count()) {

        if ((na.at(i).ipAddress == node.ipAddress) && (na.at(i).port == node.port)) {
            na.remove(i);
        }

        i++;
    }
}
