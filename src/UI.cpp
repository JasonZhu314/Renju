#include "UI.h"

// 绘制按钮
void Button::draw() const {
	setlinecolor(BLACK);
	setfillcolor(LIGHTGRAY);
	fillrectangle(x, y, x + width, y + height);
	settextstyle(20, 0, _T("Arial"));
	setbkmode(TRANSPARENT);
	outtextxy(x + (width - textwidth(label)) / 2, y + (height - textheight(label)) / 2, label);
}

// 给定鼠标位置，判断按钮是否被点击
bool Button::clicked(int xPos, int yPos) const {
	return (xPos >= x && xPos <= x + width && yPos >= y && yPos <= y + height);
}

// 初始化欢迎界面
// 结束界面与欢迎界面类似，仅需修改标题
Welcome::Welcome(int width, int height, LPCTSTR line) {
	windowW = width;
	windowH = height;
	title = line;
	NewGame = { windowW / 2 - 100, 200, 200, 50, _T("N - New Game") };
	LoadGame = { windowW / 2 - 100, 300, 200, 50, _T("L - Load Game") };
	Quit = { windowW / 2 - 100, 400, 200, 50, _T("Q - Quit") };
}

// 绘制界面 (使用EasyX作为图形库)
void Welcome::draw() const {
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(40, 0, _T("Arial"));
	setbkmode(TRANSPARENT);
	outtextxy((windowW - textwidth(title)) / 2, 100, title);
	NewGame.draw();
	LoadGame.draw();
	Quit.draw();
}

// 检测鼠标与键盘信号
// 返回值是控制器的 State
int Welcome::detect() const {
	while (true) {
		if (MouseHit()) {
			// 检测鼠标点击事件
			ExMessage msg = getmessage();
			if (msg.message == WM_LBUTTONDOWN) {
				int mx = msg.x, my = msg.y;
				if (NewGame.clicked(mx, my)) {
					return 2; // 若点击 New Game 状态转移为 2 (选择模式界面)
				}
				else if (LoadGame.clicked(mx, my)) {
					return 3; // 若点击 Load Game 状态转移为 3 (读盘启动)
				}
				else if (Quit.clicked(mx, my)) {
					return 0; // 若点击 Quit 状态转移为 0 (退出)
				}
			}
		}
		if (GetAsyncKeyState(VK_ESCAPE)) {
			return 0; // Esc 退出游戏
		}
		Sleep(10);
	}
}

// 模式选择 : PVP / PVC
Settings::Settings(int width, int height) {
	windowW = width;
	windowH = height;
	PVP = { windowW / 2 - 100, 200, 200, 50, _T("Player v.s Player") };
	PVC = { windowW / 2 - 100, 300, 200, 50, _T("Player v.s Computer") };
	Back = { windowW / 2 - 100, 400, 200, 50, _T("Back to Welcome Page") };
	Quit = { windowW / 2 - 100, 500, 200, 50, _T("Q - Quit") };
}

// 绘制界面
void Settings::draw() const {
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(40, 0, _T("Arial"));
	setbkmode(TRANSPARENT);
	outtextxy((windowW - textwidth(_T("Please select the game mode."))) / 2, 100, _T("Please select the game mode."));
	PVP.draw();
	PVC.draw();
	Back.draw();
	Quit.draw();
}

// 检测鼠标与键盘信号
// 返回值是控制器的 State
int Settings::detect() const {
	while (true) {
		if (MouseHit()) {
			ExMessage msg = getmessage();
			if (msg.message == WM_LBUTTONDOWN) {
				int mx = msg.x, my = msg.y;
				if (PVP.clicked(mx, my)) {
					return 4;
				}
				else if (PVC.clicked(mx, my)) {
					return 5;
				}
				else if (Back.clicked(mx, my)) {
					return 1; // 点击 Back 返回欢迎界面
				}
				else if (Quit.clicked(mx, my)) {
					return 0;
				}
			}
		}
		if (GetAsyncKeyState(VK_ESCAPE)) {
			return 0;
		}
		Sleep(10);
	}
}

