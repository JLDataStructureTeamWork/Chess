#define _CRT_SECURE_NO_WARNINGS
#include<queue>
#include<algorithm>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<iostream>
using namespace std;
#define BLACK 0
#define WHITE 1
#define EMPTY 2

struct Point { //点结构
  int x, y;
};
struct Step { //步结构
  Point first, second;
  int value;
};

int Board[19][19];//存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一

int ALL_EvalueFucation(int VirtualBoard[19][19],int BeginX,int EndX,int BeginY,int EndY,int ComputerSide) {//全局评价函数
  
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//不同棋子数的路的条数
  int NumOfEnemyRoad[7]= { 0,0,0,0,0,0,0 };
  int ScoreOfRoad[7] = {};//不同棋子数的路的分数
  
  for (int dir = 0; dir < 4; dir++) {
    for (int i = BeginX; i <= EndX; i++) {//对每行的路进行分析
      for (int j = BeginY; j <= EndY; j++) {
        int num = 0;
        int flag = 0;//flag来代表棋子颜色的变化，没有遇到棋子为0，遇到黑棋为1，遇到白棋为2，前后遇到不同色棋子则break
        for (int k = 0; k < 6; k++) {
          int a, b;
          if (dir == 0) {
            if (j + 5 > EndY) continue;
            a = i;
            b = j + k;
          }
          else if (dir == 1) {
            if (i + 5 > EndX) continue;
            a = i +k;
            b = j;
          }
          else if (dir==2) {
            if (i + 5 > EndX || j - 5 < BeginY) continue;
            a = i + k;
            b = j - k;
          }
          else if (dir==3) {
            if (i + 5 > EndX || j + 5 > EndY) continue;
            a = i + k;
            b = j + k;
          }
          
          if ((VirtualBoard[a][b] == BLACK && flag == 2) || (VirtualBoard[a][b] == WHITE && flag == 1)) {//前后棋子颜色不同则此路作废
            flag = -1;
            break;
          }

          if (VirtualBoard[a][b] == BLACK) {
            flag = 1;
            num++;
          }
          else if (VirtualBoard[a][b] == WHITE) {
            flag = 2;
            num++;
          }
        }
        if (flag == 1) {
          if (ComputerSide == BLACK)
            NumOfMyRoad[num]++;
          else
            NumOfEnemyRoad[num]++;
        }
        else if (flag == 2) {
          if (ComputerSide == WHITE)
            NumOfMyRoad[num]++;
          else
            NumOfEnemyRoad[num]++;
        }
      }
    }
  }
  int Score=0;
  for (int i = 0; i < 6; i++) {
    Score += NumOfMyRoad[i] * ScoreOfRoad[i] - NumOfEnemyRoad[i] * ScoreOfRoad[i];
  }
  return Score;
}
int IfNot_Road(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY, int &flag, int dir) {//判断是否是一条路
  int num = 0;
  int MoveRoadX[4] = { 0,1,1,1 };
  int MoveRoadY[4] = { 1,0,-1,1 };
  if (BeginX < 0 || BeginX >= 19 || EndX < 0 || EndX >= 19) return -1;
  if (BeginY < 0 || BeginY >= 19 || EndY < 0 || EndY >= 19) return -1;
  //2代表还未遇到棋子，1白子，0黑子。
  int a = BeginX, b = BeginY;
  for (int i = 0; i < 6; i++) {
    if (i != 0) {//不变路开头点的坐标
      a += MoveRoadX[dir];
      b += MoveRoadY[dir];
    }
    if (Board[a][b] == EMPTY) continue;
    if (Board[a][b] == BLACK && flag == 1) return -1;//返回-1代表不是路
    if (Board[a][b] == WHITE && flag == 0) return -1;
    if (Board[a][b] == BLACK) {
      num++;
      flag = 0;
    }
    if (Board[a][b] == WHITE) {
      num++;
      flag = 1;
    }
  }
  return num;//返回棋子个数
}
Step PreSeek_ReturnEmpty(int Board[19][19], int BeginX, int BeginY,int dir) {//寻找一条路上有>=4个棋子的空位
  int MoveRoadX[4] = { 0,1,1,1 };//用于路上的移动，并判断哪一点是空
  int MoveRoadY[4] = { 1,0,-1,1 };
  
  Step ReturnEmpty;
  ReturnEmpty.first.x = -1;
  ReturnEmpty.first.y = -1;
  ReturnEmpty.second.x = -1;
  ReturnEmpty.second.y = -1;
  int a = BeginX, b = BeginY;
  for (int i = 0; i < 6; i++) {
    if (i != 0) {
      a += MoveRoadX[dir];
      b += MoveRoadY[dir];
    }
    if (Board[a][b] != EMPTY) {
      if (ReturnEmpty.first.x != -1) {
        ReturnEmpty.second.x = a;
        ReturnEmpty.second.y = b;
      }
      else {
        ReturnEmpty.first.x = a;
        ReturnEmpty.first.y = b;
      }
    }
  }
  return ReturnEmpty;
}
Step PreSeek(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY) {//判断里面的所有路是否有活四，活五，眠五
  for (int i = BeginX; i <= EndX; i++) {
    for (int j = BeginY; j <= EndY; j++) {
      int num;
      int flag = 2;
      num = IfNot_Road(Board, i, i, j, j + 5, flag, 0);
      if (flag != 2) {//以该点为起点的行上的路是路
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 0);
        }
      }
      num = IfNot_Road(Board, i, i, j, j + 5, flag, 1);
      if (flag != 2) {//以该点为起点的列上的路是路
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 1);
        }
      }
      num = IfNot_Road(Board, i, i+5, j, j - 5, flag, 2);
      if (flag != 2) {//以该点为起点的向左下斜的路是路
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 2);
        }
      }
      num = IfNot_Road(Board, i, i+5, j, j + 5, flag, 3);
      if (flag != 2) {//以该点为起点的向右下斜的路是路
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 3);
        }
      }
    }
  }
  return;
}

