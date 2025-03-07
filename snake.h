/*引入必要的头文件*/
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <wchar.h>
#include <string.h>

/*宏定义*/
#define MAP_HEIGHT 20     //定义地图高度
#define MAP_WIDTH 40      //定义地图宽度
#define WIDTH 50          //双人模式的地图
#define HEIGHT 20
#define UP1 'w'            //定义上移键
#define DOWN1 's'          //定义下移键
#define LEFT1 'a'          //定义左移键
#define RIGHT1 'd'         //定义右移键
#define UP2 72
#define DOWN2 80
#define LEFT2 75
#define RIGHT2 77
#define MAX_FOOD 10
#define MAX_BARRIER 7
#define MAX_SPECIALFOOD 3

typedef struct{           //定义食物和蛇身单个节点的结构体
    int x;                //x坐标
    int y;                //y坐标
}Food, Snakenode,Barrier,SpecialFood;

typedef struct{                         //定义蛇的结构体
    Snakenode snakeNode[1000];          //蛇长最大为1000
    int length;                         //蛇的长度
    int speed;                          //蛇的移动速度
    int direction;
}Snake;

//排行榜用户记录
typedef struct {
	char name[100];
	int score;  //分数
	int count;  //用户数量
}Data;

/*函数定义*/
void GotoXY(int, int);                  //光标定位函数
void Hide();                            //隐藏光标函数
int Menu();                             //主菜单函数
void Help();                            //帮助信息
void About();                           //关于信息
void PrintFood(int); 
void PrintSpecialFood(int);
void PrintBarrier();      
int Single_player();                        //蛇移动
int MultiPlayer();
int IsCorrect();                        //自撞或撞墙检测
void SpeedControl();                    //速度控制
void InitMap_Single();
void initialize_food();
void SelfSevering();

