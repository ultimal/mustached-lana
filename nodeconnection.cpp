#include "nodeconnection.h"

nodeConnection::nodeConnection(QObject *parent, bool debug) : QTcpSocket(parent)
{

}

// Once a file has been selected for rendering send it to all the people that will render it
void nodeConnection::sendBlenderFile(QString filename) {

}

// Once the image has been rendered send it to the owner
void nodeConnection::sendRenderedImage (QString filename) {

}

// Find the position in the queue for specified na and send it to them
void nodeConnection::sendQueuePosition (nodeAddresses na) {

}

// Process Reads
void nodeConnection::processReadyRead() {

}

// Get DataStore from Server
void nodeConnection::getDataStore() {

}

// Timer: Send Keep Alive to Server
void nodeConnection::keepAlive(){

}

// Timer: Send Queue Position to Task
void nodeConnection::sendQueuePosition() {

}
