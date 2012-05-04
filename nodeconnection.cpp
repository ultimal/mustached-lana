#include "nodeconnection.h"

nodeConnection::nodeConnection(QObject *parent, bool debug) : QTcpSocket(parent)
{

}

// Once a file has been selected for rendering send it to all the people that will render it
void nodeConnection::getBlenderFile(QString filename) {

}

// Once the image has been rendered send it to the owner
void nodeConnection::getRenderedImage (QString filename) {

}

// Find the position in the queue for specified na and send it to them
void nodeConnection::getQueuePosition (nodeAddresses na) {

}

// Process Reads
void nodeConnection::processReadyRead() {
    if (currentOperation==NONE) {
        QString data = socket->readAll();
        if (data=="GETIMAGE") { currentOperation==GETIMAGE; }
        if (data=="GETBLENDERFILE") { currentOperation==GETBLENDERFILE; }
        if (data=="GETQUEUEPOSITION") { currentOperation==GETQUEUEPOSITION; }
    } else {
        if (currentOperation==GETIMAGE) { getBlenderFile();currentOperation==NONE; }
        if (currentOperation==GETBLENDERFILE) { ;currentOperation==NONE; }
    }
}
