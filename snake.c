#include "snake.h"

/*全局变量定义*/
Snake snake,player1,player2;
Barrier barrier[MAX_BARRIER];
Food food[MAX_FOOD];
SpecialFood specialfood[MAX_SPECIALFOOD];
Data data[100];
char now_Dir1 = RIGHT1;       //当前蛇头方向变量
char direction1 = RIGHT1;     //预期蛇头方向变量
char now_Dir2 = RIGHT2;       //当前蛇头方向变量
char direction2 = RIGHT2;     //预期蛇头方向变量


/*主菜单实现*/
void GotoXY(int x,int y){
    HANDLE hout;
    COORD cor;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);  //这个函数返回标准输出设备的句柄，表示当前程序的标准输出流（通常是控制台窗口）。
    cor.X = (SHORT)x;
    cor.Y = (SHORT)y;
    SetConsoleCursorPosition(hout,cor);
}

void Hide(){
    HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
    /*
    Windows API 对struct _CONSOLE_CURSOR_INFO的定义
    typedef struct _CONSOLE_CURSOR_INFO {
    DWORD dwSize;       // 光标的大小（百分比）
    BOOL  bVisible;     // 光标是否可见
    } CONSOLE_CURSOR_INFO;*/

    CONSOLE_CURSOR_INFO cor_info = {1,0};
    SetConsoleCursorInfo(hout,&cor_info);
}

int Menu(){
    GotoXY(40,8);
    printf("Welcome to Greedy Snek");
    GotoXY(43,10);
    printf("1.Single-player Start");
    GotoXY(43,12);
    printf("2.Multiplayer Start");
    GotoXY(43,14);
    printf("3.Help");
    GotoXY(43,16);
    printf("4.About");
    GotoXY(43,18);
    printf("5.Ranking");
    GotoXY(43,20);
    printf("Any other key to exit");
    Hide();                                  //隐藏光标

    char ch;
    int result=0;
    ch = (char)_getch();                           //从控制台读取用户输入的一个字符 
    switch (ch){
        case '1': result=1 ; break;
        case '2': result=2 ; break;
        case '3': result=3 ; break;
        case '4': result=4 ; break;
        case '5': result=5 ; break;
    }
    system("cls");
    return result;
}

/*关于菜单的实现*/
void About(){
    GotoXY(30,12);
    printf("杭州电子科技大学--程序设计综合实践项目");
    GotoXY(43,14);
    printf("贪吃蛇游戏");
    GotoXY(43,16);
    printf("出品人：林净凯，揭望寒");
    GotoXY(43,18);
    printf("按任意键返回上级菜单");
    Hide();
    int d = _getch();
    system("cls");
}

/*帮助菜单实现*/
void Help(){
    GotoXY(40,12);
    printf("Here is no help,");
    GotoXY(40,14);
    printf("Just your snek and itself.");
    GotoXY(40,16);
    printf("Crash or thrive,it's your call");
    GotoXY(40,18);
    printf("But walls always win,after all !");
    GotoXY(40,20);
    printf("Return any key.");
    Hide();
    int d = _getch();
    system("cls");
}


void InitMap_Single(){
    Hide();
    //设置蛇头位置在地图的中间
    snake.snakeNode[0].x = MAP_WIDTH/2 -1;
    snake.snakeNode[0].y = MAP_HEIGHT/2 -1;
    GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);     //将光标移到蛇头位置
    printf("@");                                            //输出蛇头
    snake.length = 3;                                       //设置蛇长的初始值为3节
    snake.speed = 250;
    now_Dir1 = RIGHT1;                                        //当前蛇头方向
    
    //显示蛇身
    for (int i=1 ;i<snake.length; i++){
        //设置蛇身的纵坐标位置和蛇头位置相同
        snake.snakeNode[i].y = snake.snakeNode[0].y;
        //设置蛇身坐标位置，蛇身在蛇头左边，所以横坐标依次减一
        snake.snakeNode[i].x = snake.snakeNode[i-1].x - 1;
        //移动光标到蛇身位置
        GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
        //输出蛇身
        printf("o");

    }

    //生成地图上下边界
    for (int i=0; i<MAP_WIDTH; i++){
        GotoXY(i,0);
        printf("-");
        GotoXY(i,MAP_HEIGHT-1);
        printf("-");
    }
    //生成地图左右边界
    for (int i=1; i<MAP_HEIGHT; i++){
        GotoXY(0,i);
        printf("|");
        GotoXY(MAP_WIDTH-1,i);
        printf("|");
    }

    //生成规则
    GotoXY(50,8);
    printf("▷ WASD操控蛇头走向");
    GotoXY(50,10);
    printf("▷ 普通食物$加1分，特殊食物★加3分，蛇身同步增长");
    GotoXY(50,12);
    printf("▷ 撞墙/撞到障碍物判负");
    GotoXY(50,14);
    printf("▷ 咬到自己的身体自我截断");
    GotoXY(50,16);
    printf("▷ Play with your sneck");

    //生成食物
    PrintFood(MAX_FOOD);
    PrintBarrier();
    PrintSpecialFood(MAX_SPECIALFOOD);
    //得分说明
    GotoXY(50,5);
    printf("当前得分：0");
}

