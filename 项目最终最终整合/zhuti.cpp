#include"Button.h"
#include <windows.h>
#include<fstream>
#include<conio.h>
#include<string>
#include<iostream>
//#include<bits/stdc++.h>
#include<mmsystem.h>
#pragma comment(lib,"WINMM.LIB")
#pragma comment( lib, "MSIMG32.LIB")
using namespace std;
#define enemy_Num 99 //敌人数量
#define bullet_Num 500 //子弹数量
#define enemybullet0_Num 500 //小型敌人0子弹数量
#define enemybullet1_Num 100 //中型敌人1子弹数量
#define bossbullet_Num 5 //boss子弹数量
#define height 650 //页面宽高
#define width 765

IMAGE Bullet;//玩家子弹
IMAGE role;//玩家贴图
IMAGE enemyimg[2];//敌人贴图
IMAGE Enemybullet[2];//两种敌人的子弹
IMAGE _background;//游戏界面背景
IMAGE lose_background;//失败背景
IMAGE win_background;//胜利背景
IMAGE bossimg;//boss贴图
IMAGE bossbulletimg;//boss子弹贴图
IMAGE stopimg;//暂停贴图
IMAGE stopbackground;//暂停背景
IMAGE changediff;//设置难度背景
IMAGE record_background;//查看记录背景



//登录与菜单部分的声明

//用户结构体(链表)
struct User
{
	char name[20];
	char password[20];
	//char passback_answer[3][10];安全问题暂时用不到
	int point;
	struct User* next;
};
User* p = NULL, * p1 = NULL, * p2 = NULL, * head = NULL;//读取用户信息要用到
int n = 0;//用户的数量

//各个图片素材的初始化  (有待补充)
IMAGE register_background;//注册背景
IMAGE login_background;//登录背景
IMAGE menu_background;//游戏主菜单背景
IMAGE Producerlistimg;//制作人名单

string title;//游戏的名称
char username[20] = "";// 全局变量，用于保存账号的内容
char password[20] = "";// 全局变量，用于保存密码的内容
char username1[20] = "";// 全局变量，用于保存账号的内容
char password1[20] = "";// 全局变量，用于保存密码的内容
char password2[20] = "";// 全局变量，用于保存密码的内容
const int INPUT_LAYER_ID = 1; // 文本框所在的层级ID

int ms;//敌人出现间隔
int bullet_ms; //子弹间隔
int score = 0;//游戏得分
//模型的移动单位速度
int rank;//级别,难度
int speed;//子弹移动速度
int enemybulletspeed;//敌人子弹移动速度
int boss_spawn;
bool b_boosMusic;//boss音乐的播放判定

int playerhp = 100;//玩家血量
//飞机以及各个角色的结构体初始化
struct plane {
	int x;//对象的x坐标
	int y;//对象的y坐标
	bool live;//是否存活（玩家和敌人），是否存在（是子弹的话）
	int hp;//生命值
	int enemywidth;//敌人单位宽
	int enemyheight;//敌人单位高
	int bossheight;//boss高
	int bosswidth;//boss宽


	int type;//敌机类型 小 中
}enemy[enemy_Num], player, boss, bullet[bullet_Num], enemybullet0[enemybullet0_Num], enemybullet1[enemybullet1_Num], bossbullet[3][bossbullet_Num];


//登录注册

void drawInputBox(int, int, int, int, char*);//输入框函数
void login();//登录函数
void login_action(char[], char[]);//处理登录界面操作的函数
int  login_state(char[], char[]);//登录状态判断函数
void registered();//注册函数
int file_size();//文件信息判断函数
void file_read();//读取文件信息函数
void do_free();//释放动态内存函数
void add();//添加用户
void registered_action();//处理注册界面操作的函数
void file_save();//信息保存文件函数
void GameMenuView();//游戏主菜单界面
void ChangeDifficult();//改变难度
void ProducerlistView();//制作人名单
void check_score();//查看记录


//游戏中

void GameInit();//游戏初始化
void PlayerMove();//玩家的移动
void LoadGameImg();//载入游戏贴图
void Gamedraw();//游戏单位的贴图
void PlayGame();//游戏逻辑主体
void CreateBullet();//创建子弹
void enemyhp(int i);//敌人类型生成
void CreateEnemy();//创建敌人
void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg);//透明背景
void ctrlFps(int start_time);//z帧率调整
void BossMove();//boss的移动
void GameOverview();//游戏结束
void GameWinview();//游戏胜利



void drawInputBox(int x, int y, int inwidth, int inheight, char* text) {
	setfillcolor(WHITE);
	solidrectangle(x, y, x + inwidth, y + inheight);
	settextcolor(BLACK);
	rectangle(x, y, x + inwidth, y + inheight);
	settextcolor(BLACK);
	outtextxy(x + 5, y + 5, text);
	if (msg.message == WM_LBUTTONDOWN && inArea(x, y, msg.x, msg.y, inwidth, inheight)) {
		InputBox(text, 100, "请输入", "输入框");
	}
}