void BoardRange(int Board[19][19],int &BeginX,int &EndX,int &BeginY,int &EndY) {//20,-1,20,-1//已测试
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 19; j++) {
      if (Board[i][j] != EMPTY) {
        if (i < BeginX) BeginX = i;
        if (i > EndX)EndX = i;
        if (j < BeginY)BeginY = j;
        if (j > EndY)EndY = j;
      }
    }
  }
  return;
}

int PartScore_EvalueFucation(int Board[19][19], Point FirstChess, Point LimitChess,int ComputerSide,int limit) {
  int sum = 0;
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//不同棋子数的路的条数
  int NumOfEnemyRoad[7] = { 0,0,0,0,0,0,0 };
  int ScoreOfRoad[7] = {};//不同棋子数的路的分数!!!

  for (int i = 0; i < 6; i++) {//行上的路
    Point Begin;
    Begin.y = FirstChess.y - i;
    Begin.x = FirstChess.x;
    int flag = 2;
    int num=IfNot_Road(Board, Begin.x, Begin.x, Begin.y, Begin.y + 5, flag,0);
    if ( num== -1) continue;
    if (limit == 1) {
      if (Begin.x == LimitChess.x&&LimitChess.y >= Begin.y&&LimitChess.y <= Begin.y + 5) continue;
    }
    else {
      if (flag == ComputerSide) {
        NumOfMyRoad[num]++;
      }
      else {
        NumOfEnemyRoad[num]++;
      }
    }
  }
  for (int i = 0; i < 6; i++) {//列上的路
    Point Begin;
    Begin.y = FirstChess.y ;
    Begin.x = FirstChess.x-i;
    int flag = 2;
    int num = IfNot_Road(Board, Begin.x, Begin.x+5, Begin.y, Begin.y, flag,1);
    if (num == -1) continue;
    if (limit == 1) {
      if (Begin.y == LimitChess.y&&LimitChess.x >= Begin.x&&LimitChess.x <= Begin.x + 5) continue;
    }
    else {
      if (flag == ComputerSide) {
        NumOfMyRoad[num]++;
      }
      else {
        NumOfEnemyRoad[num]++;
      }
    }
  }
  for (int i = 0; i < 6; i++) {//向左下斜
    Point Begin;
    Begin.y = FirstChess.y + i;
    Begin.x = FirstChess.x - i;
    int flag = 2;
    int num = IfNot_Road(Board, Begin.x, Begin.x+5, Begin.y, Begin.y - 5, flag,2);
    if (num == -1) continue;
    if (limit == 1) {
      if (LimitChess.y>=Begin.y&&LimitChess.y<=Begin.y+5&&LimitChess.x >= Begin.x-5&&LimitChess.x <= Begin.x ) continue;
    }
    else {
      if (flag == ComputerSide) {
        NumOfMyRoad[num]++;
      }
      else {
        NumOfEnemyRoad[num]++;
      }
    }
  }
  for (int i = 0; i < 6; i++) {//向右下斜
    Point Begin;
    Begin.y = FirstChess.y - i;
    Begin.x = FirstChess.x - i;
    int flag = 2;
    int num = IfNot_Road(Board, Begin.x, Begin.x+5, Begin.y, Begin.y+5 , flag,3);
    if (num == -1) continue;
    if (limit == 1) {
      if (LimitChess.y >= Begin.y-5&&LimitChess.y <= Begin.y  && LimitChess.x >= Begin.x - 5 && LimitChess.x <= Begin.x) continue;
    }
    else {
      if (flag == ComputerSide) {
        NumOfMyRoad[num]++;
      }
      else {
        NumOfEnemyRoad[num]++;
      }
    }
  }
  int score;
  for (int i = 0; i < 7; i++) {
    score += NumOfMyRoad[i] * ScoreOfRoad[i] - NumOfEnemyRoad[i] * ScoreOfRoad[i];
  }
  return score;
}
int Part_EvalueFucation(int Board[19][19],Point FirstChess,Point SecondChess,int ComputerSide) {//
  int VirtualBoard[19][19];
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 19; j++) {
      VirtualBoard[i][j] = Board[i][j];
    }
  }
  VirtualBoard[FirstChess.x][FirstChess.y] =ComputerSide;
  VirtualBoard[SecondChess.x][SecondChess.y] = ComputerSide;
  
  int Before=PartScore_EvalueFucation(Board, FirstChess, SecondChess, ComputerSide, 0) + PartScore_EvalueFucation(Board, SecondChess, FirstChess, ComputerSide, 1);
  int After= PartScore_EvalueFucation(VirtualBoard, FirstChess, SecondChess, ComputerSide, 0) + PartScore_EvalueFucation(VirtualBoard, SecondChess, FirstChess, ComputerSide, 1);
  
  return After - Before;
}
void EvalueFucation() {//对博弈树节点评估函数

}
queue<Step> GenerateSon() {//产生子节点队列函数

}
Step GetFrontNode(queue<Step> Son) {//得到队列头节点并pop掉
  Step ReturnNode;
  ReturnNode = Son.front();
  Son.pop();
  return ReturnNode;
}
int NegaMax(Step step, int depth) {//负极大值搜索
  int value,BestValue = -INFINITY;
  Step CurrentNode;
  if (depth <= 0) {
    EvalueFucation();
  }
  queue<Step> Son;
  Son=GenerateSon();
  while (!Son.empty()) {
    CurrentNode = GetFrontNode(Son);
    value = -NegaMax(CurrentNode, depth - 1);
    if (value >= BestValue) {
      BestValue = value;
    }
  }
  return BestValue;
}
Step machine(int TureBoard[19][19],int computerSide) {
  int Board[19][19];//为了不改变主函数中的界面
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 19; j++) {
      Board[i][j] = TureBoard[i][j];
    }
  }
  Step NextTwoStep;
  int BeginX = 20, BeginY = 20;
  int EndX = -1, EndY = -1;
  BoardRange(Board, BeginX, EndX, BeginY, EndY);//首先探明界面范围
  for (int i = 1; i <= 2; i++) {//将范围扩大两个单位，得到需要分析落子的区域
    if (BeginX != 0) BeginX--;
    if (BeginY != 0) BeginY--;
    if (EndX != 18) EndX++;
    if (EndY != 18) EndY++;
  }

  Step PreSeekStep;
  int FirstChess_Flag = 0;//1代表一颗棋子已经确定
  PreSeekStep=PreSeek(Board, BeginX, EndX, BeginY, EndY);//使用博弈树之前进行前期扫描，判断是否有活四活五等必须落子的情况
  if (PreSeekStep.first.x != -1) {
    NextTwoStep.first.x = PreSeekStep.first.x;
    NextTwoStep.first.y = PreSeekStep.first.y;
    FirstChess_Flag = 1;
  }
  if (PreSeekStep.second.x != -1) {
    NextTwoStep.second.x = PreSeekStep.second.x;
    NextTwoStep.second.y = PreSeekStep.second.y;
    return NextTwoStep;//两颗棋子均确定则直接返回
  }
  //----------博弈树部分------------//
  //对界面进行分析


}

