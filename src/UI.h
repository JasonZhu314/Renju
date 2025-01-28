#pragma once
#include "AI.h"

// 结构体 : 按钮
struct Button
{
	int x, y, width, height;
	LPCTSTR label;

	void draw() const;						// 绘制按钮
	bool clicked(int xPos, int yPos) const; // 是否被点击
};

// 欢迎界面与结束界面
class Welcome
{
public:
	Welcome(int width, int height, LPCTSTR line);
	void draw() const;	// 绘制界面
	int detect() const; // 检测鼠标与键盘信号
	LPCTSTR title;		// 标题
private:
	Button NewGame, LoadGame, Quit;
	int windowW, windowH; // 窗口规格
};

// 模式选择 : PVP / PVC / 读盘启动
class Settings
{
public:
	Settings(int width, int height);
	void draw() const;
	int detect() const;

private:
	Button PVP, PVC, Back, Quit;
	int windowW, windowH;
};

// PVC模式选择先手后手
class ChooseSide
{
public:
	ChooseSide(int width, int height);
	void draw() const;
	int detect() const;

private:
	Button Black, White, Back;
	int windowW, windowH;
};

// 游戏类 : 游戏循环
class Game
{
public:
	Game(int width, int height, int x, int y, int len);
	void draw();			 // 绘制棋盘与界面
	void init_empty_board(); // 初始化空棋盘以及绘制
	void load_board();		 // 调用 Windows API 与 Board 的读盘函数
	void save_board();		 // 调用 Windows API 与 Board 的存盘函数
	int run();				 // 游戏循环
	Board board;			 // 本次游戏使用的棋盘
private:
	Button Back, Save, Undo;
	int windowW, windowH;
	int startX, startY, gridLen; // 棋盘规格
};

// 控制器 : 完整的程序循环
class Control
{
public:
	Control(int width, int height);
	void loop(); // 程序循环
private:
	/*
	state 表示控制器的状态
	0 - 退出
	1 - 欢迎界面
	2 - 选择游戏模式
	3 - 读盘
	4 - PVP
	5 - PVC
	6 - 游戏结束
	*/
	int state;
	int windowW, windowH;
	Welcome welcomePage, endPage;
	Settings settingsPage;
	ChooseSide choosePage;
	Game game;
};