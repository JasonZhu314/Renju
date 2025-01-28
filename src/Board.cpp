#include "Board.h"

/*
Board类，其中禁手判断部分来自 botzone 上的裁判代码 judge.cpp
由于参考的裁判代码为旧版，与目前 botzone 使用的裁判代码不同
来自 judge.cpp 的函数在注释中已标明
*/

// 初始化 : 空白棋盘
// 来自 judge.cpp
Board::Board()
{
    reset();
}

// 初始化 : 复制棋盘
// 来自 judge.cpp，有改动
Board::Board(const Board *b)
{
    numPieces = b->numPieces;
    currentPlayer = b->currentPlayer;
    for (int i = 0; i < 225; i++)
    {
        undoState[i] = b->undoState[i];
    }
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            board[i][j] = b->board[i][j];
        }
    }
    state = b->state;
    is_pvp = b->is_pvp;
    is_player_black = b->is_player_black;
}

// 析构器 : 自动保存棋盘
Board::~Board(){
    saveBoard(L"auto_save.txt");
}

// 重置棋盘
// 来自 judge.cpp
void Board::reset()
{
    state.clear();
    state.insert(state.begin(), 255, -1);
    numPieces = 0;
    currentPlayer = 1;
    memset(undoState, -1, sizeof(undoState));
    memset(board, -1, sizeof(board));    
}

// 判断是否在棋盘内
// 来自 judge.cpp
bool Board::inBoard(int x, int y)
{
    return x >= 0 && x < boardLen && y >= 0 && y < boardLen;
}

// 落子
// 来自 judge.cpp
// 注意 : 棋盘上(i, j)位置用(i * boardLen + j)标记
void Board::doAction(int x, int y)
{
    board[x][y] = currentPlayer;
    preAction = 15 * x + y;
    state[preAction] = currentPlayer;
    undoState[numPieces++] = preAction;
    currentPlayer = 1 - currentPlayer;
}

// 悔棋
// 来自 judge.cpp，有改动
void Board::undo()
{
    // 撤回上步
    board[int(preAction / boardLen)][int(preAction % boardLen)] = -1;
    state[preAction] = -1;
    undoState[numPieces - 1] = -1;
    numPieces--;
    preAction = undoState[numPieces - 1];
    currentPlayer = 1 - currentPlayer;
}

// 判断游戏是否结束
// 来自 judge.cpp
// 返回值 : 2-未结束, 1-黑胜, 0-白胜, 3-平局(棋盘已满)
int Board::isGameOver()
{
    if (numPieces >= 9)
    {
        int we = win_end();
        if (we == notOver)
        {
            if (numPieces == 225)
            {
                return flatFlag;
            }
            return notOver;
        }
        return we;
    }
    return notOver;
}

// 判断获胜方
// 来自 judge.cpp
// 返回值 : 2-未结束, 1-黑胜, 0-白胜
int Board::win_end()
{
    int last_player = 1 - currentPlayer;
    if (last_player == 1)
    {
        if (long_connect())
            return 0; // 长连禁手
        if (five_connect())
            return 1; // 五连黑胜
        if (three_three())
            return 0; // 三三禁手
        if (four_four())
            return 0; // 四四禁手
    }
    else
    {
        if (five_connect())
            return 0; // 五连白胜
    }
    return notOver;
}

