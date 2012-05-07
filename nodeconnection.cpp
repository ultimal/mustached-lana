#include "nodeconnection.h"

nodeConnection::nodeConnection(QObject *parent, bool debug) : QTcpSocket(parent)
{

}

// Find the position in the queue for specified na and send it to them
void nodeConnection::getQueuePosition (nodeAddresses na) {

}

// Process Reads
void nodeConnection::processReadyRead() {
    if (currentOperation==NONE) {
        QString data = readAll();
        if (data=="GETIMAGE") { currentOperation==GETIMAGE; }
        if (data=="GETBLENDERFILE") { currentOperation==GETBLENDERFILE; }
        if (data=="GETQUEUEPOSITION") { currentOperation==GETQUEUEPOSITION; }
    } else {
        if (currentOperation==GETIMAGE) { getImageFile();currentOperation==NONE; }
        if (currentOperation==GETBLENDERFILE) { getBlenderFile();currentOperation==NONE; }
    }
}

void nodeConnection::getBlenderFile() {

}

void nodeConnection::getImageFile() {

}