//登录
void login() {
	HWND hnd = GetHWnd();
	initgraph(1024, 576);
	setbkmode(TRANSPARENT);
	cleardevice();
	mciSendString("open menu1.mp3", NULL, 0, NULL);
	mciSendString("play menu1.mp3 repeat", NULL, 0, NULL);
	//mciSendString("setaudio music volume to 8", 0, 0, 0);
	loadimage(&login_background, "res\\login\\login_background.png", 1024, 576);
	putimage(0, 0, &login_background);
	setfillcolor(LIGHTGRAY);
	char arr[] = "飞坤大战";
	char arr1[] = "账号";
	char arr2[] = "密码";
	settextstyle(100, 0, "楷体");
	settextcolor(GREEN);
	// 绘制输入
	int loginwidth = 1024 / 2 - textwidth(arr) / 2;
	int loginheight = 0;
	outtextxy(loginwidth, loginheight + 30, arr);
	char text1[] = "成为ikun";
	char text2[] = "ikun登录";
	char text3[] = "退出游戏";

	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (button(340, 400, 150, 50, text1)) {
				// 成为ikun 被点击
				//关闭当前界面
				//打开注册界面
				closegraph();
				registered();
				break;
				cout << "成为ikun 被点击" << endl;
			}
			else if (button(540, 400, 150, 50, text2)) {
				// ikun登录 被点击
				login_action(username, password);
				cout << "ikun登录 被点击" << endl;
			}
			else if (button(440, 475, 150, 50, text3)) {
				// 退出游戏 被点击
				cout << "退出游戏 被点击" << endl;
				exit(0);
				break;
			}
		}
		settextstyle(50, 0, "楷体");
		settextcolor(BLACK);
		outtextxy(200, 200, arr1);
		outtextxy(200, 300, arr2);
		drawInputBox(300, 200, 450, 50, username);
		drawInputBox(300, 300, 450, 50, password);
		FlushBatchDraw();
		BeginBatchDraw();
	}
	
	getchar();
	closegraph();
}

//实现登录界面的功能的函数
void login_action(char name[], char password[]) {
	int flag = login_state(name, password);
	switch (flag) {
	case 0:
		cout << "登录成功!" << endl;
		cleardevice();
		GameMenuView();
		break;
	case 1: {
		cout << "该账号不存在" << endl;
		//获取窗口句柄
		HWND hnd = GetHWnd();
		//弹出窗口，提示用户操作
		int ok = MessageBox(hnd, "账号信息为空或账号不存在,是否去注册", "提示", MB_OKCANCEL);
		if (ok == 1) {
			cleardevice();
			registered();
		}
		break;
	}
	case 2: {
		HWND hnd = GetHWnd();
		//弹出窗口，提示用户操作
		int ok = MessageBox(hnd, "密码为空或密码错误，请重新输入密码", "提示", MB_OKCANCEL);
		cout << "密码错误" << endl;
	}
	}
}

//注册
void registered() {
	initgraph(1024, 576);
	setbkmode(TRANSPARENT);
	cleardevice();
	// 绘制输入
	loadimage(&register_background, "res\\registered\\registered_background.png", 1024, 576);
	putimage(0, 0, &register_background);
	char text1[] = "返回登录";
	char text2[] = "确认注册";
	char arr1[] = "账号";
	char arr2[] = "密码";
	char arr3[] = "确认密码";
	while (1) {

		BeginBatchDraw();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (button(350, 450, 150, 50, text1)) {
				// 返回登录 被点击
				closegraph();
				login();
				break;
			}
			else if (button(550, 450, 150, 50, text2)) {
				// 确认注册 被点击
				registered_action();
				//closegraph();
				//主菜单界面
				cout << "按钮二被点击" << endl;

			}
		}
		settextstyle(50, 0, "楷体");
		settextcolor(BLACK);
		outtextxy(200, 150, arr1);
		outtextxy(200, 250, arr2);
		outtextxy(100, 350, arr3);
		drawInputBox(300, 150, 450, 50, username1);
		drawInputBox(300, 250, 450, 50, password1);
		drawInputBox(300, 350, 450, 50, password2);
		FlushBatchDraw();
	}
	getchar();
	closegraph();
}

//处理注册的功能的函数
void registered_action() {
	if (strlen(password1) < 6) {
		HWND hnd = GetHWnd();
		//弹出窗口，提示用户操作
		int ok = MessageBox(hnd, "密码过短", "提示", MB_OKCANCEL);
	}
	else if (strcmp(password1, password2) != 0) {
		HWND hnd = GetHWnd();
		//弹出窗口，提示用户操作
		int ok = MessageBox(hnd, "密码不一致", "提示", MB_OKCANCEL);
	}
	else {
		add();
		file_save();
		do_free();
		login();
	}
}

//文件信息判断函数
int file_size()
{
	FILE* p = fopen("user.txt", "r");
	int size = 0;
	fseek(p, 0, 2);  //范围从0起始位至末尾
	size = ftell(p);  //判断文件大小,若为空，size=0 
	fclose(p);
	return size;
}

