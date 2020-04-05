#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

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

int MoveX[8] = { 0,0,-1,1,-1,-1,1,1 };
int MoveY[8] = { -1,1,0,0,-1,1,-1,1 };//��Ӧ��,��,��,��,����,����,����,����

int ALL_EvalueFucation(int VirtualBoard[19][19],int BeginX,int EndX,int BeginY,int EndY,int ComputerSide) {//ȫ�����ۺ���
  
  int NumOfMyRoad[7] = { 0,0,0,0,0,0,0 };//��ͬ��������·������
  int NumOfEnemyRoad[7]= { 0,0,0,0,0,0,0 };
  int ScoreOfRoad[7] = {};//��ͬ��������·�ķ���
  
  int RowY[6] = { 0,1,2,3,4,5 };//����·Y�����ƶ�
  
  int ColumnX[6] = { 0,1,2,3,4,5 };//����·X�����ƶ�
  
  int LeftX[6] = {0,1,2,3,4,5};
  int LeftY[6] = {0,-1,-2,-3,-4,-5};
  
  int RightX[6] = {0,1,2,3,4,5};
  int RightY[6] = {0,1,2,3,4,5};
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
            b = j + RowY[k];
          }
          else if (dir == 1) {
            if (i + 5 > EndX) continue;
            a = i + ColumnX[k];
            b = j;
          }
          else if (dir==2) {
            if (i + 5 > EndX || j - 5 < BeginY) continue;
            a = i + LeftX[k];
            b = j + LeftY[k];
          }
          else if (dir==3) {
            if (i + 5 > EndX || j + 5 > EndY) continue;
            a = i + RightX[k];
            b = j + RightY[k];
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
int Part_EvalueFucation(int VirtualBoard[19][19],Point FirstChess,Point SecondChess, int BeginX, int EndX, int BeginY, int EndY) {//

}
Step machine(int Board[19][19],int computerSide) {
  int RowY[6] = { 0,1,2,3,4,5 };//����·Y�����ƶ�

  int ColumnX[6] = { 0,1,2,3,4,5 };//����·X�����ƶ�

  int LeftX[6] = { 0,1,2,3,4,5 };
  int LeftY[6] = { 0,-1,-2,-3,-4,-5 };

  int RightX[6] = { 0,1,2,3,4,5 };
  int RightY[6] = { 0,1,2,3,4,5 };
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
