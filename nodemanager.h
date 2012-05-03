#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include <QObject>
#include "server.h"
#include "datastore.h"

class nodeManager : public QObject
{
    Q_OBJECT
public:
    explicit nodeManager(QObject *parent = 0, QString port="666");

signals:

public slots:

private:
    Server *srv;
    dataStore *ds;

};

#endif // NODEMANAGER_H
