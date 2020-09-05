#ifndef MAINSCENE_H
#define MAINSCENE_H
#include<QSound>
#include <QMainWindow>
#include<QPainter>
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



private:
    Ui::MainScene *ui;
};

#endif // MAINSCENE_H
