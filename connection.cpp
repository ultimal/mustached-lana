#include "connection.h"

Connection::Connection(QObject *parent) : QTcpSocket(parent) {
    QObject::connect(this,SIGNAL(readyRead()),this,SLOT(processReadyRead()));

    // We have the IP Address, we will have to wait for the correct port
    newNode.ipAddress = peerAddress().toString();
    newNode.keepAlive = QTime::currentTime();

    // Starting off currentOperation should set to NONE
    currentOperation = NONE;
}

void Connection::processReadyRead() {
    /* qDebug() << "Message: " << this->readAll();
    qDebug() << peerAddress() << ":" << peerPort();
    sendDB(); */
    QString msg = this->readAll();

    // currentOperation is NONE then look for a command otherwise get data
    if (currentOperation==NONE) {
        // Register with the server
        if (msg=="REGISTER") {currentOperation = REGISTER;}
        // Asking server to send a copy of the node DB
        if (msg=="DB") {currentOperation = DB; }
        if (msg=="KEEPALIVE") {currentOperation = KEEPALIVE;}
    } else {
        // Waiting for port number from the node
        if (currentOperation==REGISTER) {
            // Register the new node
            newNode.ipAddress = this->peerAddress().toString();
            newNode.port = this->readAll();
            newNode.keepAlive = QTime::currentTime();

            ds->nodeAppend(newNode);

            currentOperation==NONE;
        }
        if (currentOperation==DB) {sendDB();}
        if (currentOperation==KEEPALIVE) {
            newNode.port=readAll();
            newNode.keepAlive = QTime::currentTime();
            emit keepAlive(newNode);
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
