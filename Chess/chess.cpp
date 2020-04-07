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
//·�����ķ���ο�github��"����˳���Լ����յ㷽��.png"
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
int IfNot_Road(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY, int &flag, int dir,int limit,Point LimitChess) {//�ж��Ƿ���һ��·
  int num = 0;//�����·�����е����Ӹ���
  int MoveRoadX[4] = { 0,1,1,1 };//��������
  int MoveRoadY[4] = { 1,0,-1,1 };
  if (BeginX < 0 || BeginX >= 19 || EndX < 0 || EndX >= 19) return -1;//��������Χ�����и���Ϊ��㣬������������Χ
  if (BeginY < 0 || BeginY >= 19 || EndY < 0 || EndY >= 19) return -1;//���ӱ������β�RangeBeginX/Y��RangeEndX/Y,Ŀǰ��EndBegin��·������յ�
  //2����δ�������ӣ�1���ӣ�0���ӡ�
  int a = BeginX, b = BeginY;
  for (int i = 0; i < 6; i++) {
    a = BeginX + i * MoveRoadX[dir];//��ͬ�ķ���
    b = BeginY + i * MoveRoadY[dir];
    if (limit==1&&LimitChess.x == a && LimitChess.y == b) return -1;//�������ۺ������ظ�����·ʹ��
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
Step PreSeek_ReturnEmpty(int Board[19][19], int BeginX, int BeginY,int dir,int WillWin,int ComputerSide) {//Ѱ��һ��·����>=4�����ӵĿ�λ

  int MoveRoadX[4] = { 0,1,1,1 };//����·�ϵ��ƶ�
  int MoveRoadY[4] = { 1,0,-1,1 };//ע�⣺�˺���������Ѿ�ȷ����num>=4һ��·
  
  Step ReturnEmpty;//�˺���Ҫ�����ߵ�Ҫ��
  ReturnEmpty.first.x = -1;
  ReturnEmpty.first.y = -1;
  ReturnEmpty.second.x = -1;
  ReturnEmpty.second.y = -1;
  
  int a = BeginX, b = BeginY;
  if (WillWin) {//�����Ѿ��γ����������·,ֱ�ӷ��ؿյ�λ�����ӣ���ֱ�ӻ�ʤ
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
  else {//�з��γ�������������
    int num = 0;//���ڲ鿴�Ƿ��л��Ļ�������
    int FirstNotEmptyFlag = 0;
    Point FirstNotEmpty;
    for (int i = 0; i < 6; i++) {//�ҵ�һ����λ�ã�����з������ڡ�
      a = BeginX + i * MoveRoadX[dir];
      b = BeginY + i * MoveRoadY[dir];
      if (i != 5 && Board[a][b] == EMPTY && Board[a + MoveRoadX[dir]][b + MoveRoadY[dir]] != EMPTY) {//ʹĿ��λ�ý�����ǰ����
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
    //��ʱ�ı�Board

    for (int i = 0; i < 6; i++) {//�л��Ļ���������������ʼ���
      a = BeginX + i * MoveRoadX[dir];
      b = BeginY + i * MoveRoadY[dir];
      if (Board[a][b] != EMPTY && FirstNotEmptyFlag == 0) {//��ǵ�һ��·�Ϸǿյ�λ�ã����ڼ����ĵȱ����������ӵ����
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
      for (int i = 0; i < 6; i++) {//�ҵ�һ����λ�ã�����з������ڡ�
        a = FirstNotEmpty.x + i * MoveRoadX[dir];
        b = FirstNotEmpty.y + i * MoveRoadY[dir];
        if (i != 5 && Board[a][b] == EMPTY && Board[a + MoveRoadX[dir]][b + MoveRoadY[dir]] != EMPTY) {//ʹĿ��λ�ý�����ǰ����
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
Step PreSeek(int Board[19][19], int BeginX, int EndX, int BeginY, int EndY,int ComputerSide) {//�ж����������·�Ƿ��л��ģ����壬����
  Step ReturnStep;
  ReturnStep.first.x = -1;
  ReturnStep.first.y = -1;
  ReturnStep.second.x = -1;
  ReturnStep.second.y = -1;

  int MoveRoadX[4] = { 0,1,1,1 };
  int MoveRoadY[4] = { 1,0,-1,1 };
  for (int i = BeginX; i <= EndX; i++) {//��Ŀǰ������Χ�����и���Ϊ���
    for (int j = BeginY; j <= EndY; j++) {
      for (int k = 0; k < 4; k++) {//һ��������ĸ�����
        int num = 0;
        int flag = 2;
        Point Fake;
        Fake.x = -1;
        Fake.y = -1;
        num = IfNot_Road(Board, i, i + 5 * MoveRoadX[k], j, j + 5 * MoveRoadY[k], flag, k,0,Fake);
        if (num == -1||num<4) continue;
        if (ComputerSide==flag) {//�������Ļ�����
          ReturnStep=PreSeek_ReturnEmpty(Board, i, j, k, 1,ComputerSide);
          return ReturnStep;
        }
        else {//�з����Ļ�����
          ReturnStep = PreSeek_ReturnEmpty(Board, i, j, k, 0,ComputerSide);
          return ReturnStep;
        }
      }
    }
  }
  return ReturnStep;
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
  int sum = 0;                                            //���Firstchess��Ϊ�գ�board����FirstChess�ı��
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//��ͬ��������·������
  int NumOfEnemyRoad[7] = { 0,0,0,0,0,0,0 };
  int ScoreOfRoad[7] = {};//��ͬ��������·�ķ���!!!
  int MoveRoadX[4] = { 0,1,1,1 };
  int MoveRoadY[4] = { 1,0,-1,1 };
  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < 6; i++) {//���ϵ�·
      Point Begin;
      Begin.y = FirstChess.y - i * MoveRoadY[k];//����㣬������·�����ķ����෴
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
  Score1 = Board[FirstChess.x][FirstChess.y];//Ϊ�˸ı�ԭ����
  Score2 = Board[SecondChess.x][SecondChess.y];
  
  int Before=PartScore_EvalueFucation(Board, FirstChess, SecondChess, ComputerSide, 0) + PartScore_EvalueFucation(Board, SecondChess, FirstChess, ComputerSide, 1);
  
  Board[FirstChess.x][FirstChess.y] = ComputerSide;
  Board[SecondChess.x][SecondChess.y] = ComputerSide;
  int After= PartScore_EvalueFucation(Board, FirstChess, SecondChess, ComputerSide, 0) + PartScore_EvalueFucation(Board, SecondChess, FirstChess, ComputerSide, 1);
  Board[FirstChess.x][FirstChess.y] = Score1;
  Board[SecondChess.x][SecondChess.y] = Score2;
  
  return After - Before;
}


queue<Step> GenerateSon(int Board[19][19],int BeginX,int EndX,int BeginY,int EndY) {//�����ӽڵ���к���
  
}
Step GetFrontNode(queue<Step> Son) {//�õ�����ͷ�ڵ㲢pop��
  Step ReturnNode;
  ReturnNode = Son.front();
  Son.pop();
  return ReturnNode;
}
int NegaMax_AlphaBeta(Step step, int Alpha,int Beta,int depth) {//������ֵ����
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
  PreSeekStep=PreSeek(Board, BeginX, EndX, BeginY, EndY,ComputerSide);//ʹ�ò�����֮ǰ����ǰ��ɨ�裬�ж��Ƿ��л��Ļ���ȱ������ӵ����
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
