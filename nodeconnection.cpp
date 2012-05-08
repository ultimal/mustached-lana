#include "nodeconnection.h"

nodeConnection::nodeConnection(QObject *parent, dataStore *ds,  bool debug) : QTcpSocket(parent)
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
        if (currentOperation==GETBLENDERFILE) {
            if (getBlenderFileOperation==SOURCEIP)      {task.node.ipAddress = this->readAll();getBlenderFileOperation==SOURCEPORT;}
            if (getBlenderFileOperation==SOURCEPORT)    {task.node.port = this->readAll();getBlenderFileOperation==BLENDERFILENAME;}
            if (getBlenderFileOperation==BLENDERFILENAME) {task.blenderFile = this->readAll();getBlenderFileOperation==FRAMENUMBER;}
            if (getBlenderFileOperation==FRAMENUMBER)   {task.frameNumber = this->readAll();getBlenderFileOperation==BLENDERFILE;}
            if (getBlenderFileOperation==BLENDERFILE)   {
                // Download and save the file
                // ...
                getBlenderFileProgress=NONE;
                currentOperation=NONE;
            }
        }
    }
}

void nodeConnection::getBlenderFile() {

}

void nodeConnection::getImageFile() {

}
