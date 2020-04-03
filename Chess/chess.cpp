#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

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

int MoveX[8] = { 0,0,-1,1,-1,-1,1,1 };
int MoveY[8] = { -1,1,0,0,-1,1,-1,1 };//对应上,下,左,右,左上,左下,右上,右下

Step machine(int Board[19][19],int computerSide) {

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
