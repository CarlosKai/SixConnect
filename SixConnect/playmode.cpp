#include "playmode.h"
#include<QtDebug>

PlayMode::PlayMode(QObject *parent) : QObject(parent)
{
    startGame(choose);

}

void PlayMode::startGame(int choose){
    //初始化棋盘
    for(int i=0;i<kBoardSizeNum;i++){
        for(int j=0;j<kBoardSizeNum;j++){
            map[i][j]=0;
        }
    }
    //若有人机参与，需要初始化评分数组
    if(choose==1){
        for(int i=0;i<kBoardSizeNum;i++){
            for(int j=0;j<kBoardSizeNum;j++){
                score[i][j]=0;
            }
        }
    }

   playerFlag = true;//设置一方的棋子选择（后期扩展可以人为进行选择颜色）
   gameStatus=PLAYING;//每次开局后重置游戏状态
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

    if((blank<2&&count==4)&&judge<1){
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
    if((blank<2&&count==4)&&judge<1){
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