// 判断长连禁手
// 来自 judge.cpp
bool Board::long_connect()
{
    int h = preAction / 15;
    int w = preAction % 15;
    int last_move = preAction;
    int ret;

    int bias = min(w, 5);
    for (int i = last_move - bias; i < last_move + 1; i++)
    {
        if (15 - 1 - i % 15 < 5)
        {
            break;
        }
        ret = 0;
        for (int k = i; k < i + 6; k++)
        {
            if (state[k] != (1 - currentPlayer))
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    bias = min(h, 5);
    for (int i = last_move - bias * 15; i < last_move + 15; i += 15)
    {
        if (15 - 1 - i / 15 < 5)
        {
            break;
        }
        ret = 0;
        for (int k = i; k < i + 6 * BOARD_LEN; k += BOARD_LEN)
        {
            if (state[k] != (currentPlayer ^ 1))
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    bias = min(min(h, 5), min(w, 5));
    for (int i = last_move - bias * BOARD_LEN - bias; i < last_move + BOARD_LEN + 1; i += BOARD_LEN + 1)
    {
        if ((BOARD_LEN - 1 - i / BOARD_LEN < 5) || (BOARD_LEN - 1 - i % BOARD_LEN < 5))
        {
            break;
        }
        ret = 0;
        for (int k = i; k < i + 6 * BOARD_LEN + 6; k += BOARD_LEN + 1)
        {
            if (state[k] != (1 - currentPlayer))
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    bias = min(min(BOARD_LEN - 1 - h, 5), min(w, 5));
    for (int i = last_move + bias * BOARD_LEN - bias; i > last_move - BOARD_LEN + 1; i += (-(BOARD_LEN) + 1))
    {
        if ((BOARD_LEN - 1 - i % BOARD_LEN < 5) || (i / BOARD_LEN < 5))
        {
            break;
        }
        ret = 0;
        for (int k = i; k > i - 6 * BOARD_LEN + 6; k += (-(BOARD_LEN) + 1))
        {
            if (state[k] != (currentPlayer ^ 1))
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    return false;
}

// 三三禁手"oo111o" "o111oo"只算一次
// 来自 judge.cpp
bool Board::tt_special_case(string &m_str, int pos, int t_case)
{
    if (t_case == 1)
    { // oo111o
        if (pos + 6 < m_str.size())
        {
            if (m_str[pos + 6] == '1')
                return true;
        }
    }
    else
    { // o111oo
        if (pos > 0)
        {
            if (m_str[pos - 1] == '1')
                return true;
        }
    }
    return false;
}

// 四四禁手 "o1111" "1111o"只算一次
// 来自 judge.cpp
bool Board::ff_special_case(string &m_str, int pos, int f_case)
{
    if (f_case == 1)
    { // oo111o
        if (pos > 0)
        {
            if (m_str[pos - 1] == '1')
                return true;
        }
        if (pos + 5 < m_str.size())
        {
            if (m_str[pos + 5] == '1')
                return true;
        }
        return false;
    }
    else if (f_case == 2)
    {
        if (pos > 0)
        {
            if (pos + 6 < m_str.size())
            {
                if (m_str[pos - 1] == '1' && ((
                                                  m_str[pos + 5] == 'o' && m_str[pos + 6] == '1') ||
                                              (m_str[pos + 5] == '0')))
                    return true;
                return false;
            }
            if (pos + 5 < m_str.size())
            {
                if (m_str[pos - 1] == '1' && m_str[pos + 5] == '0')
                    return true;
                return false;
            }
            if (m_str[pos - 1] == '1')
                return true;
            return false;
        }
        else
            return false;
    }
    else
    {
        if (pos + 5 < m_str.size())
        {
            if (pos - 2 >= 0)
            {
                if ((m_str[pos - 2] == '1' && m_str[pos - 1] == 'o') || (m_str[pos - 1] == '0') && m_str[pos + 5] == '1')
                    return true;
                return false;
            }
            else if (pos - 1 >= 0)
            {
                if (m_str[pos + 5] == '1' && m_str[pos - 1] == '0')
                    return true;
                return false;
            }
            if (m_str[pos + 5] == '1')
                return true;
            return false;
        }
        else
            return false;
    }
}

// 判断三三禁手
// 来自 judge.cpp
bool Board::three_three()
{
    int h = preAction / BOARD_LEN;
    int w = preAction % BOARD_LEN;
    int last_move = preAction;
    int width = BOARD_LEN;
    string jt1 = "o1o11o";
    string jt2 = "o11o1o";
    string ct1 = "oo111o";
    string ct2 = "o111oo";
    int three = 0;
    string m_str;
    int pos;
    int bias = min(w, 4);
    for (int i = last_move - bias; i < last_move + min(width - 1 - w, 4) + 1; i++)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos)
    {
        if (tt_special_case(m_str, pos, 1) == false)
            three++;
    }
    else
    {
        pos = m_str.find(ct2);
        if (pos != m_str.npos)
        {
            if (tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    m_str.clear();
    bias = min(h, 4);
    for (int i = last_move - bias * width; i < last_move + width * min(width - 1 - h, 4) + width; i += width)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos)
    {
        if (tt_special_case(m_str, pos, 1) == false)
            three++;
    }
    else
    {
        pos = m_str.find(ct2);
        if (pos != m_str.npos)
        {
            if (tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    m_str.clear();
    bias = min(min(h, 4), min(w, 4));
    for (int i = last_move - bias * width - bias; i < last_move + (width + 1) * min(min(width - 1 - h, width - 1 - w), 4) + width + 1; i += width + 1)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos)
    {
        if (tt_special_case(m_str, pos, 1) == false)
            three++;
    }
    else
    {
        pos = m_str.find(ct2);
        if (pos != m_str.npos)
        {
            if (tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    m_str.clear();
    bias = min(min(width - 1 - w, 4), min(h, 4));
    for (int i = last_move - bias * (width - 1); i < last_move + (width - 1) * min(min(width - 1 - h, min(w, 4)), 4) + width - 1; i += width - 1)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos)
    {
        if (tt_special_case(m_str, pos, 1) == false)
            three++;
    }
    else
    {
        pos = m_str.find(ct2);
        if (pos != m_str.npos)
        {
            if (tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    return false;
}

// 判断四四禁手
// 来自 judge.cpp
bool Board::four_four()
{
    int h = preAction / BOARD_LEN;
    int w = preAction % BOARD_LEN;
    int last_player = 1 - currentPlayer;
    int last_move = preAction;
    int width = BOARD_LEN;
    int pos;
    string jf1 = "111o1";
    string jf2 = "1o111";
    string jf3 = "11o11";
    string cf1 = "o1111";
    string cf2 = "1111o";
    int four = 0;
    string m_str;
    int bias;

    bias = min(w, 5);
    for (int i = last_move - bias; i < last_move + min(width - 1 - w, 5) + 1; i++)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 2) == false)
            four++;
    }
    else
    {
        pos = m_str.find(cf2);
        if (pos != m_str.npos)
        {
            if (ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;

    m_str.clear();
    bias = min(h, 5);
    for (int i = last_move - bias * width; i < last_move + width * min(width - 1 - h, 5) + width; i += width)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 2) == false)
            four++;
    }
    else
    {
        pos = m_str.find(cf2);
        if (pos != m_str.npos)
        {
            if (ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;

    m_str.clear();
    bias = min(min(h, 5), min(w, 5));
    for (int i = last_move - bias * width - bias; i < last_move + (width + 1) * min(min(width - 1 - h, width - 1 - w), 5) + width + 1; i += width + 1)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 2) == false)
            four++;
    }
    else
    {
        pos = m_str.find(cf2);
        if (pos != m_str.npos)
        {
            if (ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;

    m_str.clear();
    bias = min(min(width - 1 - w, 5), min(h, 5));
    for (int i = last_move - bias * (width - 1); i < last_move + (width - 1) * min(min(width - 1 - h, min(w, 5)), 5) + width - 1; i += width - 1)
    {
        if (state[i] == -1)
        {
            m_str.append(1, 'o');
        }
        else if (state[i] == 0)
        {
            m_str.append(1, '0');
        }
        else
        {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos)
    {
        if (ff_special_case(m_str, pos, 2) == false)
            four++;
    }
    else
    {
        pos = m_str.find(cf2);
        if (pos != m_str.npos)
        {
            if (ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;
    return false;
}

// 判断五连
// 来自 judge.cpp
bool Board::five_connect()
{
    int h = preAction / BOARD_LEN;
    int w = preAction % BOARD_LEN;
    int last_player = currentPlayer ^ 1;
    int last_move = preAction;
    int i, j;
    int ret;
    if (preAction == -1)
        return false;
    int bias = min(w, 4);
    for (i = last_move - bias; i < last_move + 1; i++)
    {
        if (BOARD_LEN - 1 - i % (BOARD_LEN) < 4)
        {
            break;
        }
        ret = 0;
        for (j = i; j < i + 5; j++)
        {

            if (state[j] != last_player)
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
        {
            return true;
        }
    }
    bias = min(h, 4);
    for (i = last_move - bias * BOARD_LEN; i < last_move + BOARD_LEN; i += BOARD_LEN)
    {
        if (BOARD_LEN - 1 - i / BOARD_LEN < 4)
        {
            break;
        }
        ret = 0;
        for (j = i; j < i + 5 * BOARD_LEN; j += BOARD_LEN)
        {

            if (state[j] != last_player)
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
        {
            return true;
        }
    }
    bias = min(min(h, 4), min(w, 4));
    for (i = last_move - bias * BOARD_LEN - bias; i < last_move + BOARD_LEN + 1; i += BOARD_LEN + 1)
    {
        if ((BOARD_LEN - 1 - i / BOARD_LEN < 4) || (BOARD_LEN - 1 - i % BOARD_LEN < 4))
        {
            break;
        }
        ret = 0;
        for (j = i; j < i + 5 * BOARD_LEN + 5; j += BOARD_LEN + 1)
        {

            if (state[j] != last_player)
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
        {
            return true;
        }
    }
    bias = min(min(BOARD_LEN - 1 - h, 4), min(w, 4));
    for (i = last_move + bias * BOARD_LEN - bias; i > last_move - BOARD_LEN + 1; i = i - BOARD_LEN + 1)
    {
        if ((BOARD_LEN - 1 - i % BOARD_LEN < 4) || (i / BOARD_LEN < 4))
        {
            break;
        }
        ret = 0;
        for (j = i; j > i - 5 * BOARD_LEN + 5; j = j - BOARD_LEN + 1)
        {
            if (state[j] != last_player)
            {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
        {
            return true;
        }
    }
    return false;
}

// 保存棋盘
// 注意 : 由于 Windows API 使用 Unicode 编码，此函数与下述读盘函数的参数均为 wstring 而非 string
bool Board::saveBoard(wstring file_name){
    ofstream out(file_name.c_str());
    if (!out) return false;
    out << numPieces << endl;
    out << currentPlayer << endl;
    out << preAction << endl;
    out << is_pvp << " " << is_player_black << endl;
    for (int i = 0; i < 225; i++){
        out << state[i] << " ";
    }
    out << endl;
    for (int i = 0; i < 225; i++){
        out << undoState[i] << " ";
    }
    out << endl;
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            out << board[i][j] << " ";
        }
        out << endl;
    }
    return true;
}

// 读取棋盘
bool Board::loadBoard(wstring file_name){
    ifstream in;
    reset();
    in.open(file_name.c_str());
    if (!in) return false;
    in >> numPieces;
    in >> currentPlayer;
    in >> preAction;
    in >> is_pvp >> is_player_black;
    for (int i = 0; i < 225; i++){
        in >> state[i];
    }
    for (int i = 0; i < 225; i++){
        in >> undoState[i];
    }
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            in >> board[i][j];
        }
    }
    return true;
}

// 绘制棋盘
void Board::drawBoard(int startX, int startY, int gridLen) {
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 1);
    setfillcolor(RGB(247, 165, 89));
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            fillrectangle(startX + j * gridLen, startY + i * gridLen, startX + (j + 1) * gridLen, startY + (i + 1) * gridLen);
            rectangle(startX + j * gridLen, startY + i * gridLen, startX + (j + 1) * gridLen, startY + (i + 1) * gridLen);
        }
    }
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            drawPiece(startX, startY, gridLen, i, j);
        }
    }
}

// 绘制棋子
void Board::drawPiece(int startX, int startY, int gridLen, int i, int j) {
    if (board[i][j] == 1) {
        setfillcolor(BLACK);
        setlinecolor(BLACK);
        fillcircle(startX + (2 * j + 1) * gridLen / 2, startY + (2 * i + 1) * gridLen / 2, gridLen / 3);
    }
    else if (board[i][j] == 0) {
        setfillcolor(WHITE);
        setlinecolor(WHITE);
        fillcircle(startX + (2 * j + 1) * gridLen / 2, startY + (2 * i + 1) * gridLen / 2, gridLen / 3);
    }
}

// 覆盖棋子
// 注意 : 为了完全覆盖上一步高亮的棋子，棋子半径多一个像素
void Board::coverPiece(int startX, int startY, int gridLen, int i, int j) {
    if (board[i][j] == 1) {
        setfillcolor(BLACK);
        setlinecolor(BLACK);
        fillcircle(startX + (2 * j + 1) * gridLen / 2, startY + (2 * i + 1) * gridLen / 2, gridLen / 3 + 1);
    }
    else if (board[i][j] == 0) {
        setfillcolor(WHITE);
        setlinecolor(WHITE);
        fillcircle(startX + (2 * j + 1) * gridLen / 2, startY + (2 * i + 1) * gridLen / 2, gridLen / 3 + 1);
    }
}

// 高亮棋子
// 为当前棋子加绿 / 蓝边
void Board::highlightPiece(int startX, int startY, int gridLen, int i, int j) {
    if (board[i][j] == 1) setlinecolor(RGB(0, 240, 0));
    if (board[i][j] == 0) setlinecolor(RGB(0, 255, 255));
    setlinestyle(PS_SOLID, 2);
    circle(startX + (2 * j + 1) * gridLen / 2, startY + (2 * i + 1) * gridLen / 2, gridLen / 3);
    setlinestyle(PS_SOLID, 1);
}