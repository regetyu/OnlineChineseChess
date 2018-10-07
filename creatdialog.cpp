#include "creatdialog.h"
#include "ui_creatdialog.h"

CreatDialog::CreatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatDialog)
{
    ui->setupUi(this);
    QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());
    address=info.addresses().last();
    ui->lineEdit->setText(address.toString());
}

CreatDialog::~CreatDialog()
{
    delete ui;
}


int CreatDialog::port()
{
    return ui->spinBox->value();
}
