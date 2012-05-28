#ifndef FRMNODE_H
#define FRMNODE_H

#include <QWidget>
#include "node.h"
#include "frmfromtoframe.h"
#include "datastore.h"
#include "node.h"

namespace Ui {
    class frmNode;
}

class frmNode : public QWidget
{
    Q_OBJECT

public:
    explicit frmNode(QWidget *parent = 0,
                     QString ip="127.0.0.1",
                     QString nodePort="666",
                     nodeAddresses serverAddress=nodeAddresses(),
                     bool d=false );
    ~frmNode();

    void scheduleJob();

signals:

public slots:
    void openFile();

    void setFromFrame(double fromFrame) { from = fromFrame; }
    void setToFrame (double toFrame) { to = toFrame; }
    void updateNodeCount ();

private:
    Ui::frmNode *ui;
    QString blenderFilename;
    node *n;
    frmFromToFrame *toFrom;
    double to, from;
    dataStore *ds;
    nodeServer *ns;
};

#endif // FRMNODE_H
