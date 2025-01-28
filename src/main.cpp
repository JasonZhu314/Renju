#include "UI.h"

int main() {
	// 初始化模型权重
	freopen("data.in", "r", stdin);
	init_pos_weight();
	read_weights();
	// 开始游戏循环
	Control control(800, 800);
	control.loop();
	return 0;
}