#include "UI.h"

int main() {
	// ��ʼ��ģ��Ȩ��
	freopen("data.in", "r", stdin);
	init_pos_weight();
	read_weights();
	// ��ʼ��Ϸѭ��
	Control control(800, 800);
	control.loop();
	return 0;
}