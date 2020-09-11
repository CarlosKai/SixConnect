#ifndef PLAYMODE_H
#define PLAYMODE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

const int kBoardSizeNum = 20;//棋盘大小

enum GameStatus
{
    PLAYING, //进行中
    WIN,     //游戏胜利
    DEAD,   //游戏平局
    doubleAI //AI自动下棋
};

class PlayMode : public QObject
{
    Q_OBJECT
public:
    int lastRow[kBoardSizeNum*kBoardSizeNum/2+1];//保存上一步操作的Row
    int lastCol[kBoardSizeNum*kBoardSizeNum/2+1];//保存上一步操作的Col
    int lastPlayer[kBoardSizeNum*kBoardSizeNum/2+1];//保存上一步操作的玩家方
    int position;//指向已经走到的步骤
    explicit PlayMode(QObject *parent = nullptr);

    bool playerFlag; // 标示下棋方
    int choose;//游戏模式
    int map[20][20];//棋盘矩阵，表明当前棋盘的格局情况，-1表示黑棋，0表示空，1表示白棋
    int score[20][20];// 打分矩阵，存储各个点位的评分情况，作为AI下棋依据
    GameStatus gameStatus; // 游戏状态


    void startGame(int choose); // 开始游戏
    void calculateScore(); // 计算评分
    void actionByPerson(int row, int col); // 人执行下棋
    void updateGameMap(int row, int col); // 每次落子后更新游戏棋盘

    bool isWin(int row, int col); // 判断游戏是否胜利
    bool isDead(); // 判断是否和棋
    bool isBan(int row ,int col);//进行44禁手和55禁手的判断
    void actionByAI(int &clickRow, int &clickCol); // 机器执行下棋

signals:

public slots:
};

#endif // PLAYMODE_H
