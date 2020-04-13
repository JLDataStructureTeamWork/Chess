#define _CRT_SECURE_NO_WARNINGS
#include<queue>
#include<limits.h>
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

int MoveRoadX[4] = { 0,1,1,1 };//遍历坐标移动数组
int MoveRoadY[4] = { 1,0,-1,1 };

int AlertScore=0;

int ScoreMyRoadAttack[7] = {0,10,100,150,350,250,10000};//进攻型、防守型参数
int ScoreEnemyRoadAttack[7] = {0,1,10,15,35,25,10000};
int ScoreMyRoadDefend[7] = {0, 1,10,15,35,25,10000 };
int ScoreEnemyRoadDefend[7] = {0, 10,100,150,350,250,10000 };

int Width;//博弈树宽度
int Depth;//博弈树深度

int FirstGet = 0;//==1 代表preseek已经找到一个迫著点

struct Point { //点结构
  int x, y;
};
struct Step { //步结构
  Point first, second;
  int value;
};
struct PointQueue {
  int x, y;
  int value;
  bool operator <(const PointQueue &a)const {//先弹出小的value
    return value > a.value;
  }
};
struct StepQueue { //步结构
  Point first, second;
  int value;
  bool operator <(const StepQueue &a)const {//先弹出小的value
    return value > a.value;
  }  
};
struct TreeNode {//博弈树使用的节点
  vector<Point>ComputerChess;//我方落子情况
  vector<Point>EnemyChess;//敌方落子情况
  int BeginX, BeginY;//范围
  int EndX, EndY;
  int Color;//1代表自己，0代表敌方
  int value;
};

Point First;//迫著点

int Board[19][19];//存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一、路遍历的方向参考github上"遍历顺序以及起终点方向.png"
//已测试        

int IfNot_Road(int Board[19][19], int BeginX, int BeginY, int &flag, int dir, int limit, Point LimitChess) {//判断是否是一条路
  
  int num = 0;//如果是路，其中的棋子个数
  
  if (BeginX < 0 || BeginX >19 || BeginX + 5 * MoveRoadX[dir] < 0 || BeginX + 5 * MoveRoadX[dir] > 19) return -1;//以搜索范围内所有格子为起点，扩大了搜索范围
  if (BeginY < 0 || BeginY >19 || BeginY + 5 * MoveRoadX[dir]< 0 || BeginY + 5 * MoveRoadX[dir] >19) return -1;//增加本函数形参RangeBeginX/Y、RangeEndX/Y,目前的EndBegin是路的起点终点
  //2代表还未遇到棋子，1白子，0黑子。
  
  int a;
  int b;

  for (int i = 0; i < 6; i++) {
    a = BeginX + i * MoveRoadX[dir];//不同的方向
    b = BeginY + i * MoveRoadY[dir];
    if (limit == 1 && LimitChess.x == a && LimitChess.y == b) return -1;//避免评价函数中重复评价路使用
    else if (Board[a][b] == EMPTY) continue;
    else if (Board[a][b] == BLACK && flag == 1) return -1;//返回-1代表不是路
    else if (Board[a][b] == WHITE && flag == 0) return -1;
    else if (Board[a][b] == BLACK) {
      num++;
      flag = 0;
    }
    else if (Board[a][b] == WHITE) {
      num++;
      flag = 1;
    }
  }
  return num;//返回棋子个数
}

int ALL_EvalueFucation(int VirtualBoard[19][19],int ComputerSide) {//全局评价函数
  
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//不同棋子数的路的条数
  int NumOfEnemyRoad[7]= { 0,0,0,0,0,0,0 };
  
  for (int dir = 0; dir < 4; dir++) {
    for (int i = 0; i < 19; i++) {//对每行的路进行分析
      for (int j = 0; j <19; j++) {
        int flag = 2;
        int limit = 0;
        
        Point fake;//假的，只为了填补形参，可以忽略
        fake.x = -1;fake.y = -1;
        
        int num = IfNot_Road(VirtualBoard, i, j, flag, dir, limit, fake);
        
        if (num == -1)continue;
        if (flag == ComputerSide) NumOfMyRoad[num]++;
        else NumOfEnemyRoad[num]++;
      }
    }
  }
  int Score=0;
  for (int i = 0; i < 7; i++) {
    Score += NumOfMyRoad[i] * ScoreMyRoadAttack[i] - NumOfEnemyRoad[i] * ScoreEnemyRoadAttack[i];//计算分数
  }
  return Score;
}

