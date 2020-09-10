#include "playscene.h"
#include"mypushbutton.h"
#include<QMenuBar>
#include<QBrush>
#include<QMouseEvent>
#include<QDebug>
#include<QPainter>
#include<QTimer>
#include<QSound>
#include<QMessageBox>
#include<QLabel>

const int kBoardMargin = 50; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限
const int kAIDelay = 700; // AI下棋的思考时间
static int AIcount=0;//AI之间下棋计数

//创建返回音效和下棋音效
QSound *backSound=new QSound(":/res/BackButtonSound.wav");
QSound *chessSound=new QSound(":/res/chessone.wav");
QSound *deadSound=new QSound(":/res/lose.wav");
QSound *winSound=new QSound(":/res/win.wav");


PlayScene::PlayScene(int index)
{
    if(index==3){
        QLabel *link=new QLabel(this);
        link->move(500,500);
        link->setText("123155115315");

        link->resize(100,30);
        link->show();
    }

    game=new PlayMode(this);
    game->choose=index;
    game->startGame(game->choose);

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
            readyClick[i][j]=0;
        }
    }

    setMouseTracking(true);//鼠标跟随

    //创建返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/back2.png",":/res/back.png");
    backBtn->setParent(this);
    backBtn->move(this->width()*0.97-backBtn->width(),this->height()*0.95-backBtn->height());



    //监听选择关卡按钮的信号槽
    connect(backBtn,&MyPushButton::clicked,this,[=](){
        backSound->play();
        QTimer::singleShot(400,this,[=](){
            //game->choose不需要进行调整，因为此时对象已经消亡了
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
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
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
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(50+kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
            else if (game->map[i][j] == -1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(50+kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
        }

    // 判断输赢
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        (game-> map[clickPosRow][clickPosCol] == 1 ||
            game-> map[clickPosRow][clickPosCol] == -1))
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            if(game->choose==2){
                AIcontinue->stop();
            }
            qDebug() << "win";
            game->gameStatus = WIN;
            winSound->play();
            QString str;
            if (game-> map[clickPosRow][clickPosCol] == 1)
                str = "黑棋 玩家";
            else if (game-> map[clickPosRow][clickPosCol] == -1)
                str = "白棋 玩家";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + " win!");

            // 重置游戏状态，否则容易死循环
            if (btnValue == QMessageBox::Ok)
            {
                game->startGame(game->choose);
                game->gameStatus = PLAYING;
            }
        }
    }



    // 判断死局
    if (game->isDead())
    {
        if(game->choose==2){
            AIcontinue->stop();
        }
        deadSound->play();
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
        if (btnValue == QMessageBox::Ok)
        {
            game->startGame(game->choose);
            game->gameStatus = PLAYING;
        }


    }
    update();//一定要重画，不然只会执行一次
}

void PlayScene::chessOneByPerson()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && game->map[clickPosRow][clickPosCol] == 0)
    {
        game->actionByPerson(clickPosRow, clickPosCol);
        chessSound->play();
        // 重绘
        update();
    }
    // 判断禁手
    if (game->choose==1&&clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        (game-> map[clickPosRow][clickPosCol] == 1 ||
            game-> map[clickPosRow][clickPosCol] == -1))
    {
        //qDebug()<<game->isBan(clickPosRow, clickPosCol)<<'1';
        if (game->isBan(clickPosRow, clickPosCol) && game->gameStatus == PLAYING && !game->playerFlag)
        {
            qDebug() << "ban";
            game->gameStatus = PLAYING;
            game->playerFlag=!(game->playerFlag);
            game->map[clickPosRow][clickPosCol]=0;
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "Notation!"," You can't chess here!");
            if (btnValue == QMessageBox::Ok)
            {
                game->gameStatus = PLAYING;
            }

        }

    }

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
    // 人下棋，并且不能抢机器的棋
    if (game->choose== 1 )
    {
        chessOneByPerson();
    }
   // 如果是人机模式，需要调用AI下棋
    if (game->choose == 0 )
    {
            if(game->playerFlag)
            chessOneByPerson();
            if(!game->playerFlag){
                // 用定时器做一个延迟
                QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
            }
        }
    //如果是机机对战，调用AI轮流下棋
    if(game->choose==2)
    {
        AIvsAI();
    }

}

void PlayScene::chessOneByAI()
{
    game->actionByAI(clickPosRow, clickPosCol);
    chessSound->play();
    update();
}

void PlayScene::AIvsAI()
{

    if(game->gameStatus==doubleAI){
        AIcontinue=new QTimer();
        AIcontinue->start(1000);
        game->gameStatus=PLAYING;
    }

    connect(AIcontinue,&QTimer::timeout,this,[=](){
        chessOneByAI();
    });

    AIcount++;
    if(AIcount>390)
    {AIcontinue->stop();
     AIcount=0;
    QMessageBox::StandardButton btnAI = QMessageBox::information(this, "Stop!"," Test end!");
    if (btnAI == QMessageBox::Ok)
    {
        game->startGame(game->choose);
        game->gameStatus = PLAYING;
    }
    }

}





