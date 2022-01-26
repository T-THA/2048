#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#include<windows.h>

HANDLE consolehandle;         // 操作控制台（黑框框）需要的一个变量
int w,h;                      // 高度和宽度，对应 y 和 x
int board[4][4]={0};          // 棋盘
int symbol[4][4]={0};         // 后面用到的一个移动的标志
int oldboard[4][4]={0};       // 记录上一个状态的棋盘，用以判断移动是否有效
int gameover=1;               // 游戏结束标志
int score=0;                  // 分数
int step=0;                   // 步数，代替时间

// 语料库
char pause[100]="PAUSE    Press r to restart";
char pause2[100]="         Press other keys to continue";
char win[100]="You get 2048!So cool!";
char lose[100]="You fail!Would you like to try again?";
char Continue[100]="Press c to continue";
char Restart[100]="Press r to restart!";
char tip[100]="Press Space to pause !";
char tip2[100]="Press wasd or arrow to move!";

//添加函数
void mvaddch(int y, int x, char ch);
void mvaddint(int y, int x, int number);
void mvaddstr(int y,int x,char* ch);

//打印函数
void printwelcome();
void printall();        //not include welcome
void printsquare();
void printnumber();
void printscore();
void printstep();
void printtip();

//移动函数
void play_left();
void play_up();
void play_right();
void play_down();


void random();          //随机产生2 4
void move();            //进行上下左右的移动
int judge();            //判断移动是否有效
void judge_gameover();  //判断游戏是否结束

int numlen(int x);      //算数字长度的

int game();

int main(){
    consolehandle = GetStdHandle(STD_OUTPUT_HANDLE);    //初始化操作器
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consolehandle, &csbi);   //获取屏幕信息
    w = csbi.dwSize.X;
    h = csbi.dwSize.Y;

    //光标弄掉
    CONSOLE_CURSOR_INFO cci;                            //光标信息
    cci.dwSize = 100;
    cci.bVisible = FALSE; //不可见
    SetConsoleCursorInfo(consolehandle, &cci);          //光标特性应用


    printwelcome();//欢迎语

    while(1){
        while(kbhit() != 0){
            char c =getch();
            system("cls");
            game();
        }
        
    }

    //恢复光标显示
    cci.bVisible= TRUE;
    SetConsoleCursorInfo(consolehandle, &cci); 
    Sleep(10000);
    
    return 0;
}


int game(){//游戏主体部分
    random();
    random();
    printall();
    while(gameover==1){

        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                oldboard[i][j]=board[i][j];
            }
        }
        move();
        while(judge()){
            move();
        }
        
        step+=1;
        random();
        system("cls");
        printall();

        judge_gameover();
    }

    if(gameover) mvaddstr(17,70,win);
    else mvaddstr(17,70,lose);
    mvaddstr(18,70,Restart);
    while(1){
        while(kbhit() != 0){
            char select=getch();
            if(select=='r'){
            system("cls");
            gameover=1;
            step=0;
            score=0;
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    board[i][j]=0;
                }
            }
            game();
            }
        } 
    }
   
    return 1;
}


void judge_gameover(){//判断游戏是否结束
    int flag=0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if((board[i][j]==board[i][j+1]&&j!=3)){
                flag=1;
                break;
            }
            if(board[i][j]==board[i+1][j]&&i!=3){
                flag=1;
                break;
            }
            if(board[i][j]==0){
                flag=1;
                break;
            }
        }
        if(flag) break;
    }
    if(!flag) gameover=0;
    //↑lose ↓win
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(board[i][j]==2048){
                flag = 2;
                break;
            }
        }
        if(flag==2) break;
    }
    if(flag==2) gameover=2;
}
int judge(){//判断移动是否有效
    int flag=1;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(board[i][j]!=oldboard[i][j]){
                flag=0;
                break;
            }
        }
        if(!flag) break;
    }
    return flag;
}

