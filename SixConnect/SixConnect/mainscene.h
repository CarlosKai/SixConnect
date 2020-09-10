#ifndef MAINSCENE_H
#define MAINSCENE_H
#include"playscene.h"
#include<QSound>
#include <QMainWindow>
#include<QPainter>
#include"mychesspoint.h"
namespace Ui {
class MainScene;
}

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = 0);
    ~MainScene();
    void paintEvent(QPaintEvent *);
    PlayScene *chooseModel;
    mychesspoint chesspoint;




private:
    Ui::MainScene *ui;
};

#endif // MAINSCENE_H