Step PreSeek_ReturnEmpty(int Board[19][19], int BeginX, int BeginY, int dir, int WillWin, int ComputerSide) {//寻找一条路上有>=4个棋子的空位
  int EnemySide;//敌人的颜色
  if (ComputerSide == BLACK)EnemySide = WHITE;
  else EnemySide = BLACK;
  Step ReturnEmpty;//此函数要满足活、眠的要求
  ReturnEmpty.first.x = -1;
  ReturnEmpty.first.y = -1;
  ReturnEmpty.second.x = -1;
  ReturnEmpty.second.y = -1;


  int a;
  int b;

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
    if (ReturnEmpty.second.x == -1) {
      ReturnEmpty.second.x = rand() % 19;
      ReturnEmpty.second.y = rand() % 19;
      while (Board[ReturnEmpty.second.x][ReturnEmpty.second.y] != EMPTY) {
        ReturnEmpty.second.x = rand() % 19;
        ReturnEmpty.second.y = rand() % 19;
      }
    }
    return ReturnEmpty;
  }
  else {//敌方形成连四连五的情况 

    int FirstNotEmptyFlag = 0;

    Point FirstNotEmpty;//第一个棋子的位置
    Point LastNotEmpty;//最后一个棋子的位置

    for (int i = 0; i < 6; i++) {//找第一个空位置，且与敌方子相邻。
      a = BeginX + i * MoveRoadX[dir];
      b = BeginY + i * MoveRoadY[dir];
      if (i != 5 && Board[a][b] == EMPTY && Board[a + MoveRoadX[dir]][b + MoveRoadY[dir]] == EnemySide) {//使目标位置紧贴当前棋子
        ReturnEmpty.first.x = a;
        ReturnEmpty.first.y = b;
        break;
      }
      else if (i != 0 && Board[a][b] == EMPTY && Board[a - MoveRoadX[dir]][b - MoveRoadY[dir]] == EnemySide) {
        ReturnEmpty.first.x = a;
        ReturnEmpty.first.y = b;
        break;
      }
    }
    for (int i = 0; i < 6; i++) {//有活四活五的情况，分析起始结点
      a = BeginX + i * MoveRoadX[dir];
      b = BeginY + i * MoveRoadY[dir];
      if (Board[a][b] == EnemySide && FirstNotEmptyFlag == 0) {//标记第一个路上非空的位置，用于检测活四等必须落两个子的情况
        FirstNotEmptyFlag = 1;
        FirstNotEmpty.x = a;
        FirstNotEmpty.y = b;
      }
      if (Board[a][b] == EnemySide) {
        LastNotEmpty.x = a;
        LastNotEmpty.y = b;
      }
    }

    int OriginColor = Board[ReturnEmpty.first.x][ReturnEmpty.first.y];//暂时改变board，模拟之前只落一子的情况
    Board[ReturnEmpty.first.x][ReturnEmpty.first.y] = ComputerSide;

    int flag = 2;
    int limit = 0;

    Point Fake;
    Fake.x = -1;
    Fake.y = -1;
    int num1 = IfNot_Road(Board, FirstNotEmpty.x, FirstNotEmpty.y, flag, dir, limit, Fake);
    int num2 = IfNot_Road(Board, LastNotEmpty.x - 5 * MoveRoadX[dir], LastNotEmpty.y - 5 * MoveRoadY[dir], flag, dir, limit, Fake);

    if (num1 >= 4 || num2 >= 4) {//从第一个结点正向遍历、从最后一个结点反向遍历
      Point FindSecondChess;
      if (num1 >= 4) {
        FindSecondChess.x = FirstNotEmpty.x;
        FindSecondChess.y = FirstNotEmpty.y;
      }
      else {
        FindSecondChess.x = LastNotEmpty.x - 5 * MoveRoadX[dir];
        FindSecondChess.y = LastNotEmpty.y - 5 * MoveRoadY[dir];
      }

      for (int i = 0; i < 6; i++) {//找第一个空位置，且与敌方子相邻。
        a = FindSecondChess.x + i * MoveRoadX[dir];
        b = FindSecondChess.y + i * MoveRoadY[dir];
        if (i != 5 && Board[a][b] == EMPTY && Board[a + MoveRoadX[dir]][b + MoveRoadY[dir]] == EnemySide) {//使目标位置紧贴当前棋子
          ReturnEmpty.second.x = a;
          ReturnEmpty.second.y = b;
          break;
        }
        else if (i != 0 && Board[a][b] == EMPTY && Board[a - MoveRoadX[dir]][b - MoveRoadY[dir]] == EnemySide) {
          ReturnEmpty.second.x = a;
          ReturnEmpty.second.y = b;
          break;
        }
      }
    }
    Board[ReturnEmpty.first.x][ReturnEmpty.first.y] = OriginColor;//恢复board
    return ReturnEmpty;//返回必填子的坐标
  }
  return ReturnEmpty;
}

