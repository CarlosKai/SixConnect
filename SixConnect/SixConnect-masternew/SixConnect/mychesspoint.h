#ifndef MYCHESSPOINT_H
#define MYCHESSPOINT_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include<QDebug>
#include <QPixmap>
#include <QTcpServer>
#include <QTcpSocket>   //服务器

const int N=860;
const int M=20;

namespace Ui {
class mychesspoint;
}

class mychesspoint : public QMainWindow
{
    Q_OBJECT

public:
    explicit mychesspoint(QWidget *parent = 0);
    ~mychesspoint();
    char chessbord[M][M];    //定义20*20数组
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void initchessbord();
    int chessplayer(char chess,int x,int y);     //棋盘数组入参
    int checkchess(char flag,int i,int j);       //输赢检测
    bool flag=1;                   //黑白棋标志位设定是对方先下棋
    int processing=0;
    int firstStep=0;
signals:
    void mySignal();
    void goback();



protected slots:
    void newConnection_Slots();           //网络连接
    void readyRead_Slots();

private:
    Ui::mychesspoint *ui;
    int huiqix=641,huiqiy=641;       //记录最后一步
    QPainter *pen;  //框
    QPainter *pointpen;  //线
    QPainter *Pointpoint2;
    QPixmap pix;

    bool flagjudget=1;             //对方下棋标志位
    int x=876,y=826;

    QTcpSocket *tcpSock;
    QTcpServer *tcpSever;

};

#endif // MYCHESSPOINT_H