//读取文件信息函数
void file_read()
{
	if (file_size())
	{
		FILE* fp;
		if ((fp = fopen("./user.txt", "rb")) == NULL)
		{
			printf("无法打开文件!");
			exit(0);
		}
		do
		{
			p1 = (struct User*)malloc(sizeof(struct User));
			if (n == 0)
				head = p1;
			else
				p2->next = p1;
			fread(p1, sizeof(struct User), 1, fp);//读文件，读到底p1=NULL
			p2 = p1;
			n += 1;
		} while (p2->next != NULL);
		fclose(fp);
		printf("读取完成!\n");

	}

}

//信息保存文件函数
void file_save()
{
	FILE* fp;
	struct User* p;
	if ((fp = fopen("./user.txt", "wb")) == NULL)
		printf("无法打开文件!");
	p = head;
	while (p != NULL)
	{
		if (fwrite(p, sizeof(struct User), 1, fp) != 1)
		{
			printf("写入数据出错\n");
			fclose(fp);
			break;
		}
		p = p->next;
	}
	fclose(fp);
	printf("存储完成\n");
}




int login_state(char username[], char pass[])
{
	if (file_size() == 0)
		return 1;
	file_read();
	p = head;
	while (p != NULL)
	{
		if (strcmp(p->name, username) == 0)
		{
			if (strcmp(p->password, pass) != 0)
			{
				do_free();
				return 2;//密码错误
			}
			else {
				do_free();
				return 0;
			}//登录成功
		}
		p = p->next;
	}
	do_free();
	return 1;//账号不存在
}


void add()//添加用户
{
	file_read();
	p1 = (struct User*)malloc(sizeof(struct User));
	if (n == 0)
		head = p1;
	else
		p2->next = p1;
	strcpy_s(p1->name, username1);
	strcpy_s(p1->password, password1);
	p1->next = NULL;
	p2 = p1;
	printf("添加成功");
}

//释放动态内存函数
void do_free()
{
	struct User* f;
	do
	{
		f = head;
		head = head->next;
		free(f);
	} while (head != NULL);
	n = 0;
	printf("清除完成");
}




void save_score(int mark)//分数保存函数
{
	file_read();
	p = head;
	while (p != NULL)
	{
		if (strcmp(p->name, username) == 0)
		{
			if (p->point < mark)
			{
				p->point = mark;
				file_save();
			}
			break;
		}
		else p = p->next;
	}
	do_free();
}

//查询记录
void check_score()
{
	initgraph(1024, 768);
	loadimage(&record_background, "./record_background.png", 1024, 768);
	putimage(0, 0, &record_background);
	setbkmode(TRANSPARENT);

	int nowuser_score = 0;
	file_read();//读取文件内容获得分数
	p = head;
	while (p != NULL)
	{
		if (strcmp(p->name, username) == 0) {
			if(p->point>=0)
			nowuser_score = p->point;//记录当前用户的最高得分
		}
		p = p->next;
	}
	do_free();
	string str = to_string(nowuser_score);
	char s[15];//保存分数的字符串
	strcpy(s, str.c_str());


	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "MicroSoft YaHei");
	settextcolor(BLACK);
	outtextxy(750, 250, "坤坤的最高分:");




	//settextcolor(BLUE);
	settextstyle(50, 50, "楷体");
	//setbkmode(TRANSPARENT);
	settextcolor(RGB(255, 29, 29));
	outtextxy(750, 320, s);

	while (true) {   //点击判断
		if (peekmessage(&msg, EX_MOUSE)) {}//获取鼠标消息
		BeginBatchDraw();
		//点这个可以直接返回到游戏主菜单
		if (button(320, 500, 200, 60, "返回菜单"))
		{
			return GameMenuView();
		}
		EndBatchDraw();
		msg.message = 0; // 循环结束后全局变量msg归零
	}

}

int hard = 1;//难度默认1
void ChangeDifficult()
{
	//此函数不返回值，但必须要有一个全局变量hard,来调整难度，是1,2,3档，调整完难度后，回到主菜单
	//closegraph();
	initgraph(1024, 576);
	loadimage(&changediff, "./changediff.png");
	putimage(0, 0, &changediff);
	HWND hnd = GetHWnd();
	
	int wide = 300;
	int hight = 100;
	setbkmode(TRANSPARENT);
	setbkmode(TRANSPARENT);
	settextstyle(25, 0, "MicroSoft YaHei");
	settextcolor(BLACK);
	outtextxy(618, 500, "按住WASD控制坤坤上下左右移动，按K发射篮球" );
	outtextxy(618, 520, "击败狂暴坤坤 即可获得游戏胜利");
	//可能在gamemenuview函数中已经使用Beginbatchdraw函数，此处也许不再使用
	while (1)
	{
		if (peekmessage(&msg, EX_MOUSE))
		{
			BeginBatchDraw();
			if (button((1024 - wide), 50, wide, hight, "简单"))
			{
				hard = 1;
				break;
			}
			else if (button((1024 - wide), 50 + hight + 50, wide, hight, "普通"))
			{
				hard = 2;
				break;
			}
			else if (button((1024 - wide), 50 + hight + 50 + hight + 50, wide, hight, "困难"))
			{
				hard = 3;
				break;
			}
			EndBatchDraw();
		}
	}
	return GameMenuView();
}