Step PreSeek(int Board[19][19], int RangeBeginX, int RangeBeginY, int RangeEndX, int RangeEndY, int ComputerSide) {//判断里面的所有路是否有活四，活五，眠五
  Step ReturnStep;
  ReturnStep.first.x = -1;
  ReturnStep.first.y = -1;
  ReturnStep.second.x = -1;
  ReturnStep.second.y = -1;

  for (int i = RangeBeginX; i <= RangeEndX; i++) {//以目前搜索范围内所有格子为起点
    for (int j = RangeBeginY; j <= RangeEndY; j++) {
      for (int dir = 0; dir < 4; dir++) {//一个起点有四个方向

        int num = 0;
        int flag = 2;
        int limit = 0;
        Point Fake;
        Fake.x = -1;
        Fake.y = -1;

        num = IfNot_Road(Board, i, j, flag, dir, limit, Fake);
        if (num == -1 || num < 4) continue;

        int WillWin;
        if (ComputerSide == flag) {//己方连四或连五
          WillWin = 1;
          ReturnStep = PreSeek_ReturnEmpty(Board, i, j, dir, WillWin, ComputerSide);
          return ReturnStep;
        }
        else {//敌方连四或连五
          WillWin = 0;
          ReturnStep = PreSeek_ReturnEmpty(Board, i, j, dir, WillWin, ComputerSide);
          return ReturnStep;
        }
      }
    }
  }
  return ReturnStep;
}

void BoardRange(int Board[19][19],int &RangeBeginX, int &RangeBeginY,int &RangeEndX,int& RangeEndY ) {//20,-1,20,-1//已测试
  
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 19; j++) {
      if (Board[i][j] != EMPTY) {
        if (i < RangeBeginX) RangeBeginX = i;
        if (i > RangeEndX)RangeEndX = i;
        if (j < RangeBeginY)RangeBeginY = j;
        if (j > RangeEndY)RangeEndY = j;
      }
    }
  }
  for (int i = 1; i <= 2; i++) {//将范围扩大两个单位，得到需要分析落子的区域
    if (RangeBeginX != 0) RangeBeginX--;
    if (RangeBeginY != 0) RangeBeginY--;
    if (RangeEndX != 18) RangeEndX++;
    if (RangeEndY != 18) RangeEndY++;
  }
  return;
}

