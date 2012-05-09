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
        if (data=="GETIMAGE") { currentOperation==OP_GETIMAGE; }
        if (data=="GETBLENDERFILE") { currentOperation==OP_GETBLENDERFILE; }
        if (data=="GETQUEUEPOSITION") { currentOperation==OP_GETQUEUEPOSITION; }
    } else {
        if (currentOperation==GETIMAGE) {
            if (getImageFileOperation==IMG_SOURCEIP)        { img.node.ipAddress = this->readAll(); getImageFileOperationProgress=IMG_SOURCEPORT; }
            if (getImageFileOperation==IMG_SOURCEPORT)      { img.node.port = this->readAll(); getImageFileOperationProgress=IMG_IMAGEFILENAME; }
            if (getImageFileOperation==IMG_IMAGEFILENAME)   { img.imageFile = this->readAll(); getImageFileOperationProgress=IMG_FRAMENUMBER; }
            if (getImageFileOperation==IMG_FRAMENUMBER)     { img.frameNumber = this->readAll(); getImageFileOperationProgress=IMG_BLENDERFILENAME; }
            if (getImageFileOperation==IMG_BLENDERFILENAME) { img.lenderFile = this->readAll(); getImageFileOperationProgress=IMG_IMAGEFILE; }
            if (getImageFileOperation==IMG_IMAGEFILE)       {
                QFile imageFile(QSettings::value("ProjectPath").toString() + img.blenderFile + "/" + img.imageFile);
                imageFile.open(QFile::WriteOnly);

                if (debug) { qDebug() << "Receiving Rendered Image: " << img.imageFile << " from project: " + img.blenderFile; }

                while (!atEnd()) {

                    if (debug) { qDebug() << "  Downloaded: 4096 bytes"; }

                    imageFile.write(read(4096));
                }

                imageFile.close();

                // Post the frame job as complete to the data store, so it doesn't get rescheduled
                QVector<frameListType> frames = d->getJobFrameList();

                int i = 0;

                while (!i>frames.count()) {
                    // Look for the frame in the frame list and mark it as complete
                    if (frames[i].blenderFile == img.blenderFile) {
                        if (frames[i].frameNumber==img.frameNumber) {
                            frames[i].complete = true;
                            break;
                        }
                    }

                    i++;
                }
            }

        if (currentOperation==GETBLENDERFILE) {
            if (getBlenderFileOperation==BF_SOURCEIP)      {task.node.ipAddress = this->readAll();getBlenderFileOperation=BF_SOURCEPORT;}
            if (getBlenderFileOperation==BF_SOURCEPORT)    {task.node.port = this->readAll();getBlenderFileOperation=BF_BLENDERFILENAME;}
            if (getBlenderFileOperation==BF_BLENDERFILENAME) {task.blenderFile = this->readAll();getBlenderFileOperation=BF_FRAMENUMBER;}
            if (getBlenderFileOperation==BF_FRAMENUMBER)   {task.frameNumber = this->readAll();getBlenderFileOperation=BF_BLENDERFILE;}
            if (getBlenderFileOperation==BF_BLENDERFILE)   {
                // Download and save the file
                // Saving a file
                /*
                qDebug() << "Data received";

                QFile sample("./NewFile.png");
                sample.open(QFile::WriteOnly);

                qDebug() << "File open for writing";
                while (!socket->atEnd()) {
                    qDebug() << "Looping through blocks of available data (1024)";
                    sample.write(socket->read(4096));
                }

                qDebug() << "Writing file out to disk.";
                sample.close(); */
                QFile blenderFile(QSettings::value("TemporaryPath").toString() + task.blenderFile);
                blenderFile.open(QFile::WriteOnly);

                if (debug) { qDebug() << "Receiving file: " << task.blenderFile << " from " << task.node.ipAddress << ":" << task.node.port; }

                while (!atEnd())  {
                    if (debug) { qDebug() << "  Downloaded: 4096 bytes"; }
                    blenderFile.write(read(4096));
                }

                blenderFile.close();

                // Post the task to data store
                d->taskAppend(task);
                getBlenderFileProgress=BF_NONE;
                currentOperation=OP_NONE;
            }
        }
    }
}
