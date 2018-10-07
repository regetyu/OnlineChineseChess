#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    start=false;
    myturn=false;
    reserve=false;
    timer=new QTimer(this);
    player=new QMediaPlayer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(t()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actioncreat_triggered()
{
    CreatDialog dialog(this);
    if(dialog.exec()==QDialog::Accepted)
    {
        startListen(dialog.port());
        isserver=true;
    }
}

void MainWindow::startListen(int port)
{
    qDebug()<<"listen";
    listenSocket = new QTcpServer(this);
    listenSocket->listen(QHostAddress::Any,port);
    connect(listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    waitDialog=new WaitDialog(this);
    if(waitDialog->exec()==QDialog::Rejected)
        delete listenSocket;
}

void MainWindow::_startListen(int port)
{
    qDebug()<<"_listen";
    listenSocket = new QTcpServer(this);
    listenSocket->listen(QHostAddress::Any,port);
    connect(listenSocket,SIGNAL(newConnection()),this,SLOT(_acceptConnection()));
    waitDialog=new WaitDialog(this);
    if(waitDialog->exec()==QDialog::Rejected)
        delete listenSocket;
}

void MainWindow::acceptConnection()
{
    readwriteSocket=listenSocket->nextPendingConnection();
    connect(readwriteSocket,SIGNAL(readyRead()),this,SLOT(receive()));
    qDebug()<<"lianshangle";
    waitDialog->accept();
    _start();
}

void MainWindow::_acceptConnection()
{
    readwriteSocket=listenSocket->nextPendingConnection();
    connect(readwriteSocket,SIGNAL(readyRead()),this,SLOT(receive()));
    qDebug()<<"lianshangle";
    waitDialog->accept();
    __start();
}

void MainWindow::receive()
{
    QString str;
    str=readwriteSocket->readAll();
    qDebug()<<"client receive:"<<str;
    if(str=="start1")
    {
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox *box=new QMessageBox(QMessageBox::Information,"start","Game start! You are red",QMessageBox::Ok,this);
        box->show();
        start=true;
        myturn=true;
        r_b=red;
        ini();
        update();
    }
    else if(str=="start2")
    {
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox *box=new QMessageBox(QMessageBox::Information,"start","Game start!You are black!",QMessageBox::Ok,this);
        box->show();
        start=true;
        myturn=false;
        r_b=black;
        ini();
        update();
    }
    else if(str=="_start1")
    {
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox box(QMessageBox::Information,"start","Game start! You are red",QMessageBox::Ok,this);
        box.exec();
        start=true;
        myturn=true;
        if(reserve)
            myturn=false;
        r_b=red;
        update();
    }
    else if(str=="_start2")
    {
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox box(QMessageBox::Information,"start","Game start!You are  black!",QMessageBox::Ok,this);
        box.exec();
        start=true;
        myturn=false;
        if(reserve)
            myturn=true;
        r_b=black;
        update();
    }
    else if(str[0]=='m')
    {
        move(str.mid(1,2).toInt(),str[3].digitValue(),str[4].digitValue());
        myturn=true;

    }
    else if(str[0]=='e')
    {
        eat(str.mid(1,2).toInt(),str[3].digitValue(),str[4].digitValue());
        myturn=true;
    }
    else if(str=="l")
    {
        win();
    }
    else if(str=="s")
    {
        savepu();
    }
}



void MainWindow::_start()
{
    QByteArray *array=new QByteArray;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(qrand()%2==0)
    {
        array->clear();
        array->append("start1");
        readwriteSocket->write(array->data());
        readwriteSocket->flush();
        qDebug()<<"send start1";
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox *box=new QMessageBox(QMessageBox::Information,"start","Game start!You are black!",QMessageBox::Ok,this);
        box->show();
        start=true;
        myturn=false;
        r_b=black;
        ini();
        update();
    }
    else
    {
        array->clear();
        array->append("start2");
        readwriteSocket->write(array->data());
        readwriteSocket->flush();
        qDebug()<<"send start 2";
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox *box=new QMessageBox(QMessageBox::Information,"start","Game start!You are red!",QMessageBox::Ok,this);
        box->show();
        start=true;
        myturn=true;
        r_b=red;
        ini();
        update();
    }
}

void MainWindow::__start()
{
    QByteArray *array=new QByteArray;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(qrand()%2==0)
    {
        array->clear();
        array->append("_start1");
        readwriteSocket->write(array->data());
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox box(QMessageBox::Information,"start","Game start!You are  black!",QMessageBox::Ok,this);
        box.exec();
        start=true;
        myturn=false;
        if(reserve)
            myturn=true;
        r_b=black;
        update();
    }
    else
    {
        array->clear();
        array->append("_start2");
        readwriteSocket->write(array->data());
        timer->start(1000);
        ui->lcdNumber->display(30);
        QMessageBox box(QMessageBox::Information,"start","Game start! You are red",QMessageBox::Ok,this);
        box.exec();
        start=true;
        myturn=true;
        if(reserve)
            myturn=false;
        r_b=red;
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.translate(200,200);
    QPixmap r_ju("://r_ju.png");
    QPixmap r_ma("://r_ma.png");
    QPixmap r_xiang("://r_xiang.png");
    QPixmap r_shi("://r_shi.png");
    QPixmap r_shuai("://r_jiangshuai.png");
    QPixmap r_bing("://r_bing.png");
    QPixmap r_pao("://r_pao.png");
    QPixmap b_ju("://b_ju.png");
    QPixmap b_ma("://b_ma.png");
    QPixmap b_xiang("://b_xiang.png");
    QPixmap b_shi("://b_shi.png");
    QPixmap b_shuai("://b_jiangshuai.png");
    QPixmap b_bing("://b_zubing.png");
    QPixmap b_pao("://b_pao.png");
    QPixmap background("://background.png");
    background=background.scaled(1090,1090,
                      Qt::KeepAspectRatio);
    painter.drawPixmap(-65,-60,background);
    if(start)
    {
        if(myturn)
            ui->label->setText("Your turn");
        else
            ui->label->setText("Opponent's turn");
        if(r_b==black)
        {
            for(int i=0;i<data.size();i++)
            {
                if(data[i].rb==red)
                {
                    Chessman temp(data[i]);
                    switch(temp.zi)
                    {
                    case ju:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,r_ju);
                        break;
                    case ma:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,r_ma);
                        break;
                    case xiang:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,r_xiang);
                        break;
                    case shi:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,r_shi);
                        break;
                    case shuai:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,r_shuai);
                        break;
                    case pao:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,r_pao);
                        break;
                    case bing:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,r_bing);
                        break;
                    }
                }
                else
                {
                    Chessman temp(data[i]);
                    switch(temp.zi)
                    {
                    case ju:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,b_ju);
                        break;
                    case ma:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,b_ma);
                        break;
                    case xiang:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,b_xiang);
                        break;
                    case shi:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,b_shi);
                        break;
                    case shuai:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,b_shuai);
                        break;
                    case pao:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,b_pao);
                        break;
                    case bing:
                        painter.drawPixmap(800-100*temp.x,100*temp.y,b_bing);
                        break;
                    }
                    if(temp.selected)
                    {
                        //qDebug()<<"xuanzhongle";
                        painter.drawLine(800-100*temp.x,100*temp.y,800-100*temp.x+10,100*temp.y);
                        painter.drawLine(800-100*temp.x,100*temp.y,800-100*temp.x,100*temp.y+10);
                        painter.drawLine(800-100*temp.x+75,100*temp.y,800-100*temp.x+65,100*temp.y);
                        painter.drawLine(800-100*temp.x+75,100*temp.y,800-100*temp.x+75,100*temp.y+10);
                        painter.drawLine(800-100*temp.x,100*temp.y+75,800-100*temp.x+10,100*temp.y+75);
                        painter.drawLine(800-100*temp.x,100*temp.y+75,800-100*temp.x,100*temp.y+65);
                        painter.drawLine(800-100*temp.x+75,100*temp.y+75,800-100*temp.x+65,100*temp.y+75);
                        painter.drawLine(800-100*temp.x+75,100*temp.y+75,800-100*temp.x+75,100*temp.y+65);
                    }
                }
            }
        }
        else
        {
            for(int i=0;i<data.size();i++)
            {
                if(data[i].rb==red)
                {
                    Chessman temp(data[i]);
                    switch(temp.zi)
                    {
                    case ju:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,r_ju);
                        break;
                    case ma:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,r_ma);
                        break;
                    case xiang:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,r_xiang);
                        break;
                    case shi:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,r_shi);
                        break;
                    case shuai:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,r_shuai);
                        break;
                    case pao:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,r_pao);
                        break;
                    case bing:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,r_bing);
                        break;

                    }
                    if(temp.selected)
                    {
                        //qDebug()<<"xuanzhongle";
                        painter.drawLine(100*temp.x,900-100*temp.y,10+100*temp.x,900-100*temp.y);
                        painter.drawLine(100*temp.x,900-100*temp.y,100*temp.x,910-100*temp.y);
                        painter.drawLine(75+100*temp.x,900-100*temp.y,65+100*temp.x,900-100*temp.y);
                        painter.drawLine(75+100*temp.x,900-100*temp.y,75+100*temp.x,910-100*temp.y);
                        painter.drawLine(100*temp.x,975-100*temp.y,10+100*temp.x,975-100*temp.y);
                        painter.drawLine(100*temp.x,975-100*temp.y,100*temp.x,965-100*temp.y);
                        painter.drawLine(75+100*temp.x,975-100*temp.y,65+100*temp.x,975-100*temp.y);
                        painter.drawLine(75+100*temp.x,975-100*temp.y,75+100*temp.x,965-100*temp.y);
                    }
                }
                else
                {
                    Chessman temp(data[i]);
                    switch(temp.zi)
                    {
                    case ju:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,b_ju);
                        break;
                    case ma:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,b_ma);
                        break;
                    case xiang:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,b_xiang);
                        break;
                    case shi:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,b_shi);
                        break;
                    case shuai:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,b_shuai);
                        break;
                    case pao:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,b_pao);
                        break;
                    case bing:
                        painter.drawPixmap(100*temp.x,900-100*temp.y,b_bing);
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::ini()
{
    data.clear();
    data.push_back(Chessman(0,0,ju,red));
    data.push_back(Chessman(1,0,ma,red));
    data.push_back(Chessman(2,0,xiang,red));
    data.push_back(Chessman(3,0,shi,red));
    data.push_back(Chessman(4,0,shuai,red));
    data.push_back(Chessman(5,0,shi,red));
    data.push_back(Chessman(6,0,xiang,red));
    data.push_back(Chessman(7,0,ma,red));
    data.push_back(Chessman(8,0,ju,red));
    data.push_back(Chessman(1,2,pao,red));
    data.push_back(Chessman(7,2,pao,red));
    data.push_back(Chessman(0,3,bing,red));
    data.push_back(Chessman(2,3,bing,red));
    data.push_back(Chessman(4,3,bing,red));
    data.push_back(Chessman(6,3,bing,red));
    data.push_back(Chessman(8,3,bing,red));
    data.push_back(Chessman(0,9,ju,black));
    data.push_back(Chessman(1,9,ma,black));
    data.push_back(Chessman(2,9,xiang,black));
    data.push_back(Chessman(3,9,shi,black));
    data.push_back(Chessman(4,9,shuai,black));
    data.push_back(Chessman(5,9,shi,black));
    data.push_back(Chessman(6,9,xiang,black));
    data.push_back(Chessman(7,9,ma,black));
    data.push_back(Chessman(8,9,ju,black));
    data.push_back(Chessman(1,7,pao,black));
    data.push_back(Chessman(7,7,pao,black));
    data.push_back(Chessman(0,6,bing,black));
    data.push_back(Chessman(2,6,bing,black));
    data.push_back(Chessman(4,6,bing,black));
    data.push_back(Chessman(6,6,bing,black));
    data.push_back(Chessman(8,6,bing,black));
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(myturn)
    {
        if(event->button()==Qt::LeftButton)
        {
            bool flag=false;
            for(int i=0;i<data.size();i++)
            {
                if(data[i].selected)
                {
                    flag=true;
                    break;
                }
            }
            qDebug()<<flag;
            if(flag==false)
            {
                if(r_b==red)
                {
                    for(int i=0;i<data.size();i++)
                        if(QRect(200+data[i].x*100,1100-data[i].y*100,75,75).contains(event->x(),event->y())&&data[i].rb==red)
                        {
                            data[i].selected=true;
                            //qDebug()<<data[i].x<<" "<<data[i].y;
                        }
                }

                else
                {
                    for(int i=0;i<data.size();i++)
                        if(QRect(1000-data[i].x*100,200+data[i].y*100,75,75).contains(event->x(),event->y())&&data[i].rb==black)
                        {
                            data[i].selected=true;
                            //qDebug()<<data[i].x<<" "<<data[i].y;
                        }
                }
            }
            else
            {
                if(r_b==red)
                {
                    int i;
                    bool flag=false;
                    for(i=0;i<data.size();i++)
                        if(QRect(200+data[i].x*100,1100-data[i].y*100,75,75).contains(event->x(),event->y()))
                        {
                            if(data[i].rb==red)
                            {
                                for(int i=0;i<data.size();i++)
                                {
                                    data[i].selected=false;
                                }
                                data[i].selected=true;
                                flag=true;
                                break;
                            }
                            else
                            {
                                int k;
                                for(k=0;k<data.size();k++)
                                {
                                    if(data[k].selected)
                                        break;
                                }
                                qDebug()<<"red eat attempt!";
                                if(eatlegal(k,data[i].x,data[i].y))
                                {
                                    sendeat(k,data[i].x,data[i].y);
                                    eat(k,data[i].x,data[i].y);
                                    update();
                                    return;
                                }
                                flag=true;
                            }
                            break;
                        }
                    if(flag==false)
                    {
                        int k;
                        for(k=0;k<data.size();k++)
                        {
                            if(data[k].selected)
                                break;
                        }
                        qDebug()<<"red move attempt!";
                        int x,y;
                        for(int m=0;m<=8;m++)
                            for(int n=0;n<=9;n++)
                            {
                                if(QRect(200+m*100,1100-n*100,75,75).contains(event->x(),event->y()))
                                {
                                    x=m;
                                    y=n;
                                }
                            }
                        if(movelegal(k,x,y))
                        {
                            sendmove(k,x,y);
                            move(k,x,y);
                            update();
                            return;
                        }
                    }
                }
                else
                {
                    int i;
                    bool flag=false;
                    for(i=0;i<data.size();i++)
                        if(QRect(1000-data[i].x*100,200+data[i].y*100,75,75).contains(event->x(),event->y()))
                        {
                            if(data[i].rb==black)
                            {
                                for(int i=0;i<data.size();i++)
                                {
                                    data[i].selected=false;
                                }
                                data[i].selected=true;
                                flag=true;
                                break;
                            }
                            else
                            {
                                int k;
                                for(k=0;k<data.size();k++)
                                {
                                    if(data[k].selected)
                                        break;
                                }
                                qDebug()<<"black eat attempt!";
                                if(eatlegal(k,data[i].x,data[i].y))
                                {
                                    sendeat(k,data[i].x,data[i].y);
                                    eat(k,data[i].x,data[i].y);
                                    update();
                                    return;
                                }
                                flag=true;
                            }
                            break;
                        }
                    if(flag==false)
                    {
                        int k;
                        for(k=0;k<data.size();k++)
                        {
                            if(data[k].selected)
                                break;
                        }
                        int x,y;
                        for(int m=0;m<=8;m++)
                            for(int n=0;n<=9;n++)
                            {
                                if(QRect(1000-m*100,200+n*100,75,75).contains(event->x(),event->y()))
                                {
                                    x=m;
                                    y=n;
                                }
                            }
                        qDebug()<<"black move attempt!";
                        if(movelegal(k,x,y))
                        {
                            sendmove(k,x,y);
                            move(k,x,y);
                            update();
                            return;
                        }

                    }
                }
            }
        }
    }
    update();
}

