#ifndef PLAYSCENE_H
#define PLAYSCENE_H
#include"playmode.h"
#include <QMainWindow>
#include<QTimer>
#include<QLabel>
#include"mypushbutton.h"


class PlayScene : public QMainWindow
{
    Q_OBJECT

private:
    int value=61;
    QLabel *countTime;
    QTimer *pTimer;
    int timeStop;

public:
    explicit PlayScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);//绘制背景
    PlayScene(int index);//自定义选择关卡的构造函数，游戏关卡最好不要复用

    int clickPosRow, clickPosCol; // 存储将点击的位置
    int readyClick[20][20];//光标移动位置判断棋盘
    PlayMode *game; // 游戏指针

    QTimer *AIcontinue;//在AI和AI机机模式中进行计时，让AI轮流下子

    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);
    void chessOneByPerson();//人下棋

    void AIvsAI();//机机对战模式
    void runTime();//启动定时器
    void beginCount();//计时器开始计时

signals:
    void goback();

public slots:
    void chessOneByAI(); // AI下棋
    void on_timer_timeout();//双人模式的倒计时
};

#endif // PLAYSCENE_H
