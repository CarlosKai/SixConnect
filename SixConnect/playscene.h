#ifndef PLAYSCENE_H
#define PLAYSCENE_H
#include"playmode.h"
#include <QMainWindow>



class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlayScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);//绘制背景

    int clickPosRow, clickPosCol; // 存储将点击的位置
    int readyClick[20][20];//光标移动位置判断棋盘
    PlayMode *game=new PlayMode; // 游戏指针

    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);

    void chessOneByPerson();

signals:
    void goback();

public slots:
};

#endif // PLAYSCENE_H
