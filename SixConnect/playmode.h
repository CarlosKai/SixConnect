#ifndef PLAYMODE_H
#define PLAYMODE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class PlayMode : public QObject
{
    Q_OBJECT
public:
    explicit PlayMode(QObject *parent = nullptr);
    bool playerFlag; // 标示下棋方
    int choose;//游戏类型
    int map[20][20];//-1表示黑棋，0表示空，1表示白棋

signals:

public slots:
};

#endif // PLAYMODE_H
