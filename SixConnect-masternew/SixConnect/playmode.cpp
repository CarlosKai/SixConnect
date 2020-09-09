#include "playmode.h"
#include<QtDebug>
#include<vector>
#include<QTime>
PlayMode::PlayMode(QObject *parent) : QObject(parent)
{
}

void PlayMode::startGame(int choose){
    //初始化棋盘
    for(int i=0;i<kBoardSizeNum;i++){
        for(int j=0;j<kBoardSizeNum;j++){
            map[i][j]=0;
        }
    }
    //若有人机参与，需要初始化评分数组
    if(choose==0){
        for(int i=0;i<kBoardSizeNum;i++){
            for(int j=0;j<kBoardSizeNum;j++){
                score[i][j]=0;
            }
        }
    }

   playerFlag = true;//设置一方的棋子选择（后期扩展可以人为进行选择颜色）
   gameStatus=PLAYING;//每次开局后重置游戏状态


   if(choose==2){
           QTime time;
           time= QTime::currentTime();
           qsrand(time.msec()+time.second()*1000);
           int x = qrand() % 10;    //产生10以内的随机数
           time= QTime::currentTime();
           int y = qrand() % 10;
           map[x+5][y+5]=1;
       updateGameMap(x+5,y+5);
       gameStatus=doubleAI;
   }
}

void PlayMode::updateGameMap(int row, int col)
{
    if (playerFlag)
        map[row][col] = 1;
    else
        map[row][col] = -1;

    // 换手
    playerFlag = !playerFlag;
}

