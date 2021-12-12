#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
# include "banqi.h"

/*
  1 2 3 4 5 6 7 8
 +---------------+
1|B|*|*|*|*|*|*|*|
 |---------------|
2|*|*|*|*|*|*|*|*|
 |---------------|
3|*|*|*|*|*|*|*|*|
 |---------------|
4|*|*|*|*|*|*|*|*|
 +---------------+
*/
char chess[4][8];
char secret[4][8];
int32_t status[4][8];
int32_t remain[2]={16,16};
int32_t step = 0;
char color[2] = {'x','x'};

// auto-move settings
// remind auto move function in the end
int32_t is_auto = 0;
// end
    
void initial(){
    for(int32_t i = 0; i < 4; i ++){
        for(int32_t j = 0; j < 8; j ++){
            chess[i][j] = '*';
        }
    }
    random_secret();
    return;
}

void random_secret(){
    //initial
    char start_chess[32] = {'1','2','2','3','3','4','4','5','5','6','6','7','7','7','7','7'};
    for(int i = 16; i < 32; i ++){
        start_chess[i] = (start_chess[i-16] - '1') + 'A';
    }
    //random
    srand(time(0));
    for(int32_t i = 0; i < 5000 + random()%10000; i ++){
        int8_t swap_a = random()%32;
        int8_t swap_b = random()%32;
        char tmp = start_chess[swap_a];
        start_chess[swap_a] = start_chess[swap_b];
        start_chess[swap_b] = tmp;
    }
    for(int32_t i = 0; i < 4; i ++){
        for(int32_t j = 0; j < 8; j ++){
            secret[i][j] = start_chess[i*8+j];
        }
    }
    //[debug]
    //secret[0][0] = '7';
    //secret[1][0] = 'G';
    //secret[0][1] = 'F';
    //secret[2][1] = '2';
    return;
}
void draw_map(){
    printf("  1 2 3 4 5 6 7 8 \n");
    printf(" +---------------+\n");
    printf("1");for(int i = 0; i < 8; i ++){printf("|%c",chess[0][i]);}printf("|\n");
    printf(" |---------------|\n");
    printf("2");for(int i = 0; i < 8; i ++){printf("|%c",chess[1][i]);}printf("|\n");
    printf(" |---------------|\n");
    printf("3");for(int i = 0; i < 8; i ++){printf("|%c",chess[2][i]);}printf("|\n");
    printf(" |---------------|\n");
    printf("4");for(int i = 0; i < 8; i ++){printf("|%c",chess[3][i]);}printf("|\n");
    printf(" +---------------+\n");
    return;  
}

void game_start(){
    int32_t player = 0;
    while(step < 50){
        step ++;
        draw_map();
        //printf("[debug]p1: %c, p2: %c\n", color[0],color[1]);
        do_action(player);
        if(remain[0] == 0){
            printf("Player 2 win\n");
            return;
        }else if(remain[1] == 0){
            printf("Player 1 win\n");
            return;
        }
        player = (player+1)%2;
    }
    printf("Game Tie\n");
    return;
}

void do_action(int32_t player){
    int32_t x, y;
    do{
        printf("Player %d (x,y):",player + 1);
        if(!is_auto)scanf("%d,%d",&x,&y);
    }while(pick(player, x-1, y-1) == -1);
    return;
}

int32_t pick(int32_t player, int32_t x, int32_t y){
    if(x < 0 || x >= 8 || y < 0 || y >= 4){return -1;}
    if(color[player] == 'x'){
        chess[y][x] = secret[y][x];
        if('1' <= chess[y][x] && chess[y][x] <= '7'){
            color[player] = '1';
            color[(player+1)%2] = 'A';
        }else{
            color[player] = 'A';
            color[(player+1)%2] = '1';
        }
        step = 0;
        return 0;
    }
    if(chess[y][x] == '*'){
       chess[y][x] = secret[y][x];
       step = 0;
       return 0;
    }
    if(0 <= chess[y][x] - color[player] && chess[y][x] - color[player] <= 6){
        move(player, x, y);
        return 0;
    }
    return -1;
}