void ProducerlistView() {
	initgraph(797, 800);
	loadimage(&Producerlistimg, "./Producerlistimg.png");
	//清空画面
	putimage(0, 0, &Producerlistimg);
	//加载背景图片
		while (true) {   //点击判断
			if (peekmessage(&msg, EX_MOUSE)) {}//获取鼠标消息
			BeginBatchDraw();
			//点这个可以直接返回到游戏主菜单
			if (button(0, 400, 250, 60, "返回菜单"))
			{
				return GameMenuView();
			}
			EndBatchDraw();
			msg.message = 0; // 循环结束后全局变量msg归零
		}
	
	

}







//游戏主菜单界面
void GameMenuView() {
	initgraph(1024, 768, 0);
	loadimage(&menu_background, "res\\game_menu\\game_menu_background.jpg", getwidth(), getheight());
	putimage(0, 0, &menu_background);
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(150, 80, "楷体");
	setbkmode(TRANSPARENT);
	settextcolor(RGB(172, 172, 172));
	outtextxy(100, 56, "蔡单");

	while (true)
	{
		peekmessage(&msg, EX_MOUSE);
		BeginBatchDraw();
		if (button(700, 100, 200, 60, "开始游戏"))
		{
			mciSendString("close menu1.mp3", NULL, 0, NULL);
			LoadGameImg();
			PlayGame();
			
		}
		if (button(700, 200, 200, 60, "修改难度"))
		{
			cout << "ChangeDifficult()" << endl;
			ChangeDifficult();
		}
		if (button(700, 300, 200, 60, "查看记录"))
		{
			cout << "CheckRecording()" << endl;
				check_score();
		}
		if (button(700, 400, 200, 60, "制作人"))
		{
			cout << "ProducerlistView()" << endl;
			ProducerlistView();
		}
		if (button(700, 500, 200, 60, "退出游戏"))
		{
			exit(0);
		}
		EndBatchDraw();
		msg.message = 0;
	}
	cleardevice();
	getchar();
}







void GameOverview() {

	initgraph(1024, 768);	//创建窗口和调试，整合的时候得删
	//清空画面
	cleardevice();
	//加载背景图片
	loadimage(&lose_background, "./lose.jpg", 1024, 768);
	putimage(0, 0, &lose_background);
	//文字部分
	settextstyle(72, 0, "隶书");	//文本格式：(高度，宽度<自适应就填0>，字体)
	settextcolor(RED);//文本颜色
	setbkmode(TRANSPARENT);//文本背景模式：透明
	outtextxy(30, 100, "挑战失败");
	settextcolor(BLACK);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(35, 200, "疑似你练习时长不足两年半");
	settextcolor(RED);
	outtextxy(35, 270, "把眼镜摘了");
	settextcolor(BLACK);
	outtextxy(140, 270, "歇一歇试试？");
	outtextxy(36, 520, "或者...");
	PlaySound("gamefail.wav", NULL, SND_FILENAME | SND_ASYNC);
	//按钮部分
	while (true) {//点击判断
		if (peekmessage(&msg, EX_MOUSE)) {}//获取鼠标消息
		//双缓冲绘图解决频闪刷新问题：
		BeginBatchDraw();

		if (button(35, 330, 280, 60, "返回菜单"))
		{//点击返回菜单，跳转至菜单界面
			mciSendString("open menu1.mp3", NULL, 0, NULL);
			mciSendString("play menu1.mp3 repeat", NULL, 0, NULL);
			mciSendString("setaudio music volume to 8", 0, 0, 0);
			GameMenuView();    
		}

		if (button(35, 420, 280, 60, "重新开始"))
		{//点击重新开始，跳转至游戏画面绘制然后开始游戏
			Gamedraw();
			PlayGame();
		}

		if (button(36, 550, 280, 60, "原地复活！"))
		{//点击弹出对话框
			HWND hnd = GetHWnd();
			//MessageBox(hnd, "原则上是不可以的~\n但是假如你愿意V hali 50我也不是不能瞒着小落...", "你想原地复活吗？", MB_OKCANCEL);
			if (MessageBox(NULL, ("原则上是不可以的~\n但是假如你愿意V hali 50我也不是不能瞒着小落..."), ("你想原地复活吗？"), MB_YESNO | MB_ICONINFORMATION) == IDYES) {//判断点击的按钮是不是 "是"
				MessageBox(hnd, "那也不行~", "嘿嘿", MB_OKCANCEL);
			}
			else MessageBox(hnd, "其实你就算愿意我也没办法复活你", "嘿嘿", MB_OKCANCEL);
		}
		
		EndBatchDraw();
		//将绘图过程放在两个函数之间
		msg.message = 0;//循环结束后全局变量msg归零
	}
}