bool MainWindow::eatlegal(int i,int x,int y)
{
    Chessman temp=data[i];
    switch(temp.zi)
    {
    case ju:
        if(temp.x==x)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==x&&(data[k].y-y)*(data[k].y-temp.y)<0)
                    return false;
            }
            return true;
        }
        else if(temp.y==y)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].y==y&&(data[k].x-x)*(data[k].x-temp.x)<0)
                    return false;
            }
            return true;
        }
        return false;
        break;
    case ma:
        if(x==temp.x+1&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y+1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x+2&&y==temp.y+1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y)
                    return false;

            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y+1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y)
                    return false;

            }
            return true;
        }
        else if(x==temp.x-1&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y+1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x+1&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y-1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x+2&&y==temp.y-1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y)
                    return false;

            }
            return true;
        }
        else if(x==temp.x-1&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y-1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y-1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y)
                    return false;

            }
            return true;
        }
        return false;
        break;
    case xiang:
        if(temp.rb==red)
        {
            if(y>4)
                return false;
        }
        else
        {
            if(y<5)
                return false;
        }
        if(x==temp.x+2&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y+1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x+2&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y-1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y+1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y+1)
                    return false;

            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y-1)
                    return false;

            }
            return true;
        }
        return false;
        break;
    case shi:
        if(temp.rb==red&&y>2)
            return false;
        if(temp.rb==black&&y<7)
            return false;
        if(x<3||x>5)
            return false;
        if(abs(temp.x-x)==1&&abs(temp.y-y)==1)
            return true;
        return false;
        break;
    case shuai:
        if(temp.rb==red)
        {
            int i;
            for(i=0;i<data.size();i++)
                if(data[i].rb==black&&data[i].zi==shuai)
                    break;
            if(x==data[i].x&&y==data[i].y&&x==temp.x)
            {
               bool flag=true;
               for(int k=0;k<data.size();k++)
                   if(data[k].x==x&&data[k].y>temp.y&&data[k].y<data[i].y)
                       flag=false;
               if(flag==true)
                   return true;
            }
        }
        if(temp.rb==black)
        {
            int i;
            for(i=0;i<data.size();i++)
                if(data[i].rb==red&&data[i].zi==shuai)
                    break;
            if(x==data[i].x&&y==data[i].y&&x==temp.x)
            {
               bool flag=true;
               for(int k=0;k<data.size();k++)
                   if(data[k].x==x&&data[k].y>data[i].y&&data[k].y<temp.y)
                       flag=false;
               if(flag==true)
                   return true;
            }
        }
        if(temp.rb==red&&y>2)
            return false;
        if(temp.rb==black&&y<7)
            return false;
        if(x<3||x>5)
            return false;
        if(abs(temp.x-x)+abs(temp.y-y)==1)
            return true;
        return false;
        break;
    case pao:
        if(temp.x==x)
        {
            int count=0;
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==x&&(data[k].y-y)*(data[k].y-temp.y)<0)
                    count++;
            }
            if(count==1)
                return true;
            return false;
        }
        else if(temp.y==y)
        {
            int count=0;
            for(int k=0;k<data.size();k++)
            {
                if(data[k].y==y&&(data[k].x-x)*(data[k].x-temp.x)<0)
                    count++;
            }
            if(count==1)
                return true;
            return false;
        }
        return false;
        break;
    case bing:
        if(temp.rb==red)
        {
            if(temp.y<=4)
            {
                if(x==temp.x&&y==temp.y+1)
                    return true;
                return false;
            }
            else
            {
                if(x==temp.x&&y==temp.y+1)
                    return true;
                else if(x==temp.x-1&&y==temp.y)
                    return true;
                else if(x==temp.x+1&&y==temp.y)
                    return true;
            }
            return false;
        }
        else
        {
            if(temp.y>=5)
            {
                if(x==temp.x&&y==temp.y-1)
                    return true;
                return false;
            }
            else
            {
                if(x==temp.x&&y==temp.y-1)
                    return true;
                else if(x==temp.x-1&&y==temp.y)
                    return true;
                else if(x==temp.x+1&&y==temp.y)
                    return true;
            }
            return false;
        }
        break;
    }
}