int PartScore_EvalueFucation(int Board[19][19], Point FirstChess, Point LimitChess,int ComputerSide,int limit,int BaseScore) {
  int sum = 0;                           //如果Firstchess不为空，board中有FirstChess的标记
  
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//不同棋子数的路的条数
  int NumOfEnemyRoad[7] = { 0,0,0,0,0,0,0 };
  
  for (int dir = 0; dir < 4; dir++) {
    for (int i = 0; i < 6; i++) {
      Point Begin;
      Begin.y = FirstChess.y - i * MoveRoadY[dir];//定起点，与正常路遍历的方向相反
      Begin.x = FirstChess.x - i * MoveRoadX[dir];
      int flag = 2;
      int num = IfNot_Road(Board, Begin.x, Begin.y, flag, dir,limit,LimitChess);
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
  int score=0;
  for (int i = 0; i < 7; i++) {
    score += NumOfMyRoad[i] * ScoreMyRoadAttack[i] - NumOfEnemyRoad[i] * ScoreEnemyRoadAttack[i];//进攻
  }
  if (BaseScore + score > AlertScore) return score;
  else {
    score = 0;
    for (int i = 0; i < 7; i++) {
      score += NumOfMyRoad[i] * ScoreMyRoadDefend[i] - NumOfEnemyRoad[i] * ScoreEnemyRoadDefend[i];//防守
    }
  }
  return score;
}

int Part_EvalueFucation(int Board[19][19],Point FirstChess,Point SecondChess,int ComputerSide,int BaseScore) {//这两个棋子已经填入了board
 
  Board[FirstChess.x][FirstChess.y] = EMPTY;//改变原数组
  Board[SecondChess.x][SecondChess.y] = EMPTY;
  int Before=PartScore_EvalueFucation(Board, FirstChess, SecondChess, ComputerSide, 0,BaseScore) + PartScore_EvalueFucation(Board, SecondChess, FirstChess, ComputerSide, 1,BaseScore);
  
  Board[FirstChess.x][FirstChess.y] = ComputerSide;//恢复原数组
  Board[SecondChess.x][SecondChess.y] = ComputerSide;
  int After= PartScore_EvalueFucation(Board,FirstChess, SecondChess, ComputerSide, 0,BaseScore) + PartScore_EvalueFucation(Board,SecondChess, FirstChess, ComputerSide, 1,BaseScore);
  
  return After - Before;//返回局部评分
}


Step GetFrontNode(queue<Step> Son) {//得到队列头节点并pop掉
  Step ReturnNode;
  ReturnNode = Son.front();
  Son.pop();
  return ReturnNode;
}

queue<Step>GeneraSon(int Board[19][19], int  RangeBeginX, int RangeBeginY, int RangeEndX, int RangeEndY, int ComputerSide, int SeekOne, int MaxSelf,int BaseScore) {
  int EnemySide;//敌人的颜色
  if (ComputerSide == BLACK)EnemySide = WHITE;
  else EnemySide = BLACK;

  queue<Step> GeneraSonReturn;
  priority_queue <PointQueue> FindPoint;//先找点
  priority_queue<StepQueue> FindStep;//找两个点
  
  PointQueue FirstChessQueue;//存入优先队列的结点
  StepQueue TwoChessQueue;//存入队列的结点
 
  int NumFindPoint = 0;
  int NumFindStep = 0;

  for (int i = RangeBeginX; i <= RangeEndX; i++) {
    for (int j = RangeBeginY; j <= RangeEndY; j++) {
      
      if (Board[i][j] != EMPTY) continue;//非空点直接舍弃
      
      if(MaxSelf) Board[i][j] = ComputerSide;//模拟下子
      else Board[i][j] = EnemySide;
      
      Point FirstChess;
      FirstChess.x = i;
      FirstChess.y = j;
      
      int score = Part_EvalueFucation(Board, FirstChess, FirstChess, ComputerSide,BaseScore);//局部评价函数
      
      Board[i][j] = EMPTY;//恢复棋盘
      
      if (MaxSelf == 0) {
        score = score * (-1);//找敌方的点，则需要弹出最大的点，取相反数
      }
      FirstChessQueue.x = i;
      FirstChessQueue.y = j;
      FirstChessQueue.value = score;

      if (NumFindPoint >= Width) {//当超过Width个数的时候
        int TempValue=FindPoint.top().value;
        if (TempValue > score) continue;
        FindPoint.pop();
      }
      FindPoint.push(FirstChessQueue);
      NumFindPoint++;
    }
  }
  if (SeekOne) {//有一个迫著点的情况
    while (!FindPoint.empty()) {
      Step ReturnStep;
      ReturnStep.first.x = First.x;
      ReturnStep.first.y = First.y;
      ReturnStep.second.x = FindPoint.top().x;
      ReturnStep.second.y = FindPoint.top().y;
      GeneraSonReturn.push(ReturnStep);//压入返回队列
      FindPoint.pop();
    }
    return GeneraSonReturn;
  }
  while (!FindPoint.empty()) {//没有迫著点
    int a = FindPoint.top().x;//前Width个点
    int b = FindPoint.top().y;
    FindPoint.pop();  
    Point FirstChess, SecondChess;//两个落子，用于局部评估
    FirstChess.x = a;
    FirstChess.y = b;
    
    if(MaxSelf) Board[a][b] = ComputerSide;//模拟落点
    else Board[a][b] = EnemySide;

    for (int i = RangeBeginX; i <= RangeEndX; i++) {
      for (int j = RangeBeginY; j <= RangeEndY; j++) {
        if (Board[i][j] != EMPTY) continue;
        
        SecondChess.x = i;
        SecondChess.y = j;

        if (MaxSelf) Board[i][j] = ComputerSide;//模拟落点
        else Board[i][j] = EnemySide;
        
        int score = Part_EvalueFucation(Board, FirstChess, SecondChess, ComputerSide,BaseScore);
        if (MaxSelf == 0) score = score * (-1);
        if (NumFindStep >= Width) {//队列满了的处理
          int CurrentValue=FindStep.top().value;
          if (CurrentValue > score) continue;
          FindStep.pop();
        }
        TwoChessQueue.first.x = a; TwoChessQueue.second.x = i;//压入返回队列
        TwoChessQueue.first.y = b; TwoChessQueue.second.y = j;
        FindStep.push(TwoChessQueue);
      }
    }
  }
  while (!FindStep.empty()) {
    Step ReturnStep;
    ReturnStep.first.x = FindStep.top().first.x;
    ReturnStep.first.y = FindStep.top().first.y;
    ReturnStep.second.x = FindStep.top().second.x;
    ReturnStep.second.y = FindStep.top().second.y;
    GeneraSonReturn.push(ReturnStep);
    FindStep.pop();
  }
  return GeneraSonReturn;
}

int NegaMax_AlphaBeta(TreeNode *Node, int Alpha,int Beta,int depth,int ComputerSide) {//负极大值搜索
  
  if (depth <= 0) {
    return Node->value;//返回评估值结束递归
  }
  int NegaBoard[19][19];//当前父节点界面
  
  int EnemySide;//敌人的颜色
  if (ComputerSide == BLACK)EnemySide = WHITE;
  else EnemySide = BLACK;

  for (int i = 0; i < 19; i++) {//数组初始化
    for (int j = 0; j < 19; j++) {
      NegaBoard[i][j] = EMPTY;
    }
  }
  for (int i = 0; i < Node->ComputerChess.size(); i++) {
    NegaBoard[Node->ComputerChess[i].x][Node->ComputerChess[i].y] = ComputerSide;
  }
  for (int i = 0; i < Node->EnemyChess.size(); i++) {
    NegaBoard[Node->EnemyChess[i].x][Node->EnemyChess[i].y] = EnemySide;
  }
  
  int value;
 
  queue<Step> SonQueue;//儿子队列
  int SonColor,MaxSelf;
  if (Node->Color == 1) {
    SonColor = EnemySide;
    MaxSelf = 0;
  }
  else {
    SonColor = ComputerSide;
    MaxSelf = 1;
  }
  SonQueue=GeneraSon(Board,Node->BeginX,Node->BeginY,Node->EndX,Node->EndY,ComputerSide,0,MaxSelf,Node->value);//通过父节点的界面来生成儿子结点
  
  while (!SonQueue.empty()) {//对每个可能的两个落子建立博弈树
    
    Step NextSonStep;
    NextSonStep = GetFrontNode(SonQueue);
    
    Point FirstStep, SecondStep;//两步落子
    FirstStep.x = NextSonStep.first.x;
    FirstStep.y = NextSonStep.first.y;
    SecondStep.x = NextSonStep.second.x;
    SecondStep.y = NextSonStep.second.y;
    NegaBoard[FirstStep.x][FirstStep.y] = SonColor;
    NegaBoard[SecondStep.x][SecondStep.y] = SonColor;

    TreeNode *TreeSon = (TreeNode *)malloc(sizeof(TreeNode));
    TreeSon->ComputerChess.assign(Node->ComputerChess.begin(), Node->ComputerChess.end());
    TreeSon->EnemyChess.assign(Node->EnemyChess.begin(), Node->EnemyChess.end());
    TreeSon->ComputerChess.push_back(FirstStep);
    TreeSon->ComputerChess.push_back(SecondStep);
    if (Node->Color == 1) TreeSon->Color = 0;
    else TreeSon->Color = 1;
    
    TreeSon->value = Part_EvalueFucation(NegaBoard, FirstStep, SecondStep, ComputerSide,Node->value)+Node->value;
    
    value = -NegaMax_AlphaBeta(TreeSon, -Beta, -Alpha, depth - 1, ComputerSide);//论文中的方法，简洁了代码，但没看懂(

    if (value >= Alpha) {
      Alpha = value;
    }
    if (Alpha >= Beta) {
      break;
    }
  }
  return Alpha;
}

Step machine(int ComputerSide) {
  
  int EnemySide;//敌人的颜色
  if (ComputerSide == BLACK)EnemySide = WHITE;
  else EnemySide = BLACK;


  int RangeBeginX = 20; int RangeBeginY = 20;//目前搜索范围初始化
  int RangeEndX = -1; int RangeEndY = -1;
  BoardRange(Board, RangeBeginX, RangeBeginY, RangeEndX, RangeEndY);//首先探明界面范围

  Step PreSeekStep;
  Step Current;
 
  PreSeekStep=PreSeek(Board, RangeBeginX, RangeBeginY, RangeEndX, RangeEndY, ComputerSide);//使用博弈树之前进行前期扫描，判断是否有活四活五等必须落子的情况
  
  if (PreSeekStep.second.x != -1) {
    return PreSeekStep;
  }
  else {
    First.x = PreSeekStep.first.x;
    First.y = PreSeekStep.second.y;
    FirstGet = 1;
  }
  //----------博弈树部分------------//

  int Max_Score = INT_MIN;

  Step ReturnStep;//返回值初始化
  ReturnStep.first.x = -1; ReturnStep.first.y = -1;
  ReturnStep.second.x = -1; ReturnStep.second.y = -1;
  
  queue<Step> ImpossibleFact;//根节点儿子队列
  int BaseScore = ALL_EvalueFucation(Board, ComputerSide);
  ImpossibleFact=GeneraSon(Board, RangeBeginX,RangeBeginY,RangeEndX,RangeEndY,ComputerSide,FirstGet,1,BaseScore);
  
  while (!ImpossibleFact.empty()) {//把所有可能的第一层的下子情况，进行NegaMax估值
    
    int Alpha = INT_MIN;
    int Beta = INT_MAX;
    
    Current= GetFrontNode(ImpossibleFact);

    Board[Current.first.x][Current.first.y] = ComputerSide;//落两子,后面有恢复
    Board[Current.second.x][Current.second.y] = ComputerSide;
    
    TreeNode *Node = (TreeNode *)malloc(sizeof(TreeNode));//根节点
    
    for (int i = 0; i < 19; i++) {
      for (int j = 0; j < 19; j++) {
        Point Chess;
        Chess.x = i; Chess.y = j;
        if (Board[i][j] == EMPTY) continue;
        else if (Board[i][j] == ComputerSide) Node->ComputerChess.push_back(Chess);//加入树中的结点vector
        else if (Board[i][j] == EnemySide) Node->EnemyChess.push_back(Chess);
      }
    }

    Node->BeginX = RangeBeginX; Node->BeginY = RangeBeginY;
    Node->EndX = RangeEndX; Node->EndY = RangeEndY;
    Node->Color = 1;//自己
    Node->value = ALL_EvalueFucation(Board, ComputerSide);

    int temp = NegaMax_AlphaBeta(Node, Alpha, Beta, Depth,ComputerSide);//得到根节点评分
    if (temp > Max_Score) {
      ReturnStep.first.x = Current.first.x;//保留目前分数最高的两个落子
      ReturnStep.first.y = Current.first.y;
      ReturnStep.second.x = Current.second.x;
      ReturnStep.second.y = Current.second.y;
    }
    Board[Current.first.x][Current.first.y] = EMPTY;//恢复棋盘
    Board[Current.second.x][Current.second.y] = EMPTY;
  }
  return ReturnStep;//返回两个落子
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
      
      
      
      step = machine(computerSide);

      
      
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
