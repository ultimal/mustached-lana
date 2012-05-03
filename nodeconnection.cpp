#include "nodeconnection.h"

nodeConnection::nodeConnection(QObject *parent) :
    QTcpSocket(parent)
{

}



// Find the position in the queue for specified na and send it to them
void nodeConnection::sendQueuePosition (nodeAddresses na){

}
