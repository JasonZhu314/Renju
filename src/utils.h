#pragma once

#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <cstdio>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <queue>

using namespace std;

extern double weights[243], pos_weights[15][15];

/*
uitls.h 包含所有需要的头文件，以及 AI 的参数作为外部变量
注意 : utils.h 中定义的函数为第一版(即TUI版)中使用，本版本中已不再使用。
*/

char getCommand();                                       // 获取命令(含错误处理)
void getCoordinate(int &x, int &y, const string prompt); // 获取坐标(含错误处理)
fstream openFile(const string &prompt);                  // 打开文件(含错误处理)