//根据输入选择move模式
void move(){
    char c =getch();
    if(c==-32){
        c=getch();
    }
    if(c=='a'||c==75){
        play_left();
    }
    if(c=='w'||c==72){
        play_up();
    }
    if(c=='d'||c==77){
        play_right();
    }
    if(c=='s'||c==80){
        play_down();
    }
    if(c==' '){
        mvaddstr(17,70,pause);
        mvaddstr(18,70,pause2);
        char select=getch();
        if(select=='r'){
            system("cls");
            gameover=1;
            step=0;
            score=0;
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    board[i][j]=0;
                }
            }
            game();
        }
        system("cls");
        printall();
    }
}


//move模式
void play_left(){
    for(int i=0;i<4;i++){
        for(int j=1;j<4;j++){
            if(board[i][j]!=0){
                int tmp_loc=j;
                while(tmp_loc>0&&board[i][tmp_loc-1]==0){
                    board[i][tmp_loc-1]=board[i][tmp_loc];
                    board[i][tmp_loc]=0;
                    tmp_loc-=1;
                }
                if(tmp_loc!=0){
                    if(board[i][tmp_loc-1]==board[i][tmp_loc]&&symbol[i][tmp_loc-1]==0){
                        board[i][tmp_loc-1]=2*board[i][tmp_loc];
                        board[i][tmp_loc]=0;
                        score+=board[i][tmp_loc-1];//得分
                        symbol[i][tmp_loc-1]=1;//例如 2 2 4 左移得到 4 4 而不是 8 为了
                                               //避免这种情况，加一个判断
                    }
                }
            }
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            symbol[i][j]=0;
        }
    }
}
void play_right(){
    for(int i=0;i<4;i++){
        for(int j=3;j>=0;j--){
            if(board[i][j]!=0){
                int tmp_loc=j;
                while(tmp_loc<3&&board[i][tmp_loc+1]==0){
                    board[i][tmp_loc+1]=board[i][tmp_loc];
                    board[i][tmp_loc]=0;
                    tmp_loc+=1;
                }
                if(tmp_loc!=3){
                    if(board[i][tmp_loc+1]==board[i][tmp_loc]&&symbol[i][tmp_loc+1]==0){
                        board[i][tmp_loc+1]=2*board[i][tmp_loc];
                        board[i][tmp_loc]=0;
                        score+=board[i][tmp_loc+1];
                        symbol[i][tmp_loc+1]=1;
                    }
                }
            }
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            symbol[i][j]=0;
        }
    }
}
void play_down(){
    for(int i=0;i<4;i++){
        for(int j=3;j>=0;j--){
            if(board[j][i]!=0){
                int tmp_loc=j;
                while(tmp_loc<3&&board[tmp_loc+1][i]==0){
                    board[tmp_loc+1][i]=board[tmp_loc][i];
                    board[tmp_loc][i]=0;
                    tmp_loc+=1;
                }
                if(tmp_loc!=3){
                    if(board[tmp_loc+1][i]==board[tmp_loc][i]&&symbol[tmp_loc+1][i]==0){
                        board[tmp_loc+1][i]=2*board[tmp_loc][i];
                        board[tmp_loc][i]=0;
                        score+=board[tmp_loc+1][i];
                        symbol[tmp_loc+1][i]=1;
                    }
                }
            }
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            symbol[i][j]=0;
        }
    }
}
void play_up(){
    for(int i=0;i<4;i++){
        for(int j=1;j<4;j++){
            if(board[j][i]!=0){
                int tmp_loc=j;
                while(tmp_loc>0&&board[tmp_loc-1][i]==0){
                    board[tmp_loc-1][i]=board[tmp_loc][i];
                    board[tmp_loc][i]=0;
                    tmp_loc-=1;
                }
                if(tmp_loc!=0){
                    if(board[tmp_loc-1][i]==board[tmp_loc][i]&&symbol[tmp_loc-1][i]==0){
                        board[tmp_loc-1][i]=2*board[tmp_loc][i];
                        board[tmp_loc][i]=0;
                        score+=board[tmp_loc-1][i];
                        symbol[tmp_loc-1][i]=1;
                    }
                }
            }
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            symbol[i][j]=0;
        }
    }
}