//食物初始化
void initialize_food() {
    for (int i = 0; i < MAX_FOOD; i++) {
        food[i].x = 0;
        food[i].y = 0;
    }
}

/*生成食物的函数*/
void PrintFood(int num){;
    num = rand() % num + 1;
    for (int i=0;i<num;i++){
        int flag = 1;
        while (flag){
            flag = 0;
            //设置随机的食物坐标位置
            food[i].x = rand() % (MAP_WIDTH-2) + 1;
            food[i].y = rand() % (MAP_HEIGHT-2) + 1;
            //循环判断食物位置和蛇的位置是否重叠，如果重叠，则需要重新设置食物的位置
            for (int k=0; k <=snake.length -1; k++){
                if (snake.snakeNode[k].x==food[i].x && snake.snakeNode[k].y==food[i].y){
                    flag = 1;            //位置有重叠，需要继续循环
                    break;
                }
            }
        }GotoXY(food[i].x, food[i].y);
        printf("$");
    }
}

/*生成障碍物的函数*/
void PrintBarrier(){
    int num = rand() % (MAX_BARRIER) + 1;
    for (int i=0;i<num;i++){
        int flag = 1;
        while (flag){
            flag = 0;
            barrier[i].x = rand() % (MAP_WIDTH-2) + 1;
            barrier[i].y = rand() % (MAP_HEIGHT-2) + 1;
            for (int k = 0; k <= snake.length - 1; k++) {
                //判断障碍物位置是否与蛇身重合
                if (snake.snakeNode[k].x == barrier[i].x && snake.snakeNode[k].y == barrier[i].y ) {
                    flag = 1;                //位置有重叠，需要继续循环
                    break;
                }
            }
            for (int p=0;p <= MAX_FOOD; p++){
                if (food[p].x == barrier[i].x && food[p].y == barrier[i].y){
                    flag = 1;
                    break;
                }
            }
        }GotoXY(barrier[i].x,barrier[i].y);
        printf("■");
    }
}

void PrintSpecialFood(int num){
    for (int i=0;i<num;i++){
        int flag = 1;
        while (flag){
            flag = 0;
            specialfood[i].x = rand() % (MAP_WIDTH-2) + 1;
            specialfood[i].y = rand() % (MAP_HEIGHT-2) + 1;
            for (int k = 0; k <= snake.length - 1; k++) {
                //判断障碍物位置是否与蛇身重合
                if (snake.snakeNode[k].x == barrier[i].x && snake.snakeNode[k].y == barrier[i].y ) {
                    flag = 1;                //位置有重叠，需要继续循环
                    break;
                }
            }//判断是否与普通食物重合
            for (int p=0;p <= MAX_FOOD; p++){
                if (food[p].x == specialfood[i].x && food[p].y == specialfood[i].y){
                    flag = 1;
                    break;
                }
            }//判断是否与障碍物重合
            for (int q=0;q <= MAX_BARRIER; q++){
                if (barrier[q].x == specialfood[i].x && barrier[q].y == specialfood[i].y){
                    flag = 1;
                    break;
                }
            }
        }GotoXY(specialfood[i].x,specialfood[i].y);
        printf("★");
    }
}

