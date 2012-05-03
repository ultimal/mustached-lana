#include "nodemanager.h"

nodeManager::nodeManager(QObject *parent, QString port) : QObject(parent)
{
    // Create a new dataStore
    ds = new dataStore(this);

    // Start the server
    srv  = new Server(this, port, ds);

}
