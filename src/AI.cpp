#include "AI.h"

/*
SV : static evaluation function，代表对给定局面的估值
SV 函数的设计详见PPT
delta_SV 计算 player下 x行 y列后SV函数的变化量
由于 delta_SV 计算速度明显高于 SV ，而且对于给定局面，每一步的 SV 的相对大小与 delta_SV 的相对大小相同，故将 SV 改为 delta_SV (本程序始终未用到SV)
*/
double delta_SV(int board[][15], int x, int y, int player)
{
    double result = 0.0;
    // 扫描含 x y 位置的列
    int lower = max(-4, -x), upper = min(4, 14 - x), id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x + i][y] + 1;
        }
        result -= pos_weights[x + lower + 2][y] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x + i][y] + 1; // 此处是为加速计算，不用每次重新计算 id
        result -= pos_weights[x + i - 2][y] * weights[id];
    }
    // 扫描含 x y 位置的行
    lower = max(-4, -y);
    upper = min(4, 14 - y);
    id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x][y + i] + 1;
        }
        result -= pos_weights[x][y + lower + 2] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x][y + i] + 1;
        result -= pos_weights[x][y + i - 2] * weights[id];
    }
    // 扫描含 x y 位置的斜线
    lower = max(-4, max(-x, -y));
    upper = min(4, min(14 - x, 14 - y));
    id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x + i][y + i] + 1;
        }
        result -= pos_weights[x + lower + 2][y + lower + 2] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x + i][y + i] + 1;
        result -= pos_weights[x + i - 2][y + i - 2] * weights[id];
    }
    // 扫描含 x y 位置的斜线
    lower = max(-4, max(-x, y - 14));
    upper = min(4, min(14 - x, y));
    id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x + i][y - i] + 1;
        }
        result -= pos_weights[x + lower + 2][y - lower - 2] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x + i][y - i] + 1;
        result -= pos_weights[x + i - 2][y - i + 2] * weights[id];
    }
    // 模拟落子，准备再次扫描
    board[x][y] = player;
    // 扫描含 x y 位置的列
    lower = max(-4, -x);
    upper = min(4, 14 - x);
    id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x + i][y] + 1;
        }
        result += pos_weights[x + lower + 2][y] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x + i][y] + 1;
        result += pos_weights[x + i - 2][y] * weights[id];
    }
    // 扫描含 x y 位置的行
    lower = max(-4, -y);
    upper = min(4, 14 - y);
    id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x][y + i] + 1;
        }
        result += pos_weights[x][y + lower + 2] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x][y + i] + 1;
        result += pos_weights[x][y + i - 2] * weights[id];
    }
    // 扫描含 x y 位置的斜线
    lower = max(-4, max(-x, -y));
    upper = min(4, min(14 - x, 14 - y));
    id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x + i][y + i] + 1;
        }
        result += pos_weights[x + lower + 2][y + lower + 2] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x + i][y + i] + 1;
        result += pos_weights[x + i - 2][y + i - 2] * weights[id];
    }
    // 扫描含 x y 位置的斜线
    lower = max(-4, max(-x, y - 14));
    upper = min(4, min(14 - x, y));
    id = 0;
    if (lower + 4 <= upper)
    {
        for (int i = lower; i < lower + 5; i++)
        {
            id = id * 3 + board[x + i][y - i] + 1;
        }
        result += pos_weights[x + lower + 2][y - lower - 2] * weights[id];
    }
    for (int i = lower + 5; i <= upper; i++)
    {
        id = 3 * (id % 81) + board[x + i][y - i] + 1;
        result += pos_weights[x + i - 2][y - i + 2] * weights[id];
    }
    // 撤销上一步
    board[x][y] = -1;
    return result;
}

// 初始化 pos_weight 数组
void init_pos_weight()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            pos_weights[i][j] = 1.0 - penalty * abs(i - 7) * abs(j - 7); // 越靠近中心打分越高
        }
    }
}

// (重定向后)读取243个参数
void read_weights()
{
    string trash;
    for (int i = 0; i < 243; i++)
    {
        cin >> trash >> weights[i];
    }
}

