#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
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

    //按钮音效
    QSound *chooseSound =new QSound(":/res/TapButtonSound.wav",this);


    //按钮跳跃特效
    connect(firstchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseSound->play();
        firstchoosebutton->zoomdown();
        firstchoosebutton->zoomup();
    });
    connect(secondchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseSound->play();
        secondchoosebutton->zoomdown();
        secondchoosebutton->zoomup();
    });
    connect(thirdchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseSound->play();
        thirdchoosebutton->zoomdown();
        thirdchoosebutton->zoomup();
    });
    connect(fourthchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseSound->play();
        fourthchoosebutton->zoomdown();
        fourthchoosebutton->zoomup();
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