bool MainWindow::movelegal(int i,int x,int y)
{
    Chessman temp=data[i];
    switch(temp.zi)
    {
    case pao:
    case ju:
        if(temp.x==x)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==x&&(data[k].y-y)*(data[k].y-temp.y)<0)
                    return false;
            }
            return true;
        }
        else if(temp.y==y)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].y==y&&(data[k].x-x)*(data[k].x-temp.x)<0)
                    return false;
            }
            return true;
        }
        return false;
        break;
    case ma:
        if(x==temp.x+1&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y+1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x+2&&y==temp.y+1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y)
                    return false;
            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y+1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y)
                    return false;
            }
            return true;
        }
        else if(x==temp.x-1&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y+1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x+1&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y-1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x+2&&y==temp.y-1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y)
                    return false;
            }
            return true;
        }
        else if(x==temp.x-1&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x&&data[k].y==temp.y-1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y-1)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y)
                    return false;
            }
            return true;
        }
        return false;
        break;
    case xiang:
        if(temp.rb==red)
        {
            if(y>4)
                return false;
        }
        else
        {
            if(y<5)
                return false;
        }
        if(x==temp.x+2&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y+1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x+2&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x+1&&data[k].y==temp.y-1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y+1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y+2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y+1)
                    return false;
            }
            return true;
        }
        else if(x==temp.x-2&&y==temp.y-2)
        {
            for(int k=0;k<data.size();k++)
            {
                if(data[k].x==temp.x-1&&data[k].y==temp.y-1)
                    return false;
            }
            return true;
        }
        return false;
        break;
    case shi:
        if(temp.rb==red&&y>2)
            return false;
        if(temp.rb==black&&y<7)
            return false;
        if(x<3||x>5)
            return false;
        if(abs(temp.x-x)==1&&abs(temp.y-y)==1)
            return true;
        return false;
        break;
    case shuai:
        if(temp.rb==red&&y>2)
            return false;
        if(temp.rb==black&&y<7)
            return false;
        if(x<3||x>5)
            return false;
        if(abs(temp.x-x)+abs(temp.y-y)==1)
            return true;
        return false;
        break;
    case bing:
        if(temp.rb==red)
        {
            if(temp.y<=4)
            {
                if(x==temp.x&&y==temp.y+1)
                    return true;
                return false;
            }
            else
            {
                if(x==temp.x&&y==temp.y+1)
                    return true;
                else if(x==temp.x-1&&y==temp.y)
                    return true;
                else if(x==temp.x+1&&y==temp.y)
                    return true;
            }
            return false;
        }
        else
        {
            if(temp.y>=5)
            {
                if(x==temp.x&&y==temp.y-1)
                    return true;
                return false;
            }
            else
            {
                if(x==temp.x&&y==temp.y-1)
                    return true;
                else if(x==temp.x-1&&y==temp.y)
                    return true;
                else if(x==temp.x+1&&y==temp.y)
                    return true;
            }
            return false;
        }
        break;
    }
}