void PlayMode::actionByAI(int &clickRow, int &clickCol)
{
    // 计算评分
    calculateScore();

    // 从评分中找出最大分数的位置
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;
    for (int row = 0; row < kBoardSizeNum; row++)
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // 前提是这个坐标是空的
            if (map[row][col] == 0)
            {
                if (score[row][col] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = score[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (score[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }

    // 随机落子，如果有多个点的话
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();
    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // 记录落子点
    clickCol = pointPair.second;
    updateGameMap(clickRow, clickCol);
}

// 最关键的计算评分函数
void PlayMode::calculateScore()
{
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数

    // 清空评分数组
    for(int i=0;i<kBoardSizeNum;i++){
        for(int j=0;j<kBoardSizeNum;j++){
            score[i][j]=0;
        }
    }

    // 计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI智能程度以及攻守风格）
    for (int row = 0; row < kBoardSizeNum; row++)
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // 空白点就算
            if (row >= 0 && col >= 0 &&
                map[row][col] == 0)
            {
                // 遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // 原坐标不算
                        if (!(y == 0 && x == 0))
                        {
                            // 每个方向延伸4个子

                            // 对玩家白子评分（正反两个方向）
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    map[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         map[row + i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    map[row - i * y][col - i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         map[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (personNum == 1)                      // 杀二
                                score[row][col] += 10;
                            else if (personNum == 2)                 // 杀三
                            {
                                if (emptyNum == 1)
                                    score[row][col] += 30;
                                else if (emptyNum == 2)
                                    score[row][col] += 40;
                            }
                            else if (personNum == 3)                 // 杀四
                            {
                                // 量变空位不一样，优先级不一样
                                if (emptyNum == 1)
                                    score[row][col] += 60;
                                else if (emptyNum == 2)
                                    score[row][col] += 110;
                            }
                            else if (personNum == 4)                 // 杀五
                                score[row][col] += 10100;

                            // 进行一次清空
                            emptyNum = 0;

                            // 对AI黑子评分
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    map[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         map[row +i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    map[row - i * y][col - i * x] == -1) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         map[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (botNum == 0)                      // 普通下子
                                score[row][col] += 5;
                            else if (botNum == 1)                 // 活二
                                score[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)                // 死三
                                    score[row][col] += 25;
                                else if (emptyNum == 2)
                                    score[row][col] += 50;  // 活三
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)                // 死四
                                    score[row][col] += 55;
                                else if (emptyNum == 2)
                                    score[row][col] += 100; // 活四
                            }
                            else if (botNum >= 4)
                                score[row][col] += 10000;   // 活五

                        }
                    }

            }
        }
}


void PlayMode::actionByPerson(int row, int col)
{
    updateGameMap(row, col);
}


bool PlayMode::isWin(int row, int col)
{
        // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
        // 水平方向
        for (int i = 0; i < 6; i++)
        {
            // 往左5个，往右匹配4个子，20种情况
            if (map[row][col]!=0&&
                col - i > 0 &&
                col - i + 5 < kBoardSizeNum &&
                map[row][col - i] == map[row][col - i + 1] &&
                map[row][col - i] == map[row][col - i + 2] &&
                map[row][col - i] == map[row][col - i + 3] &&
                map[row][col - i] == map[row][col - i + 4] &&
                map[row][col - i] == map[row][col - i + 5])
                return true;
        }

        // 竖直方向(上下延伸4个)
        for (int i = 0; i < 6; i++)
        {
            if (map[row][col]!=0&&
                row - i > 0 &&
                row - i + 5 < kBoardSizeNum &&
                map[row - i][col] == map[row - i + 1][col] &&
                map[row - i][col] == map[row - i + 2][col] &&
                map[row - i][col] == map[row - i + 3][col] &&
                map[row - i][col] == map[row - i + 4][col] &&
                map[row - i][col] == map[row - i + 5][col]  )
                return true;
        }

        // 左斜方向
        for (int i = 0; i < 6; i++)
        {
            if (map[row][col]!=0&&
                row + i < kBoardSizeNum &&
                row + i - 5 > 0 &&
                col - i > 0 &&
                col - i + 5 < kBoardSizeNum &&
                map[row + i][col - i] == map[row + i - 1][col - i + 1] &&
                map[row + i][col - i] == map[row + i - 2][col - i + 2] &&
                map[row + i][col - i] == map[row + i - 3][col - i + 3] &&
                map[row + i][col - i] == map[row + i - 4][col - i + 4] &&
                map[row + i][col - i] == map[row + i - 5][col - i + 5])
                return true;
        }

        // 右斜方向
        for (int i = 0; i < 6; i++)
        {
            if (map[row][col]!=0&&
                row - i > 0 &&
                row - i + 5 < kBoardSizeNum &&
                col - i > 0 &&
                col - i + 5 < kBoardSizeNum &&
                map[row - i][col - i] == map[row - i + 1][col - i + 1] &&
                map[row - i][col - i] == map[row - i + 2][col - i + 2] &&
                map[row - i][col - i] == map[row - i + 3][col - i + 3] &&
                map[row - i][col - i] == map[row - i + 4][col - i + 4] &&
                map[row - i][col - i] == map[row - i + 5][col - i + 5] )
                return true;
        }

        return false;
}

bool PlayMode::isDead()
{
    // 所有空格全部填满
    for (int i = 0; i < kBoardSizeNum; i++)
        for (int j = 0; j < kBoardSizeNum; j++)
        {
            if (!(map[i][j] == 1 || map[i][j] == -1))
                return false;
        }
    return true;
}

bool PlayMode::isBan(int row ,int col){
    int fourconnect=0;
    int fiveconnect=0;

    //判断44禁手的情况
    //横向
    //横向左侧
    int i,j=0,blank,count,left,right,judge;
    blank=0;
    count=1;
    left=0;
    right=0;
   // judge=0;
    for(i=1;col-i>=0&&i<=3;i++){
        if(map[row][col-i]==map[row][col]){count++;}
        else if(map[row][col-i]==0){blank++;}
        else{break;}
        j=i;
    }
    while (map[row][col-j]==0) {
        blank--;
        j--;
    }
  //  if(map[row][col-j]==-map[row][col])
     //   judge++;
    //横向右侧
    for(i=1;col+i<kBoardSizeNum&&i<=3;i++){
        if(map[row][col+i]==map[row][col]){count++;}
        else if(map[row][col+i]==0){blank++;}
        else{break;}
        j=i;
    }
    while (map[row][col+j]==0) {
        blank--;
        j--;
    }
   // if(map[row][col+j]==-map[row][col])
   // judge++;

    if((blank<2&&count==4)){
        fourconnect++;
    }
    //qDebug()<<blank<<count<<fourconnect;


    //纵向上侧
    blank=0;
    count=1;
    left=0;
    right=0;
    judge=0;
    for(i=1;row-i>=0&&i<=3;i++){
        if(map[row-i][col]==map[row][col]){count++;}
        else if(map[row-i][col]==0){blank++;}
        else{break;}
        j=i;
    }
    while (map[row-j][col]==0) {
        blank--;
        j--;
    }
    if(map[row-j][col]==-map[row][col])
    judge++;
    //纵向下侧
    for(i=1;col+i<kBoardSizeNum&&i<=3;i++){
        if(map[row+i][col]==map[row][col]){count++;}
        else if(map[row+i][col]==0){blank++;}
        else{break;}
        j=i;
    }
    while (map[row+j][col]==0) {
        blank--;
        j--;
    }
    if(map[row+j][col]==-map[row][col])
            judge++;
    if((blank<2&&count==4)){
        fourconnect++;
    }
     //qDebug()<<blank<<count<<fourconnect;

    //左上连右下左上侧
    blank=0;
    count=1;
    left=0;
    right=0;
    judge=0;
    for(i=1;row-i>=0&&col-i>=0&&i<=3;i++){
        if(map[row-i][col-i]==map[row][col]){count++;left++;}
        else if(map[row-i][col-i]==0){blank++;}
        else{break;}
    }

    for(i=1;col+i<kBoardSizeNum&&row+i<kBoardSizeNum&&i<=3;i++){
        if(map[row+i][col+i]==map[row][col]){count++;right++;}
        else if(map[row+i][col+i]==0){blank++;}
        else{break;}
    }
    if((blank<2&&count==4)||left==3||right==3){
        fourconnect++;
    }


    //右上连左下
    blank=0;
    count=1;
    left=0;
    right=0;
    for(i=1;row+i<kBoardSizeNum&&col-i>=0&&i<=3;i++){
        if(map[row+i][col-i]==map[row][col]){count++;left++;}
        else if(map[row+i][col-i]==0){blank++;}
        else{break;}
    }
    for(i=1;col+i<kBoardSizeNum&&row-i>=0&&i<=3;i++){
        if(map[row-i][col+i]==map[row][col]){count++;right++;}
        else if(map[row-i][col+i]==0){blank++;}
        else{break;}
    }
    if((blank<2&&count==4)||left==3||right==3){
        fourconnect++;
    }

    if(fourconnect>=2)
        return true;

    //判断55禁手的情况
    //横向
    blank=0;
    count=1;
    left=0;
    right=0;
    for(i=1;col-i>=0&&i<=4;i++){
        if(map[row][col-i]==map[row][col]){count++;left++;}
        else if(map[row][col-i]==0){blank++;}
        else{break;}
    }
    for(i=1;col+i<kBoardSizeNum&&i<=4;i++){
        if(map[row][col+i]==map[row][col]){count++;right++;}
        else if(map[row][col+i]==0){blank++;}
        else{break;}
    }
    if((blank<2&&count==5)||left==4||right==4){
        fiveconnect++;
    }

    //纵向
    blank=0;
    count=1;
    left=0;
    right=0;
    for(i=1;row-i>=0&&i<=3;i++){
        if(map[row-i][col]==map[row][col]){count++;left++;}
        else if(map[row-i][col]==0){blank++;}
        else{break;}
    }
    for(i=1;col+i<kBoardSizeNum&&i<=3;i++){
        if(map[row+i][col]==map[row][col]){count++;right++;}
        else if(map[row+i][col]==0){blank++;}
        else{break;}
    }
    if((blank<2 && count==5)||left==3||right==3){
        fiveconnect++;
    }

    //左上连右下
    blank=0;
    count=1;
    left=0;
    right=0;
    for(i=1;row-i>=0&&col-i>=0&&i<=4;i++){
        if(map[row-i][col-i]==map[row][col]){count++;left++;}
        else if(map[row-i][col-i]==0){blank++;}
        else{break;}
    }
    for(i=1;col+i<kBoardSizeNum&&row+i<kBoardSizeNum&&i<=4;i++){
        if(map[row+i][col+i]==map[row][col]){count++;right++;}
        else if(map[row+i][col+i]==0){blank++;}
        else{break;}
    }
    if((blank<2&&count==5)||left==4||right==4){
        fiveconnect++;
    }


    //右上连左下
    blank=0;
    count=1;
    left=0;
    right=0;
    for(i=1;row+i<kBoardSizeNum&&col-i>=0&&i<=4;i++){
        if(map[row+i][col-i]==map[row][col]){count++;left++;}
        else if(map[row+i][col-i]==0){blank++;}
        else{break;}
    }
    for(i=1;col+i<kBoardSizeNum&&row-i>=0&&i<=4;i++){
        if(map[row-i][col+i]==map[row][col]){count++;right++;}
        else if(map[row-i][col+i]==0){blank++;}
        else{break;}
    }
    if((blank<2&&count==5)||left==4||right==4){
        fiveconnect++;
    }


    if(fiveconnect>=2)
        return true;

    return false;

}
