#pragma once
#include <easyx.h>
//定义消息结构体变量
ExMessage msg = { 0 };
//鼠标在某个区域的判定函数 inArea(区域左上角x坐标,区域左上角y坐标,鼠标x坐标,鼠标y坐标,区域宽度,区域高度)
bool inArea(int x, int y, int mx, int my, int w, int h) {
	if (mx > x && mx<(x + w) && my>y && my < (y + h)) {
		return true;
	}
	return false;
}
//按钮的创建函数 button(按钮左上角x坐标,按钮左上角y坐标,按钮宽度,按钮高度,按钮内文本)
bool button(int x, int y, int w, int h, const char* text) {
	//鼠标在按钮上是一个颜色
	if (inArea(x, y, msg.x, msg.y, w, h)) {
		setfillcolor(RGB(255, 191, 107));
	}

	else {//不在就是另一个
		setfillcolor(RGB(233, 157, 66));
	}

	//画一个矩形按钮
	fillroundrect(x, y, x + w, y + h, 55, 55);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);//文本背景模式：透明
	settextstyle(35, 0, "微软雅黑");
	int hSpace = (w - textwidth(text)) / 2;
	int vSpace = (h - textheight(text)) / 2;
	outtextxy(x + hSpace, y + vSpace, text);
	if (msg.message == WM_LBUTTONDOWN && inArea(x, y, msg.x, msg.y, w, h)) {
		return true;
	}
	return false;
}