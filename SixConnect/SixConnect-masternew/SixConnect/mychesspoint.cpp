#include "mychesspoint.h"
#include "ui_mychesspoint.h"
#include<QPainter>
#include<QPen>
#include <QBrush>
#include<QDebug>
#include<QMessageBox>
#include<QMenuBar>
#include <QHostAddress>
#include"mypushbutton.h"
#include<QTimer>
#include<QSound>

QSound *chessSound1=new QSound(":/res/chessone.wav");
QSound *backSound1=new QSound(":/res/BackButtonSound.wav");

mychesspoint::mychesspoint(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mychesspoint)
{
    ui->setupUi(this);
    //固定窗口大小
    //固定场景的大小
    this->setFixedSize(1200,850);
    //设置应用图片
    this->setWindowIcon(QPixmap(":/res/logo.png"));
    //设置窗口标题
    this->setWindowTitle("六子棋 SixConnect");
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
    connect(quitAction2,&QAction::triggered,this,&mychesspoint::close);
    connect(quitAction1,&QAction::triggered,this,&mychesspoint::close);
    connect(quitAction3,&QAction::triggered,this,&mychesspoint::close);
    connect(quitAction4,&QAction::triggered,this,&mychesspoint::close);

    initchessbord();   //初始化数组
    ui->textEdit->setStyleSheet("background-color:transparent");

    //创建返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/back2.png",":/res/back.png");
    backBtn->setParent(this);
    backBtn->move(this->width()*0.97-backBtn->width(),this->height()*0.95-backBtn->height());

    //异步交互按钮
    MyPushButton *tempBackBtn=new MyPushButton(":/res/tempback2.png",":/res/tempback.png");
    tempBackBtn->setParent(this);
    tempBackBtn->move(this->width()*0.97-tempBackBtn->width(),this->height()*0.85-tempBackBtn->height());

    tcpSever=NULL;
    tcpSock=NULL;

    //监听选择关卡按钮的信号槽
    connect(backBtn,&MyPushButton::clicked,this,[=](){
        initchessbord();   //初始化数组
        backSound1->play();
        processing=0;

        if(tcpSock==NULL)
        {
            qDebug()<<tcpSock;
        }
        //主动断开链接
        if(tcpSock){
            qDebug()<<tcpSock;
            tcpSock->disconnectFromHost();
            tcpSock->close();

        }

        QTimer::singleShot(400,this,[=](){
            emit this->goback();
        });
    });

    connect(tempBackBtn,&MyPushButton::clicked,this,[=](){
        backSound1->play();
        processing=1;
        QTimer::singleShot(400,this,[=](){
            emit this->goback();

        });
    });


    //实例化套接字对象
    tcpSever = new QTcpServer(this);
    //监听
    tcpSever->listen(QHostAddress::AnyIPv4,8888);
    //等待连接
    connect(tcpSever,SIGNAL(newConnection()),this,SLOT(newConnection_Slots()));



}


mychesspoint::~mychesspoint()
{
    delete ui;
}

