#pragma once
#include <stdio.h>
#include <stdint.h>

void auto_traversal_move(int32_t player, int32_t x, int32_t y);
static int32_t auto_legal_move(int32_t player, int32_t x, int32_t y, int32_t tar_x, int32_t tar_y);
static int32_t auto_can_eat(char a, char b);
int* auto_move(int32_t player, char chess[][8], char each_color[]);