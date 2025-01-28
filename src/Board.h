#pragma once

#include "utils.h"
#include "graphics.h"

using namespace std;

const int BOARD_LEN = 15;

class Board
{
private:
    const int boardLen = 15;

public:
    const int notOver = 2;                                                  // 状态 : 未结束
    const int flatFlag = 3;                                                 // 状态 : 平局
    int numPieces = 0;                                                      // 当前棋盘上的棋子数
    int undoState[225];                                                     // 下棋记录(用于悔棋)
    int currentPlayer = 1;                                                  // 当前玩家
    int preAction = -1;                                                     // 上一步棋的位置
    std::vector<int> state;                                                 // 棋盘状态
    int board[15][15];                                                      // 棋盘
    bool is_pvp, is_player_black;                                           // 当前游戏是否为 PVP 模式，如果是 PVC 模式，玩家是否为先手
    Board();                                                                // 初始化 : 空白棋盘
    Board(const Board *board);                                              // 初始化 : 复制棋盘
    ~Board();                                                               // 析构函数(含自动保存棋盘至 auto_save.txt)
    void reset();                                                           // 重置棋盘
    bool inBoard(int x, int y);                                             // 判断位置是否在棋盘内
    void doAction(int x, int y);                                            // 落子
    void undo();                                                            // 悔棋一步
    int isGameOver();                                                       // 判断游戏是否结束
    int win_end();                                                          // 判断获胜方
    bool long_connect();                                                    // 判断长连禁手
    bool tt_special_case(std::string &m_str, int pos, int t_case);          // 三三禁手"oo111o" "o111oo"只算一次
    bool ff_special_case(std::string &m_str, int pos, int f_case);          // 四四禁手 "o1111" "1111o"只算一次
    bool three_three();                                                     // 判断三三禁手
    bool four_four();                                                       // 判断四四禁手
    bool five_connect();                                                    // 判断五连
    bool saveBoard(wstring file_name);                                      // 保存棋盘
    bool loadBoard(wstring file_name);                                      // 读取棋盘
    void drawPiece(int startX, int startY, int gridLen, int i, int j);      // 绘制棋子
    void highlightPiece(int startX, int startY, int gridLen, int i, int j); // 高亮棋子
    void coverPiece(int startX, int startY, int gridLen, int i, int j);     // 覆盖棋子
    void drawBoard(int startX, int startY, int gridLen);                    // 绘制棋盘
};