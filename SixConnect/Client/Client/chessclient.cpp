#include "chessclient.h"
#include "ui_chessclient.h"
#include"mypushbutton.h"
#include<QPainter>
#include<QPen>
#include <QBrush>                       //设置画刷
#include<QDebug>
#include<QMessageBox>
#include <QHostAddress>
#include<QMenuBar>
#include<QTimer>

chessClient::chessClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chessClient)
{
    ui->setupUi(this);
    //固定窗口大小
    //固定场景的大小
    this->setFixedSize(1200,850);
    //设置应用图片
    this->setWindowIcon(QPixmap(":/reso/logo.png"));
    //设置窗口标题
    this->setWindowTitle("六子棋 SixConnect Client Black");
    //创建菜单栏
    QMenuBar *bar=this->menuBar();
    this->setMenuBar(bar);
    //创建开始菜单
    QMenu *startMenu1=bar->addMenu("开始游戏");
    QMenu *startMenu2=bar->addMenu("了解更多");
    //创建按钮菜单项
    QAction *quitAction1=startMenu1->addAction("规则介绍");
    QAction *quitAction2=startMenu1->addAction("退出游戏");
    QAction *quitAction3=startMenu2->addAction("版本更新");
    QAction *quitAction4=startMenu2->addAction("关于作者");
    //退出按键设置
    connect(quitAction2,&QAction::triggered,this,&chessClient::close);
    connect(quitAction1,&QAction::triggered,this,&chessClient::close);
    connect(quitAction3,&QAction::triggered,this,&chessClient::close);
    connect(quitAction4,&QAction::triggered,this,&chessClient::close);

    //创建返回按钮
    MyPushButton *reconnect=new MyPushButton(":/reso/reconnect.png",":/reso/reconnect2.png");
    reconnect->setParent(this);
    reconnect->move(this->width()*0.97-reconnect->width(),this->height()*0.95-reconnect->height());

    connect(reconnect,&MyPushButton::clicked,this,[=](){
         initchessbord();
        QTimer::singleShot(400,this,[=](){
            //game->choose不需要进行调整，因为此时对象已经消亡了

            //主动和对方断开链接
            tcpSock->disconnectFromHost();
            tcpSock->close();
            tcpSock=NULL;
            tcpSock = new QTcpSocket(this);

            tcpSock->connectToHost(QHostAddress("192.168.137.1"),8888); //改到本机

            connect(tcpSock,SIGNAL(readyRead()),this,SLOT(readyRead_Slots()));
        });
    });


    initchessbord();
    ui->textEdit->setStyleSheet("background-color:transparent");//设置EditLine背景为透明

   //读linedit上的值，然后判断连接是否成功

    tcpSock = new QTcpSocket(this);

    tcpSock->connectToHost(QHostAddress("192.168.137.1"),8888); //改到本机

    connect(tcpSock,SIGNAL(readyRead()),this,SLOT(readyRead_Slots()));
}

chessClient::~chessClient()
{
    delete ui;
}

void chessClient:: paintEvent(QPaintEvent *event)
{
    //创建画家
    QPainter huajia(this);
    QPixmap pix;
    //绘制背景
    pix.load(":/reso/background.png");
    huajia.drawPixmap(0,0,this->width(),this->height(),pix);


    //创建画笔
    QPen  pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);

    //将笔交给画家
    huajia.setPen(pen);

    huajia.drawLine(100,50,860,50);  //框横
    huajia.drawLine(860,50,860,810);   //右竖
    huajia.drawLine(100,50,100,810);//左竖
    huajia.drawLine(100,810,860,810);//下横

#if 1
    //两个画笔放一块会有冲突
    QPainter huajia2(this);
    QPen pointpen;
    pointpen.setWidth(2);
    pointpen.setColor(Qt::black);

    huajia2.setPen(pointpen);
   for(int x=100;x<=N+1 ;x+=40)
   {
     huajia2.drawLine(x,50,x,810);
   }
   for(int y=50;y<=N+1;y+=40)
   {
      huajia2.drawLine(100,y,860,y);
   }
