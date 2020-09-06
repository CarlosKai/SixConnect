#include "playscene.h"
#include"mypushbutton.h"
#include<QMenuBar>
#include<QBrush>
#include<QMouseEvent>
#include<QDebug>
#include<QPainter>
#include<QTimer>

const int kBoardSizeNum = 20;//棋盘大小
const int kBoardMargin = 50; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限
const int kAIDelay = 700; // AI下棋的思考时间

PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
{
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
    connect(quitAction2,&QAction::triggered,this,&PlayScene::close);
    connect(quitAction1,&QAction::triggered,this,&PlayScene::close);
    connect(quitAction3,&QAction::triggered,this,&PlayScene::close);
    connect(quitAction4,&QAction::triggered,this,&PlayScene::close);

    //初始化
    for(int i=0;i<kBoardSizeNum;i++){
        for(int j=0;j<kBoardSizeNum;j++){
            game->map[i][j]=0;
            readyClick[i][j]=0;
        }
    }
    game->playerFlag=1;//暂时初始化为0
    setMouseTracking(true);//鼠标跟随

    //创建返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/back2.png",":/res/back.png");
    backBtn->setParent(this);
    backBtn->move(this->width()*0.97-backBtn->width(),this->height()*0.95-backBtn->height());

    connect(backBtn,&MyPushButton::clicked,this,[=](){
        QTimer::singleShot(400,this,[=](){
            this->hide();
            emit this->goback();
        });
    });

}

void PlayScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    //绘制背景
    pix.load(":/res/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    for (int i = 0; i < kBoardSizeNum ; i++)
    {
        painter.drawLine(50+kBoardMargin + kBlockSize * i, kBoardMargin, 50+kBoardMargin + kBlockSize * i,  kBoardMargin+kBlockSize*19);
        painter.drawLine(50+kBoardMargin, kBoardMargin + kBlockSize * i, 50+kBoardMargin+kBlockSize*19, kBoardMargin + kBlockSize * i);
    }

    QBrush brush;
//    clickPosRow=1;
//    clickPosCol=1;落子标记的函数是正确的
    brush.setStyle(Qt::SolidPattern);
    // 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow >= 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol >= 0 && clickPosCol < kBoardSizeNum &&
        this->readyClick[clickPosCol][clickPosRow] == 0)
    {
        //qDebug()<<clickPosRow<<" "<<clickPosCol<<'\n';
        if (game->playerFlag)
            brush.setColor(Qt::white);
        else
            brush.setColor(Qt::black);
        painter.setBrush(brush);
        painter.drawRect(50+kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }

    // 绘制棋子,测试功能成功
//    map[1][8]=-1;
//    map[0][19]=1;
    for (int i = 0; i < kBoardSizeNum; i++)
        for (int j = 0; j < kBoardSizeNum; j++)
        {
            if (game->map[i][j] == 1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(50+kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
            else if (game->map[i][j] == -1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(50+kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
        }

    update();//一定要重画，不然只会执行一次
}

void PlayScene::mouseMoveEvent(QMouseEvent *event)
{

    // 通过鼠标的hover确定落子的标记

    int x = event->x();
    int y = event->y();
    //qDebug()<<x<<" "<<y<<'\n';
    // 棋盘边缘不能落子
    if (x >= 50+kBoardMargin - kBlockSize / 2 &&x <= 50+kBoardMargin +19.5*kBlockSize &&
            y >= kBoardMargin - kBlockSize / 2 &&y < kBoardMargin+ 19.5*kBlockSize)
    {
        // 获取最近的左上角的点
        int col = (x-50-kBoardMargin) / kBlockSize;
        int row = (y-kBoardMargin) / kBlockSize;

        int leftTopPosX =  50+kBoardMargin + kBlockSize * col;
        int leftTopPosY = kBoardMargin + kBlockSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickPosRow = -1; // 初始化最终的值
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }
    // 存了坐标后也要重绘
    update();
}



void PlayScene::mouseReleaseEvent(QMouseEvent *event)
{

}