void MainWindow::sendmove(int x,int y, int z)
{
    myturn=false;
    QString temp;
    if(x<10)
        temp="0"+QString::number(x);
    else
        temp=QString::number(x);
    QByteArray *array=new QByteArray;
    array->clear();
    array->append("m"+temp+QString::number(y)+QString::number(z));
    readwriteSocket->write(array->data());
    qDebug()<<"sendmove"<<"m"+temp+QString::number(y)+QString::number(z);
}

void MainWindow::sendeat(int x,int y,int z)
{
    myturn=false;
    QString temp;
    if(x<10)
        temp="0"+QString::number(x);
    else
        temp=QString::number(x);
    QByteArray *array=new QByteArray;
    array->clear();
    array->append("e"+temp+QString::number(y)+QString::number(z));
    readwriteSocket->write(array->data());
    qDebug()<<"sendeat"<<"e"+temp+QString::number(y)+QString::number(z);
}

void MainWindow::move(int i,int x,int y)
{
    data[i].x=x;
    data[i].y=y;
    for(int i=0;i<data.size();i++)
    {
        data[i].selected=false;
    }
    timer->stop();
    timer->start(1000);
    ui->lcdNumber->display(30);
    if(judgejiangjun())
        playjiangjun();
    update();
}

void MainWindow::eat(int i,int x,int y)
{
    data[i].x=x;
    data[i].y=y;
    int k;
    for(k=0;k<data.size();k++)
    {
        if(data[k].x==x&&data[k].y==y&&k!=i)
            break;
    }
    data.removeAt(k);
    for(int i=0;i<data.size();i++)
    {
        data[i].selected=false;
    }
    if(judgewin())
    {
        win();
        return;
    }
    if(judgelose())
    {
        lose();
        return;
    }
    timer->stop();
    timer->start(1000);
    ui->lcdNumber->display(30);
    if(judgejiangjun())
        playjiangjun();
    update();

}