int main()
{
  Step step;//临时步结构
  char message[256];//通信消息缓冲
  int computerSide;//己方执棋颜色
  int start = 0;//对局开始标记
  srand(int(time(0)));
  //此处放置初始化代码
    //...

  while (1)	//程序主循环
  {
    fflush(stdout);//不要删除此语句，否则程序会出问题
    scanf("%s", message);//获取平台命令消息
        //分析命令
    if (strcmp(message, "name?") == 0)//向对战平台发送队名
    {
      fflush(stdin);
      /***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
      /*******/		printf("name 快乐第一位\n");		/**只修改令狐冲，不要删除name空格****/
      /***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
    }
    else if (strcmp(message, "new") == 0)//建立新棋局
    {
      int i, j;
      scanf("%s", message);//获取己方执棋颜色
      fflush(stdin);
      if (strcmp(message, "black") == 0)	computerSide = BLACK;  //执黑
      else  computerSide = WHITE;   //执白

      for (i = 0; i < 19; ++i)   //初始化棋局
        for (j = 0; j < 19; ++j)
          Board[i][j] = EMPTY;
      start = 1;

      if (computerSide == BLACK)
      {
        /**********生成第一手着法，并保存在step结构中，落子坐标为(step.first.x,step.first.y)**********/
        /****************************在下方填充代码，并替换我的示例代码******************************/


        step.first.x = 9;
        step.first.y = 9;


        /******************************在上面填充第一步行棋代码*******************************************/

        Board[step.first.x][step.first.y] = computerSide;//处理己方行棋
        printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//输出着法
      }
    }
    else if (strcmp(message, "move") == 0)//行棋,本程序核心
    {
      scanf("%s", message);//获取对手行棋着法
      fflush(stdin);
      step.first.x = message[0] - 'A';		step.first.y = message[1] - 'A';
      step.second.x = message[2] - 'A';		step.second.y = message[3] - 'A';
      //处理对手行棋
      Board[step.first.x][step.first.y] = 1 - computerSide;
      if (!(step.second.x == -1 && step.second.y == -1)) Board[step.second.x][step.second.y] = 1 - computerSide;

    /**********************************************************************************************************/
    /***生成落子的坐标，保存在step结构中，第1子下在(step.first.x,step.first.y)，第2子下在(step.first.x,step.first.y)*****/
    /**************************************在下方填充代码，并替换我的示例代码*****************************************/
      
      
      
      step = machine(Board,computerSide);

      
      
    /*****************************************在上面填充代码******************************************************/
    /**********************************************************************************************************/

      printf("move %c%c%c%c\n", step.first.x + 'A', step.first.y + 'A', step.second.x + 'A', step.second.y + 'A');//输出着法
    }
    else if (strcmp(message, "error") == 0)//着法错误
    {
      fflush(stdin);
    }
    else if (strcmp(message, "end") == 0)//对局结束
    {
      fflush(stdin);
      start = 0;
    }
    else if (strcmp(message, "quit") == 0)//退出引擎
    {
      fflush(stdin);
      printf("Quit!\n");
      break;
    }
  }
  return 0;
}
