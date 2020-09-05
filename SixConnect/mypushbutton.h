#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include<QDebug>
#include <QWidget>
#include<QPushButton>
#include<QPropertyAnimation>
class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);

    //按钮使用两个图片进行动画移动
    MyPushButton(QString normalImg , QString pressImg="");
    QString normalImgPath;//默认显示路径
    QString pressImgPath;//按下后显示路径

    //向下跳跃，向上跳跃
    void zoomdown();
    void zoomup();

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
