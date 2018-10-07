#ifndef CREATDIALOG_H
#define CREATDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QHostInfo>
#include <QtNetwork/QHostAddress>

namespace Ui {
class CreatDialog;
}

class CreatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatDialog(QWidget *parent = nullptr);
    ~CreatDialog();
    QHostAddress address;
    int port();

private slots:

private:
    Ui::CreatDialog *ui;
};

#endif // CREATDIALOG_H
