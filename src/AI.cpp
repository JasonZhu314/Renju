#include "AI.h"

/*
SV : static evaluation function������Ը�������Ĺ�ֵ
SV ������������PPT
delta_SV ���� player�� x�� y�к�SV�����ı仯��
���� delta_SV �����ٶ����Ը��� SV �����Ҷ��ڸ������棬ÿһ���� SV ����Դ�С�� delta_SV ����Դ�С��ͬ���ʽ� SV ��Ϊ delta_SV (������ʼ��δ�õ�SV)
*/
double delta_SV(int board[][15], int x, int y, int player)
{
    double result = 0.0;
    // ɨ�躬 x y λ�õ���
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
        id = 3 * (id % 81) + board[x + i][y] + 1; // �˴���Ϊ���ټ��㣬����ÿ�����¼��� id
        result -= pos_weights[x + i - 2][y] * weights[id];
    }
    // ɨ�躬 x y λ�õ���
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
    // ɨ�躬 x y λ�õ�б��
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
    // ɨ�躬 x y λ�õ�б��
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
    // ģ�����ӣ�׼���ٴ�ɨ��
    board[x][y] = player;
    // ɨ�躬 x y λ�õ���
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
    // ɨ�躬 x y λ�õ���
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
    // ɨ�躬 x y λ�õ�б��
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
    // ɨ�躬 x y λ�õ�б��
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
    // ������һ��
    board[x][y] = -1;
    return result;
}

// ��ʼ�� pos_weight ����
void init_pos_weight()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            pos_weights[i][j] = 1.0 - penalty * abs(i - 7) * abs(j - 7); // Խ�������Ĵ��Խ��
        }
    }
}

// (�ض����)��ȡ243������
void read_weights()
{
    string trash;
    for (int i = 0; i < 243; i++)
    {
        cin >> trash >> weights[i];
    }
}

// ����λ�õĴ�����ɽڵ�
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

// ʵ�� alpha-beta ��֦�� minimax �㷨
double minimax(Board& b, int depth, int width, double value, double alpha, double beta, int player) {
    int winner = b.win_end();
    if(winner == 1) return -1e9;
    if(winner == 0) return 1e9;
    if(depth == 0) {
        return value;
    }
    // ���� width ���ڵ�
    vector<Move> moves = generate_moves(b.board, player, width);
    // �׷� : ���
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
    // �ڷ� : ��С��
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
    act = moves[0].x * 15 + moves[0].y; // ����ʱ������������ delta_SV ̰�ĵ�ѡȡ
    if (b.numPieces == 0) return 112;   // ��Ϊ��һ����ʼ����λ�� 7 7
    /*
    ��Ϸ��ͬ�׶β��ò�ͬ���������
    ��Ϸ�տ�ʼʱ�������ռ�ܴ󣬵��Ǻõ��߷����࣬�������λ���ǶԳƵģ����������Ȳ��úܴ�
    ����Խ��(�ܳ�Խ��)����Ҫ̽�����߷�Խ�࣬������Ļ�����࣬��˿������Ⱦ���Ӵ�
    ��Ϸ������ڣ�����߷����������˼�֦���࣬�������죬�Ӵ��������Ҳ���׳�ʱ
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
    // ����������������3�㿪ʼ
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
            // ��������ĳһ�̳��� 2000ms ǿ���ж�˼�����ߵ�ǰ�����߷�
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