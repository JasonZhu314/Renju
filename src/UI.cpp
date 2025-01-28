#include "UI.h"

// ���ư�ť
void Button::draw() const {
	setlinecolor(BLACK);
	setfillcolor(LIGHTGRAY);
	fillrectangle(x, y, x + width, y + height);
	settextstyle(20, 0, _T("Arial"));
	setbkmode(TRANSPARENT);
	outtextxy(x + (width - textwidth(label)) / 2, y + (height - textheight(label)) / 2, label);
}

// �������λ�ã��жϰ�ť�Ƿ񱻵��
bool Button::clicked(int xPos, int yPos) const {
	return (xPos >= x && xPos <= x + width && yPos >= y && yPos <= y + height);
}

// ��ʼ����ӭ����
// ���������뻶ӭ�������ƣ������޸ı���
Welcome::Welcome(int width, int height, LPCTSTR line) {
	windowW = width;
	windowH = height;
	title = line;
	NewGame = { windowW / 2 - 100, 200, 200, 50, _T("N - New Game") };
	LoadGame = { windowW / 2 - 100, 300, 200, 50, _T("L - Load Game") };
	Quit = { windowW / 2 - 100, 400, 200, 50, _T("Q - Quit") };
}

// ���ƽ��� (ʹ��EasyX��Ϊͼ�ο�)
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

// ������������ź�
// ����ֵ�ǿ������� State
int Welcome::detect() const {
	while (true) {
		if (MouseHit()) {
			// ���������¼�
			ExMessage msg = getmessage();
			if (msg.message == WM_LBUTTONDOWN) {
				int mx = msg.x, my = msg.y;
				if (NewGame.clicked(mx, my)) {
					return 2; // ����� New Game ״̬ת��Ϊ 2 (ѡ��ģʽ����)
				}
				else if (LoadGame.clicked(mx, my)) {
					return 3; // ����� Load Game ״̬ת��Ϊ 3 (��������)
				}
				else if (Quit.clicked(mx, my)) {
					return 0; // ����� Quit ״̬ת��Ϊ 0 (�˳�)
				}
			}
		}
		if (GetAsyncKeyState(VK_ESCAPE)) {
			return 0; // Esc �˳���Ϸ
		}
		Sleep(10);
	}
}

// ģʽѡ�� : PVP / PVC
Settings::Settings(int width, int height) {
	windowW = width;
	windowH = height;
	PVP = { windowW / 2 - 100, 200, 200, 50, _T("Player v.s Player") };
	PVC = { windowW / 2 - 100, 300, 200, 50, _T("Player v.s Computer") };
	Back = { windowW / 2 - 100, 400, 200, 50, _T("Back to Welcome Page") };
	Quit = { windowW / 2 - 100, 500, 200, 50, _T("Q - Quit") };
}

// ���ƽ���
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

// ������������ź�
// ����ֵ�ǿ������� State
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
					return 1; // ��� Back ���ػ�ӭ����
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

// PVCģʽѡ�����ֺ���
ChooseSide::ChooseSide(int width, int height) {
	windowW = width;
	windowH = height;
	Black = { windowW / 2 - 100, 200, 200, 50, _T("Black") };
	White = { windowW / 2 - 100, 300, 200, 50, _T("White") };
	Back = { windowW / 2 - 100, 400, 200, 50, _T("Back") };
}

// ���ƽ���
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

// ������������ź�
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
					return -5; // ע�� : ��ѡ��׷������� -5
				}
				else if (Back.clicked(mx, my)) {
					return 2; // ��� Back ����ѡ��ģʽ����
				}
			}
		}
		if (GetAsyncKeyState(VK_ESCAPE)) {
			return 0;
		}
		Sleep(10);
	}
}

// ��Ϸ�� : ��Ϸѭ��
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

// ���ƽ��漰����
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

// ��ʼ������ƿ�����
void Game::init_empty_board() {
	board.reset();
	draw();
}

// ����
void Game::load_board() {
	// ���� Windows API GetOpenFileName �����������ļ���
	OPENFILENAME ofn;
	wchar_t name[200] = L""; // ע�⣬����ֵΪ LPWSTR ����c-string �� string
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"txt";
	if (GetOpenFileName(&ofn) == TRUE) {
		board.loadBoard(wstring(ofn.lpstrFile)); // ���� Board ��ĳ�Ա��������
		draw();
	}
	else {
		MessageBoxW(nullptr, L"Failed to load board! Automatically starting empty game in PVP mode.", L"ERROR", MB_OK | MB_ICONERROR);
		init_empty_board(); // ������ʧ�ܣ����Զ���ʼ��PVPģʽ�Ŀ�����
		board.is_pvp = true;
	}
}

// ����
void Game::save_board() {
	// ���� Windows API GetSaveFileName �����������ļ���
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
		board.saveBoard(wstring(ofn.lpstrFile)); // ���� Board ��ĳ�Ա��������
		MessageBoxW(nullptr, L"Successfully saved game!", L"Success", MB_OK | MB_ICONINFORMATION);
	}
	else {
		board.saveBoard(wstring(L"auto_save.txt")); // ��ʧ�ܣ����Զ������� auto_save.txt
	}
}

// ��Ϸѭ��
int Game::run() {
	int win_end, i, j;
	while (win_end = board.isGameOver(), win_end == board.notOver) {
		// �ֵ��������
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
								if (!board.is_pvp) board.undo(); // ��ΪPVCģʽ��������Ҫ����
								board.undo();
								draw(); // ��������»�������
							}
							else {
								MessageBeep(MB_ICONWARNING); // �������ӣ�����������
							}
							break;
						}
						else if (Save.clicked(mx, my)) {
							save_board();
							continue;
						}
						i = (my - startY) / gridLen; j = (mx - startX) / gridLen;
						if (board.inBoard(i, j) && board.board[i][j] == -1) {
							// ע�� : Ϊ����������ÿ��������Ӷ����ػ�����
							if (board.preAction != -1) board.coverPiece(startX, startY, gridLen, board.preAction / 15, board.preAction % 15); // ��ס��һ������������
							board.doAction(i, j);                                                                                             // ������� i�� j��
							board.drawPiece(startX, startY, gridLen, i, j);                                                                   // ��������
							board.highlightPiece(startX, startY, gridLen, i, j);                                                              // ��������
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
		// �ֵ� AI ����
		else {
			int move = action(board, !board.is_player_black);
			if (board.preAction != -1) board.coverPiece(startX, startY, gridLen, board.preAction / 15, board.preAction % 15);
			board.doAction(move / 15, move % 15);
			board.drawPiece(startX, startY, gridLen, move / 15, move % 15);
			board.highlightPiece(startX, startY, gridLen, move / 15, move % 15);
		}
	}
	// �׷�Ӯ
	if (win_end == 0) {
		MessageBoxW(nullptr, L"White won!", L"Game Over", MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
	}
	// �ڷ�Ӯ
	else if (win_end == 1) {
		MessageBoxW(nullptr, L"Black won!", L"Game Over", MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
	}
	// ƽ��
	else if (win_end == 3) {
		MessageBoxW(nullptr, L"It's a draw.", L"Game Over", MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
	}
	// ����״̬ 6 (��������)
	return 6;
}

// ������ : �����ĳ���ѭ��
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
0 - �˳�
1 - ��ӭ����
2 - ѡ����Ϸģʽ
3 - ����
4 - PVP
5 - PVC
6 - ��Ϸ����
*/
void Control::loop() {
	initgraph(windowW, windowH);
	// ״̬ת��
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
			// ע�� : �˴�����������������ֺ���
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