void AddFood(){
    int flag = 1;
    int k=0;
    while (flag){
        if (food[k].x == snake.snakeNode[0].x && food[k].y == snake.snakeNode[0].y){
            food[k].x = food[0].x;
            food[k].y = food[0].y;
            PrintFood(1);
            break;
        }k++;
        if (k>MAX_FOOD-1)  break;

    }
}

void AddSpecialFood(){
    int flag = 1;
    int k=0;
    while (flag){
        if (specialfood[k].x == snake.snakeNode[0].x && specialfood[k].y == snake.snakeNode[0].y){
            //将食物数组中第一个食物和新生成是食物交换坐标
            specialfood[k].x = specialfood[0].x;
            specialfood[k].y = specialfood[0].y;
            PrintSpecialFood(1);
            break;
        }k++;
        if (k<MAX_SPECIALFOOD-1)  break;
    }
}

void FoodAte(){
    Snakenode temp;
    int flag = 0; 
    temp = snake.snakeNode[snake.length-1];          //记录蛇尾
    //判断是否吃到了食物，如果蛇头位置和食物位置相同表示吃到了食物
    for (int i=0;i<MAX_FOOD;i++){
        if (snake.snakeNode[0].x==food[i].x&&snake.snakeNode[0].y==food[i].y){
            snake.length++;             //吃到食物，蛇长加1节
            flag = 1;                   //flag为1，表示迟到食物；为0，表示没有吃到食物
            snake.snakeNode[snake.length-1] = temp;     //temp是移动前蛇尾的位置
            data[data[0].count - 1].score = snake.length - 3;
        } 
    }//若没有吃到食物，则原来的蛇尾显示一个空格，去掉原来的蛇尾
    if (!flag){
        GotoXY(temp.x,temp.y);
        printf(" ");
    }//吃到了食物，则需要在地图上重新更新一个食物
    else{
        AddFood();
        GotoXY(50,5);
        printf("当前得分：%d",snake.length-3);
    }
}
void SpecialFoodAte(){
    Snakenode temp;
    int flag = 0; 
    temp = snake.snakeNode[snake.length-1];          //记录蛇尾
    //判断是否吃到了食物，如果蛇头位置和食物位置相同表示吃到了食物
    for (int i=0;i<MAX_SPECIALFOOD;i++){
        if (snake.snakeNode[0].x==specialfood[i].x&&snake.snakeNode[0].y==specialfood[i].y){
            snake.length = snake.length + 3;             //吃到特殊食物，蛇长加3节
            flag = 1;                   //flag为1，表示迟到食物；为0，表示没有吃到食物
            snake.snakeNode[snake.length-3] = temp;     //temp是移动前蛇尾的位置
            data[data[0].count - 1].score = snake.length - 3;
        } 
    }//若没有吃到食物，则原来的蛇尾显示一个空格，去掉原来的蛇尾
    if (!flag){
        GotoXY(temp.x,temp.y);
        printf(" ");
    }//吃到了食物，则需要在地图上重新更新一个食物
    else{
        for (int k=2;k>=0;k--){
            temp = snake.snakeNode[snake.length-k];
            GotoXY(temp.x,temp.y);
            printf("o");
        }AddSpecialFood();
        GotoXY(50,5);
        printf("当前得分：%d",snake.length-3);
    }
}
/*蛇移动函数的实现，返回值为1表示蛇继续移动，为0表示蛇停止移动*/
int Single_player(){
    for (int i=snake.length-1; i>=1; i--){
        snake.snakeNode[i] = snake.snakeNode[i-1];   //将所有蛇身向前移动一位
    }GotoXY(snake.snakeNode[1].x,snake.snakeNode[1].y);
    printf("o");                                     //在原来蛇头的位置打印一个蛇身节点o，其他蛇身不需要显示

    //非阻塞地响应键盘输入事件
    if (_kbhit()){                           //检查当前是否有键盘输入，若有，则返回一个非0的值，否则返回0
        direction1 = (char)_getch();
        switch (direction1){
            case UP1:                         //按下w键
                if (now_Dir1 != DOWN1){        //如果蛇头向上，按向下移动的w键时不起作用
                    now_Dir1 = direction1;
                }break;
            case DOWN1:                       //按下s键
                if (now_Dir1 != UP1){          //如果蛇头向上，按向下移动的s键时不起作用
                    now_Dir1 = direction1;
                }break;
            case LEFT1:                       //按下a键
                if (now_Dir1 != RIGHT1){       //如果蛇头向上，按向下移动的a键时不起作用
                    now_Dir1 = direction1;
                }break;
            case RIGHT1:                      //按下d键
                if (now_Dir1 != LEFT1){        //如果蛇头向上，按向下移动的d键时不起作用
                    now_Dir1 = direction1;
                }break;
        }
    }
    switch (now_Dir1){                        //根据现在的方向改变蛇头位置
        case UP1:    snake.snakeNode[0].y--;   break;        //向上移动
        case DOWN1:  snake.snakeNode[0].y++;   break;        //向下移动
        case LEFT1:  snake.snakeNode[0].x--;   break;        //向左移动
        case RIGHT1: snake.snakeNode[0].x++;   break;        //向右移动
    }

    //显示蛇头
    GotoXY(snake.snakeNode[0].x,snake.snakeNode[0].y);
    printf("@");
    FoodAte();
    SpecialFoodAte();
    SelfSevering();
    //判断是否死亡
    if (!IsCorrect()){
        system("cls");
        GotoXY(45,14);
        printf("Game Over!");
        GotoXY(45,16);
        printf("Final Score:%d",snake.length-3);
        GotoXY(45,18);
        printf("Any key to exit");
        int d = _getch();
        system("cls");
        return 0;
    }

    //调整速度
    SpeedControl();
    Sleep((DWORD)snake.speed);      //将进程挂起一段时间，用于控制蛇的移动速度
    return 1;
}