int32_t dirx[5] = {-1,0,0,1,0};
int32_t diry[5] = {0,1,-1,0,0};

//save possible move and count it.
int32_t legal_x[8], legal_y[8];
int32_t legal_cnt = 0;
int32_t eat_score = 0;//for auto-move
//this will try all move, and save possible move.
void traversal_move(int32_t player, int32_t x, int32_t y){
    legal_cnt = 0;
    eat_score = 0;
    for(int i = 0; i < 4; i ++){
        if(y + diry[i] < 0 || y + diry[i] >= 4 || x + dirx[i] < 0 || x + dirx[i] >= 8)continue;
        if(legal_move(player, x, y, x+dirx[i],y + diry[i])){
            legal_x[legal_cnt] = x + dirx[i];
            legal_y[legal_cnt] = y + diry[i];
            legal_cnt++;
        }
    }
    //Cannon's special 
    if(chess[y][x] == '6' || chess[y][x] == 'F'){
        for(int i = 0; i < 4; i ++){
            int32_t has_fort = 0;//*(target)  *(fort)  6(cannon)
            int32_t cannon_move = 1;
            while(y + diry[i]*cannon_move >= 0 && y + diry[i]*cannon_move < 4 && 
            x + dirx[i]*cannon_move >= 0 && x + dirx[i]*cannon_move < 8){
                if(chess[y + diry[i]*cannon_move][x + dirx[i]*cannon_move] != ' '){
                    if(has_fort){
                        if(legal_move(player,x,y, x + dirx[i]*cannon_move, y + diry[i]*cannon_move)){
                            legal_x[legal_cnt] = x + dirx[i]*cannon_move;
                            legal_y[legal_cnt] = y + diry[i]*cannon_move;
                            legal_cnt++;
                            break;
                        }else{
                            break;
                        }                        
                    }
                    has_fort = 1;
                }
                cannon_move++;
            }
        }
    }
    return;
}
//player call it to try move chess(x,y)
int32_t to_x, to_y;
void move(int32_t player, int32_t x, int32_t y){
    //is player's chess

    //determine
    traversal_move(player, x, y);
    if(legal_cnt == 0){
        printf("You can't move, so PASS automatically\n");
        return;
    }
    //user to move
    int32_t success = 0;
    while(1){
        printf("To (x,y):");
        if(!is_auto){scanf("%d,%d", &to_x, &to_y);to_x --;to_y --;}
        for(int i = 0; i < legal_cnt; i ++){
            if(legal_x[i] == to_x && legal_y[i] == to_y){
                if(chess[to_y][to_x] != ' ' && chess[to_y][to_x] != '*'){
                    printf("[debug] %c eat %c\n",chess[y][x],chess[to_y][to_x]);
                    remain[(player+1)%2] --;
                    step = 0;
                }
                chess[to_y][to_x] = chess[y][x];
                chess[y][x] = ' ';
                success = 1;
                break;
            }
        }
        if(success)break;
    }
    return;
}

//for auto-move
int32_t rank_auto;
//
int32_t legal_move(int32_t player, int32_t x, int32_t y, int32_t tar_x, int32_t tar_y){
    //int32_t tar_y = y + diry[dir_idx];
    //int32_t tar_x = x + dirx[dir_idx];
    if(chess[tar_y][tar_x] == ' ')return 1;
    if(chess[tar_y][tar_x] == '*')return 0;
    //printf("[debug] legal?: %d %d\n",x+1,y+1);
    //printf("from: %c, to: %c",chess[y][x],chess[tar_y][tar_x]);
    if(0 <= abs(chess[tar_y][tar_x]-chess[y][x]) && abs(chess[tar_y][tar_x]-chess[y][x]) <= 6){
        //printf("[debug] same color:%d %d\n",tar_x, tar_y);
        //printf("[debug] on: %d %d\n",x+1,y+1);
        //same color between target and initial
        return 0;
    }
    if(chess[y][x] == '6' || chess[y][x] == 'F'){
        if(abs(tar_x - x) + abs(tar_y - y) < 2){
            return 0;
        }
    }
    if(can_eat(chess[y][x],chess[tar_y][tar_x])){
        eat_score +=rank_auto;
        return 1;
    }
    return 0;
}