void GameWinview() {

	//创建窗口和调试，整合的时候得删
	initgraph(1024, 768);
	//mciSendString("close song", NULL, 0, NULL);
	PlaySound("gamewin.wav", NULL, SND_FILENAME | SND_ASYNC);
	//清空画面
	cleardevice();
	//加载背景图片
	loadimage(&win_background, "./win.png", 1024, 768);
	putimage(0, 0, &win_background);
	//文字部分
	settextstyle(90, 0, "隶书");	//文本格式：(高度，宽度<自适应就填0>，字体)
	settextcolor(RED);//文本颜色
	setbkmode(TRANSPARENT);//文本背景模式：透明
	outtextxy(750, 70, "WIN!!!!");//
	settextstyle(40, 0, "隶书");
	outtextxy(840, 170, "得分");
	outtextxy(755, 225, "114514");
	//分数显示框
	setfillcolor(WHITE);//填充颜色
	solidrectangle(750, 220, 1000, 290);//无边框的实心矩形

	string winbkg_score = "test";//创建字符串用以接收计分的数据
	outtextxy(755, 235, ("分数:" + to_string(score)).data());//分数显示位置
	settextcolor(RGB(184, 134, 248));
	settextstyle(30, 0, "微软雅黑");
	outtextxy(785, 295, "哥哥为你 笔！ 芯！");
	//按钮部分
	while (true) {//点击判断
		if (peekmessage(&msg, EX_MOUSE)) {}//获取鼠标消息
		//双缓冲绘图解决频闪刷新问题：
		BeginBatchDraw();

		if (button(750, 355, 250, 60, "返回菜单"))
		{//点击返回菜单，跳转至菜单界面
			save_score(score);
			mciSendString("open menu1.mp3", NULL, 0, NULL);
			mciSendString("play menu1.mp3 repeat", NULL, 0, NULL);
			mciSendString("setaudio music volume to 8", 0, 0, 0);
			GameMenuView();  //函数没声明,先注释着
		}

		if (button(750, 425, 250, 60, "重新开始"))
		{//点击重新开始，跳转至游戏画面绘制然后开始游戏
			Gamedraw();
			PlayGame();
		}
		EndBatchDraw();
		//将绘图过程放在两个函数之间
		msg.message = 0;//循环结束后全局变量msg归零
	}

}


//暂停界面
void Stopview() {
	initgraph(1024, 768);
	//清空画面
	cleardevice();
	//mciSendString("pause battle1.mp3", NULL, 0, NULL);
	if (b_boosMusic) {
		//system("pause");
		mciSendString("pause battle1.mp3", NULL, 0, NULL);
	}
	else {
		//system("pause");
		mciSendString("pause boss.mp3", NULL, 0, NULL);
	}
	//mciSendString("close song", NULL, 0, NULL);
	//加载背景图片
	while (1) {
		putimage(0, 0, &stopbackground);
		
		while (true) {   //点击判断
			if (peekmessage(&msg, EX_MOUSE)) {}//获取鼠标消息

			BeginBatchDraw();

			//点这个可以回到当前游戏
			if (button(750, 220, 250, 60, "继续游戏"))
			{
				if (b_boosMusic) {
					mciSendString("resume battle1.mp3", NULL, 0, NULL);
					initgraph(height, width);// 回到原来游戏的分辨率
					return;
				}
				else {
					mciSendString("resume boss.mp3", NULL, 0, NULL);
					initgraph(height, width);// 回到原来游戏的分辨率
					return;
				}
				
			}

			//点这个可以直接返回到游戏主菜单
			if (button(0, 220, 250, 60, "返回菜单"))
			{
				mciSendString("close battle1.mp3", NULL, 0, NULL);
				mciSendString("open menu1.mp3", NULL, 0, NULL);
				mciSendString("play menu1.mp3 repeat", NULL, 0, NULL);
				mciSendString("setaudio music volume to 8", 0, 0, 0);
				return GameMenuView();
			}
			EndBatchDraw();
			msg.message = 0; // 循环结束后全局变量msg归零
		}
	}
}







void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //实现透明贴图
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void GameInit() {
	player.x = width / 2;
	player.y = height - 65;
	player.live = true;
	playerhp = 100;
	score = 0;
	enemybulletspeed = 5;
	//初始化玩家的每个子弹的位置
	for (int i = 0; i < bullet_Num; i++) {
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].live = false;
	}

	//初始化敌人状态
	for (int i = 0; i < enemy_Num; i++) {
		enemy[i].live = false;
		enemyhp(i);
	}
	//初始化敌人的每个子弹的状态
	
	for (int i = 0; i < enemybullet0_Num; i++) {
		enemybullet0[i].x = 0;
		enemybullet0[i].y = 0;
		enemybullet0[i].live = false;
	}

	for (int i = 0; i < enemybullet1_Num; i++) {
		enemybullet1[i].x = 0;
		enemybullet1[i].y = 0;
		enemybullet1[i].live = false;
	}

	//初始化boss的每个子弹的状态
	for (int i = 0; i < bossbullet_Num; i++) {
		for (int j = 0; j < 3; j++) {
			bossbullet[j][i].x = 0;
			bossbullet[j][i].y = 0;
			bossbullet[j][i].live = false;
		}
}
	//初始化boss

	boss.live = true;
	boss.x = 300;
	boss.y = -210;
	boss.hp = 100;
	boss.bossheight = 250;
	boss.bosswidth = 210;
	b_boosMusic = true;

	
	/*测试
	enemybullet[0].live = true;
	enemybullet[0].x = 50;
	enemybullet[0].y = 50;
	*/
}