/*判断是否子撞或撞墙，返回值为0表示自撞或撞墙，否则为1*/
int IsCorrect(){
    //判断蛇头是否撞墙
    if (snake.snakeNode[0].x==0 || snake.snakeNode[0].y==0 || snake.snakeNode[0].x==MAP_WIDTH-1 || snake.snakeNode[0].y==MAP_HEIGHT-1){
        return 0;
    }//判断蛇头和蛇身是否重叠，重叠表示自撞
    // for (int i=1; i<snake.length; i++){
    //     if (snake.snakeNode[0].x==snake.snakeNode[i].x && snake.snakeNode[0].y==snake.snakeNode[i].y){
    //         return 0;
    //     }
    // }//判断蛇头是否和障碍物重合
    for (int k=0;k<MAX_BARRIER;k++){
        if (snake.snakeNode[0].x==barrier[k].x && snake.snakeNode[0].y==barrier[k].y){
            return 0;
        }
    }
    return 1;
}

void SelfSevering(){
    for (int i=1;i<snake.length;i++){
        //自我截断
        if (snake.snakeNode[0].x == snake.snakeNode[i].x && snake.snakeNode[0].y == snake.snakeNode[i].y){
            for (int k=i-1;k<snake.length;k++){
                GotoXY(snake.snakeNode[k].x,snake.snakeNode[k].y);
                printf(" ");
            }snake.length = i;
            GotoXY(50,5);
            printf("当前得分：%d",snake.length-3);
        }
    }
}
/*移动速度调整函数*/
void SpeedControl(){
    //根据蛇长调整蛇的移动速度
    switch (snake.length){   
        case 6:   snake.speed=200; break;
        case 9:   snake.speed=180; break;
        case 12:  snake.speed=160; break;
        case 15:  snake.speed=140; break;
        case 18:  snake.speed=120; break;
        case 21:  snake.speed=100; break;
        case 24:  snake.speed=80;  break;
        case 27:  snake.speed=60;  break;
        case 30:  snake.speed=40;  break;
        default:  break;
    }
}



// 绘制边框
void drawBorder() {
    system("cls");
    for (int i = 0; i <= WIDTH; i++) {
        GotoXY(i, 0);
        printf("#");
        GotoXY(i, HEIGHT);
        printf("#");
    }
    for (int i = 0; i <= HEIGHT; i++) {
        GotoXY(0, i);
        printf("#");
        GotoXY(WIDTH, i);
        printf("#");
    }
    GotoXY(WIDTH+5,8);
    printf("▷上蛇：WASD | 下蛇：↑↓←→");
    GotoXY(WIDTH+5,10);
    printf("▷你的目的是用你的身体截杀你的对手(头)");
    GotoXY(WIDTH+5,12);
    printf("▷食物是你的手段而不是目的");
    GotoXY(WIDTH+5,14);
    printf("▷自撞或撞墙是愚者的行为");
    
    
}

