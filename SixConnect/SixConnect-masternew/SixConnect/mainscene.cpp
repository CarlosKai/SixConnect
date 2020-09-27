#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include"mychesspoint.h"
#include<QTimer>
#include<QSound>
#include<QMessageBox>
#include<QPropertyAnimation>

//按钮音效
QSound *chooseSound =new QSound(":/res/TapButtonSound.wav");
//背景音乐
QSound *backgroundSound =new QSound(":/res/backSound.wav");

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);


    //固定场景的大小
    this->setFixedSize(1200,850);
    //设置应用图片
    this->setWindowIcon(QPixmap(":/res/logo.png"));
    //设置窗口标题
    this->setWindowTitle("六子棋 SixConnect");
    //退出按键设置
    connect(ui->quitAction,&QAction::triggered,this,&MainScene::close);
    connect(ui->guideAction,&QAction::triggered,this,[=](){
        QMessageBox box;
            //设置文本框的大小
        QPixmap pix1;
        pix1.load(":/res/logo.png");
            box.setStyleSheet("QLabel{"
                              "min-width: 900px;"
                              "min-height: 500px; "
                              "font-size:10px;"
                              "}");

            box.setText(
                        "<font size='10' color='blue'>  &nbsp;&nbsp;&nbsp;&nbsp; 下棋只是为了消遣，其所以能使这样多人嗜此不疲者，是因为它颇合人类好斗的本能，这是一种“斗智不斗力”的游戏。所以瓜棚豆架之下，与世无争的村夫野老不免一枰相对，"
                        "消此永昼；闹市茶寮之中，常有有闲阶级的人士下棋消遣，“不为无益之事，何以遣此有涯之生？”宦海里翻过身最后退隐东山的大人先生们，髀肉复生，而英雄无用武之地，也"
                        "只好闲来对弈，了此残生，下棋全是“剩余精力”的发泄。人总是要斗的，总是要钩心斗角地和人争逐的。与其和人争权夺利，还不如在棋盘上抽上一车。宋人笔记曾载有一段故事："
                        "“李讷仆射，性卞急，酷好弈棋，每下子安祥，极于宽缓，往往躁怒作，家人辈则密以弈具陈于前，讷赌，便忻然改容，以取其子布弄，都忘其恚矣。”（南部新书）。下棋，有没有"
                        "这样陶冶性情之功，我不敢说，不过有人下起棋来确实是把性命都可置诸度外。我有两个朋友下棋，警报作，不动声色，俄而弹落，棋子被震得在盘上跳荡，屋瓦乱飞，其中棋瘾较"
                        "小者变色而起，被对方一把拉住：“你走！那就算是你输了。”此公深得棋中之趣。<br/></font>"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font size='10' color='blue'>————梁实秋《下棋》<br/>"
                        "<font size='10' color='red'></hr>"
                        " 游戏模式一：人 机 对 战      ~<br/>"
                        " 游戏模式二：人 人 对 战      ~<br/>"
                        " 游戏模式三：机 机 对 战      ~<br/>"
                        " 游戏模式四：网 络 对 战      ~<br/>"
                        "<font size='10' color='MediumSlateBlue'>"
                        "1.对峙双方谁的六个棋子先连在一条线即为胜者。<br/>"
                        "2.当有四个子连成一条直线时，就应采取防守，堵住四子的一端，否则就会输掉比赛。<br/>"
                        "3.应当避免在比赛内出现四四禁手、五五禁手等情况，否则就会不小心输掉比赛。<br/>"
                        "</font>"
                        );

            box.setWindowTitle("Guide");
            box.setWindowIcon(pix1);
            box.exec();

    });

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


    chooseModel=NULL;


    backgroundSound->play();







    //按钮跳跃特效,以及跳转问题设置
    connect(firstchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseModel=new PlayScene(0);//建立一个新的游戏场景, 根据选择模式的不同做出不同选择
        chooseSound->play();
        firstchoosebutton->zoomdown();
        firstchoosebutton->zoomup();
        QTimer::singleShot(400,this,[=](){
            this->hide();
            chooseModel->setGeometry(this->geometry());
            chooseModel->show();
//            backgroundSound->stop();
        });

        //监听playscene的返回按钮
        connect(chooseModel,&PlayScene::goback,this,[=](){
           this->setGeometry(chooseModel->geometry());
            this->show();
           // chooseModel->close();
            delete chooseModel;
            chooseModel=NULL;
//            backgroundSound->play();

          // chooseModel->game->startGame(-1);
        });
    });
    connect(secondchoosebutton,&MyPushButton::clicked,this,[=](){
        chooseModel=new PlayScene(1);//建立一个新的游戏场景, 根据选择模式的不同做出不同选择
        chooseSound->play();
        secondchoosebutton->zoomdown();
        secondchoosebutton->zoomup();
        QTimer::singleShot(500,this,[=](){
            this->hide();
            chooseModel->setGeometry(this->geometry());
            chooseModel->show();
            backgroundSound->stop();
        });

        //监听playscene的返回按钮
        connect(chooseModel,&PlayScene::goback,this,[=](){
           this->setGeometry(chooseModel->geometry());
            this->show();
           // chooseModel->close();
            delete chooseModel;
            chooseModel=NULL;
            backgroundSound->play();

          // chooseModel->game->startGame(-1);
        });

    });
    connect(thirdchoosebutton,&MyPushButton::clicked,this,[=](){
       chooseModel=new PlayScene(2);//建立一个新的游戏场景, 根据选择模式的不同做出不同选择
        chooseSound->play();
        thirdchoosebutton->zoomdown();
        thirdchoosebutton->zoomup();

        QTimer::singleShot(500,this,[=](){
            this->hide();
            chooseModel->setGeometry(this->geometry());
            chooseModel->show();
//            backgroundSound->stop();
        });

        //监听playscene的返回按钮
        connect(chooseModel,&PlayScene::goback,this,[=](){
           this->setGeometry(chooseModel->geometry());
            this->show();
            chooseModel->close();
            delete chooseModel;
            chooseModel=NULL;
//            backgroundSound->play();

          // chooseModel->game->startGame(-1);
        });
    });
    connect(fourthchoosebutton,&MyPushButton::clicked,this,[=](){
        //建立一个新的游戏场景, 根据选择模式的不同做出不同选择
        chooseSound->play();
        fourthchoosebutton->zoomdown();
        fourthchoosebutton->zoomup();
        QTimer::singleShot(400,this,[=](){
            this->hide();
            chesspoint.setGeometry(this->geometry());
            chesspoint.show();
            backgroundSound->stop();
        });
        if(chesspoint.processing==0){
            QMessageBox:: StandardButton result= QMessageBox::information(NULL, "Title", "是否选择先手？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            switch (result)
            {
            case QMessageBox::Yes:
                 chesspoint.firstStep=1;
                break;
            case QMessageBox::No:

                break;
            default:
                break;
            }
            chesspoint.processing==0;
        }
        //监听playscene的返回按钮
        connect(&chesspoint,&mychesspoint::goback,this,[=](){
           this->setGeometry(chesspoint.geometry());
            this->show();
          chesspoint.hide();
          backgroundSound->play();
        });
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