// 根据位置的打分生成节点
vector<Move> generate_moves(int board[][15], int player, int width){
    priority_queue<Move> move_heap;
    vector<Move> result;
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            if (board[i][j] != -1) continue;
            move_heap.push({i, j, delta_SV(board, i, j, player) * (1 - 2 * player)});
        }
    }
    while (width-- && !move_heap.empty()) {
        result.push_back(move_heap.top());
        move_heap.pop();
    }
    return result;
}

// 实现 alpha-beta 剪枝的 minimax 算法
double minimax(Board& b, int depth, int width, double value, double alpha, double beta, int player) {
    int winner = b.win_end();
    if(winner == 1) return -1e9;
    if(winner == 0) return 1e9;
    if(depth == 0) {
        return value;
    }
    // 生成 width 个节点
    vector<Move> moves = generate_moves(b.board, player, width);
    // 白方 : 最大化
    if(player == 0) {
        double maxEval = -1e9;
        for(auto& move : moves) {
            b.doAction(move.x, move.y);
            double eval = minimax(b, depth - 1, width, value + (1 - 2 * player) * move.score, alpha, beta, 1);
            b.undo();
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if(beta <= alpha)
                break;
        }
        return maxEval;
    }
    // 黑方 : 最小化
    else {
        double minEval = 1e9;
        for(auto& move : moves) {
            b.doAction(move.x, move.y);
            double eval = minimax(b, depth - 1, width, value + (1 - 2 * player) * move.score, alpha, beta, 0);
            b.undo();
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if(beta <= alpha)
                break;
        }
        return minEval;
    }
}

int action(Board& b, int player)
{
    int max_depth, max_w1, max_w2, act;
    vector<Move> moves = generate_moves(b.board, player, 15);
    vector<Move> patch = generate_moves(b.board, 1 - player, 1);
    auto start = chrono::high_resolution_clock::now();
    act = moves[0].x * 15 + moves[0].y; // 若无时间搜索，根据 delta_SV 贪心地选取
    if (b.numPieces == 0) return 112;   // 若为第一步，始终下位置 7 7
    /*
    游戏不同阶段采用不同的深度与宽度
    游戏刚开始时，搜索空间很大，但是好的走法不多，而且许多位置是对称的，因此深度与宽度不用很大
    棋子越多(周长越大)，需要探索的走法越多，而且输的机会更多，因此宽度与深度均需加大
    游戏进入后期，许多走法会输掉，因此剪枝更多，搜索更快，加大宽度与深度也不易超时
    */
    if (b.numPieces <= 6) {
        max_depth = 6;
        max_w1 = 8;
        max_w2 = 6;
    }
    else if (b.numPieces <= 12) {
        max_depth = 7;
        max_w1 = 12;
        max_w2 = 8;
    }
    else {
        max_depth = 9;
        max_w1 = 15;
        max_w2 = 12;
    }
    // 迭代加深搜索，从3层开始
    for (int d = 3; d <= max_depth; d++) {
        double best_value = -INFINITY, move_value;
        int best_x, best_y;
        bool time_out = false;
        for (int w1 = 0; w1 < max_w1; w1++) {
            b.doAction(moves[w1].x, moves[w1].y);
            double temp = minimax(b, d, max_w2, (1 - 2 * player) * moves[w1].score, -9e8, 9e8, 1 - player);
            move_value = (1 - 2 * player) * temp;
            if (move_value > best_value) {
                best_x = moves[w1].x;
                best_y = moves[w1].y;
                best_value = move_value;
            }
            b.undo();
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            // 若搜索到某一刻超过 2000ms 强制中断思考，走当前最优走法
            if (duration > 2000) {
                time_out = true;
                break;
            }
            if (move_value >= 1e9) break;
        }
        if (move_value <= -1e9) {
            act = best_x * 15 + best_y;
            break;
        }
        if (!time_out) act = best_x * 15 + best_y;
        if (time_out) break;
    }
    return act;
}