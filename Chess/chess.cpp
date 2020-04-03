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

Step machine(int Board[19][19],int computerSide) {

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
