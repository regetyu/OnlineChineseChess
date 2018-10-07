#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QList>
#include <QTime>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include "creatdialog.h"
#include "waitdialog.h"
#include "chessman.h"
#include "connecttohostdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startListen(int port);
    void _startListen(int port);
    void _start();
    void __start();
    void ini();
    bool movelegal(int i,int x,int y);
    bool eatlegal(int i,int x,int y);
    void sendmove(int i,int x,int y);
    void sendeat(int i,int x,int y);
    void move(int i,int x,int y);
    void eat(int i,int x,int y);
    void sendlose();
    void lose();
    void win();
    bool judgewin();
    bool judgelose();
    bool judgejiangjun();
    void playjiangjun();
    void readpu();
    void savepu();
    void sendsave();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void on_actioncreat_triggered();
    void acceptConnection();
    void _acceptConnection();
    void receive();
    void t();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionplay_canju_triggered();

    void on_actionconnect_to_server_triggered();

    void on_actionconnect_to_canju_triggered();
    void con();

private:
    Ui::MainWindow *ui;
    QTcpServer *listenSocket;
    QTcpSocket * readwriteSocket;
    WaitDialog *waitDialog;
    bool start;
    bool myturn;
    bool reserve;
    bool isserver;
    _b r_b;
    QList <Chessman> data;
    QTimer *timer;
    QMediaPlayer *player;
};

#endif // MAINWINDOW_H
