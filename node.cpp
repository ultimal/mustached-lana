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
    connect (socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    connect (socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

    socket->connectToHost(serverAddress.ipAddress, serverAddress.port.toInt());

    // TODO: Server connect timeout needs to be a user set thing
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

    // If this is this sockets first Connection to the server
    if (firstConnection) {
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

        firstConnection = false;
    }

    // Do whatever else for all subsequent connections

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

void node::processReadyRead() {
    // Get Data Store from server
    QString tmp = socket->readAll();
    if (currentOperation==NONE) {
        if (tmp=="SENDDB") { currentOperation = SENDDB; }
    } else {
        if (currentOperation==SENDDB) {
            // Read the DB
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);

            int i = 0;

            QVector<nodeAddresses> nodeList;
            nodeAddresses node;

            out << socket->readAll();

            while (!out.atEnd()) {
                out >> node;
                ds->nodeAppend(node);
            }

            currentOperation == NONE;
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

void node::sendQueuePosition(nodeAddresses na, double position) {
    // Send Queue Positions to all Client Nodes
    socket->connectToHost(na.ipAddress,na.port);

    if (socket->waitForConnected(10000)) {
        socket->write ("GETQUEUEPOSITION");
        socket->flush();
        socket->write (QString::number(position));
        socket->flush();
    }
}