// 生成多个食物
void generateFood() {
    for (int i = 0; i < MAX_FOOD; i++) {
        food[i].x = rand() % (WIDTH - 2) + 1;
        food[i].y = rand() % (HEIGHT - 2) + 1;
        GotoXY(food[i].x, food[i].y);
        printf("*");
    }
}

// 初始化游戏
void initGame() {
    drawBorder();
    player1.length = player2.length = 3;
    player1.direction = RIGHT1;
    player2.direction = LEFT2;
    
    for (int i = 0; i < player1.length; i++) {
        player1.snakeNode[i].x = 10 - i;
        player1.snakeNode[i].y = 5;
    }
    for (int i = 0; i < player2.length; i++) {
        player2.snakeNode[i].x = 40 + i;
        player2.snakeNode[i].y = 15;
    }
    generateFood();
}

// 处理输入
void input() {
    if (_kbhit()) {
        int ch = _getch();
        switch (ch) {
            case UP1: if (player1.direction != DOWN1) player1.direction = UP1; break;
            case DOWN1: if (player1.direction != UP1) player1.direction = DOWN1; break;
            case LEFT1: if (player1.direction != RIGHT1) player1.direction = LEFT1; break;
            case RIGHT1: if (player1.direction != LEFT1) player1.direction = RIGHT1; break;
            case 0xE0:
                ch = _getch();
                switch (ch) {
                    case UP2: if (player2.direction != DOWN2) player2.direction = UP2; break;
                    case DOWN2: if (player2.direction != UP2) player2.direction = DOWN2; break;
                    case LEFT2: if (player2.direction != RIGHT2) player2.direction = LEFT2; break;
                    case RIGHT2: if (player2.direction != LEFT2) player2.direction = RIGHT2; break;
                }
                break;
        }
    }
}

// 移动蛇并处理吃食物
void moveSnake(Snake *player) {
    Snakenode temp = player->snakeNode[player->length - 1];
    for (int i = player->length - 1; i > 0; i--) {
        player->snakeNode[i] = player->snakeNode[i - 1];
    }

    switch (player->direction) {
        case UP1: case UP2: player->snakeNode[0].y--; break;
        case DOWN1: case DOWN2: player->snakeNode[0].y++; break;
        case LEFT1: case LEFT2: player->snakeNode[0].x--; break;
        case RIGHT1: case RIGHT2: player->snakeNode[0].x++; break;
    }

    // 吃食物
    for (int i = 0; i < MAX_FOOD; i++) {
        if (player->snakeNode[0].x == food[i].x && player->snakeNode[0].y == food[i].y) {
            player->length++;
            food[i].x = rand() % (WIDTH - 2) + 1;
            food[i].y = rand() % (HEIGHT - 2) + 1;
            GotoXY(food[i].x, food[i].y);
            printf("*");
        }
    }

    GotoXY(temp.x, temp.y);
    printf(" ");
    GotoXY(player->snakeNode[0].x, player->snakeNode[0].y);
    printf("O");
    GotoXY(player->snakeNode[player->length - 1].x, player->snakeNode[player->length - 1].y);
    printf("o");
}

// 检测碰撞
int checkWallCollision(Snake *player) {
    //处理撞墙逻辑
    if (player->snakeNode[0].x <= 0 || player->snakeNode[0].x >= WIDTH || player->snakeNode[0].y <= 0 || player->snakeNode[0].y >= HEIGHT) {
        return 1;
    }return 0;
}
int checkSelfCollision(Snake *player){
    for (int i = 1; i < player->length; i++) {
        if (player->snakeNode[0].x == player->snakeNode[i].x && player->snakeNode[0].y == player->snakeNode[i].y) {
            return 2;
        }
    }
    return 0;
}
int checkSlayOther(Snake *p1,Snake *p2){
    if (p1->snakeNode[0].x==p2->snakeNode[0].x && p1->snakeNode[0].y==p2->snakeNode[0].y){
        return 3;
    }
    for (int m=1;m<p2->length;m++){
        if (p1->snakeNode[0].x==p2->snakeNode[m].x && p1->snakeNode[0].y==p2->snakeNode[m].y){
            return 2;  //p2赢
        }
    }
    for (int n=1;n<p1->length;n++){
        if (p2->snakeNode[0].x==p1->snakeNode[n].x && p2->snakeNode[0].y==p1->snakeNode[n].y){
            return 1;  //p1赢
        }
    }return 0;
}
 
