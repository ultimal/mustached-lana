#ifndef FRMNODE_H
#define FRMNODE_H

#include <QWidget>

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

private:
    Ui::frmNode *ui;
    QString blenderFilename;
};

#endif // FRMNODE_H
