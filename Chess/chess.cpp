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
//路遍历的方向参考github上"遍历顺序以及起终点方向.png"
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
int IfNot_Road(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY, int &flag, int dir,int limit,Point LimitChess) {//判断是否是一条路
  int num = 0;//如果是路，其中的棋子个数
  int MoveRoadX[4] = { 0,1,1,1 };//方向数组
  int MoveRoadY[4] = { 1,0,-1,1 };
  if (BeginX < 0 || BeginX >= 19 || EndX < 0 || EndX >= 19) return -1;//以搜索范围内所有格子为起点，扩大了搜索范围
  if (BeginY < 0 || BeginY >= 19 || EndY < 0 || EndY >= 19) return -1;//增加本函数形参RangeBeginX/Y、RangeEndX/Y,目前的EndBegin是路的起点终点
  //2代表还未遇到棋子，1白子，0黑子。
  int a = BeginX, b = BeginY;
  for (int i = 0; i < 6; i++) {
    a = BeginX + i * MoveRoadX[dir];//不同的方向
    b = BeginY + i * MoveRoadY[dir];
    if (limit==1&&LimitChess.x == a && LimitChess.y == b) return -1;//避免评价函数中重复评价路使用
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
Step PreSeek_ReturnEmpty(int Board[19][19], int BeginX, int BeginY,int dir,int WillWin,int ComputerSide) {//寻找一条路上有>=4个棋子的空位

  int MoveRoadX[4] = { 0,1,1,1 };//用于路上的移动
  int MoveRoadY[4] = { 1,0,-1,1 };//注意：此函数输入的已经确认是num>=4一条路
  
  Step ReturnEmpty;//此函数要满足活、眠的要求
  ReturnEmpty.first.x = -1;
  ReturnEmpty.first.y = -1;
  ReturnEmpty.second.x = -1;
  ReturnEmpty.second.y = -1;
  
  int a = BeginX, b = BeginY;
  if (WillWin) {//己方已经形成连四连五的路,直接返回空的位置下子，可直接获胜
    for (int i = 0; i < 6; i++) {
      a = BeginX + i * MoveRoadX[dir];
      b = BeginY + i * MoveRoadY[dir];
      if (Board[a][b] == EMPTY) {
        if (ReturnEmpty.first.x != -1) {
          ReturnEmpty.second.x = a;
          ReturnEmpty.second.y = b;
          return ReturnEmpty;
        }
        else {
          ReturnEmpty.first.x = a;
          ReturnEmpty.first.y = b;
        }
      }
    }
    return ReturnEmpty;
  }
  else {//敌方形成连四连五的情况
    int num = 0;//用于查看是否有活四活五的情况
    int FirstNotEmptyFlag = 0;
    Point FirstNotEmpty;
    for (int i = 0; i < 6; i++) {//找第一个空位置，且与敌方子相邻。
      a = BeginX + i * MoveRoadX[dir];
      b = BeginY + i * MoveRoadY[dir];
      if (i != 5 && Board[a][b] == EMPTY && Board[a + MoveRoadX[dir]][b + MoveRoadY[dir]] != EMPTY) {//使目标位置紧贴当前棋子
        ReturnEmpty.first.x = a;
        ReturnEmpty.first.y = b;
        break;
      }
      else if (i == 5 && Board[a][b] == EMPTY) {
        ReturnEmpty.first.x = a;
        ReturnEmpty.first.y = b;
        break;
      }
    }
    //暂时改变Board

    for (int i = 0; i < 6; i++) {//有活四活五的情况，分析起始结点
      a = BeginX + i * MoveRoadX[dir];
      b = BeginY + i * MoveRoadY[dir];
      if (Board[a][b] != EMPTY && FirstNotEmptyFlag == 0) {//标记第一个路上非空的位置，用于检测活四等必须落两个子的情况
        FirstNotEmptyFlag = 1;
        FirstNotEmpty.x = a;
        FirstNotEmpty.y = b;
      }
    }
    int OriginColor = Board[ReturnEmpty.first.x][ReturnEmpty.first.y];
    Board[ReturnEmpty.first.x][ReturnEmpty.first.y] = ComputerSide;
    
    int flag = 2;
    Point Fake;
    Fake.x = -1;
    Fake.y = -1;
    num = IfNot_Road(Board, FirstNotEmpty.x, FirstNotEmpty.x + 5 * MoveRoadX[dir], FirstNotEmpty.y, FirstNotEmpty.y + 5 * MoveRoadY[dir], flag, dir,0,Fake);
    if (num >= 4) {
      for (int i = 0; i < 6; i++) {//找第一个空位置，且与敌方子相邻。
        a = FirstNotEmpty.x + i * MoveRoadX[dir];
        b = FirstNotEmpty.y + i * MoveRoadY[dir];
        if (i != 5 && Board[a][b] == EMPTY && Board[a + MoveRoadX[dir]][b + MoveRoadY[dir]] != EMPTY) {//使目标位置紧贴当前棋子
          ReturnEmpty.second.x = a;
          ReturnEmpty.second.y = b;
          break;
        }
        else if (i == 5 && Board[a][b] == EMPTY) {
          ReturnEmpty.second.x = a;
          ReturnEmpty.second.y = b;
          break;
        }
      }
    }
    Board[ReturnEmpty.first.x][ReturnEmpty.first.y] = OriginColor;
    return ReturnEmpty;
  }
  return ReturnEmpty;
}
Step PreSeek(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY,int ComputerSide) {//判断里面的所有路是否有活四，活五，眠五
  Step ReturnStep;
  ReturnStep.first.x = -1;
  ReturnStep.first.y = -1;
  ReturnStep.second.x = -1;
  ReturnStep.second.y = -1;

  int MoveRoadX[4] = { 0,1,1,1 };
  int MoveRoadY[4] = { 1,0,-1,1 };
  for (int i = BeginX; i <= EndX; i++) {//以目前搜索范围内所有格子为起点
    for (int j = BeginY; j <= EndY; j++) {
      for (int k = 0; k < 4; k++) {//一个起点有四个方向
        int num = 0;
        int flag = 2;
        Point Fake;
        Fake.x = -1;
        Fake.y = -1;
        num = IfNot_Road(Board, i, i + 5 * MoveRoadX[k], j, j + 5 * MoveRoadY[k], flag, k,0,Fake);
        if (num == -1||num<4) continue;
        if (ComputerSide==flag) {//己方连四或连五
          ReturnStep=PreSeek_ReturnEmpty(Board, i, j, k, 1,ComputerSide);
          return ReturnStep;
        }
        else {//敌方连四或连五
          ReturnStep = PreSeek_ReturnEmpty(Board, i, j, k, 0,ComputerSide);
          return ReturnStep;
        }
      }
    }
  }
  return ReturnStep;
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
  int sum = 0;                                            //如果Firstchess不为空，board中有FirstChess的标记
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//不同棋子数的路的条数
  int NumOfEnemyRoad[7] = { 0,0,0,0,0,0,0 };
  int ScoreOfRoad[7] = {};//不同棋子数的路的分数!!!
  int MoveRoadX[4] = { 0,1,1,1 };
  int MoveRoadY[4] = { 1,0,-1,1 };
  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < 6; i++) {//行上的路
      Point Begin;
      Begin.y = FirstChess.y - i * MoveRoadY[k];//定起点，与正常路遍历的方向相反
      Begin.x = FirstChess.x - i * MoveRoadX[k];
      int flag = 2;
      int num = IfNot_Road(Board, Begin.x, Begin.x+5*MoveRoadX[k], Begin.y, Begin.y + 5*MoveRoadY[k], flag, k,limit,LimitChess);
      if (num == -1) continue;
      else {
        if (flag == ComputerSide) {
          NumOfMyRoad[num]++;
        }
        else {
          NumOfEnemyRoad[num]++;
        }
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
  int Score1, Score2;
  Score1 = Board[FirstChess.x][FirstChess.y];//为了改变原数组
  Score2 = Board[SecondChess.x][SecondChess.y];
  
  int Before=PartScore_EvalueFucation(Board, FirstChess, SecondChess, ComputerSide, 0) + PartScore_EvalueFucation(Board, SecondChess, FirstChess, ComputerSide, 1);
  
  Board[FirstChess.x][FirstChess.y] = ComputerSide;
  Board[SecondChess.x][SecondChess.y] = ComputerSide;
  int After= PartScore_EvalueFucation(Board, FirstChess, SecondChess, ComputerSide, 0) + PartScore_EvalueFucation(Board, SecondChess, FirstChess, ComputerSide, 1);
  Board[FirstChess.x][FirstChess.y] = Score1;
  Board[SecondChess.x][SecondChess.y] = Score2;
  
  return After - Before;
}


queue<Step> GenerateSon(int Board[19][19],int BeginX,int EndX,int BeginY,int EndY) {//产生子节点队列函数
  
}
Step GetFrontNode(queue<Step> Son) {//得到队列头节点并pop掉
  Step ReturnNode;
  ReturnNode = Son.front();
  Son.pop();
  return ReturnNode;
}
int NegaMax_AlphaBeta(Step step, int Alpha,int Beta,int depth) {//负极大值搜索
  int value,BestValue = -INFINITY;
  Step CurrentNode;
  if (depth <= 0) {
    
  }
  queue<Step> Son;
 // Son=GenerateSon();
  while (!Son.empty()) {
    CurrentNode = GetFrontNode(Son);
    value = -NegaMax_AlphaBeta(CurrentNode, -Beta,-Alpha,depth - 1);
    if (value >= Alpha) {
      Alpha = value;
    }
    if (Alpha >= Beta) {
      break;
    }
  }
  return Alpha;
}
Step machine(int TureBoard[19][19],int ComputerSide) {
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
  PreSeekStep=PreSeek(Board, BeginX, EndX, BeginY, EndY,ComputerSide);//使用博弈树之前进行前期扫描，判断是否有活四活五等必须落子的情况
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
