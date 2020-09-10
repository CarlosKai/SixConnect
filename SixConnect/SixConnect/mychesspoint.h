#ifndef MYCHESSPOINT_H
#define MYCHESSPOINT_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>    //填加鼠标事件
#include <QKeyEvent>
#include<QDebug>
#include <QPixmap>
#include <QTcpServer>
#include <QTcpSocket>   //服务器

const int bifenBAI=0,bidenHEI=0;

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
    int checkchess(char flag,int i,int j);          //输赢检测
signals:
    void mySignal();
    void goback();

private slots:
    void on_pushButton_clicked();

protected slots:
    void newConnection_Slots();           //网络连接
    void readyRead_Slots();

private:
    Ui::mychesspoint *ui;
    int huiqix=641,huiqiy=641;       //记录最后一步
    QPainter *pen;  //框
    QPainter *pointpen;  //线
    QPainter *Pointpoint;   //点
     QPainter *Pointpoint2;
    QPixmap pix;
    bool flag=1;                   //黑白棋标志位设定是对方先下棋
    bool flagjudget=1;             //对方下棋标志位
    int x=876,y=826;

    QTcpServer *tcpSever;
    QTcpSocket *tcpSock;
};

#endif // MYCHESSPOINT_H
