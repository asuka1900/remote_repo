#include "snake.c"

int main(){

    // 设置控制台输出为 UTF-8 编码2
    SetConsoleOutputCP(65001);
    srand((unsigned int)time(0));        //生成随机数种子
    int end = 1, result;
    while (end){
        result = Menu();                 //显示主菜单，并根据用户选择菜单选项决定游戏的执行
        switch (result){
            case 1:
                Name();
                InitMap_Single();               //初始化地图
                while (Single_player());       //如果返回0，则蛇停止移动；返回1，蛇继续移动
                Rank();
                break;
            case 2:
                // InitMap_Multi();               //初始化地图
                MultiPlayer();       //如果返回0，则蛇停止移动；返回1，蛇继续移动
                break;
            case 3:
                Help();                  //显示帮助信息
                break;
            case 4:
                About();                 //显示关于信息12
                break;
            case 5:
                List();
                break;
            case 0:
                end = 0;
                Write();
                break;    
        }
    }
}

