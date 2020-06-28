//Copyright 2020 Lab Mikazu. All Rights Reserved.
//Project "Mayim Mayim" 1st. work

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define PLAYER1 1
#define PLAYER2 2
#define SEARCHBEAM1 5
#define SEARCHBEAM2 5
#define ADVERSCONS 0.8
#define PRUNEADV 0.9
#define GREEDYRATE 0.5
#define ADVGREEDYRATE 0.5

//棋盘使用Unicode画的，一个符号占两个char，所以要*3，+1是为了末尾的'\0'
char aInitDisplayBoardArray[SIZE+1][SIZE*3+1] = 
{
	"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
	"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛",
	"A B C D E F G H I J K L M N O" 
};
//此数组用于显示
char aDisplayBoardArray[SIZE+1][SIZE*3+1];
//此数组用于算法计算使用
int aRecordBoard[SIZE][SIZE];//
char play1Pic[]="●";
char play2Pic[]="◎";
char _play1Pic[]="▲";
char _play2Pic[]="△";
//棋手最近一次落子时的坐标
int current_x;//行号
int current_y;//列号
int current_player;

struct point{
 int x;
 int y;
 int value;
};

void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);
void set_piece(int x,int y, int player);
int narugo(int x, int y, int PLAYER);
int judge(int x, int y, int PLAYER);
int muda(int x, int y);
int kinte(int x, int y, int PLAYER);
int bekaru(int x, int y, int PLAYER);
int ikiyo(int x, int y, int PLAYER);
int okiyo(int x, int y, int PLAYER);
int real_ikimi(int x, int y, int PLAYER);
int nagatsure(int x, int y, int PLAYER);
int autoscore(int x, int y, int PLAYER);
struct point autoret();
struct point ret();
int score[SIZE][SIZE];
struct point mystart[6]={{7,7,0}, {8,7,0}, {9,7,0}, {8,8,0}, {9,8,0}, {9,9,0}};
int myrandomizer=-1, myrandoms;

//程序交互界面部分


void main(){
 void ningendoushi();
 void wagakokoro();
 char C1, C2, C3;
 for(;;){
  system("clear");
  printf(" 五   子   棋\nFive in a Row\n   五目並べ\n\nＰＲＥＳＳ　ＥＮＴＥＲ\n\nMikazu S. Takahashi (Ziyue Ji)\n\n");
  for(;getchar()!='\n';);
  system("clear");
  printf("ＳＥＬＥＣＴ　ＹＯＵＲ　ＭＯＤＥ\n1=对战程序\n2=双人游戏\n\n");
  for(;(C1=getchar())!='1'&&C1!='2';);
  if(C1=='1'){
   system("clear");
   printf("Versus The Computer - プログラムに挑戦する\n   继续？Y/N\n");
   for(;(C2=getchar())!='Y'&&C2!='y'&&C2!='N'&&C2!='n';);
   if(C2=='Y'||C2=='y'){
    wagakokoro();
    printf("继续？Y/N\n");
    for(;(C3=getchar())!='Y'&&C3!='y'&&C3!='N'&&C3!='n';);
    if(C3=='N'||C3=='n'){
     break;
    }else{
     getchar();
    }
   }
  }else{
   system("clear");
   printf("Person versus person - 二人で遊ぶ\n   继续？Y/N\n");
   for(;(C2=getchar())!='Y'&&C2!='y'&&C2!='N'&&C2!='n';);
   if(C2=='Y'||C2=='y'){
    ningendoushi();
    printf("继续？Y/N\n");
    for(;(C3=getchar())!='Y'&&C3!='y'&&C3!='N'&&C3!='n';);
    if(C3=='N'||C3=='n'){
     break;
    }else{
     getchar();
    }
   }
  }
 }
}

void wagakokoro(){

	int a1, b1, i, kachi1, kachi2, CHAL, MYAI;
	char C4;
	struct point RET;

	system("clear");
	printf("挑战者，请您选择自己喜欢的颜色。\nY=Black            N=White\n");
	for(;(C4=getchar())!='Y'&&C4!='y'&&C4!='N'&&C4!='n';);
	if(C4=='Y'||C4=='y'){
	 CHAL=PLAYER1;
	 MYAI=PLAYER2;
	}else if(C4=='N'||C4=='n'){
	 CHAL=PLAYER2;
	 MYAI=PLAYER1;
	}

	initRecordBorard();
	recordtoDisplayArray();
	displayBoard();
	current_player=PLAYER1;


	for(i=0;i<225;i++){
	 if(current_player==CHAL){
	  for(;;){
	   printf("请输入您的落子位置。示例：8,8\n");
	   scanf("%d, %d", &a1, &b1);
	   if(muda(a1-1, b1-1)==1){
	    displayBoard();
	    printf("无法在此处落子。\n");
	   }else{
	    break;
	   }
	  }if(kinte(a1-1, b1-1, CHAL)==1){
	   displayBoard();
	   printf("很遗憾您下出了禁手（详见wikipedia五子棋规则），\n因此我获胜了。\n");
	   break;
	  }else{
	   kachi1=judge(a1-1, b1-1, CHAL);
	   set_piece(a1-1,b1-1,CHAL);
	  }recordtoDisplayArray();
	  displayBoard();
	  if(kachi1==1){
	   printf("您获胜了。　恭喜！\n");
	   break;
	  }
	 }else if(current_player==MYAI){





	  RET=autoret();
	  if(i==0){
	   for(;(myrandoms=(rand()%6))==myrandomizer;);
	   myrandomizer=myrandoms;
	   RET=mystart[myrandomizer];
	  }kachi2=judge(RET.x, RET.y, MYAI);
	  set_piece(RET.x, RET.y, MYAI);
	  recordtoDisplayArray();
	  displayBoard();
	  if(kachi2==1){
	   printf("我获胜了，　您还需精进技艺。\n");
	   break;
	  }printf("我的出招是(%d,%d)，您的对策如何？\n", RET.x+1, RET.y+1);
	 }


	 if(current_player==CHAL){
	  current_player=MYAI;
	 }else if(current_player==MYAI){
	  current_player=CHAL;
	 }





	}if(i>=225){
	 printf("你真牛逼。\n");
	}printf("ＰＲＥＳＳ　ＥＮＴＥＲ\n");
	getchar();
	for(;getchar()!='\n';);
	system("clear");

}