//随机产生2 4 
void random(){
    srand((unsigned)time(NULL));
    int notzero[16],count=0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(!board[i][j]){
                notzero[count]=4*i+j;
                count+=1;
            }
        }
    }
    int location=rand() %count;
    int location_x=notzero[location]%4;
    int location_y=notzero[location]/4;
    if(!board[location_y][location_x]){
        int tmp=rand();
        if(tmp%2==0){
            board[location_y][location_x]=2;
        }else{
            board[location_y][location_x]=4;
        }
    }else{
        Sleep(1);
        random();
    }
}


void printwelcome(){
    printf("\n\n\n\n\n\n");
    printf("                                             Welcome to TTHA's 2048 game!\n\n\n");
    Sleep(2500);
    printf("                                     You can use wasd or arrow keys to move numbers!\n\n\n\n\n");
    Sleep(3500);
    printf("                                                    !!ATTENTION!!\n\n");
    Sleep(1500);
    printf("                                    !!!IF YOU SEE THE SCREEN FLASHING,IT'S NORMAL!!!\n\n\n\n\n");
    Sleep(4500);
    printf("                                 So now you can press arbitrary key to start your game!!!\n\n");
}
void printall(){
    printf("\n\n\n\n\n\n\n");
    printsquare();
    printnumber();
    printscore();
    printstep();
    printtip();
}
//印提示
void printtip(){
    mvaddstr(7,70,tip);
    mvaddstr(8,70,tip2);
}
//印步数
void printstep(){
    mvaddch(13,83,'S');
    mvaddch(13,84,'t');
    mvaddch(13,85,'e');
    mvaddch(13,86,'p');
    mvaddch(13,87,'=');
    mvaddint(13,89,step);
}
//印分数
void printscore(){
    mvaddch(13,70,'S');
    mvaddch(13,71,'c');
    mvaddch(13,72,'o');
    mvaddch(13,73,'r');
    mvaddch(13,74,'e');
    mvaddch(13,75,'=');
    mvaddint(13,77,score);
}
//印16个数的函数,若为0则不打印
void printnumber(){//10,35 10,43 10,51 10,59 +4 +8数字的坐标
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(board[i][j]) mvaddint(9+4*i,34+8*j-numlen(board[i][j])/2,board[i][j]);
        }
    }
}
//印棋盘的函数
void printsquare(){ 
    printf("                              * * * * * * * * * * * * * * * * *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              * * * * * * * * * * * * * * * * *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              * * * * * * * * * * * * * * * * *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              * * * * * * * * * * * * * * * * *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              *       *       *       *       *\n");
    printf("                              * * * * * * * * * * * * * * * * *\n");
}


// 在界面某位置输东西的函数
void mvaddch(int y, int x, char ch){//输入char的函数
    COORD co = (COORD){.X =x , .Y = y};
    SetConsoleCursorPosition(consolehandle, co); //设置我的光标位置
    putchar(ch); 
}
void mvaddstr(int y,int x,char* ch){
    COORD co = (COORD){.X =x , .Y = y};
    SetConsoleCursorPosition(consolehandle, co); //设置我的光标位置
    printf("%s",ch);
}
void mvaddint(int y, int x, int number){//输入2 4 8 ...的函数
    COORD co = (COORD){.X =x , .Y = y};
    SetConsoleCursorPosition(consolehandle, co); //设置我的光标位置
    printf("%d",number); //在这里打印一个字符
}


//数字的位数
int numlen(int x){
    int count=0;
    while(x){
        count+=1;
        x=x/10;
    }
    return count;
}