void enemyhp(int i) {

	int randflag = rand() % 10;
	
	//randflag用来设定概率
	if (randflag==4) {
		enemy[i].type = 1;//中型
		enemy[i].hp = 3;//生命值
		enemy[i].enemyheight = 115;
		enemy[i].enemywidth = 106;
	}
	else {
		enemy[i].type = 0;//小型
		enemy[i].hp = 1;//生命值
		enemy[i].enemyheight = 49;
		enemy[i].enemywidth = 70;
	}
}



//玩家子弹创建
void CreateBullet() {
	for(int i=0;i<bullet_Num;i++){
		if(bullet[i].live==false){
		bullet[i].x=player.x+35;
		bullet[i].y=player.y;
		bullet[i].live=true;
		break;
		}
	}
}

//敌人子弹创建
void CreateEnemyBullet() {

	for (int j = 0; j < enemy_Num; j++) {
		if (enemy[j].live == true) {
			if (enemy[j].type == 1) {
				for (int i = 0; i < enemybullet1_Num; i++) {
					if (enemybullet1[i].live == false) {
						enemybullet1[i].x = enemy[j].x ;
						enemybullet1[i].y = enemy[j].y;
						enemybullet1[i].live = true;
						break;
					}
				}
			}

			if (enemy[j].type == 0) {
				for (int i = 0; i < enemybullet0_Num; i++) {
					if (enemybullet0[i].live == false) {
						enemybullet0[i].x = enemy[j].x + 35;
						enemybullet0[i].y = enemy[j].y;
						enemybullet0[i].live = true;
						break;
					}
				}
			}

		}
	}
}

//创建boss的子弹
void CreateBossBullet() {
	
	if (boss.live == true) {
		for (int i = 0; i < bossbullet_Num; i++) {
			for (int j = 0; j < 3; j++) {
				if (bossbullet[j][i].live == false) {
					bossbullet[j][i].x = boss.x + 35;
					bossbullet[j][i].y = boss.y;
					bossbullet[j][i].live = true;
				}
			}
		}
	}
}


void BulletMove() {//子弹移动

	for (int i = 0; i < bullet_Num; i++) {
		if (bullet[i].live == true) {
			bullet[i].y -= 15;//子弹只有上下的移动
				if (bullet[i].y < 0) {//子弹出界了
					bullet[i].live = false;//子弹没了
				}
		}
	}
}


void EnemyBulletMove() {//敌人子弹移动
	//中型敌人
	for (int i = 0; i < enemybullet1_Num; i++) {
		if (enemybullet1[i].live == true) {
			enemybullet1[i].y += enemybulletspeed;;//子弹只有上下的移动
			if (enemybullet1[i].y > width) {//子弹出界了
				enemybullet1[i].live = false;//子弹没了
			}
		}
	}

	//小型敌人
	for (int i = 0; i < enemybullet0_Num; i++) {
		if (enemybullet0[i].live == true) {
			enemybullet0[i].y += enemybulletspeed;//子弹只有上下的移动
			if (enemybullet0[i].y > width) {//子弹出界了
				enemybullet0[i].live = false;//子弹没了
			}
		}
	}
	
}

void BossBulletMove() {

	//第一种 向下射
	for (int i = 0; i < bossbullet_Num; i++) {
		if (bossbullet[0][i].live == true) {
			bossbullet[0][i].y += enemybulletspeed;//子弹只有下的移动
			if (bossbullet[0][i].y > width) {//子弹出界了
				bossbullet[0][i].live = false;//子弹没了
			}
		}
	}

	//第二种 向右下射
	for (int i = 0; i < bossbullet_Num; i++) {
		if (bossbullet[1][i].live == true) {
			bossbullet[1][i].y += enemybulletspeed;
			bossbullet[1][i].x -= 5;//子弹只有下的移动
			if (bossbullet[1][i].y > width) {//子弹出界了
				bossbullet[1][i].live = false;//子弹没了
			}
		}
	}

	//第三种 向左下射
	for (int i = 0; i < bossbullet_Num; i++) {
		if (bossbullet[2][i].live == true) {
			bossbullet[2][i].y += enemybulletspeed;
			bossbullet[2][i].x += 5;//子弹只有下的移动
			if (bossbullet[2][i].y > width) {//子弹出界了
				bossbullet[2][i].live = false;//子弹没了
			}
		}
	}


}

void PlayerMove()
{

	speed = 10;
	if (GetAsyncKeyState('A')) {
		if (player.x > 0)
			player.x -= speed;
	}//左移
	if (GetAsyncKeyState('D')) {
		if (player.x + 175 < width)
			player.x += speed;
	}//右移

	if (GetAsyncKeyState('W')) {
		if (player.y > 0)
			player.y -= speed;
	}//上移

	if (GetAsyncKeyState('S')) {
		if (player.y - 50 < height)
			player.y += speed;
	}//下移


	static DWORD t1 = 0, t2 = 0;
	if (GetAsyncKeyState('K') && t2-t1 > 100) {
		PlaySound("shoot1_01.wav", NULL, SND_FILENAME | SND_ASYNC);
		CreateBullet();
		t1 = t2;
		//子弹射出的间隔实现
	}//按k生成子弹
	t2 = GetTickCount();
}