void ningendoushi()
{
	initRecordBorard();
	recordtoDisplayArray();
	displayBoard();
	int a1, b1, a2, b2, i, kachi1, kachi2;
	for(i=0;i<113;i++){



	 for(;;){
	  printf("玩家1请输入落子位置：\n");
	  current_player=PLAYER1;
	  scanf("%d, %d", &a1, &b1);
	  if(muda(a1-1, b1-1)==1){
	   displayBoard();
	   printf("此处无法落子。\n");
	  }else{
	   break;
	  }
	 }if(kinte(a1-1, b1-1, PLAYER1)==1){
	  displayBoard();
	  printf("您下出了禁手（详见wikipedia五子棋规则），\n玩家2获胜了！　恭喜！\n");
	  break;
	 }else{
	  kachi1=judge(a1-1, b1-1, PLAYER1);
	  set_piece(a1-1,b1-1,PLAYER1);
	 }recordtoDisplayArray();
	 displayBoard();
	 if(kachi1==1){
	  printf("玩家1获胜了！　恭喜！\n");
	  break;
	 }printf("对方出招为(%d,%d)，您的对策如何？\n", a1, b1);






	 for(;;){
	  printf("玩家2请输入落子位置：\n");
	  current_player=PLAYER2;
	  scanf("%d, %d", &a2, &b2);
	  if(muda(a2-1, b2-1)==1){
	   displayBoard();
	   printf("此处无法落子。\n");
	  }else{
	   break;
	  }
	 }if(kinte(a2-1, b2-1, PLAYER2)==1){
	  displayBoard();
	  printf("您下出了禁手（详见wikipedia五子棋规则），\n玩家1获胜了！　恭喜！\n");
	  break;
	 }else{
	  kachi2=judge(a2-1, b2-1, PLAYER2);
	  set_piece(a2-1,b2-1,PLAYER2);
	 }recordtoDisplayArray();
	 displayBoard();
	 if(kachi2==1){
	  printf("玩家2获胜了！　恭喜！\n");
	  break;
	 }printf("对方出招为(%d,%d)，您的对策如何？\n", a2, b2);





	}if(i>=113){
	 printf("你们都真牛逼。\n");
	}printf("ＰＲＥＳＳ　ＥＮＴＥＲ\n");
	getchar();
	for(;getchar()!='\n';);
	system("clear");

}

///////////////////////////////////////////////////////////// 
// 函数名称： set_piece 
// 作用：将棋手player的棋子落在x行y列

//  参数：无 
// 返回值：无 
/////////////////////////////////////////////////////////////
void set_piece(int x,int y, int player){
	aRecordBoard[x][y]=player;
	current_x=x;
	current_y=y;
}

///////////////////////////////////////////////////////////// 
// 函数名称： kinte 
// 作用：禁じ手を判断する

//  参数：x, y, PLAYER
// 返回值：0/1 
/////////////////////////////////////////////////////////////
int muda(int x, int y){
 if(x<0||x>=SIZE||y<0||y>=SIZE||(aRecordBoard[x][y]!=0)){
  return 1;
 }return 0;
}
int kinte(int x, int y, int PLAYER){
 if(muda(x, y)==1){
  return 1;
 }if(judge(x, y, PLAYER)){
  return 0;
 }if(ikiyo(x, y, PLAYER)+okiyo(x, y, PLAYER)>1){
  return 1;
 }if(real_ikimi(x, y, PLAYER)>1){
  return 1;
 }if(nagatsure(x, y, PLAYER)){
  return 1;
 }return 0;
}

///////////////////////////////////////////////////////////// 
// 函数名称： judge 
// 作用：「そこ」では勝つか否かを判断する

