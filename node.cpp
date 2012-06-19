#include "node.h"

node::node(QObject *parent, QString nodePort, dataStore *ds, nodeAddresses serverAddress, bool d ) :  QObject(parent)
{
    debug = d;

    serverInfo = serverAddress;

    // Class is being initialized for the first time
    firstConnection = true;

    socket = new QTcpSocket(this);

    connect (socket,SIGNAL(connected()),this,SLOT(connected()));
    connect (socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socket,SIGNAL(readyRead()),this,SLOT(processReadyRead()));

    np = nodePort;

    currentOperation = NONE;
    waitForACK = false;

    // Start the nodeServer
    ns = new nodeServer (this, np, ds, debug);

    socket->connectToHost (serverAddress.ipAddress, serverAddress.port.toShort());

    if (socket->waitForConnected(3000)==false) {
        // If unable to connect
        if (debug) { qDebug() << "NODE: Unable to connect: " << serverInfo.ipAddress << ":" << serverInfo.port; }
        qApp->quit();
    }
}

// Verify if ACK was received
bool node::gotACK(QByteArray b) {
    if (QString::fromUtf8(b) == "ACK" ) {
        return true;
    } else {
        return false;
    }
}

// Called connected
void node::connected() {

    if (debug) { qDebug() << "Connected to server..." << endl; }

    // If this is this sockets first Connection to the server
    if (firstConnection) {
        // Once connected send the COMMAND to the server
        socket->write(tr("REGISTER").toUtf8());                    if (debug) { qDebug() << "REGISTER: Request sent..."; }

        currentOperation = REGISTER;
        waitForACK = true;

        firstConnection = false;
    }

    // Do whatever else for all subsequent connections

}

void node::processReadyRead() {
                                                                if (debug) { qDebug() << "Data Received: Processing"; }
    if (currentOperation==RECEIVEDB) {
                                                                if (debug) { qDebug() << "SENDDB: Reading DB"; }
        // Read the DB
        QByteArray block;
        QDataStream out(&block, QIODevice::ReadWrite);

        QVector<nodeAddresses> nodeList;
        nodeAddresses node;

        out << socket->readAll();

        while (!out.atEnd()) {
            out >> node;                                        if (debug) { qDebug() << "SENDDB: Received: " << node.ipAddress << ":" << node.port << " - " << node.keepAlive; }
            ds->nodeAppend(node);
        }

        if (debug) { qDebug() << "SENDDB: dataStore Count: " << ds->nodeCount(); }
        currentOperation=NONE;
        emit dbComplete();
    }

    // Get Data Store from server
    if (waitForACK && currentOperation==REGISTER) {
        if (gotACK(socket->readAll())) {

            waitForACK = false;

            // Now send the node PORT for registration with the server
            socket->write(np.toUtf8());                         if (debug) { qDebug() << "REGISTER: PORT: " << np << " Sent..."; }

            currentOperation = RECEIVEDB;
        }
    }
}

void node::sendKeepAlive() {

    socket->write("KEEPALIVE");
    socket->flush();

    // Send the port number, since that is what differentiates
    // multiple computers on the same network
    socket->write(np.toUtf8());
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
bool node::sendBlenderFile (frameListType f) {
    QFile blenderFile (f.blenderFile);
    if (!blenderFile.open(QIODevice::ReadOnly)) return false;

    // Connect to node and send file
    QTcpSocket *tmpSocket = new QTcpSocket(this);

    QString ip = f.node.ipAddress;
    quint16 port = f.node.port.toInt();

    tmpSocket->connectToHost(ip,port);

    // TODO: Connection timeout should be a user setting
    if (!tmpSocket->waitForConnected(10000)) {
            if (debug) { qDebug() << "  Node: " << f.node.ipAddress << ":" << f.node.port << " not found"; }
            return false;
        } else {
            // If connected
            if (debug) { qDebug() << "* Node: " << f.node.ipAddress << ":" << f.node.port << " connected"; }

            // Send file
            tmpSocket->write("GETBLENDERFILE");
            tmpSocket->flush();

            // Since it is a public internet ip address the
            // remote node needs to get my ip from its socket

            // Send Local Port
            tmpSocket->write(ds->getMyPort().toUtf8());
            tmpSocket->flush();

            // Send File Name
            tmpSocket->write(f.blenderFile.toUtf8());
            tmpSocket->flush();

            // Send Frame Number
            tmpSocket->write(QString::number(f.frameNumber).toUtf8());
            tmpSocket->flush();

            while (!blenderFile.atEnd()) {
                if (!tmpSocket->write(blenderFile.read(4096))) { return false; }
            }

            if (debug) { qDebug() << "* Node: " << f.node.ipAddress << ":" << f.node.port << " file sent"; }

            blenderFile.close();
            tmpSocket->close();

            return true;
        }
}

// Once the image has been rendered send it to the owner
void node::sendRenderedImage (QString filename) {
    QFile blenderFile (filename);
    if (!blenderFile.open(QIODevice::ReadOnly)) return;
    while (!blenderFile.atEnd()) {
        socket->write(blenderFile.read(1024));
    }
    blenderFile.close();
}

// Called when data available for reading
//void node::readyRead() {
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
// }

void node::sendQueuePosition(nodeAddresses na, double position) {
    // Send Queue Positions to all Client Nodes
    socket->connectToHost(na.ipAddress,na.port.toShort());

    if (socket->waitForConnected(10000)) {
        socket->write (QString("GETQUEUEPOSITION").toUtf8());
        socket->flush();
        socket->write (QString::number(position).toUtf8());
        socket->flush();
    }
}