int32_t can_eat(char a, char b){
    int32_t rank_a, rank_b;
    if('1' <= a && a <= '7'){
        rank_a = (a - '1') + 1;
        rank_b = (b - 'A') + 1;
    }else{
        rank_b = (b - '1') + 1;
        rank_a = (a - 'A') + 1;        
    }
    rank_auto = (8-rank_b);//auto-move
    //printf("[debug]rank_a:%c %d, rank_b:%c %d\n",a,rank_a,b,rank_b);
    //max 1 2 3 4 5 6 7 min
    if(rank_a == 6)return 1;
    if(rank_a == 7 && rank_b == 1)return 1;
    if(rank_a == 1 && rank_b == 7)return 0;
    if(rank_a <= rank_b)return 1;
    return 0;
}

//auto-move function

void auto_move(void){
    is_auto = 1;
    int32_t player = 0;
    while(step < 50){
        int32_t max_score = -1;
        int32_t max_x = 0, max_y = 0;
        step ++;
        draw_map();
        //printf("[debug]p1: %c, p2: %c\n", color[0],color[1]);
        for(int x = 0; x < 8; x ++){
        for(int y = 0; y < 4; y ++){
            if(chess[y][x] == '*'){
                if(10 > max_score){
                    max_score = 10;
                    max_x = x, max_y = y;
                }
            }
            if(0 <= abs(chess[y][x] - color[player]) && abs(chess[y][x] - color[player]) <= 6){
                traversal_move(player, x, y);
                if(legal_cnt+eat_score*100 > max_score){
                    max_score = legal_cnt+eat_score*100;
                    max_x = x, max_y = y;
                }
            }
        }}
        traversal_move(player, max_x, max_y);
        rank_auto = 0;
        int32_t max_rank = -1;
        for(int i = 0; i < legal_cnt; i ++){
            if(chess[legal_y[i]][legal_x[i]] != ' ' && 
            chess[legal_y[i]][legal_x[i]] != '*' &&
            can_eat(chess[max_y][max_x],chess[legal_y[i]][legal_x[i]])&& rank_auto > max_rank){
                to_x = legal_x[i];
                to_y = legal_y[i];
                max_rank = rank_auto;
            }
        }
        if(max_rank == -1){
            int32_t rand_move = (legal_cnt > 0 ?random()%legal_cnt:0);    
            to_x = legal_x[rand_move];
            to_y = legal_y[rand_move];
        }
        printf("[player%d] %c\n",player+1,color[player]);
        printf("choose: %d,%d(%c)\n",max_x+1,max_y+1,chess[max_y][max_x]);
        if(chess[max_y][max_x]!='*')printf("target: %d,%d(%c)\n",to_x+1,to_y+1,chess[to_y][to_x]);
        //printf("eat: %d\n",eat_score);
        printf("remain 1: %d, remain 2: %d\n",remain[0],remain[1]);
        if(chess[max_y][max_x] == '6' || chess[max_y][max_x] == 'F'){
            usleep(300000);
        }else{
            usleep(200000);
        }
        pick(player,max_x,max_y);
        if(remain[0] == 0){
            printf("Player 2 win\n");
            return;
        }else if(remain[1] == 0){
            printf("Player 1 win\n");
            return;
        }
        player = (player+1)%2;
        system("clear");
        
    }
    printf("Game Tie\n");

}