void MainWindow::sendlose()
{
    QByteArray *array=new QByteArray;
    array->clear();
    array->append('l');
    readwriteSocket->write(array->data());
}

void MainWindow::win()
{
    timer->stop();
    ui->lcdNumber->display(30);
    QMessageBox box(QMessageBox::Information,"game ends","You win!",QMessageBox::Ok,this);
    box.exec();
    myturn=false;
}

void MainWindow::lose()
{
    timer->stop();
    ui->lcdNumber->display(30);
    QMessageBox box(QMessageBox::Information,"game ends","You lose!",QMessageBox::Ok,this);
    box.exec();
    myturn=false;
    timer->stop();
    ui->label->setText("30");
}

bool MainWindow::judgewin()
{
    for(int i=0;i<data.size();i++)
    {
        if(data[i].zi==shuai&&data[i].rb!=r_b)
            return false;
    }
     return true;
}

bool MainWindow::judgelose()
{
    for(int i=0;i<data.size();i++)
    {
        if(data[i].zi==shuai&&data[i].rb==r_b)
            return false;
    }
     return true;
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray *array=new QByteArray;
    array->clear();
    array->append('l');
    readwriteSocket->write(array->data());
    lose();
}

void MainWindow::t()
{
    int time=ui->lcdNumber->intValue();
    if(time==0)
    {
        if(myturn)
        {
            sendlose();
            lose();
        }
    }
    else
    {
        time--;
        ui->lcdNumber->display(time);
    }
}