void mychesspoint:: paintEvent(QPaintEvent *event)
{

    //创建画家
    QPainter huajia(this);
    QPixmap pix;
    //绘制背景
    pix.load(":/res/background.png");
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
    if(x<875&&y<825)          //设定是客户端先下棋，然后服务器下棋
    {
        if(flag==1)  //   flag初始化为1,更新对方下的棋,服务器下白棋,客户端下黑棋
        {
            if(flagjudget==0)
            {
                if(1==chessplayer('x',(x-85)/40,(y-35)/40))         //检测坐标是否重复，重复则不会进入数组
                {
                    ui->lineEditzuobiao->setText("请点击正确的坐标!");
                }
                flag=0;
            }
        }
        else if(flag==0)
        {
            if(1==chessplayer('o',(x-85)/40,(y-35)/40))
            {
                ui->lineEditzuobiao->setText("请点击正确的坐标!");
            }
            flag=1;
            flagjudget=1;
            qDebug()<<"服务器棋已经下了，等待对方:白棋坐标"<<x<<y;
        }
    }

    //刷新棋局绘图
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
            if(chessbord[i][j]=='x')
            {
                huajia3.save();
                QBrush brush(Qt::black);
                huajia3.setBrush(brush);
                huajia3.drawEllipse(i*40+85,j*40+35,30,30);
                huajia3.restore();
            }
        }
    }


    //判断输赢
    if(x<875&&y<825)
    {
        QString str2 ;
        if(1==checkchess('x',(x-85)/40,(y-35)/40)||1==checkchess('o',(x-85)/40,(y-35)/40))  //等于-1说明棋盘中还有棋子
        {
            QString str;
            if(1==checkchess('x',(x-85)/40,(y-35)/40))
            {
                str= "黑棋获胜！";
                str2 = "#777#777";
            }
            else if(1==checkchess('o',(x-85)/40,(y-35)/40))
            {
                str= "白棋获胜！";
                str2 = "#888#888";
            }
            //和棋
            else if(0==checkchess('x',(x-85)/40,(y-35)/40))
            {
                str= "平棋！";
                str2 = "#888#888";
            }

            QByteArray arry;     //发送输赢消息,此消息会加载到坐标消息后边
            arry.append(str2);
            tcpSock->write(arry);

            x=876;y=826;
            ui->lineEditzuobiao->setText(str);
            QMessageBox msgBox;
            msgBox.setText(str);
            msgBox.setInformativeText("Do you want to again?");
            msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Ok );
            msgBox.setDefaultButton(QMessageBox::Save);
            int ret = msgBox.exec();

            if(ret>=3000)
            {
                qDebug()<<"ret="<<ret<<endl;
                emit mySignal();   //返回主页面
            }
            else if(ret<=3000)
            {
                qDebug()<<"ret="<<ret<<endl;
                QString STR1;
                STR1="再来一局";
                ui->lineEditzuobiao->setText(STR1);
            }
            initchessbord();
        }
    }
}



void mychesspoint:: mousePressEvent(QMouseEvent *event)
{
    for(int p=0;p<20;p++)
    {
        for(int q=0;q<20;q++){
            qDebug()<<chessbord[p][q];
        }
    }
    QString str;
    if(event->button()==Qt::LeftButton)
    {
        str = "New-location:" + QString::number((event->x()-85)/40) + "   " + QString::number((event->y()-35)/40);
        ui->lineEditzuobiao->setText(str);
        if(firstStep==1){
            firstStep=0;
            chessbord[7][10]='o';
            update();
        }
        else if(flag==1)          //如果对方未下棋，标志位不会改变
        {
            QMessageBox msgBox;
            msgBox.setText("请等待客户端连接网络或下棋！！");
            msgBox.exec();
            qDebug()<<"等待对方";
        }
        else if(flag==0)
        {
            chessSound1->play();
            x=event->x();y=event->y();
            QString str = QString::number(x) + "#" + QString::number(y);
            QByteArray arry;
            arry.append(str);
            tcpSock->write(arry);
        }
        qDebug() <<"New-location:"<<event->x()<<event->y()<<endl;
    }
    update();  //要加重绘事件

}



void mychesspoint::readyRead_Slots()    //jieshou
{
    QByteArray arry = tcpSock->readAll();
    QString str = QString(arry);


    x=str.section("#",0,0).toInt(); //#前坐标  接收对方坐标
    y=str.section("#",1,1).toInt();

    qDebug()<<x<<y;

    if(x<875&&y<825)                    //如果对方发送了有效坐标，
    {
        if(flagjudget==1)
        {
            flagjudget=0;

        }
        qDebug()<<"服务器："<<flagjudget;
    }
    update();
}



void mychesspoint:: initchessbord()
{
    for(int i=0;i<M;i++)            //M为20
        for(int j=0;j<M;j++)
        {
            chessbord[i][j]=' ';
        }
}


int mychesspoint::chessplayer(char chess,int x,int y)   //进入数组
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
    if(chess=='x')
    {
        chessbord[x][y]='x';
    }
    qDebug()<<chess<<endl;
    return 0;
}


void mychesspoint::newConnection_Slots()
{
    //建立连接，产生新的套接字
    tcpSock = tcpSever->nextPendingConnection();
    //
    connect(tcpSock,SIGNAL(readyRead()),this,SLOT(readyRead_Slots()));
}


