#pragma once

#include "Board.h"

const double penalty = 0.01;

// 结构体 : Move
struct Move
{
    int x, y;     // 棋盘上 x行 y列
    double score; // 位置的打分，用 (1 - 2 * player) * delta_SV(i, j) 表示，因此分数越高表示该步对该玩家越有利

    bool operator<(const Move &other) const // < 运算符，为优先队列使用
    {
        return score < other.score;
    }
};

void read_weights();                                                                                 // 从 data.in 读取243个参数进入 weights 数组
void init_pos_weight();                                                                              // 初始化 pos_weight 数组，奖励靠近棋盘中心的位置
double delta_SV(int board[][15], int x, int y, int player);                                          // player下 x行 y列后SV函数的变化量
vector<Move> generate_moves(int board[][15], int player, int width);                                 // 根据位置的打分生成节点
double minimax(Board &b, int depth, int width, double value, double alpha, double beta, int player); // 实现 alpha-beta 剪枝的 minimax 算法
int action(Board &b, int player);                                                                    // 计算 player 的最佳走法