//加载图片
void LoadGameImg() {
	loadimage(&_background, "./beijing.png");
	loadimage(&role, "./player1.png");
	loadimage(&Bullet, "./bullet.png");
	loadimage(&enemyimg[0], "./enemy0.png");
	loadimage(&enemyimg[1], "./enemy1.png");
	loadimage(&Enemybullet[0], "./enemybullet0.png");
	loadimage(&Enemybullet[1], "./enemybullet1.png");
	loadimage(&bossimg, "./boss.png");
	loadimage(&bossbulletimg, "./bossbullet.png");
	loadimage(&stopimg, "./stop.png");
	loadimage(&stopbackground, "./stopbackground.png",1024,768);
	
}


void Gamedraw() {

	BeginBatchDraw();
	putimage(0, 0, &_background);
	//暂停键显示
	
	transparentimage3(NULL, 560, 40, &stopimg);
	//分数显示
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "MicroSoft YaHei");
	settextcolor(BLACK);
	outtextxy(0, 0, ("分数:" + to_string(score)).data());

	//玩家生命显示
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "MicroSoft YaHei");
	settextcolor(RED);
	outtextxy(525, 0, ("HP:" + to_string(playerhp)).data());
	

	//putimage(player.x, player.y, &role);
	transparentimage3(NULL, player.x, player.y, &role);

	//子弹绘制
	
	//玩家
	for (int i = 0; i < bullet_Num; i++) {
		if (bullet[i].live == true) {
			transparentimage3(NULL, bullet[i].x, bullet[i].y, &Bullet);
		}
	}


	//敌人
	
	//中型敌人
			for (int i = 0; i < enemybullet1_Num; i++) {
				if (enemybullet1[i].live == true&&enemy[i].type==1) {
					transparentimage3(NULL, enemybullet1[i].x, enemybullet1[i].y, &Enemybullet[1]);
				}
			}
		

	//小型敌人
			for(int i = 0; i < enemybullet0_Num; i++) {
				if (enemybullet0[i].live == true&&enemy[i].type==0) {
					transparentimage3(NULL, enemybullet0[i].x, enemybullet0[i].y, &Enemybullet[0]);
				}
			}
		
		
     //boss
			


	//敌人绘制
	for (int i = 0; i < enemy_Num; i++) {
		if (enemy[i].live == true) {
			if (enemy[i].type == 1) {//中型的
				transparentimage3(NULL, enemy[i].x, enemy[i].y, &enemyimg[1]);
			}

			if (enemy[i].type == 0) {//小型的
				transparentimage3(NULL, enemy[i].x, enemy[i].y, &enemyimg[0]);
			}

		}
		
	}
	 

	//boss的绘制+子弹的绘制
	if (score >= boss_spawn && boss.live == true) {
		
		transparentimage3(NULL, boss.x, boss.y, &bossimg);
		CreateBossBullet();
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < bossbullet_Num; i++) {
				if (bossbullet[j][i].live == true) {
					transparentimage3(NULL, bossbullet[j][i].x, bossbullet[j][i].y, &bossbulletimg);
				}
			}
		}
		BossBulletMove();
		BossMove();

	}
	
	if (boss.y > -210 && b_boosMusic)
	{
		b_boosMusic = false;
		mciSendString("close battle1.mp3", NULL, 0, NULL);
		mciSendString("open boss.mp3", NULL, 0, NULL);
		mciSendString("play boss.mp3 repeat", NULL, 0, NULL);
	}

	EndBatchDraw();
			
}

void CreateEnemy() {//创建敌人
	for (int i = 0; i < enemy_Num; i++) {
		if (enemy[i].live == false) {
			enemy[i].live = true;
			enemy[i].x = rand() % (width);
			enemy[i].y = 0;
			enemyhp(i);

			static DWORD t5 = 0, t6 = 0;
			if (t6 - t5 > bullet_ms) {
				t5 = t6;
				CreateEnemyBullet();
			}

			t6 = clock();
			break;
		}
	}
}




void ctrlFps(int start_time) //控制帧率
{
	clock_t running_time = clock() - start_time;
	if ((13 - running_time) >= 0)//防止睡眠函数使用负数
		Sleep(13 - running_time);//动态睡眠

	int FPS = 1000 / (clock() - start_time);
	
	settextcolor(0xFFA500);
	settextstyle(30, 0, TEXT("微软雅黑"));
	
}


void EnemyMove() {
		int speed = 1;
		for (int i = 0; i < enemy_Num; i++) {
			if (enemy[i].live == true) {
				enemy[i].y += speed;//敌人只有上下的移动 的速度
				if (enemy[i].y > height) {//敌人出界了
					enemy[i].live = false;//敌人没了
				}

				//玩家与敌人的碰撞判定
				if (player.x > enemy[i].x && player.x < enemy[i].x + enemy[i].enemywidth
					&& player.y>enemy[i].y && player.y < enemy[i].y + enemy[i].enemyheight) {
					enemy[i].live = false;
					if(enemy[i].type==0)//碰到小型敌人
					playerhp -= 10;
					if (enemy[i].type == 1)//碰到中型敌人
					playerhp -= 20;
				}
			}
		}
	
}