// PVC模式选择先手后手
ChooseSide::ChooseSide(int width, int height) {
	windowW = width;
	windowH = height;
	Black = { windowW / 2 - 100, 200, 200, 50, _T("Black") };
	White = { windowW / 2 - 100, 300, 200, 50, _T("White") };
	Back = { windowW / 2 - 100, 400, 200, 50, _T("Back") };
}

// 绘制界面
void ChooseSide::draw() const {
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(40, 0, _T("Arial"));
	setbkmode(TRANSPARENT);
	outtextxy((windowW - textwidth(_T("Please select your color (black will go first)."))) / 2, 100, _T("Please select your color (black will go first)."));
	Black.draw();
	White.draw();
	Back.draw();
}

// 检测鼠标与键盘信号
int ChooseSide::detect() const {
	while (true) {
		if (MouseHit()) {
			ExMessage msg = getmessage();
			if (msg.message == WM_LBUTTONDOWN) {
				int mx = msg.x, my = msg.y;
				if (Black.clicked(mx, my)) {
					return 5;
				}
				else if (White.clicked(mx, my)) {
					return -5; // 注意 : 若选择白方，返回 -5
				}
				else if (Back.clicked(mx, my)) {
					return 2; // 点击 Back 返回选择模式界面
				}
			}
		}
		if (GetAsyncKeyState(VK_ESCAPE)) {
			return 0;
		}
		Sleep(10);
	}
}

// 游戏类 : 游戏循环
Game::Game(int width, int height, int x, int y, int len)
{
	windowW = width;
	windowH = height;
	startX = x;
	startY = y;
	gridLen = len;
	Back = { 0, 0, 100, 50, _T("Back") };
	Save = { 100, 0, 100, 50, _T("Save") };
	Undo = { 200, 0, 100, 50, _T("Undo") };

}

// 绘制界面及棋盘
void Game::draw() {
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(40, 0, _T("Arial"));
	Back.draw();
	Save.draw();
	Undo.draw();
	board.drawBoard(startX, startY, gridLen);
}

// 初始化与绘制空棋盘
void Game::init_empty_board() {
	board.reset();
	draw();
}

// 读盘
void Game::load_board() {
	// 调用 Windows API GetOpenFileName 函数，返回文件名
	OPENFILENAME ofn;
	wchar_t name[200] = L""; // 注意，返回值为 LPWSTR 而非c-string 或 string
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"txt";
	if (GetOpenFileName(&ofn) == TRUE) {
		board.loadBoard(wstring(ofn.lpstrFile)); // 调用 Board 类的成员函数读盘
		draw();
	}
	else {
		MessageBoxW(nullptr, L"Failed to load board! Automatically starting empty game in PVP mode.", L"ERROR", MB_OK | MB_ICONERROR);
		init_empty_board(); // 若读盘失败，则自动初始化PVP模式的空棋盘
		board.is_pvp = true;
	}
}

// 存盘
void Game::save_board() {
	// 调用 Windows API GetSaveFileName 函数，返回文件名
	OPENFILENAME ofn;
	wchar_t name[200] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"txt";
	if (GetSaveFileName(&ofn) == TRUE) {
		board.saveBoard(wstring(ofn.lpstrFile)); // 调用 Board 类的成员函数存盘
		MessageBoxW(nullptr, L"Successfully saved game!", L"Success", MB_OK | MB_ICONINFORMATION);
	}
	else {
		board.saveBoard(wstring(L"auto_save.txt")); // 若失败，则自动保存至 auto_save.txt
	}
}

