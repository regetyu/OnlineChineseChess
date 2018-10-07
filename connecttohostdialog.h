#ifndef CONNECTTOHOSTDIALOG_H
#define CONNECTTOHOSTDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include <QDebug>

namespace Ui {
class ConnectToHostDialog;
}

class ConnectToHostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectToHostDialog(QWidget *parent = nullptr);
    ~ConnectToHostDialog();
    QHostAddress address();
    int port();
    bool isIpAddr();

private:
    Ui::ConnectToHostDialog *ui;
};

#endif // CONNECTTOHOSTDIALOG_H
