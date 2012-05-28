#include "connection.h"

Connection::Connection(QObject *parent, dataStore *dataS, bool d) : QTcpSocket(parent) {
    ds = dataS;

    debug = d;

    QObject::connect(this,SIGNAL(readyRead()),this,SLOT(processReadyRead()));

    // We have the IP Address, we will have to wait for the correct port
    newNode.ipAddress = peerAddress().toString();
    newNode.keepAlive = QTime::currentTime();

    // Starting off currentOperation should set to NONE
    currentOperation = NONE;

    if (this->waitForReadyRead(10000)) {
        QString msg = this->readAll();

    }
}

void Connection::processReadyRead() {
    /* qDebug() << "Message: " << this->readAll();
    qDebug() << peerAddress() << ":" << peerPort();
    sendDB(); */
    QString msg = this->readAll(); if (debug) { qDebug() << msg; }

    // currentOperation is NONE then look for a command otherwise get data
    if (currentOperation==NONE) {
        // Register with the server
        if (msg=="REGISTER") { currentOperation = REGISTER;                 if (debug) { qDebug() << "REGISTER request received."; }}
        // Asking server to send a copy of the node DB
        if (msg=="SENDDB") { currentOperation = SENDDB;                     if (debug) { qDebug() << "SENDDB request received."; }}
        if (msg=="KEEPALIVE") { currentOperation = KEEPALIVE;               if (debug) { qDebug() << "KEEPALIVE request received."; }}
    } else {
        // Waiting for port number from the node
        if (currentOperation==REGISTER) {
                                                                            if (debug) { qDebug() << "REGISTER: Starting..."; }
            // Register the new node
            newNode.ipAddress = this->peerAddress().toString();             if (debug) { qDebug() << "REGISTER: peerAddress: " << newNode.ipAddress; }
            newNode.port = this->readAll();                                 if (debug) { qDebug() << "REGISTER: PORT: " << newNode.port; }
            newNode.keepAlive = QTime::currentTime();                       if (debug) { qDebug() << "REGISTER: KEEPALIVE: " << newNode.keepAlive.toString(); }

            ds->nodeAppend(newNode);                                        if (debug) { qDebug() << "REGISTER: appended to dataStore."; }

            currentOperation=NONE;                                          if (debug) { qDebug() << "REGISTER: complete."; }
        }

        if (currentOperation==SENDDB) {sendDB();                            if (debug) { qDebug() << "SENDDB: Starting..."; }}

        if (currentOperation==KEEPALIVE) {
                                                                            if (debug) { qDebug() << "KEEPALIVE: Starting..."; }
            newNode.port=readAll();
            newNode.keepAlive = QTime::currentTime();
            emit keepAlive(newNode);
                                                                            if (debug) { qDebug() << "KEEPALIVE: complete."; }
        }
    }
}

void Connection::sendDB() {
    /*
    qDebug() << "Opening and sending file...";
    QFile sample("./file.png");

    if (!sample.open(QFile::ReadOnly)) {
        qDebug() << "Unable to open file";
    } else {
        qDebug() << "File open";

        while (!sample.atEnd()) {
            qDebug() << "Looping through blocks of data (1024)";
            write(sample.read(4096));
        }

        qDebug() << "Flushing data";
        flush();
    } */

    QByteArray block;
    QDataStream out(&block, QIODevice::ReadOnly);

    int i = 0;

    QVector<nodeAddresses> nodes = ds->getNodeList();

    while (!i>nodes.count()) {
        out << nodes.at(i);
        i++;
    }

    write(block);
    flush();
                                                                            if (debug) { qDebug() << "SENDDB: complete."; }
    this->close();
                                                                            if (debug) { qDebug() << "Connection closed."; }
}

QDataStream &operator <<(QDataStream &stream, const nodeAddresses &myclass) {
    stream << myclass.ipAddress;
    stream << myclass.port;
    stream << myclass.keepAlive;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, nodeAddresses &myclass) {
    stream >> myclass.ipAddress;
    stream >> myclass.port;
    stream >> myclass.keepAlive;
    return stream;
}
