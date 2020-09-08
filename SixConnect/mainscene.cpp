#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include<QTimer>
#include<QSound>

//按钮音效
QSound *chooseSound =new QSound(":/res/TapButtonSound.wav");

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //固定场景的大小
    this->setFixedSize(1200,800);
    //设置应用图片
    this->setWindowIcon(QPixmap(":/res/logo.png"));
    //设置窗口标题
    this->setWindowTitle("六子棋 SixConnect");
    //退出按键设置
    connect(ui->quitAction,&QAction::triggered,this,&MainScene::close);

    //创建首页面的按钮
    MyPushButton *firstchoosebutton=new MyPushButton(":/res/choose1.png");
    firstchoosebutton->setParent(this);
    firstchoosebutton->move(this->width()*0.13-firstchoosebutton->width()*0.5,this->height()*0.55);
    MyPushButton *secondchoosebutton=new MyPushButton(":/res/choose2.png");
    secondchoosebutton->setParent(this);
    secondchoosebutton->move(this->width()*0.37-secondchoosebutton->width()*0.5,this->height()*0.55);
    MyPushButton *thirdchoosebutton=new MyPushButton(":/res/choose3.png");
    thirdchoosebutton->setParent(this);
    thirdchoosebutton->move(this->width()*0.61-thirdchoosebutton->width()*0.5,this->height()*0.55);
    MyPushButton *fourthchoosebutton=new MyPushButton(":/res/choose4.png");
    fourthchoosebutton->setParent(this);
    fourthchoosebutton->move(this->width()*0.85-fourthchoosebutton->width()*0.5,this->height()*0.55);


    chooseModel=new PlayScene;//建立一个新的游戏场景
    //按钮跳跃特效
    connect(firstchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseModel->game->choose=0;//根据选择模式的不同做出不同选择
        chooseSound->play();
        firstchoosebutton->zoomdown();
        firstchoosebutton->zoomup();
        QTimer::singleShot(400,this,[=](){
            this->hide();
            chooseModel->show();
        });
    });
    connect(secondchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseModel->game->choose=1;
        chooseSound->play();
        secondchoosebutton->zoomdown();
        secondchoosebutton->zoomup();
        QTimer::singleShot(500,this,[=](){
            this->hide();
            chooseModel->show();
        });

    });
    connect(thirdchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseModel->game->choose=2;
        chooseSound->play();
        thirdchoosebutton->zoomdown();
        thirdchoosebutton->zoomup();
        QTimer::singleShot(500,this,[=](){
            this->hide();
            chooseModel->show();
        });
    });
    connect(fourthchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseModel->game->choose=3;
        chooseSound->play();
        fourthchoosebutton->zoomdown();
        fourthchoosebutton->zoomup();
        QTimer::singleShot(400,this,[=](){
            this->hide();
            chooseModel->show();
        });
    });

    //监听playscene的返回按钮
    connect(chooseModel,&PlayScene::goback,this,[=](){
       chooseModel->hide();
       this->show();
       chooseModel->game->startGame(-1);
    });

}


void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    //绘制背景
    pix.load(":/res/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //绘制标题
    pix.load(":/res/title.png");
    pix=pix.scaled(pix.width()*0.4,pix.height()*0.4);
    painter.drawPixmap(this->width()*0.5-0.5*pix.width(),60,pix.width(),pix.height(),pix);

}


MainScene::~MainScene()
{
    delete ui;
}
