#include"many.h"
#include"single.h"

int level = 0;
int jingyan = 0;
char ww[8] = "";



void n() {
	setbkcolor(EGERGB(200, 228, 176));
	char le[8];
	char jy[8];
a:
	inputbox_getline("选择或新建用户", "请输入用户名", ww, 8);
	string str = (string)"./" + ww + ".ini";
	FILE *fp = fopen(str.c_str(), "r");
	if (fp == NULL) {
		if (MessageBox(GetForegroundWindow(), "用户不存在,是否新建?", "询问", MB_YESNO | MB_ICONQUESTION) == IDYES) {
			WritePrivateProfileString("User", "Name", ww, str.c_str());
			WritePrivateProfileString("User", "Level", "0", str.c_str());
			WritePrivateProfileString("User", "Exper", "0", str.c_str());
		} else {
			goto a;
		}
	}
	return;
}

mouse_msg msg;

int FrontPage() {
	msg.x = 0;
	msg.y = 0;
	string str = (string)"./" + ww + ".ini";
	GetPrivateProfileString("User", "Name", "None", name, sizeof(name), str.c_str());
	level = GetPrivateProfileInt("User", "Level", 0, str.c_str());
	jingyan = GetPrivateProfileInt("User", "Exper", 0, str.c_str());
	settextjustify(CENTER_LINE, CENTER_TEXT);
	int i = 0;
	setfont(200, 0, "方正姚体");
	settextcolor(EGERGBA(0, 0, 150, 0xFF));
	ege_drawtext("猜灯谜大作战", cx / 2, cy / 5);
	BOOL ispress1 = TRUE;
	setfillcolor(EGERGB(50, 150, 255));
	setfont(60, 0, "华文楷体");
	settextcolor(WHITE);
	fillrect(cx / 10 * 3, cy / 2, cx / 10 * 7, cy / 2 + 60);
	ege_drawtext("练习模式", cx / 2, cy / 2);
	fillrect(cx / 10 * 3, cy / 2 + 80, cx / 10 * 7, cy / 2 + 140);
	ege_drawtext("闯关模式", cx / 2, cy / 2 + 80);
	fillrect(cx / 10 * 5.5, cy / 2 + 160, cx / 10 * 7, cy / 2 + 220);
	ege_drawtext("退出", cx / 10 * 6.25, cy / 2 + 160);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	setfont(20, 0, "华文楷体");
	settextcolor(EGERGB(0, 0, 0));
	ege_drawtext(name, 0, cy - 60);
	ege_drawtext("等级:", 0, cy - 40);
	ege_drawtext(to_string(level).c_str(), 50, cy - 40);
	ege_drawtext("经验值:", 0, cy - 20);
	ege_drawtext(to_string(jingyan).c_str(), 70, cy - 20);
	delay_fps(60);

	while (ispress1) {
c:
		msg = getmouse();
		int x = msg.x;
		int y = msg.y;
		if (cy / 2 < y && y < cy / 2 + 60 && cx / 10 * 3 < x && x < cx / 10 * 7) {
			//单人
			if (msg.is_left() && msg.is_down()) {
				return 1;
			}

		}
		if (cy / 2 + 80 < y && y < cy / 2 + 140 && cx / 10 * 3 < x && x < cx / 10 * 7) {
			//多人
			if (msg.is_left() && msg.is_down()) {
				return 2;
			}

		}
		if (cy / 2 + 160 < y && y < cy / 2 + 220 && cx / 10 * 3 < x && x < cx / 10 * 7) {
			if (msg.is_left() && msg.is_down()) {
				exit(0);
			}
		}
	}
	return -1;
}

int main() {
	srand(time(0));
	ncx = cx;
	ncy = cy;
	initgraph(-1, -1, INIT_NOBORDER);
	setrendermode(RENDER_MANUAL);
	setlinecolor(EGERGB(0, 0, 255));
	setlinewidth(10);
	n();
	while (1) {
		cleardevice();
		delay_fps(60);
		int what = FrontPage();
		switch (what) {
			case 1:
				single();
				break;
			case 2:
				many();
				break;
			default:
				break;
		}
		Sleep(100);
	}
	return 0;
}
