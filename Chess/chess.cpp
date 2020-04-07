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

struct Point { //��ṹ
  int x, y;
};
struct Step { //���ṹ
  Point first, second;
  int value;
};

int Board[19][19];//�洢������Ϣ����Ԫ��ֵΪ BLACK, WHITE, EMPTY ֮һ

int ALL_EvalueFucation(int VirtualBoard[19][19],int BeginX,int EndX,int BeginY,int EndY,int ComputerSide) {//ȫ�����ۺ���
  
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//��ͬ��������·������
  int NumOfEnemyRoad[7]= { 0,0,0,0,0,0,0 };
  int ScoreOfRoad[7] = {};//��ͬ��������·�ķ���
  
  for (int dir = 0; dir < 4; dir++) {
    for (int i = BeginX; i <= EndX; i++) {//��ÿ�е�·���з���
      for (int j = BeginY; j <= EndY; j++) {
        int num = 0;
        int flag = 0;//flag������������ɫ�ı仯��û����������Ϊ0����������Ϊ1����������Ϊ2��ǰ��������ͬɫ������break
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
          
          if ((VirtualBoard[a][b] == BLACK && flag == 2) || (VirtualBoard[a][b] == WHITE && flag == 1)) {//ǰ��������ɫ��ͬ���·����
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
int IfNot_Road(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY, int &flag, int dir) {//�ж��Ƿ���һ��·
  int num = 0;
  int MoveRoadX[4] = { 0,1,1,1 };
  int MoveRoadY[4] = { 1,0,-1,1 };
  if (BeginX < 0 || BeginX >= 19 || EndX < 0 || EndX >= 19) return -1;
  if (BeginY < 0 || BeginY >= 19 || EndY < 0 || EndY >= 19) return -1;
  //2����δ�������ӣ�1���ӣ�0���ӡ�
  int a = BeginX, b = BeginY;
  for (int i = 0; i < 6; i++) {
    if (i != 0) {//����·��ͷ�������
      a += MoveRoadX[dir];
      b += MoveRoadY[dir];
    }
    if (Board[a][b] == EMPTY) continue;
    if (Board[a][b] == BLACK && flag == 1) return -1;//����-1������·
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
  return num;//�������Ӹ���
}
Step PreSeek_ReturnEmpty(int Board[19][19], int BeginX, int BeginY,int dir) {//Ѱ��һ��·����>=4�����ӵĿ�λ
  int MoveRoadX[4] = { 0,1,1,1 };//����·�ϵ��ƶ������ж���һ���ǿ�
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
Step PreSeek(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY) {//�ж����������·�Ƿ��л��ģ����壬����
  for (int i = BeginX; i <= EndX; i++) {
    for (int j = BeginY; j <= EndY; j++) {
      int num;
      int flag = 2;
      num = IfNot_Road(Board, i, i, j, j + 5, flag, 0);
      if (flag != 2) {//�Ըõ�Ϊ�������ϵ�·��·
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 0);
        }
      }
      num = IfNot_Road(Board, i, i, j, j + 5, flag, 1);
      if (flag != 2) {//�Ըõ�Ϊ�������ϵ�·��·
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 1);
        }
      }
      num = IfNot_Road(Board, i, i+5, j, j - 5, flag, 2);
      if (flag != 2) {//�Ըõ�Ϊ����������б��·��·
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 2);
        }
      }
      num = IfNot_Road(Board, i, i+5, j, j + 5, flag, 3);
      if (flag != 2) {//�Ըõ�Ϊ����������б��·��·
        if (num >= 4) {
          return PreSeek_ReturnEmpty(Board, i, j, 3);
        }
      }
    }
  }
  return;
}

void BoardRange(int Board[19][19],int &BeginX,int &EndX,int &BeginY,int &EndY) {//20,-1,20,-1//�Ѳ���
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
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//��ͬ��������·������
  int NumOfEnemyRoad[7] = { 0,0,0,0,0,0,0 };
  int ScoreOfRoad[7] = {};//��ͬ��������·�ķ���!!!

  for (int i = 0; i < 6; i++) {//���ϵ�·
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
  for (int i = 0; i < 6; i++) {//���ϵ�·
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
  for (int i = 0; i < 6; i++) {//������б
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
  for (int i = 0; i < 6; i++) {//������б
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
void EvalueFucation() {//�Բ������ڵ���������

}
queue<Step> GenerateSon() {//�����ӽڵ���к���

}
Step GetFrontNode(queue<Step> Son) {//�õ�����ͷ�ڵ㲢pop��
  Step ReturnNode;
  ReturnNode = Son.front();
  Son.pop();
  return ReturnNode;
}
int NegaMax(Step step, int depth) {//������ֵ����
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
  int Board[19][19];//Ϊ�˲��ı��������еĽ���
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 19; j++) {
      Board[i][j] = TureBoard[i][j];
    }
  }
  Step NextTwoStep;
  int BeginX = 20, BeginY = 20;
  int EndX = -1, EndY = -1;
  BoardRange(Board, BeginX, EndX, BeginY, EndY);//����̽�����淶Χ
  for (int i = 1; i <= 2; i++) {//����Χ����������λ���õ���Ҫ�������ӵ�����
    if (BeginX != 0) BeginX--;
    if (BeginY != 0) BeginY--;
    if (EndX != 18) EndX++;
    if (EndY != 18) EndY++;
  }

  Step PreSeekStep;
  int FirstChess_Flag = 0;//1����һ�������Ѿ�ȷ��
  PreSeekStep=PreSeek(Board, BeginX, EndX, BeginY, EndY);//ʹ�ò�����֮ǰ����ǰ��ɨ�裬�ж��Ƿ��л��Ļ���ȱ������ӵ����
  if (PreSeekStep.first.x != -1) {
    NextTwoStep.first.x = PreSeekStep.first.x;
    NextTwoStep.first.y = PreSeekStep.first.y;
    FirstChess_Flag = 1;
  }
  if (PreSeekStep.second.x != -1) {
    NextTwoStep.second.x = PreSeekStep.second.x;
    NextTwoStep.second.y = PreSeekStep.second.y;
    return NextTwoStep;//�������Ӿ�ȷ����ֱ�ӷ���
  }
  //----------����������------------//
  //�Խ�����з���


}

int main()
{
  Step step;//��ʱ���ṹ
  char message[256];//ͨ����Ϣ����
  int computerSide;//����ִ����ɫ
  int start = 0;//�Ծֿ�ʼ���
  srand(int(time(0)));
  //�˴����ó�ʼ������
    //...

  while (1)	//������ѭ��
  {
    fflush(stdout);//��Ҫɾ������䣬�������������
    scanf("%s", message);//��ȡƽ̨������Ϣ
        //��������
    if (strcmp(message, "name?") == 0)//���սƽ̨���Ͷ���
    {
      fflush(stdin);
      /***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
      /*******/		printf("name ���ֵ�һλ\n");		/**ֻ�޸�����壬��Ҫɾ��name�ո�****/
      /***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
    }
    else if (strcmp(message, "new") == 0)//���������
    {
      int i, j;
      scanf("%s", message);//��ȡ����ִ����ɫ
      fflush(stdin);
      if (strcmp(message, "black") == 0)	computerSide = BLACK;  //ִ��
      else  computerSide = WHITE;   //ִ��

      for (i = 0; i < 19; ++i)   //��ʼ�����
        for (j = 0; j < 19; ++j)
          Board[i][j] = EMPTY;
      start = 1;

      if (computerSide == BLACK)
      {
        /**********���ɵ�һ���ŷ�����������step�ṹ�У���������Ϊ(step.first.x,step.first.y)**********/
        /****************************���·������룬���滻�ҵ�ʾ������******************************/


        step.first.x = 9;
        step.first.y = 9;


        /******************************����������һ���������*******************************************/

        Board[step.first.x][step.first.y] = computerSide;//����������
        printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//����ŷ�
      }
    }
    else if (strcmp(message, "move") == 0)//����,���������
    {
      scanf("%s", message);//��ȡ���������ŷ�
      fflush(stdin);
      step.first.x = message[0] - 'A';		step.first.y = message[1] - 'A';
      step.second.x = message[2] - 'A';		step.second.y = message[3] - 'A';
      //�����������
      Board[step.first.x][step.first.y] = 1 - computerSide;
      if (!(step.second.x == -1 && step.second.y == -1)) Board[step.second.x][step.second.y] = 1 - computerSide;

    /**********************************************************************************************************/
    /***�������ӵ����꣬������step�ṹ�У���1������(step.first.x,step.first.y)����2������(step.first.x,step.first.y)*****/
    /**************************************���·������룬���滻�ҵ�ʾ������*****************************************/
      
      
      
      step = machine(Board,computerSide);

      
      
    /*****************************************������������******************************************************/
    /**********************************************************************************************************/

      printf("move %c%c%c%c\n", step.first.x + 'A', step.first.y + 'A', step.second.x + 'A', step.second.y + 'A');//����ŷ�
    }
    else if (strcmp(message, "error") == 0)//�ŷ�����
    {
      fflush(stdin);
    }
    else if (strcmp(message, "end") == 0)//�Ծֽ���
    {
      fflush(stdin);
      start = 0;
    }
    else if (strcmp(message, "quit") == 0)//�˳�����
    {
      fflush(stdin);
      printf("Quit!\n");
      break;
    }
  }
  return 0;
}