bool MainWindow::judgejiangjun()
{
    int i1,i2;
    for(i1=0;i1<data.size();i1++)
    {
        if(data[i1].zi==shuai&&data[i1].rb==red)
            break;
    }
    for(i2=0;i2<data.size();i2++)
    {
        if(data[i2].zi==shuai&&data[i2].rb==black)
            break;
    }
    if(i1==data.size()||i2==data.size())
        return false;
    for(int i=0;i<data.size();i++)
    {
        if(data[i].rb==black)
            if(eatlegal(i,data[i1].x,data[i1].y))
                return true;
    }
    for(int i=0;i<data.size();i++)
    {
        if(data[i].rb==red)
            if(eatlegal(i,data[i2].x,data[i2].y))
                return true;
    }
    return false;
}

void MainWindow::playjiangjun()
{
    qDebug()<<"jiangjun";
    player->setMedia(QUrl("qrc:/new/prefix1/Alarm01.wav"));
    qDebug()<<player->currentMedia().isNull();
    player->setVolume(50);
    player->play();
}

void MainWindow::readpu()
{
    QString filename=QFileDialog::getOpenFileName(this,"open","","(*.txt)");
    if(filename!="")
    {
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream str(&file);
            QString s=str.readLine();
            if(s=="red")
            {
                data.clear();
                s=str.readLine();
                int i;
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shuai,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shi,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),xiang,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ma,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ju,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),pao,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),bing,red));
                //black
                str.readLine();
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shuai,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shi,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),xiang,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ma,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ju,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),pao,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),bing,black));
            }
            else
            {
                reserve=true;
                int i;
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shuai,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shi,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),xiang,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ma,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ju,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),pao,black));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),bing,black));
                //red
                str.readLine();
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shuai,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),shi,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),xiang,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ma,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),ju,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),pao,red));
                s=str.readLine();
                for(i=0;i<s.size();i++)
                    if(s[i]==",")
                        data.push_back(Chessman(s[i-1].digitValue(),s[i+1].digitValue(),bing,red));
            }
        }
    }
}

