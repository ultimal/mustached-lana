#include "node.h"

node::node(QObject *parent, QString nodePort, nodeAddresses nodeA, bool d ) :  QObject(parent)
{
    debug = d;

    socket = new QTcpSocket(this);

    connect (socket,SIGNAL(connected()),this,SLOT(connected()));
    connect (socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    connect (socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

    socket->connectToHost(nodeA.ipAddress, nodeA.port.toInt());

    if (!socket->waitForConnected(10000)) {
        if (debug) { qDebug() << "Server not found"; }
    } else {
        if (debug) { qDebug() << "Node started..." << endl; }
    }

    np = nodePort;

    currentOperation == NONE;

    // Start the nodeServer
    ns = new nodeServer(this, np, ds, debug);

}

// Called connected
void node::connected() {

    if (debug) { qDebug() << "Connected to server..." << endl; }

    // Once connected send the COMMAND to the server
    socket->write("REGISTER");
    socket->flush();

    // Now send the node PORT for registration with the server
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << np;
    socket->write(block);
    socket->flush();

    // Now request the database
    socket->write("SENDDB");
    socket->flush();
}

// Called when disconnected
void node::disconnected() {

}

// Called when data written
void node::bytesWritten(qint64 bytes) {
    qDebug() << "Bytes sent: " << bytes;
}

// Once a file has been selected for rendering send it to all the people that will render it
void nodeConnection::sendBlenderFile(QString filename, nodeAddresses node) {
    QFile blenderFile (filename);
    if (!blenderFile.open(QIODevice::ReadOnly)) return;
    while (!blenderFile.atEnd()) {
        write(blenderFile.read(1024));
    }
    blenderFile.close();
}

// Once the image has been rendered send it to the owner
void nodeConnection::sendRenderedImage (QString filename) {
    QFile blenderFile (filename);
    if (!blenderFile.open(QIODevice::ReadOnly)) return;
    while (!blenderFile.atEnd()) {
        write(blenderFile.read(1024));
    }
    blenderFile.close();
}

// Called when data available for reading
void node::readyRead() {
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

    if (currentOperation==NONE) {
        QString data = socket->readAll();
        if (data=="SENDDB") { currentOperation==SENDDB; }
        if (data=="SENDIMAGE") { currentOperation==SENDIMAGE; }
        if (data=="SENDBLENDERFILE") { currentOperation==SENDBLENDERFILE; }
    } else {
        if (currentOperation==SENDDB) { currentOperation==NONE; }
        if (currentOperation==SENDIMAGE) { currentOperation==NONE; }
        if (currentOperation==SENDBLENDERFILE) { currentOperation==NONE; }
    }
}
