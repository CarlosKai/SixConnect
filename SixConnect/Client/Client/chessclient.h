#ifndef CHESSCLIENT_H
#define CHESSCLIENT_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>    //填加鼠标事件
#include <QKeyEvent>
#include<QDebug>
#include <QPixmap>
#include <QTcpSocket>

const int N=860;
const int M=20;
namespace Ui {
class chessClient;
}

class chessClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit chessClient(QWidget *parent = 0);
    ~chessClient();
     char chessbord[M][M];    //定义20*20数组
     int chessplayer(char chess,int x,int y);    //放数组
    void paintEvent(QPaintEvent *event);       //绘棋盘，棋子
    void mousePressEvent(QMouseEvent *event);
    void initchessbord();

private slots:
//    void on_pushButton_clicked();

protected slots:
    void readyRead_Slots();

private:

    QPainter *pen;  //框
    QPainter *pointpen;  //线
    QPainter *Pointpoint;   //点
    QPainter *Pointpoint2;

    bool flagjudget=1;
    QPixmap pix;
    QTcpSocket *tcpSock;
    bool flag=0;

    int x=876,y=826;

private:
    Ui::chessClient *ui;
};

#endif // CHESSCLIENT_H
