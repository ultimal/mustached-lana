#include <QtGui/QApplication>

#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>

#include "frmnode.h"
#include "frmserver.h"

const char* program_name;

void print_usage (FILE* stream, int exit_code)
{
    // Prints usage information for this program to STREAM (stdout or stderr)
    fprintf (stream, "Usage: %s --node --server --host:[ip_address] --port:[port_number]\n", program_name);
    fprintf (stream,
             "   -n   --node                   Run as a node. Must specify --host and --port.\n"
             "   -s   --server                 Run in server mode. Needs --port.\n"
             "   -p   --port [number]          Used by both --node and --server options. Is a value between 0 and 65536.\n"
             "   -h   --host [ip_address]      Host is the server's IP address.\n\n"
             "   -c   --code                   Writes the source code to disk.\n"
             "   -d   --debug                  Run in debug mode. Only available in DEV version.\n"
             "    ?   --help                   Prints this information.\n\n");

    exit (exit_code);
}

int main(int argc, char *argv[])
{

    bool debug = false;

    // Grab the program name from the command line
    program_name = argv[0];

    QApplication a(argc, argv);

    // Configure Settings Object
    QCoreApplication::setOrganizationName("Haider Raza");
    QCoreApplication::setOrganizationDomain("haiderraza.com");
    QCoreApplication::setApplicationName("Mustached-Lana");

    frmNode *node;
    frmServer *server;

    QSettings settings;

    QString serverIp = settings.value("Server/IP",ui->txtServerPublicIP);
    QString serverPort = settings.value("Server/Port",ui->txtServerPort);
    QString tempFolder = settings.value("Node/TempFolder",ui->txtTempFolder);
    QString port = settings.value("Node/Port", ui->txtNodePort);

    /*
    QFile in("file.png");
    QFile out("NewFile.png");
    in.open(QFile::ReadOnly);
    out.open(QFile::WriteOnly);
    while (!in.atEnd()) {
        out.write(in.read(1024));
    }
    in.close();
    out.close(); */

    // Arguments from command-line parameters
    QString port = settings.value("nodePort");

    // Connect to this server if launched without arguments
    QString hostAddress= settings.value("serverIP");

    // Command line options and flags
    static int isNode, isServer;

    // Variable used by getopt_long to keep track of options
    int next_option;

    // Short options
    const char* const short_options = "cdh:np:s";

    // Long options
    const struct option long_options[] = {
        { "code",   no_argument,        NULL,   'c' },
        { "debug",  no_argument,        NULL,   'd' },
        { "help",   no_argument,        NULL,   '?' },
        { "host",   required_argument,  NULL,   'h' },
        { "node",   no_argument,        NULL,   'n' },
        { "port",   required_argument,  NULL,   'p' },
        { "server", no_argument,        NULL,   's' }
    };

    do {
        next_option = getopt_long (argc, argv, short_options, long_options, NULL);

        switch (next_option)
        {
        case 'c':
            // Extract code from end of executable or download it from the internet
            break;
        case 'd':
            // Debug mode on
            debug=true;
            qDebug() << "Debug mode: ON";
            break;
        case 'h':
            // Get the host ip address. This is the ip the node connects to.
            hostAddress = optarg;
            break;
        case 'n':
            isNode = 1;
            break;
        case 's':
            isServer=1;
            break;
        case 'p':
            // Get the port value from the command line
            port = optarg;
            break;
        case '?':
            print_usage(stderr, 1);
        case -1:
            // Done with options
            break;
        default:
            // Some unexpected results
            abort();
        }
    } while (next_option != -1);

    // If launched as node
    if (isNode) {
        // If the application was launched as Node but the port was not specified
        if (port=="0") {
            // Use the standard port
            port="666";
        }

        // Launch in Node mode
        if (debug) { qDebug() << "Launching in NODE mode with DEFAULT port"; }

        // launchNodeDialog();
        node = new frmNode(0,);
        node->show();
    }

    // If launched as server
    if (isServer) {

        // If the port was not specified use standard port
        if (port=="0") {
            port = "666";

            if (debug) { qDebug() << "Launching in SERVER mode with DEFAULT port."; }
        }

        if (debug) { qDebug() << "Launching in SERVER mode"; }

        server = new frmServer(0,port,debug);
        server->show();
    }

    // If simply launched without any arguments, run in node mode for internet server
    if ((!isNode) && (!isServer)) {
        nodeAddresses n;
        n.ipAddress = "127.0.0.1";
        n.port = "666";

        if (debug) { qDebug() << "Launching in INTERNET NODE mode using DEFAULT PORT and DEFAULT HOST ADDRESS";}
        node = new frmNode (0,NULL,n,debug);
        node.show();
    }

    if (debug) { qDebug() << "isNode: " << isNode << " | host_address: " << hostAddress << " | port: " << port; }
    if (debug) { qDebug() << "isServer: " << isServer; }

    return a.exec();
}
