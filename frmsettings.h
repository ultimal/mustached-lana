#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class frmSettings;
}

class frmSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit frmSettings(QWidget *parent = 0);
    ~frmSettings();

public slots:
    void saveAndClose();
    
private:
    Ui::frmSettings *ui;
    QSettings settings;
};

#endif // FRMSETTINGS_H