void OverGameRecall(int op){
    system("cls");
    GotoXY(30,12);
    switch (op){
        case 1:   //撞墙死亡
            printf("Walls are friends,");
            GotoXY(30,14);
            printf("not food!");
            break;
        case 2:
            printf("Chance of survival:");
            GotoXY(30,14);
            printf("You vs. You → 0%%");
            break;
        case 31:
        case 32:
            printf("Darwin Award goes to...");
            GotoXY(30,14);
            printf("Most Creative Slay 🧬");
            break;
        case 33:
            printf("Congratulations!");
            GotoXY(30,14);
            printf("You birthed a Two-Headed Snek");
            break;
    }GotoXY(30,16);                             
    printf("Any key to exit");
    Hide();
    int d = _getch();
    system("cls");
}

// 检测胜利
int checkWinner(Snake *p1, Snake *p2) {
    if (checkWallCollision(p1)||checkWallCollision(p2)) return 1;
    if (checkSelfCollision(p1)||checkSelfCollision(p2)) return 2;
    int op = checkSlayOther(p1,p2);
    if (op){
        switch (op){
            case 1:
                return 31;
            case 2:
                return 32;
            case 3:
                return 33;
        }
    }
    return -1;
}

// 主循环
int MultiPlayer() {
    initGame();
    while (1) {
        input();
        moveSnake(&player1);
        moveSnake(&player2);
        int result = checkWinner(&player1, &player2);
        if (result != -1) {
            OverGameRecall(result);
            break;
        }
        Sleep(200);
    }
    return 0;
}

/*积分排行榜函数*/
void Read() {
    FILE* file;
    file = fopen("scores.txt", "rb");
    fscanf(file, "%d", &data[0].count);
    for (int i = 0; i < data[0].count; i++) {
        fscanf(file, "%s", &data[i].name);
        fscanf(file, "%d", &data[i].score);
        data[i].count = data[0].count;
    }
    fclose(file);
}
void List() {
    GotoXY(48, 12);
    printf("用户名");
    GotoXY(58, 12);
    printf("分数");
    int i;
    for (i = 0; i < 3; i++) {
        GotoXY(48, 12 + (i + 1) * 2);
        printf("%s", data[i].name);
        GotoXY(58, 12 + (i + 1) * 2);
        printf("%d", data[i].score);
    }
    GotoXY(48, 12 + (i + 1) * 2);
    printf("按任意键返回菜单");
    Hide();
    char ch = _getch();
    system("cls");
}
void Name() {
    GotoXY(48, 12);
    printf("请输入用户名：");
    GotoXY(48, 14);
    scanf("%s", &data[data[0].count].name);
    data[0].count++;
    char ch = _getch();
    system("cls");
}

void Rank() {
    int max_index;
    char a[50];

    for (int i = 0; i < data[0].count; i++) {
        max_index = i;
        for (int j = i + 1; j < data[0].count; j++) {
            if (data[j].score > data[i].score) {
                max_index = j;
            }
        }
        strcpy(a, data[i].name);
        strcpy(data[i].name, data[max_index].name);
        strcpy(data[max_index].name, a);
        int temp = data[i].score;
        data[i].score = data[max_index].score;
        data[max_index].score = temp;

    }
    List();
}
void Write() {
    FILE* file;
    file = fopen("scores.txt", "wb");
    fprintf(file, "%d\n", data[0].count);
    for (int i = 0; i < data[0].count; i++) {
        fprintf(file, "%s ", data[i].name);
        fprintf(file, "%d", data[i].score);
        fprintf(file, "\n");
    }
    fclose(file);
}