int mychesspoint::checkchess(char flag,int i,int j)
{
    int N=M;
    int x = i, y = j;
    int begin = 0;
    int end = 0;

    int begin1 = 0;
    int end1 = 0;

    //判断列是否满足条件
    (i - 5) > 0 ? begin = i - 5 : begin = 1; //已经将列减4 或者从边界算
    (i + 5) < N ? end = N : end = (i + 5);


    for (i = begin; i + 5 <= end; i++)
    {
        if (chessbord[i][j] == flag&&chessbord[i + 1][j] == flag&&chessbord[i + 2][j] == flag&&chessbord[i + 3][j] == flag&&chessbord[i + 4][j] == flag&&chessbord[i + 5][j] == flag)
        {
            return 1;
        }
    }

    //判断行是否满足条件
    i = x, j = y;
    (j - 5) > 0 ? begin = j - 5 : begin = 1;  //已经将横坐标减4
    (j + 5) < N ? end = N : end = (j + 5); //如果末尾大于N则，从N算最后一个


    for (j = begin; j + 5 <= end; j++)
    {
        if (chessbord[i][j] == flag&&chessbord[i][j + 1] == flag&&chessbord[i][j + 2] == flag&&chessbord[i][j + 3] == flag&&chessbord[i][j + 4] == flag&&chessbord[i][j + 5] == flag)
        {
            return 1;
        }
    }


    //判断正对角线是否满足
    int len = 0;
    i = x, j = y;
    i >j ? len = j - 1 : len = i - 1;   //判断i>j的作用，按照离边距小的坐标减做初始值
    if (len > 5)
        len = 5;
    begin = i - len;       //横坐标的起始位置
    begin1 = j - len;      //纵坐标，，因为是正斜边所以两边要减相同数


    i > j ? len = (N - i) : len = (N - j);   //结束坐标也要按照距离边界近的点去减
    if (len>5)
        len = 5;
    end= i + len;       //横坐标的结束位置
    end1 = j + len;      //纵坐标的结束位置


    for (int i = begin, j = begin1; (i + 5 < end) && (j + 5 < end1); i++, j++)
    {
        if (chessbord[i][j] == flag&&chessbord[i + 1][j + 1] == flag&&chessbord[i + 2][j + 2] == flag&&chessbord[i + 3][j + 3] == flag&&chessbord[i + 4][j + 4] == flag&&chessbord[i + 5][j + 5] == flag)
        {
            return 1;
        }
    }
    //补充代码  // '\'
    for (int i = end, j = end1; (i - 5 > begin) && (j - 5 > begin1); i--, j--)
    {
        if (chessbord[i][j] == flag&&chessbord[i - 1][j - 1] == flag&&chessbord[i - 2][j - 2] == flag&&chessbord[i - 3][j - 3] == flag&&chessbord[i - 4][j - 4] == flag&&chessbord[i - 5][j - 5] == flag)
        {
            return 1;
        }
    }


    //判断负对角线是否满足
    i = x, j = y;
    (i - 1) >(N - j) ? len = (N - j) : len = i - 1;  //判断负对角线坐标的位置是否在上下减小的距离
    if (len > 5)
        len = 5;
    begin = i - len;       //横坐标的起始位置
    begin1 = j + len;      //纵坐标的起始位置


    (N - i) > (j - 1) ? len = (j - 1) : len = (N - i);
    if (len>5)
        len = 5;
    end = i + len;       //横坐标的结束位置
    end1 = j - len;      //纵坐标的结束位置


    for (int i = begin, j = begin1; (i + 5 < end) && (j - 5 > end1); i++, j--)
    {
        if (chessbord[i][j] == flag&&chessbord[i + 1][j - 1] == flag&&chessbord[i + 2][j - 2] == flag&&chessbord[i + 3][j - 3] == flag&&chessbord[i + 4][j - 4] == flag&&chessbord[i + 5][j - 5] == flag)
        {
            return 1;
        }
    }
    //补充代码
    for (int i = end, j = end1; (i - 5 > begin) && (j + 5 < begin1); i--, j++)
    {
        if (chessbord[i][j] == flag&&chessbord[i - 1][j +1] == flag&&chessbord[i - 2][j + 2] == flag&&chessbord[i - 3][j + 3] == flag&&chessbord[i - 4][j + 4] == flag&&chessbord[i - 5][j +5] == flag)
        {
            return 1;
        }
    }


    //判断棋盘是否下满
    for (int z = 1; z < N + 1; z++)           //棋盘有没有下满
    {
        for (int l = 1; l < N + 1; l++)
        {
            if (chessbord[z][l] == ' ')
                return -1;      //说明棋盘中还有空格
        }
    }
    return 0;

}