#endif
   //开始画棋盘上的圆
      QPainter huajia3(this);
      QPen Pointpoint;

      huajia3.save();
      huajia3.setPen(Pointpoint);
      QBrush brush(Qt::black);
      huajia3.setBrush(brush);

      Pointpoint.setStyle(Qt::NoPen);  //去掉画笔
      huajia3.restore();

      //刷新数组
  if(x<875&&y<825) //设定是客户端先下棋，然后服务器下棋
  {
         if(flag==1)
          {
              if(flagjudget==0)   //刷新白棋
              {
                  if(1==chessplayer('o',(x-85)/40,(y-35)/40))          //只要是服务器发过来存储为白棋
                  {

                      ui->lineEdit->setText("请点击正确的坐标!");
                  }
                flag=0;
              }
          }
          else if(flag==0)  //黑棋
          {
             if(1==chessplayer('x',(x-85)/40,(y-35)/40))
             {
                ui->lineEdit->setText("请点击正确的坐标!");
             }
             flag=1;                //初始化
             flagjudget=1;
             qDebug()<<"黑棋已经下了，等待对方";
          }

     x=876,y=826;
   }


  //刷新棋局
  for(int i=0;i<M;i++)
  {
      for(int j=0;j<M;j++)
      {
        if(chessbord[i][j]=='o')
        {
            huajia3.save();
            QBrush brush(Qt::white);
            huajia3.setBrush(brush);
            huajia3.drawEllipse(i*40+85,j*40+35,30,30);
            huajia3.restore();
        }
      else  if(chessbord[i][j]=='x')
        {
            huajia3.save();
            QBrush brush(Qt::black);
            huajia3.setBrush(brush);
            huajia3.drawEllipse(i*40+85,j*40+35,30,30);
            huajia3.restore();
        }
      }
  }



}

int chessClient:: chessplayer(char chess,int x,int y)
{
    //'o'代表白色   ，‘x’代表黑色

       if(chessbord[x][y]=='o'||chessbord[x][y]=='x')   //检测下棋是否重复
       {
           return 1;
       }
       if(chess=='o')
       {
         chessbord[x][y]='o';
       }
       else if(chess=='x')
       {
       chessbord[x][y]='x';
       }
       qDebug()<<chess<<endl;
       return 0;

}


 void  chessClient::mousePressEvent(QMouseEvent *event)
 {
     QString str;

    if(event->button()==Qt::LeftButton)
     {
         str = "New-location:" + QString::number((event->x()-85)/40) + "   " + QString::number((event->y()-35)/40);
         ui->lineEdit->setText(str);
         if(flag==1)         //如果对方未下棋，标志位不会改变
         {
             QMessageBox msgBox;
             msgBox.setText("请等待服务器下棋！！");
             msgBox.exec();
             qDebug()<<"等待对方";
         }
         else if(flag==0)    //初始化flag=0；发送自己的坐标
         {
         x=event->x();y=event->y();
         QString str2 = QString::number(x) + "#" + QString::number(y);
         QByteArray arry;
         arry.append(str2);
         tcpSock->write(arry);
         }
        qDebug() <<"New-location:"<<event->x()<<event->y()<<x<<y<<endl;
    }

    update();  //要加重绘事件

 }


 void  chessClient::initchessbord()
 {

     for(int i=0;i<M;i++)            //M此刻是20
         for(int j=0;j<M;j++)
         {
             chessbord[i][j]=' ';
         }

 }

void chessClient::on_pushButton_clicked()
{
    initchessbord();
    x=876;y=826;
    flagjudget=1;
    flag=0;
    update();  //要加重绘事件
}


void chessClient::readyRead_Slots()
{
    QByteArray arry = tcpSock->readAll();
    QString str = QString(arry);
    x = str.section("#",0,0).toInt();
    y = str.section("#",1,1).toInt();
    qDebug() <<"客户端接收到的服务器坐标："<<x<<y<<endl;


    if(x<875&&y<825)
    {
        if(flagjudget==1)
        {
        flagjudget=0;
        }
        qDebug()<<"flagjudget="<<flagjudget;
    }
    int z=0;
    z=str.section("#",2,2).toInt();
    if(z>0)
    {
        QString strjudget;
        if(z==777)
        {
          strjudget="黑棋胜利";
          flag=0;
        }
        else if(z==888)
        {
           strjudget="白棋胜利";
           flag=0;
        }
        else if(z==999)
        {
           strjudget="平局";
           flag=0;
        }
        QMessageBox msgBox;
        msgBox.setText(strjudget);
        msgBox.setInformativeText("Do you want to again?");
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        if(ret>=3000)
        {
             qDebug()<<"ret="<<ret<<endl;
             ui->lineEdit->setText("over!");
        }
        else if(ret<=3000)
        {
            qDebug()<<"ret="<<ret<<endl;
            QString STR1;
            STR1="再来一局";        //比分在此显示
            ui->lineEdit->setText(STR1);
        }
         initchessbord();

    }
    qDebug()<<"x="<<x<<"z="<<z;
    update();
}


