#ifndef PLAYSCENE_H
#define PLAYSCENE_H
#include"playmode.h"
#include <QMainWindow>
#include<QTimer>
#include"mypushbutton.h"


class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlayScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);//绘制背景

    int clickPosRow, clickPosCol; // 存储将点击的位置
    int readyClick[20][20];//光标移动位置判断棋盘
    PlayMode *game=new PlayMode; // 游戏指针

    QTimer *AIcontinue;//在AI和AI机机模式中进行计时，让AI轮流下子

    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);
    void chessOneByPerson();//人下棋

    void AIvsAI();

signals:
    void goback();

public slots:
    void chessOneByAI(); // AI下棋
};

#endif // PLAYSCENE_H