//  参数：x, y, player
// 返回值：0/1
/////////////////////////////////////////////////////////////
int bekaru(int x, int y, int PLAYER){
 if(x<0||x>=SIZE||y<0||y>=SIZE||aRecordBoard[x][y]!=PLAYER){
  return 0;
 }else{
  return 1;
 }
}

int narugo(int x, int y, int PLAYER){
 int r=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int i=x, j=y;
 for(i=x-4;i<=x;i++){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j, PLAYER)&&bekaru(i+2, j, PLAYER)&&bekaru(i+3, j, PLAYER)&&bekaru(i+4, j, PLAYER)){
   if(PLAYER==PLAYER1){
    if(!bekaru(i-1, j, PLAYER)&&!bekaru(i+5, j, PLAYER)){
     r++;
     break;
    }else{
     break;
    }
   }else{
    r++;
    break;
   }
  }
 }i=x;
 j=y;
 for(j=y-4;j<=y;j++){
  if(bekaru(i, j, PLAYER)&&bekaru(i, j+1, PLAYER)&&bekaru(i, j+2, PLAYER)&&bekaru(i, j+3, PLAYER)&&bekaru(i, j+4, PLAYER)){
   if(PLAYER==PLAYER1){
    if(!bekaru(i, j-1, PLAYER)&&!bekaru(i, j+5, PLAYER)){
     r++;
     break;
    }else{
     break;
    }
   }else{
    r++;
    break;
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y-4;i<=x;i++,j++){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j+1, PLAYER)&&bekaru(i+2, j+2, PLAYER)&&bekaru(i+3, j+3, PLAYER)&&bekaru(i+4, j+4, PLAYER)){
   if(PLAYER==PLAYER1){
    if(!bekaru(i-1, j-1, PLAYER)&&!bekaru(i+5, j+5, PLAYER)){
     r++;
     break;
    }else{
     break;
    }
   }else{
    r++;
    break;
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y+4;i<=x;i++,j--){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j-1, PLAYER)&&bekaru(i+2, j-2, PLAYER)&&bekaru(i+3, j-3, PLAYER)&&bekaru(i+4, j-4, PLAYER)){
   if(PLAYER==PLAYER1){
    if(!bekaru(i-1, j+1, PLAYER)&&!bekaru(i+5, j-5, PLAYER)){
     r++;
     break;
    }else{
     break;
    }
   }else{
    r++;
    break;
   }
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int judge(int x, int y, int PLAYER){
 if(muda(x, y)){
  return 0;
 }if(narugo(x, y, PLAYER)){
  return 1;
 }else{
  return 0;
 }
}

/////////////////////////////////////////////////////////////
// 函数名称：initRecordBorard
// 作用：初始化二维数组aRecordBoard
//  参数：无
// 返回值：无
/////////////////////////////////////////////////////////////
void initRecordBorard(void){

	//通过双重循环，将aRecordBoard清0
        int i, j;
        for(i=0;i<SIZE;i++){
         for(j=0;j<SIZE;j++){
          aRecordBoard[i][j]=0;
         }
        }

}

/////////////////////////////////////////////////////////////
// 函数名称：recordtoDisplayArray
// 作用：将aRecordBoard中记录的棋子位置，转化到aDisplayBoardArray中
//  参数：无
// 返回值：无
/////////////////////////////////////////////////////////////
void recordtoDisplayArray(void){
	int i,j;

	//第一步：将aInitDisplayBoardArray中记录的空棋盘，复制到aDisplayBoardArray中
        for(i=0;i<SIZE+1;i++){
         for(j=0;j<(SIZE)*3+1;j++){
          aDisplayBoardArray[i][j]=aInitDisplayBoardArray[i][j];
         }
        }

	//第二步：扫描aRecordBoard，当遇到非0的元素，将●或者◎复制到aDisplayBoardArray的相应位置上
	//注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占3个字节。●和◎也是中文字符，每个也占3个字节。
	for(i=0;i<SIZE;i++){
	 for(j=0;j<SIZE;j++){
	  if(aRecordBoard[i][j]==1){
	   aDisplayBoardArray[i][3*j]=play1Pic[0];
	   aDisplayBoardArray[i][3*j+1]=play1Pic[1];
	   aDisplayBoardArray[i][3*j+2]=play1Pic[2];
	   if(i==current_x&&j==current_y){
	    aDisplayBoardArray[i][3*j]=_play1Pic[0];
	    aDisplayBoardArray[i][3*j+1]=_play1Pic[1];
	    aDisplayBoardArray[i][3*j+2]=_play1Pic[2];
	   }
	  }else if(aRecordBoard[i][j]==2){
	   aDisplayBoardArray[i][3*j]=play2Pic[0];
	   aDisplayBoardArray[i][3*j+1]=play2Pic[1];
	   aDisplayBoardArray[i][3*j+2]=play2Pic[2];
	   if(i==current_x&&j==current_y){
	    aDisplayBoardArray[i][3*j]=_play2Pic[0];
	    aDisplayBoardArray[i][3*j+1]=_play2Pic[1];
	    aDisplayBoardArray[i][3*j+2]=_play2Pic[2];
	   }
	  }
	 }
	}
}

/////////////////////////////////////////////////////////////
// 函数名称：displayBoard
// 作用：将aDisplayBoardArray中记录的棋盘，显式到屏幕上
//  参数：无
// 返回值：无
/////////////////////////////////////////////////////////////
void displayBoard(void){
	int i, j;

	//第一步：清屏
	system("clear");

	//第二步：将aDisplayBoardArray输出到屏幕上
        printf("Copyright (C) 高橋·Ｓ·毅航 2017.1.6 - 2020.6.15\n");
        for(i=0;i<SIZE+1;i++){
         printf("%s\n", aDisplayBoardArray[i]);
        }/*for(i=0;i<SIZE;i++){
	 for(j=0;j<SIZE;j++){
          printf("%d	", score[i][j]/100);
	 }printf("\n");
        }*/
} 

/////////////////////////////////////////////////////////////
// 函数名称：autoret
// 作用：我が魂はクズなり。
//  参数：无
// 返回值：struct point
/////////////////////////////////////////////////////////////
void myminiheap(struct point *a, int l, struct point new){
 int i=0;
 a[i]=new;
 struct point temp;
 while(2*i+1<l){
  if(a[i].value>a[2*i+1].value){
   temp = a[2*i+1];
   a[2*i+1] = a[i];
   a[i] = temp;
   i = 2*i+1;
  }else if(2*i+2<l&&a[i].value>a[2*i+2].value){
   temp = a[2*i+2];
   a[2*i+2] = a[i];
   a[i] = temp;
   i = 2*i+2;
  }else{
   break;
  }
 }
}

struct point autoret(){
 struct point r, temp;
 struct point ord1[SEARCHBEAM1], ord2[SEARCHBEAM1][SEARCHBEAM2];
 int max1[SEARCHBEAM1], min2[SEARCHBEAM2];
 int adversarial_player = !(current_player-1)+1;
 int i=0, j=0, m=0, n=0;
 int i0=0, j0=0;
 int max, min;
 int tmpscr;

 //开始初始化一级与二级最小堆；
 for(i=0; i<SEARCHBEAM1; i++){
  ord1[i].value=-2147483648;
  ord1[i].x=0;
  ord1[i].y=0;
  for(j=0; j<SEARCHBEAM2; j++){
   ord2[i][j].value=-2147483648;
   ord2[i][j].x=0;
   ord2[i][j].y=0;
  }
 }

 //一级剪枝
 for(i=0;i<SIZE;i++){
  for(j=0;j<SIZE;j++){
   temp.x=i;
   temp.y=j;
   temp.value=autoscore(i, j, current_player)+PRUNEADV*autoscore2(i, j, adversarial_player);
   if(temp.value>ord1[0].value&&!muda(i, j)){
    myminiheap(ord1, SEARCHBEAM1, temp);
   }
  }
 }for(m=0; m<SEARCHBEAM1; m++){
  ord1[m].value=autoscore(ord1[m].x, ord1[m].y, current_player);
 }

 //二级剪枝
 for(m=0; m<SEARCHBEAM1; m++){
  aRecordBoard[ord1[m].x][ord1[m].y]=current_player;

  for(i=0;i<SIZE;i++){
   for(j=0;j<SIZE;j++){
    temp.x=i;
    temp.y=j;
    temp.value=autoscore(i, j, adversarial_player)+PRUNEADV*autoscore2(i, j, current_player);
    if(temp.value>ord2[m][0].value&&!muda(i, j)){
     myminiheap(ord2[m], SEARCHBEAM2, temp);
    }
   }
  }for(n=0; n<SEARCHBEAM2; n++){
   ord2[m][n].value=autoscore(ord2[m][n].x, ord2[m][n].y, adversarial_player);
  }

  aRecordBoard[ord1[m].x][ord1[m].y]=0;
 }

 //极大极小搜索
 for(m=0; m<SEARCHBEAM1; m++){

  //第一层伪落子
  aRecordBoard[ord1[m].x][ord1[m].y]=current_player;

  //进入第二层搜索框
  for(n=0; n<SEARCHBEAM2; n++){

   //第二层伪落子
   aRecordBoard[ord2[m][n].x][ord2[m][n].y]=adversarial_player;
   max = -2147483648;

   //三级剪枝
   for(i=0;i<SIZE;i++){
    for(j=0;j<SIZE;j++){
     tmpscr=autoscore(i, j, current_player)+PRUNEADV*autoscore2(i, j, adversarial_player);
     if(tmpscr>max){
      max=tmpscr;
      i0=i;
      j0=j;
     }
    }
   }

   //三级剪枝完毕，填充第二层搜索框一位并去掉伪子
   min2[n] = 1/(1+GREEDYRATE)*ord1[m].value - 1/(1+ADVGREEDYRATE)*ADVERSCONS*ord2[m][n].value + GREEDYRATE/(1+GREEDYRATE)*autoscore(i0, j0, current_player) - ADVGREEDYRATE/(1+ADVGREEDYRATE)*PRUNEADV*autoscore2(i0, j0, adversarial_player);
   aRecordBoard[ord2[m][n].x][ord2[m][n].y]=0;
  }

  //第二层搜索框返回所有结果，取极小值填充第一层搜索框的第m位内容、取掉伪子
  min=2147483647;
  for(i=0; i<SEARCHBEAM2; i++){
   if(min2[i]<min){
    min = min2[i];
   }
  }max1[m]=min;
  aRecordBoard[ord1[m].x][ord1[m].y]=0;
 }

 //检索第一层搜索框，取极大值落子
 max = -2147483648;
 for(i=0;i<SIZE;i++){
  for(j=0;j<SIZE;j++){
   score[i][j]=0;
  }
 }

 for(i=0; i<SEARCHBEAM1; i++){
  if(max1[i]>max){
   max = max1[i];
   r.x = ord1[i].x;
   r.y = ord1[i].y;
   r.value = max1[i];
  }score[ord1[i].x][ord1[i].y]=max1[i];
 }
 return r;
}

/////////////////////////////////////////////////////////////
// 函数名称：autoscore
// 作用：empirical energy function
//  参数：x, y, PLAYER
// 返回值：int
/////////////////////////////////////////////////////////////
int autoscore(int x, int y, int PLAYER){
 if(kinte(x, y, PLAYER)){
  return 0;
 }else{
  int hi=tanehi(x, y, PLAYER), hu=ikihu(x, y, PLAYER), omi=okimi(x, y, PLAYER), mi=ikimi(x, y, PLAYER), oyo=okiyo(x, y, PLAYER), his1=hissatsu1(x, y, PLAYER), his2=hissatsu2(x, y, PLAYER), yo=ikiyo(x, y, PLAYER), itsu=narugo(x, y, PLAYER);
  return (hi+120*hu+130*omi+12000*mi+105000*oyo+90000*his2+95000*his1+300000*yo+3000000*itsu);
 }
}

int autoscore2(int x, int y, int PLAYER){
 if(kinte(x, y, PLAYER)){
  return 0;
 }else{
  int hi=tanehi(x, y, PLAYER), hu=ikihu(x, y, PLAYER), omi=okimi(x, y, PLAYER), mi=ikimi(x, y, PLAYER), oyo=okiyo(x, y, PLAYER), his1=hissatsu1(x, y, PLAYER), his2=hissatsu2(x, y, PLAYER), yo=ikiyo(x, y, PLAYER), itsu=narugo(x, y, PLAYER);
  return (hi+100*hu+120*omi+10000*mi+12000*oyo+90000*his2+95000*his1+300000*yo+1000000*itsu);
 }
}

/////////////////////////////////////////////////////////////
// 函数名称：
// 作用：形を基準に得点を計算する
//  参数：
// 返回值：
/////////////////////////////////////////////////////////////
int ikiyo(int x, int y, int PLAYER){
 int r=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int i=x, j=y;
 for(i=x-3;i<=x;i++){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j, PLAYER)&&bekaru(i+2, j, PLAYER)&&bekaru(i+3, j, PLAYER)){
   if(judge(i-1, j, PLAYER)&&judge(i+4, j, PLAYER)){
    r++;
    break;
   }
  }
 }i=x;
 j=y;
 for(j=y-3;j<=y;j++){
  if(bekaru(i, j, PLAYER)&&bekaru(i, j+1, PLAYER)&&bekaru(i, j+2, PLAYER)&&bekaru(i, j+3, PLAYER)){
   if(judge(i, j-1, PLAYER)&&judge(i, j+4, PLAYER)){
    r++;
    break;
   }
  }
 }i=x;
 j=y;
 for(i=x-3,j=y-3;i<=x;i++,j++){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j+1, PLAYER)&&bekaru(i+2, j+2, PLAYER)&&bekaru(i+3, j+3, PLAYER)){
   if(judge(i-1, j-1, PLAYER)&&judge(i+4, j+4, PLAYER)){
    r++;
    break;
   }
  }
 }i=x;
 j=y;
 for(i=x-3,j=y+3;i<=x;i++,j--){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j-1, PLAYER)&&bekaru(i+2, j-2, PLAYER)&&bekaru(i+3, j-3, PLAYER)){
   if(judge(i-1, j+1, PLAYER)&&judge(i+4, j-4, PLAYER)){
    r++;
    break;
   }
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int okiyo(int x, int y, int PLAYER){
 int r=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int i=x, j=y, k=0;
 for(i=x-4;i<=x;i++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j, PLAYER)+bekaru(i+2, j, PLAYER)+bekaru(i+3, j, PLAYER)+bekaru(i+4, j, PLAYER)==4){
   for(k=0;k<=4&&bekaru(i+k, j, PLAYER);k++);
   if(judge(i+k, j, PLAYER)){
    if(k==0||k==4){
     if((k==0&&!judge(i+5, j, PLAYER))||(k==4&&!judge(i-1, j, PLAYER))){
      r++;
     }
    }else{
     r++;
    }
   }
  }
 }i=x;
 j=y;
 for(j=y-4;j<=y;j++){
  if(bekaru(i, j, PLAYER)+bekaru(i, j+1, PLAYER)+bekaru(i, j+2, PLAYER)+bekaru(i, j+3, PLAYER)+bekaru(i, j+4, PLAYER)==4){
   for(k=0;k<=4&&bekaru(i, j+k, PLAYER);k++);
   if(judge(i, j+k, PLAYER)){
    if(k==0||k==4){
     if((k==0&&!judge(i, j+5, PLAYER))||(k==4&&!judge(i, j-1, PLAYER))){
      r++;
     }
    }else{
     r++;
    }
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y-4;i<=x;i++,j++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j+1, PLAYER)+bekaru(i+2, j+2, PLAYER)+bekaru(i+3, j+3, PLAYER)+bekaru(i+4, j+4, PLAYER)==4){
   for(k=0;k<=4&&bekaru(i+k, j+k, PLAYER);k++);
   if(judge(i+k, j+k, PLAYER)){
    if(k==0||k==4){
     if((k==0&&!judge(i+5, j+5, PLAYER))||(k==4&&!judge(i-1, j-1, PLAYER))){
      r++;
     }
    }else{
     r++;
    }
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y+4;i<=x;i++,j--){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j-1, PLAYER)+bekaru(i+2, j-2, PLAYER)+bekaru(i+3, j-3, PLAYER)+bekaru(i+4, j-4, PLAYER)==4){
   for(k=0;k<=4&&bekaru(i+k, j-k, PLAYER);k++);
   if(judge(i+k, j-k, PLAYER)){
    if(k==0||k==4){
     if((k==0&&!judge(i+5, j-5, PLAYER))||(k==4&&!judge(i-1, j+1, PLAYER))){
      r++;
     }
    }else{
     r++;
    }
   }
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int real_ikimi(int x, int y, int PLAYER){
 int r=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int i=x, j=y, k=0;
 for(i=x-3;i<=x;i++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j, PLAYER)+bekaru(i+2, j, PLAYER)+bekaru(i+3, j, PLAYER)==3){
   for(k=0;k<=3&&bekaru(i+k, j, PLAYER);k++);
   if(ikiyo(i+k, j, PLAYER)){
    if(!kinte(i+k, j, PLAYER)){
     r++;
     break;
    }
   }
  }
 }i=x;
 j=y;
 for(j=y-3;j<=y;j++){
  if(bekaru(i, j, PLAYER)+bekaru(i, j+1, PLAYER)+bekaru(i, j+2, PLAYER)+bekaru(i, j+3, PLAYER)==3){
   for(k=0;k<=3&&bekaru(i, j+k, PLAYER);k++);
   if(ikiyo(i, j+k, PLAYER)){
    if(!kinte(i, j+k, PLAYER)){
     r++;
     break;
    }
   }
  }
 }i=x;
 j=y;
 for(i=x-3,j=y-3;i<=x;i++,j++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j+1, PLAYER)+bekaru(i+2, j+2, PLAYER)+bekaru(i+3, j+3, PLAYER)==3){
   for(k=0;k<=3&&bekaru(i+k, j+k, PLAYER);k++);
   if(ikiyo(i+k, j+k, PLAYER)){
    if(!kinte(i+k, j+k, PLAYER)){
     r++;
     break;
    }
   }
  }
 }i=x;
 j=y;
 for(i=x-3,j=y+3;i<=x;i++,j--){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j-1, PLAYER)+bekaru(i+2, j-2, PLAYER)+bekaru(i+3, j-3, PLAYER)==3){
   for(k=0;k<=3&&bekaru(i+k, j-k, PLAYER);k++);
   if(ikiyo(i+k, j-k, PLAYER)){
    if(!kinte(i+k, j-k, PLAYER)){
     r++;
     break;
    }
   }
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int nagatsure(int x, int y, int PLAYER){
 int r=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int i=x, j=y;
 for(i=x-5;i<=x;i++){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j, PLAYER)&&bekaru(i+2, j, PLAYER)&&bekaru(i+3, j, PLAYER)&&bekaru(i+4, j, PLAYER)&&bekaru(i+5, j, PLAYER)){
   r++;
   break;
  }
 }i=x;
 j=y;
 for(j=y-5;j<=y;j++){
  if(bekaru(i, j, PLAYER)&&bekaru(i, j+1, PLAYER)&&bekaru(i, j+2, PLAYER)&&bekaru(i, j+3, PLAYER)&&bekaru(i, j+4, PLAYER)&&bekaru(i, j+5, PLAYER)){
   r++;
   break;
  }
 }i=x;
 j=y;
 for(i=x-5,j=y-5;i<=x;i++,j++){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j+1, PLAYER)&&bekaru(i+2, j+2, PLAYER)&&bekaru(i+3, j+3, PLAYER)&&bekaru(i+4, j+4, PLAYER)&&bekaru(i+5, j+5, PLAYER)){
   r++;
   break;
  }
 }i=x;
 j=y;
 for(i=x-5,j=y+5;i<=x;i++,j--){
  if(bekaru(i, j, PLAYER)&&bekaru(i+1, j-1, PLAYER)&&bekaru(i+2, j-2, PLAYER)&&bekaru(i+3, j-3, PLAYER)&&bekaru(i+4, j-4, PLAYER)&&bekaru(i+5, j-5, PLAYER)){
   r++;
   break;
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int ikihu(int x, int y, int PLAYER){
 int r=0, i=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 for(i=-1;i<=1;i+=2){
  if(bekaru(x+i, y, PLAYER)&&ikimi(x-i, y, PLAYER)){
   r++;
  }if(bekaru(x, y+i, PLAYER)&&ikimi(x, y-i, PLAYER)){
   r++;
  }if(bekaru(x+i, y+i, PLAYER)&&ikimi(x-i, y-i, PLAYER)){
   r++;
  }if(bekaru(x+i, y-i, PLAYER)&&ikimi(x-i, y+i, PLAYER)){
   r++;
  }if(bekaru(x+2*i, y, PLAYER)&&ikimi(x+i, y, PLAYER)){
   r++;
  }if(bekaru(x, y+2*i, PLAYER)&&ikimi(x, y+i, PLAYER)){
   r++;
  }if(bekaru(x+2*i, y+2*i, PLAYER)&&ikimi(x+i, y+i, PLAYER)){
   r++;
  }if(bekaru(x+2*i, y-2*i, PLAYER)&&ikimi(x+i, y-i, PLAYER)){
   r++;
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int tanehi(int x, int y, int PLAYER){
 int r=0, i=0;
 if(muda(x, y)==1){
  return 0;
 }for(i=-1;i<=1;i+=2){
  r+=(bekaru(x+i, y, PLAYER)+bekaru(x, y+i, PLAYER)+bekaru(x+i, y+i, PLAYER)+bekaru(x+i, y-i, PLAYER));
 }return r;
}

int real_okimi(int x, int y, int PLAYER){
 int r=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int i=x, j=y, k=0;
 for(i=x-4;i<=x;i++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j, PLAYER)+bekaru(i+2, j, PLAYER)+bekaru(i+3, j, PLAYER)+bekaru(i+4, j, PLAYER)==3){
   if(!bekaru(i, j, PLAYER)&&!bekaru(i+4, j, PLAYER)){
    for(k=-1;k<=5;k++){
     if(!bekaru(i+k, j, PLAYER)&&okiyo(i+k, j, PLAYER)&&!kinte(i+k, j, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i+3, j, PLAYER)&&!bekaru(i+4, j, PLAYER)){
    for(k=-1;k<=4;k++){
     if(!bekaru(i+k, j, PLAYER)&&okiyo(i+k, j, PLAYER)&&!kinte(i+k, j, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i+4, j, PLAYER)){
    for(k=0;k<=4;k++){
     if(!bekaru(i+k, j, PLAYER)&&okiyo(i+k, j, PLAYER)&&!kinte(i+k, j, PLAYER)){
      r++;
      break;
     }
    }
   }
  }
 }i=x;
 j=y;
 for(j=y-4;j<=y;j++){
  if(bekaru(i, j, PLAYER)+bekaru(i, j+1, PLAYER)+bekaru(i, j+2, PLAYER)+bekaru(i, j+3, PLAYER)+bekaru(i, j+4, PLAYER)==3){
   if(!bekaru(i, j, PLAYER)&&!bekaru(i, j+4, PLAYER)){
    for(k=-1;k<=5;k++){
     if(!bekaru(i, j+k, PLAYER)&&okiyo(i, j+k, PLAYER)&&!kinte(i, j+k, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i, j+3, PLAYER)&&!bekaru(i, j+4, PLAYER)){
    for(k=-1;k<=4;k++){
     if(!bekaru(i, j+k, PLAYER)&&okiyo(i, j+k, PLAYER)&&!kinte(i, j+k, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i, j+4, PLAYER)){
    for(k=0;k<=4;k++){
     if(!bekaru(i, j+k, PLAYER)&&okiyo(i, j+k, PLAYER)&&!kinte(i, j+k, PLAYER)){
      r++;
      break;
     }
    }
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y-4;i<=x;i++,j++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j+1, PLAYER)+bekaru(i+2, j+2, PLAYER)+bekaru(i+3, j+3, PLAYER)+bekaru(i+4, j+4, PLAYER)==3){
   if(!bekaru(i, j, PLAYER)&&!bekaru(i+4, j+4, PLAYER)){
    for(k=-1;k<=5;k++){
     if(!bekaru(i+k, j+k, PLAYER)&&okiyo(i+k, j+k, PLAYER)&&!kinte(i+k, j+k, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i+3, j+3, PLAYER)&&!bekaru(i+4, j+4, PLAYER)){
    for(k=-1;k<=4;k++){
     if(!bekaru(i+k, j+k, PLAYER)&&okiyo(i+k, j+k, PLAYER)&&!kinte(i+k, j+k, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i+4, j+4, PLAYER)){
    for(k=0;k<=4;k++){
     if(!bekaru(i+k, j+k, PLAYER)&&okiyo(i+k, j+k, PLAYER)&&!kinte(i+k, j+k, PLAYER)){
      r++;
      break;
     }
    }
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y+4;i<=x;i++,j--){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j-1, PLAYER)+bekaru(i+2, j-2, PLAYER)+bekaru(i+3, j-3, PLAYER)+bekaru(i+4, j-4, PLAYER)==3){
   if(!bekaru(i, j, PLAYER)&&!bekaru(i+4, j-4, PLAYER)){
    for(k=-1;k<=5;k++){
     if(!bekaru(i+k, j-k, PLAYER)&&okiyo(i+k, j-k, PLAYER)&&!kinte(i+k, j-k, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i+3, j-3, PLAYER)&&!bekaru(i+4, j-4, PLAYER)){
    for(k=-1;k<=4;k++){
     if(!bekaru(i+k, j-k, PLAYER)&&okiyo(i+k, j-k, PLAYER)&&!kinte(i+k, j-k, PLAYER)){
      r++;
      break;
     }
    }
   }else if(bekaru(i, j, PLAYER)&&bekaru(i+4, j-4, PLAYER)){
    for(k=0;k<=4;k++){
     if(!bekaru(i+k, j-k, PLAYER)&&okiyo(i+k, j-k, PLAYER)&&!kinte(i+k, j-k, PLAYER)){
      r++;
      break;
     }
    }
   }
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int hissatsu1(int x, int y, int PLAYER){
 if(kinte(x, y, PLAYER)){
  return 0;
 }if(okiyo(x, y, PLAYER)+ikimi(x, y, PLAYER)>1){
  return 1;
 }return 0;
}

int hisnum1(int x, int y, int PLAYER){
 if(kinte(x, y, PLAYER)){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int r=0, i;
 for(i=-4;i<=4;i++){
  if(hissatsu1(x+i, y, PLAYER)){
   r++;
  }if(hissatsu1(x, y+i, PLAYER)){
   r++;
  }if(hissatsu1(x+i, y+i, PLAYER)){
   r++;
  }if(hissatsu1(x+i, y-i, PLAYER)){
   r++;
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int hissatsu2(int x, int y, int PLAYER){
 if(kinte(x, y, PLAYER)){
  return 0;
 }if(hissatsu1(x, y, PLAYER)){
  return 0;
 }if(okiyo(x, y, PLAYER)+ikimi(x, y, PLAYER)+hisnum1(x, y, PLAYER)>1){
  return 1;
 }return 0;
}

/////////////////////////////////////////////////////////////
// 函数名称：
// 作用：誰も見たことのない新世界。
//  参数：
// 返回值：
/////////////////////////////////////////////////////////////
int hameyo(int x, int y, int PLAYER){
 int r=0;
 if(muda(x, y)==1){
  return 0;
 }aRecordBoard[x][y]=PLAYER;
 int i=x, j=y, k=0;
 for(i=x-4;i<=x;i++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j, PLAYER)+bekaru(i+2, j, PLAYER)+bekaru(i+3, j, PLAYER)+bekaru(i+4, j, PLAYER)==4&&bekaru(i, j, PLAYER)&&bekaru(i+4, j, PLAYER)){
   if((judge(i+1, j, PLAYER)&&x!=i)||(judge(i+3, j, PLAYER)&&x!=i+4)){
    r++;
   }
  }
 }i=x;
 j=y;
 for(j=y-4;j<=y;j++){
  if(bekaru(i, j, PLAYER)+bekaru(i, j+1, PLAYER)+bekaru(i, j+2, PLAYER)+bekaru(i, j+3, PLAYER)+bekaru(i, j+4, PLAYER)==4&&bekaru(i, j, PLAYER)&&bekaru(i, j+4, PLAYER)){
   if((judge(i, j+1, PLAYER)&&y!=j)||(judge(i, j+3, PLAYER)&&y!=j+4)){
    r++;
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y-4;i<=x;i++,j++){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j+1, PLAYER)+bekaru(i+2, j+2, PLAYER)+bekaru(i+3, j+3, PLAYER)+bekaru(i+4, j+4, PLAYER)==4&&bekaru(i, j, PLAYER)&&bekaru(i+4, j+4, PLAYER)){
   if((judge(i+1, j+1, PLAYER)&&x!=i)||(judge(i+3, j+3, PLAYER)&&x!=i+4)){
    r++;
   }
  }
 }i=x;
 j=y;
 for(i=x-4,j=y+4;i<=x;i++,j--){
  if(bekaru(i, j, PLAYER)+bekaru(i+1, j-1, PLAYER)+bekaru(i+2, j-2, PLAYER)+bekaru(i+3, j-3, PLAYER)+bekaru(i+4, j-4, PLAYER)==4&&bekaru(i, j, PLAYER)&&bekaru(i+4, j-4, PLAYER)){
   if((judge(i+1, j-1, PLAYER)&&x!=i)||(judge(i+3, j-3, PLAYER)&&x!=i+4)){
    r++;
   }
  }
 }aRecordBoard[x][y]=0;
 return r;
}

int ikimi(int x, int y, int PLAYER){
 return real_ikimi(x, y, PLAYER)-hameyo(x, y, PLAYER);
}

int okimi(int x, int y, int PLAYER){
 return real_okimi(x, y, PLAYER)+hameyo(x, y, PLAYER);
}
