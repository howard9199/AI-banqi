#pragma once
#include <stdio.h>
#include <stdint.h>

void initial();
void random_secret();
void draw_map();
void game_start();
void do_action(int32_t player);
int32_t pick(int32_t player, int32_t x, int32_t y);
void move(int32_t player, int32_t x, int32_t y);
int32_t legal_move(int32_t player, int32_t x, int32_t y, int32_t tar_x, int32_t tar_y);
int32_t can_eat(char a, char b);
void auto_move(void);
void traversal_move(int32_t player, int32_t x, int32_t y);
