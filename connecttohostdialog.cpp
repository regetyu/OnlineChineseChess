#include "connecttohostdialog.h"
#include "ui_connecttohostdialog.h"

ConnectToHostDialog::ConnectToHostDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectToHostDialog)
{
    ui->setupUi(this);
}

ConnectToHostDialog::~ConnectToHostDialog()
{
    delete ui;
}

QHostAddress ConnectToHostDialog::address()
{
    return QHostAddress(ui->lineEdit->text());
}

int ConnectToHostDialog::port()
{
    return ui->spinBox->value();
}
bool ConnectToHostDialog::isIpAddr()
{
    QRegExp rx2("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    if(rx2.exactMatch(ui->lineEdit->text()))
    {
        return true;
    }
    return false;
}
