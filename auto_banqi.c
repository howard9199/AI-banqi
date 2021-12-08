#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "auto_banqi.h"

void auto_traversal_move(int32_t player, int32_t x, int32_t y);
static int32_t auto_legal_move(int32_t player, int32_t x, int32_t y, int32_t tar_x, int32_t tar_y);
static int32_t auto_can_eat(char a, char b);

static char chess[4][8];
static char color[2] = {'x','x'};

// auto-move settings
// remind auto move function in the end
static int32_t is_auto = 0;
// end

static int32_t dirx[5] = {-1,0,0,1,0};
static int32_t diry[5] = {0,1,-1,0,0};

//save possible move and count it.
static int32_t legal_x[8], legal_y[8];
static int32_t legal_cnt = 0;
static int32_t eat_score = 0;//for auto-move
//this will try all move, and save possible move.
void auto_traversal_move(int32_t player, int32_t x, int32_t y){
    //printf("[debug] player%d auto_traversal: %d,%d\n",player,x,y);
    legal_cnt = 0;
    eat_score = 0;
    for(int i = 0; i < 4; i ++){
        if(y + diry[i] < 0 || y + diry[i] >= 4 || x + dirx[i] < 0 || x + dirx[i] >= 8)continue;
        if(auto_legal_move(player, x, y, x+dirx[i],y + diry[i])){
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
                        if(auto_legal_move(player,x,y, x + dirx[i]*cannon_move, y + diry[i]*cannon_move)){
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
static int32_t to_x, to_y;
//for auto-move
static int32_t rank_auto;
//
static int32_t auto_legal_move(int32_t player, int32_t x, int32_t y, int32_t tar_x, int32_t tar_y){
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
    //printf("[debug] exam auto_can_eat(%c->%c)\n",chess[y][x],chess[tar_y][tar_x]);
    if(auto_can_eat(chess[y][x],chess[tar_y][tar_x])){
        eat_score +=rank_auto;
        return 1;
    }
    return 0;
}

static int32_t auto_can_eat(char a, char b){
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
int32_t ans[4];
int* auto_move(int32_t player, char other_chess[][8], char each_color[]){
    color[0] = each_color[0];
    color[1] = each_color[1];
    is_auto = 1;
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 8; j ++){
            chess[i][j] = other_chess[i][j];
        }
    }
        int32_t max_score = -1;
        int32_t max_x = 0, max_y = 0;
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
                auto_traversal_move(player, x, y);
                if(legal_cnt+eat_score*100 > max_score){
                    max_score = legal_cnt+eat_score*100;
                    max_x = x, max_y = y;
                }
            }
        }}
        auto_traversal_move(player, max_x, max_y);
        rank_auto = 0;
        int32_t max_rank = -1;
        for(int i = 0; i < legal_cnt; i ++){
            if(chess[legal_y[i]][legal_x[i]] != ' ' && 
            chess[legal_y[i]][legal_x[i]] != '*' &&
            auto_can_eat(chess[max_y][max_x],chess[legal_y[i]][legal_x[i]])&& rank_auto > max_rank){
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
        
        ans[0] = max_x+1;
        ans[1] = max_y+1;
        ans[2] = to_x+1;
        ans[3] = to_y+1;
        //printf("[debug] legal_cnt: %d\n",legal_cnt);
        //printf("[player%d] %c\n",player+1,color[player]);
        printf("choose: %d,%d(%c)\n",max_x+1,max_y+1,chess[max_y][max_x]);
        if(chess[max_y][max_x]!='*')printf("target: %d,%d(%c)\n",to_x+1,to_y+1,chess[to_y][to_x]);
        //printf("eat: %d\n",eat_score);
        if(chess[max_y][max_x] == '6' || chess[max_y][max_x] == 'F'){
            usleep(2000000);
        }else{
            usleep(1000000);
        }
        system("clear");
        return ans;
}