void MainWindow::savepu()
{
    QList <Chessman> red_shuai;
    QList <Chessman> red_shi;
    QList <Chessman> red_xiang;
    QList <Chessman> red_ma;
    QList <Chessman> red_ju;
    QList <Chessman> red_pao;
    QList <Chessman> red_bing;
    QList <Chessman> black_shuai;
    QList <Chessman> black_shi;
    QList <Chessman> black_xiang;
    QList <Chessman> black_ma;
    QList <Chessman> black_ju;
    QList <Chessman> black_pao;
    QList <Chessman> black_bing;
    for(int i=0;i<data.size();i++)
    {
        Chessman temp=data[i];
        if(temp.zi==shuai&&temp.rb==red)
            red_shuai.push_back(temp);
        else if(temp.zi==shi&&temp.rb==red)
            red_shi.push_back(temp);
        else if(temp.zi==xiang&&temp.rb==red)
            red_xiang.push_back(temp);
        else if(temp.zi==ma&&temp.rb==red)
            red_ma.push_back(temp);
        else if(temp.zi==ju&&temp.rb==red)
            red_ju.push_back(temp);
        else if(temp.zi==pao&&temp.rb==red)
            red_pao.push_back(temp);
        else if(temp.zi==bing&&temp.rb==red)
            red_bing.push_back(temp);
        else if(temp.zi==shuai&&temp.rb==black)
            black_shuai.push_back(temp);
        else if(temp.zi==shi&&temp.rb==black)
            black_shi.push_back(temp);
        else if(temp.zi==xiang&&temp.rb==black)
            black_xiang.push_back(temp);
        else if(temp.zi==ma&&temp.rb==black)
            black_ma.push_back(temp);
        else if(temp.zi==ju&&temp.rb==black)
            black_ju.push_back(temp);
        else if(temp.zi==pao&&temp.rb==black)
            black_pao.push_back(temp);
        else if(temp.zi==bing&&temp.rb==black)
            black_bing.push_back(temp);
    }
    QString filename=QFileDialog::getSaveFileName(this,"save","","(*.txt)");
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream str(&file);
        if((myturn==true&&r_b==red)||(myturn==false&&r_b==black))
        {
            str<<"red"<<endl;
            str<<red_shuai.size()<<" ";
            for(int i=0;i<red_shuai.size();i++)
            {
                str<<"<"<<red_shuai[i].x<<","<<red_shuai[i].y<<">";
            }
            str<<endl;
            str<<red_shi.size()<<" ";
            for(int i=0;i<red_shi.size();i++)
            {
                str<<"<"<<red_shi[i].x<<","<<red_shi[i].y<<">";
            }
            str<<endl;
            str<<red_xiang.size()<<" ";
            for(int i=0;i<red_xiang.size();i++)
            {
                str<<"<"<<red_xiang[i].x<<","<<red_xiang[i].y<<">";
            }
            str<<endl;
            str<<red_ma.size()<<" ";
            for(int i=0;i<red_ma.size();i++)
            {
                str<<"<"<<red_ma[i].x<<","<<red_ma[i].y<<">";
            }
            str<<endl;
            str<<red_ju.size()<<" ";
            for(int i=0;i<red_ju.size();i++)
            {
                str<<"<"<<red_ju[i].x<<","<<red_ju[i].y<<">";
            }
            str<<endl;
            str<<red_pao.size()<<" ";
            for(int i=0;i<red_pao.size();i++)
            {
                str<<"<"<<red_pao[i].x<<","<<red_pao[i].y<<">";
            }
            str<<endl;
            str<<red_bing.size()<<" ";
            for(int i=0;i<red_bing.size();i++)
            {
                str<<"<"<<red_bing[i].x<<","<<red_bing[i].y<<">";
            }
            str<<endl;
            str<<"black"<<endl;
            str<<black_shuai.size()<<" ";
            for(int i=0;i<black_shuai.size();i++)
            {
                str<<"<"<<black_shuai[i].x<<","<<black_shuai[i].y<<">";
            }
            str<<endl;
            str<<black_shi.size()<<" ";
            for(int i=0;i<black_shi.size();i++)
            {
                str<<"<"<<black_shi[i].x<<","<<black_shi[i].y<<">";
            }
            str<<endl;
            str<<black_xiang.size()<<" ";
            for(int i=0;i<black_xiang.size();i++)
            {
                str<<"<"<<black_xiang[i].x<<","<<black_xiang[i].y<<">";
            }
            str<<endl;
            str<<black_ma.size()<<" ";
            for(int i=0;i<black_ma.size();i++)
            {
                str<<"<"<<black_ma[i].x<<","<<black_ma[i].y<<">";
            }
            str<<endl;
            str<<black_ju.size()<<" ";
            for(int i=0;i<black_ju.size();i++)
            {
                str<<"<"<<black_ju[i].x<<","<<black_ju[i].y<<">";
            }
            str<<endl;
            str<<black_pao.size()<<" ";
            for(int i=0;i<black_pao.size();i++)
            {
                str<<"<"<<black_pao[i].x<<","<<black_pao[i].y<<">";
            }
            str<<endl;
            str<<black_bing.size()<<" ";
            for(int i=0;i<black_bing.size();i++)
            {
                str<<"<"<<black_bing[i].x<<","<<black_bing[i].y<<">";
            }
            str<<endl;
        }
        else
        {
            str<<"black"<<endl;
            str<<black_shuai.size()<<" ";
            for(int i=0;i<black_shuai.size();i++)
            {
                str<<"<"<<black_shuai[i].x<<","<<black_shuai[i].y<<">";
            }
            str<<endl;
            str<<black_shi.size()<<" ";
            for(int i=0;i<black_shi.size();i++)
            {
                str<<"<"<<black_shi[i].x<<","<<black_shi[i].y<<">";
            }
            str<<endl;
            str<<black_xiang.size()<<" ";
            for(int i=0;i<black_xiang.size();i++)
            {
                str<<"<"<<black_xiang[i].x<<","<<black_xiang[i].y<<">";
            }
            str<<endl;
            str<<black_ma.size()<<" ";
            for(int i=0;i<black_ma.size();i++)
            {
                str<<"<"<<black_ma[i].x<<","<<black_ma[i].y<<">";
            }
            str<<endl;
            str<<black_ju.size()<<" ";
            for(int i=0;i<black_ju.size();i++)
            {
                str<<"<"<<black_ju[i].x<<","<<black_ju[i].y<<">";
            }
            str<<endl;
            str<<black_pao.size()<<" ";
            for(int i=0;i<black_pao.size();i++)
            {
                str<<"<"<<black_pao[i].x<<","<<black_pao[i].y<<">";
            }
            str<<endl;
            str<<black_bing.size()<<" ";
            for(int i=0;i<black_bing.size();i++)
            {
                str<<"<"<<black_bing[i].x<<","<<black_bing[i].y<<">";
            }
            str<<endl;
            str<<"red"<<endl;
            str<<red_shuai.size()<<" ";
            for(int i=0;i<red_shuai.size();i++)
            {
                str<<"<"<<red_shuai[i].x<<","<<red_shuai[i].y<<">";
            }
            str<<endl;
            str<<red_shi.size()<<" ";
            for(int i=0;i<red_shi.size();i++)
            {
                str<<"<"<<red_shi[i].x<<","<<red_shi[i].y<<">";
            }
            str<<endl;
            str<<red_xiang.size()<<" ";
            for(int i=0;i<red_xiang.size();i++)
            {
                str<<"<"<<red_xiang[i].x<<","<<red_xiang[i].y<<">";
            }
            str<<endl;
            str<<red_ma.size()<<" ";
            for(int i=0;i<red_ma.size();i++)
            {
                str<<"<"<<red_ma[i].x<<","<<red_ma[i].y<<">";
            }
            str<<endl;
            str<<red_ju.size()<<" ";
            for(int i=0;i<red_ju.size();i++)
            {
                str<<"<"<<red_ju[i].x<<","<<red_ju[i].y<<">";
            }
            str<<endl;
            str<<red_pao.size()<<" ";
            for(int i=0;i<red_pao.size();i++)
            {
                str<<"<"<<red_pao[i].x<<","<<red_pao[i].y<<">";
            }
            str<<endl;
            str<<red_bing.size()<<" ";
            for(int i=0;i<red_bing.size();i++)
            {
                str<<"<"<<red_bing[i].x<<","<<red_bing[i].y<<">";
            }
            str<<endl;
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    savepu();
    sendsave();
}

void MainWindow::on_actionplay_canju_triggered()
{
    readpu();
    CreatDialog dialog(this);
    if(dialog.exec()==QDialog::Accepted)
    {
        _startListen(dialog.port());
        isserver=true;
    }
}
void MainWindow::sendsave()
{
    qDebug()<<"sendsave";
    QByteArray *array=new QByteArray;
    array->clear();
    array->append("s");
    readwriteSocket->write(array->data());
}

void MainWindow::on_actionconnect_to_server_triggered()
{
    ConnectToHostDialog dialog(this);
    if(dialog.exec()==QDialog::Accepted)
    {
        if(dialog.isIpAddr())
        {
            readwriteSocket=new QTcpSocket(this);
            connect(readwriteSocket,SIGNAL(readyRead()),this,SLOT(receive()));
            readwriteSocket->connectToHost(dialog.address(),dialog.port());
            isserver=false;
        }
        else
        {
            QMessageBox box(QMessageBox::Critical,"Error","invalid IP address!",QMessageBox::Ok,this);
            box.exec();
        }
    }
}

void MainWindow::on_actionconnect_to_canju_triggered()
{
    readpu();
    ConnectToHostDialog dialog(this);
    if(dialog.exec()==QDialog::Accepted)
    {
        if(dialog.isIpAddr())
        {
            readwriteSocket=new QTcpSocket(this);
            connect(readwriteSocket,SIGNAL(readyRead()),this,SLOT(receive()));
            readwriteSocket->connectToHost(dialog.address(),dialog.port());
            isserver=false;
        }
        else
        {
            QMessageBox box(QMessageBox::Critical,"Error","invalid IP address!",QMessageBox::Ok,this);
            box.exec();
        }
    }
}

void MainWindow::con()
{
    QMessageBox *a=new QMessageBox(QMessageBox::Information,"1","1");
    a->show();
    connect(readwriteSocket,SIGNAL(readyRead()),this,SLOT(receive()));
}