//boss的移动
void BossMove() {
	
	static int bossspeed = 5;
	
		if (boss.y < 0)								//boss的前摇(装逼进场)
		{
			boss.y +=1 ;
		}

		else
		{
			boss.x += bossspeed;						//boss的左右摇摆
			if (boss.x + 250 > height || boss.x < 0)
			{
				bossspeed =  - bossspeed;
			}
		}
	

}
void ShootingEnemy() {
	int flag = 0;
	//射敌人
	for (int i = 0; i < enemy_Num; i++) {
		if (enemy[i].live == false) {
			continue;
		}
		flag = 0;
		for (int j = 0; j < bullet_Num; j++) {
			if (bullet[j].live == false) {
				continue;
			}
			if (bullet[j].x > enemy[i].x && bullet[j].x < enemy[i].x + enemy[i].enemywidth
				&& bullet[j].y>enemy[i].y && bullet[j].y < enemy[i].y + enemy[i].enemyheight) {
				bullet[j].live = false;
				enemy[i].hp--;
				flag = 1;
			}
			if (enemy[i].hp <= 0) {
				if (enemy[i].type == 1&&flag==1)score += 20;
				if (enemy[i].type == 0&&flag==1)score += 10;
				enemy[i].live = false;
			}
		}
	}

	//射boss
	for (int j = 0; j < bullet_Num; j++) {
		if (bullet[j].live == false) {
			continue;
		}
		if (bullet[j].x > boss.x && bullet[j].x < boss.x + boss.bosswidth
			&& bullet[j].y>boss.y && bullet[j].y < boss.y + boss.bossheight) {
			bullet[j].live = false;
			boss.hp--;

		}
		if (boss.hp <= 0) {//boss死了游戏结束
			boss.live = false;
			//gameover();
		}
	}
}
 

//敌人攻击玩家
void Shootingplayer() {
	//中型敌人判定
	for (int i = 0; i < enemybullet1_Num; i++) {
		if (enemybullet1[i].live == false) continue;
		
		if(enemybullet1[i].x+10>=player.x&& enemybullet1[i].x<=player.x+40&&
			enemybullet1[i].y>=player.y&&enemybullet1[i].y<=player.y+40){
			enemybullet1[i].live = false;
			playerhp-=20;
		
		}
	}

	//小型敌人判定
	for (int i = 0; i < enemybullet0_Num; i++) {

		if (enemybullet0[i].live == false) continue;

		if (enemybullet0[i].x+10 >= player.x && enemybullet0[i].x <= player.x + 40 &&
			enemybullet0[i].y >= player.y && enemybullet0[i].y <= player.y + 40) {
			enemybullet0[i].live = false;
			playerhp -= 10;
		}
	}

	//boss判定
	for (int i = 0; i < bossbullet_Num; i++) {
		for (int j = 0; j < 3; j++) {
			if (bossbullet[j][i].live == false) continue;
			if (bossbullet[j][i].x + 10 >= player.x && bossbullet[j][i].x <= player.x + 40 &&
				bossbullet[j][i].y >= player.y && bossbullet[j][i].y <= player.y + 40) {
				bossbullet[j][i].live = false;
				playerhp -=5 ;
			}
		}
	}



}


//难度判断
void judgedifficulty() {
	
	
	if (hard == 1) {
		ms = 1200;
		bullet_ms = 1000;
		enemybulletspeed = 3;
		boss_spawn = 300;
		boss.hp == 300;
	}
	else if (hard == 2) {
		ms = 1000;
		bullet_ms = 700;
		boss_spawn = 700;
		boss.hp = 500;
		enemybulletspeed = 5;
	}
	else if (hard == 3) {
		ms = 500;
		boss_spawn = 1300;
		boss.hp = 700;
		bullet_ms = 400;
		enemybulletspeed = 7;
	}
}






void PlayGame() {//游戏函数

	initgraph(height, width);

	mciSendString("open battle1.mp3", NULL, 0, NULL);
	mciSendString("play battle1.mp3 repeat", NULL, 0, NULL);

	
	GameInit();
	judgedifficulty();
	while (1) {

		if (playerhp <= 0) {
			mciSendString("close battle1.mp3", NULL, 0, NULL);
			mciSendString("close boss.mp3", NULL, 0, NULL);
			GameOverview();//游戏失败
		}
		if (boss.hp <= 0) {
			mciSendString("close battle1.mp3", NULL, 0, NULL);
			mciSendString("close boss.mp3", NULL, 0, NULL);
			GameWinview();//游戏胜利
		}
		if ((peekmessage(&msg, EX_MOUSE)) && msg.message == WM_LBUTTONDOWN && inArea(560, 40, msg.x, msg.y, 100, 100)) {
				Stopview();
		}//游戏暂停

		Gamedraw();
		PlayerMove();
		BulletMove();
		Shootingplayer();
		static DWORD t3 = 0, t4 = 0;
		if (t4 - t3 > ms) {
			t3 = t4;
			CreateEnemy();
		}

		t4 = clock();
		ShootingEnemy();
		EnemyBulletMove();
		EnemyMove();
		int start_time = clock();
		ctrlFps(start_time);
	}
}



int main() {

	login();
	
}