// 游戏循环
int Game::run() {
	int win_end, i, j;
	while (win_end = board.isGameOver(), win_end == board.notOver) {
		// 轮到玩家落子
		if (board.is_pvp || (board.is_player_black ^ (!board.currentPlayer))) {
			while (true) {
				if (MouseHit()) {
					ExMessage msg = getmessage();
					if (msg.message == WM_LBUTTONDOWN) {
						int mx = msg.x, my = msg.y;
						if (Back.clicked(mx, my)) {
							return 2;
						}
						else if (Undo.clicked(mx, my)) {
							if (board.numPieces) {
								if (!board.is_pvp) board.undo(); // 若为PVC模式，悔棋需要两步
								board.undo();
								draw(); // 悔棋后重新绘制棋盘
							}
							else {
								MessageBeep(MB_ICONWARNING); // 若无棋子，发出警告音
							}
							break;
						}
						else if (Save.clicked(mx, my)) {
							save_board();
							continue;
						}
						i = (my - startY) / gridLen; j = (mx - startX) / gridLen;
						if (board.inBoard(i, j) && board.board[i][j] == -1) {
							// 注意 : 为避免闪屏，每步添加棋子而非重画棋盘
							if (board.preAction != -1) board.coverPiece(startX, startY, gridLen, board.preAction / 15, board.preAction % 15); // 盖住上一步高亮的棋子
							board.doAction(i, j);                                                                                             // 玩家落子 i行 j列
							board.drawPiece(startX, startY, gridLen, i, j);                                                                   // 绘制棋子
							board.highlightPiece(startX, startY, gridLen, i, j);                                                              // 高亮棋子
							break;
						}
					}
				}
				if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState('q')) {
					return 0;
				}
				Sleep(10);
			}
		}
		// 轮到 AI 落子
		else {
			int move = action(board, !board.is_player_black);
			if (board.preAction != -1) board.coverPiece(startX, startY, gridLen, board.preAction / 15, board.preAction % 15);
			board.doAction(move / 15, move % 15);
			board.drawPiece(startX, startY, gridLen, move / 15, move % 15);
			board.highlightPiece(startX, startY, gridLen, move / 15, move % 15);
		}
	}
	// 白方赢
	if (win_end == 0) {
		MessageBoxW(nullptr, L"White won!", L"Game Over", MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
	}
	// 黑方赢
	else if (win_end == 1) {
		MessageBoxW(nullptr, L"Black won!", L"Game Over", MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
	}
	// 平局
	else if (win_end == 3) {
		MessageBoxW(nullptr, L"It's a draw.", L"Game Over", MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
	}
	// 返回状态 6 (结束界面)
	return 6;
}

// 控制器 : 完整的程序循环
Control::Control(int width, int height) :
	windowW(width),
	windowH(height),
	state(1),
	welcomePage(width, height, _T("Welcome to Renju!")),
	endPage(width, height, _T("Game Over! What's Next?")),
	settingsPage(width, height),
	choosePage(width, height),
	game(width, height, 100, 100, (width - 200) / 15){ }

/*
0 - 退出
1 - 欢迎界面
2 - 选择游戏模式
3 - 读盘
4 - PVP
5 - PVC
6 - 游戏结束
*/
void Control::loop() {
	initgraph(windowW, windowH);
	// 状态转移
	while (state) {
		switch (state) {
		case 1:
			welcomePage.draw();
			state = welcomePage.detect();
			break;
		case 2:
			settingsPage.draw();
			state = settingsPage.detect();
			break;
		case 3:
			game.load_board();
			state = game.run();
			break;
		case 4:
			game.board.is_pvp = true;
			game.init_empty_board();
			state = game.run();
			break;
		case 5:
			game.board.is_pvp = false;
			choosePage.draw();
			switch (choosePage.detect()) {
			case 0:
				state = 0;
				break;
			case 2:
				state = 2;
				break;
			case 5:
				game.board.is_player_black = true;
				game.init_empty_board();
				state = game.run();
				break;
			// 注意 : 此处用正负区分玩家先手后手
			case -5:
				game.board.is_player_black = false;
				game.init_empty_board();
				state = game.run();
				break;
			}
			break;
		case 6:
			endPage.draw();
			state = endPage.detect();
			break;
		}
	}
}
