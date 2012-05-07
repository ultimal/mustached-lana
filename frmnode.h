#ifndef FRMNODE_H
#define FRMNODE_H

#include <QWidget>
#include "node.h"
#include "frmfromtoframe.h"
#include "datastore.h"
#include "nodeserver.h"

namespace Ui {
    class frmNode;
}

class frmNode : public QWidget
{
    Q_OBJECT

public:
    explicit frmNode(QWidget *parent = 0);
    ~frmNode();

signals:

public slots:
    void openFile();

    void setFromFrame(double fromFrame) { from = fromFrame; }
    void setToFrame (double toFrame) { to = toFrame; }

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
