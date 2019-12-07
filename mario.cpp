///////////////////////////////////////////////////////////////////
//                                                               //
//         Sample Program of 2-D Graphics  sd_2d_v1.cpp          //
//                 Original 2013/Oct/16 Nishi                    //
//                 Modified 2013/Oct/20 Nakazawa                 //
//                 Modified 2015/Dec/10 Takahashi                //
///////////////////////////////////////////////////////////////////

#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>
#include<fstream>
#include<sstream>
#include "sdglib.h"

using namespace SDGLibF;
using namespace std;
static int act(0), nump(0), numh(0), nume(0), nums(0), goalline1(5000), point(0), situation(0),e1(0),e2(0),e3(0);
static int nump2(0), numh2(0), nume2(0), nums2(0), goalline2(6500);
static int nump3(0), numh3(0), nume3(0), nums3(0), goalline3(5000);
static double xx(-360), yy, vp(50), j(0), dt(0), jdt(0.005), n(0), ve(10), xkusa(0), jumppower1(450.0), jumpa(-8.0), vj(jumppower1), dvj(0), goaldown(0);

//台を作成するクラスの宣言。コンストラクタの引数として、始まりの座標、終わりの座標、上辺の高さ、下辺の高さ、赤色の度合いの値の５つとした。、
class plate {
public:
	double px1, px2, high, low, color;
	plate(double cpx1, double cpx2, double chigh, double clow, double ccolor);
	void platedraw();
};

//台を作成するコンストラクタ
plate::plate(double cpx1, double cpx2, double chigh, double clow, double ccolor) {
	px1 = cpx1;
	px2 = cpx2;
	high = chigh;
	low = clow;
	color = ccolor;
}

//台を描くメンバ関数。立体的に見えるように奥行きを出した
void plate::platedraw() {
	using namespace SDGLibF;
	px1 -= dt*vp; px2 -= dt*vp;
	SetColor(color, 0.0, 0.0);
	for (int i(low); i < high; i++) {
		DrawLine(1.0, px1, -300 + i, px2, -300 + i);
	}
	for (int i(0); i < 15; i++) {
		DrawLine(1.5, px1 + i / 15.0 * 20, -300 + high + i, px2 + i / 15.0*20.0, -300 + high + i);
	}
	for (int i(low); i < high; i++) {
		DrawLine(1, px2, -300 + i, px2 + 20, -300 + 15 + i);
	}
	SetColor(0.0, 0.0, 0.0);
	DrawLine(1.0, px1, -300 + low, px1, high - 300);
	DrawLine(1.0, px1, -300 + low, px2, -300 + low);
	DrawLine(1.0, px1, high - 300, px2, high - 300);
	DrawLine(1.0, px2, high - 300, px2, -300 + low);
	DrawLine(1.0, px2, -300 + low, px2 + 20, -300 + 15 + low);
	DrawLine(1.0, px2, high - 300, px2 + 20, high - 300 + 15);
	DrawLine(1.0, px2 + 20, -300 + 15 + low, px2 + 20, -300 + 15 + high);
	DrawLine(1.0, px1, high - 300, px1 + 20, high - 300 + 15);
	DrawLine(1.0, px1 + 20, high - 300 + 15, px2 + 20, -300 + high + 15);
}

//穴を作成するクラスの宣言。コンストラクタの引き数として始まりの座標と終わりの座標の２つを与える
class hole {
public:
	double hx1, hx2;
	hole(double chx1, double chx2);
	void holedraw();
};

//穴を作成するコンストラクタ
hole::hole(double chx1, double chx2) {
	hx1 = chx1;
	hx2 = chx2;
}

//穴を描くメンバ関数
void hole::holedraw() {
	SetColor(0, 0, 0);
	hx1 -= dt*vp; hx2 -= dt*vp;
	for (int i(0); i <= hx2 - hx1; i++) {
		DrawLine(1, hx1 + i, -300, hx1 + 25 * 1.1 + i, -300 + 26);
	}
}

//敵を作成するクラスの宣言
class enemy {
public:
	double ex;
	enemy(double cex);
	void enemydraw();
};

//敵を作成するコンストラクタ
enemy::enemy(double cex) {
	ex = cex;
}

//敵を描くメンバ関数
void enemy::enemydraw() {
	SetColor(0, 0, 0);
	ex -= vp*dt + ve*dt;
	/*for (int i(0); i <= 10; i += 2) {
	DrawCircle(2, ex, -295, i);
	}*/
	SetColor(0.7, 0, 0);//顔
	for (int i(0); i <= 17; i++) {
		DrawLine(1, ex, -280, ex - 8.5 + i, -293);
	}
	SetColor(0, 0, 0);
	for (int i(0); i <= 2; i += 1) {//左目
		DrawCircle(1, ex - 3, -287, i);
	}
	for (int i(0); i <= 2; i += 1) {//右目
		DrawCircle(1, ex + 3, -287, i);
	}
	//眉毛
	DrawLine(1, ex - 1, -288, ex - 4, -284);
	DrawLine(1, ex + 1, -288, ex + 4, -284);
}

//スターを作成するクラスの宣言
class star {
public:
	double sx, sy;
	star(double csx, double csy);
	void stardraw();
};

//スターを作成するコンストラクタ
star::star(double csx, double csy) {
	sx = csx;
	sy = csy;
}

//スターを描くメンバ関数
void star::stardraw() {
	sx -= vp*dt;
	SetColor(8 / 10.0, 8 / 10.0, 0);
	DrawLine(2, sx, sy + 6, sx - 3, sy - 3 * sqrt(3));
	DrawLine(2, sx + 3 * sqrt(3), sy + 3, sx - 3, sy - 3 * sqrt(3));
	DrawLine(2, sx + 3 * sqrt(3), sy + 3, sx - 3 * sqrt(3), sy + 3);
	DrawLine(2, sx - 3 * sqrt(3), sy + 3, sx + 3, sy - 3 * sqrt(3));
	DrawLine(2, sx, sy + 6, sx + 3, sy - 3 * sqrt(3));
}

//ここからクラスで定義した台と穴と敵とスターのオブジェクトを作成した。配列で作成することによって、メンバ関数を呼び出す手間を減らした。また、配列の個数をsizeof関数を使って数えることにより。オブジェクトを付け加えたり減らしたりするときにほかの変数も自動的に変更されるようにした。
plate platen[] = { plate(-200,-70,50,0,1), plate(100, 250, 50, 40, 0.7),plate(180, 280, 100, 90, 0.8),plate(280,500,150,140,1),plate(600,700,40, 0, 0.9),plate(700,900,10, 0, 1),plate(1000,1100,30, 0, 0.5),
plate(1100, 1200,60, 0, 0.3), plate(1200, 1300,90, 0, 0.2),plate(1300, 1400,120, 0, 0.3), plate(1400, 1500,150, 0, 0.4),plate(1500,1600,180, 0, 0.7),
plate(1600,1700,210, 0, 0.8),plate(1700,1800,240, 0, 0.9),plate(1800,2100,80, 0, 0.5),plate(3000,3100,20, 0, 0.5) ,plate(3100,3200,30, 0, 0.5) ,
plate(3300,3400,40, 0, 0.5),plate(3400,3500,50, 0, 0.5),plate(3500,3600,70, 0, 0.5),plate(3900,4000,20,10,0.5) };

plate platen2[] = { plate(90,100,30,0,0.5),plate(200,300,30,0,1),plate(300,400,60,0,0.5),plate(400,500,90,0,1),plate(1100,1500,50,40,1),plate(1300,1500,100,90,1),plate(1400,1500,150,140,0.9),
plate(1550,1600,180,170,0.8),plate(2100,2200,40,0,0.7),plate(2300,2400,40,0,0.6), plate(2500,2600,40,0,0.5),plate(2700,2730,50,0,0.4),plate(2800,2830,60,0,0.3),plate(2900,2930,70,0,0.2),
plate(3500,3600,30,0,0.1),plate(3600,3700,60,0,1),plate(3700,3800,90,0,1),plate(3800,3900,120,0,1),plate(3900,4000,150,0,1),plate(4000,4100,180,0,1),plate(4100,4200,210,0,1), };

plate platen3[] = { plate(0,30,50,0,1), plate(240,250,170,160,0.6), plate(190,250,130,120,1), plate(70,250,70,60,1), plate(60,420,30,20,0.8), plate(320,370,90,80,0.7), plate(420,490,140,130,0.4), plate(470,480,60,50,0.7), plate(530,540,50,0,1), plate(530,630,140,130,9.9), plate(650,660,50,40,0.6), plate(730,740,50,40,0.2), plate(820,830,50,40,0.8), plate(650,890,140,130,0.7), plate(880,1170,190,180,0.7), 
plate(1070,1150,20,10,0.7), plate(1220,1300,60,50,1), plate(1370,1450,100,90,1), plate(1520,1600,140,130,0.9), plate(1670,1750,180,170,0.9), plate(1820,1900,220,210,0.8), plate(1970,2050,250,240,0.8), plate(2120,2200,280,270,0.7), plate(2270,2350,310,300,0.6), plate(2420,2500,340,330,0.8)//この行は大階段1070〜2040
,plate(1850,2050,40,35,1),plate(3000,3400,50,0,0.8), plate(3600,3700,60,0,1),plate(3700,3800,90,0,1),plate(3800,3900,120,0,0.8) };

hole holen[] = { hole(330,380),hole(920,950),hole(2800,2850) };
hole holen2[] = { hole(800,880) ,hole(4700,4780),hole(5900,5980),hole(5200,5080) };
hole holen3[] = {hole(1400,1470),hole(1500,1600) ,hole(1800,2100), hole(2800,2870), };

enemy enemyn[] = { enemy(30),enemy(3000),enemy(3900) };
enemy enemyn2[] = { enemy(5500),enemy(5900),enemy(6000) };
enemy enemyn3[] = { enemy(2500),enemy(2600),enemy(2900) };

star starn[] = { star(80,-230),star(90,-200),star(290,-200),star(300,-180),star(310,-175) ,star(480,-230),star(490,-200),star(600,-180),star(710,-175),
star(2080,-130),star(2090,-100),star(2100,-80),star(2110,-175) ,star(2080,-230),star(1090,-100),star(2400,-180),star(2310,-175) ,star(2280,-230),star(2290,-200),star(2500,-80),star(2410,-175),
star(2080,-230),star(3090,-200),star(2100,-190),star(3110,-195) ,star(880,-280),star(3090,-28),star(3100,-180),star(4110,-175) ,star(-80,-230),star(390,-200),star(155,-180),star(180,-175) ,star(4000,-260),star(4300,-260), star(4500,-260) ,star(4400,-260), star(3800,-260) ,star(3900,-260) };

star starn2[] = { star(200,40 - 300),star(520,120 - 300),star(540,100 - 300),star(560,80 - 300),star(580,60 - 300),star(1650,240 - 300),star(1680,210 - 300),star(1710,180 - 300),star(1740,150 - 300),star(1770,120 - 300),star(1810,90 - 300),
star(1810,90),star(1810,90 - 300), star(1810,90 - 300) };

star starn3[] = { star(200,40 - 300) ,star(3000,60 - 300) ,star(3300,60 - 300), star(3100,60 - 300), star(3200,60 - 300),star(1070,-260),star(1220,-220),star(1370,-180),star(1520,-150),star(1680,-110),star(1730,-70),star(1880,-30),star(4000,-260),star(4300,-260), star(4500,-260) ,star(4400,-260), star(3800,-260) ,star(3900,-260) };

//オブジェクトをメンバ関数を使って描写する関数
void Draw(plate p[], hole h[], enemy e[], star s[], double goalline, int np, int nh, int ne, int ns) {
	SetColor(0, 1, 0);
	for (int i(0); i < np; i++) {//台の描写
		p[i].platedraw();
	}

	for (int i(0); i < nh; i++) {//穴の描写
		h[i].holedraw();
	}

	for (int i(0); i < ne; i++) {//敵の描写
		e[i].enemydraw();
	}

	for (int i(0); i < ns; i++) {//スターの描写
		s[i].stardraw();
	}

	SetColor(1, 0, 0);//ゴールの描写
	DrawLine(2.0, goalline - n - 360, -300, goalline - n - 360, 200);
	for (int i(0); i < 50; i++) {
		DrawLine(1.0, goalline - n - 400, 175 + goaldown, goalline - n - 360, 150 + i + goaldown);
	}
}

void Drawscene() {
	//地面と草の描写
	SetColor(149.0 / 204, 65.0 / 204, 28.0 / 204);
	for (int i(10); i <= 100; i++) {
		DrawLine(1.0, -400, -300 - i, 400, -300 - i);
	}
	SetColor(0, 1, 0);
	DrawLine(10, -400, -305, 400, -305);
	for (int k(-400); k < goalline1 + 10000; k += 60) {
		for (int i(0); i < 60; i += 2) {
			SetColor(0, 1.0 - i / 360.0, 0);
			DrawLine(2, i + k - n, -305, 30 + k - n, -315);
		}
	}
	for (int i(0); i <= 100; i++) {
		SetColor(0, 0.8 + i / 400.0, 0);
		DrawLine(1, -400, -300 + i / 4, 400, -300 + i / 4);
	}
}

void DrawHuman() {
	SetColor(0.0, 0.0, 1.0);//走る人間の描写
	for (double i(0); i < 2.5; i += 0.5) {//顔
		DrawCircle(1, xx, yy + 7.5, i);
	}
	DrawLine(1, xx, yy + 5, xx, yy - 5);//胴体
	DrawLine(1, xx - 7, yy + 3, xx + 7, yy + 3);//肩
	DrawLine(1, xx - 7, yy + 3, xx - 7, yy);//手
	DrawLine(1, xx + 7, yy + 6, xx + 7, yy + 3);//手
	DrawLine(1, xx - 5, yy - 7, xx, yy - 5);//左足
	DrawLine(1, xx - 5, yy - 7, xx - 8, yy - 7);
	DrawLine(1, xx, yy - 5, xx + 5, yy - 5);//右足
	DrawLine(1, xx + 7, yy - 7, xx + 5, yy - 5);
}

//点数を表示させる関数
void DrawScore() {
	stringstream points;
	points << "Score : " << point;
	SetColor(1, 0, 0);
	DrawString(300, 250, points.str());
}

//ジャンプするときに呼び出される関数
void jampup(plate b[], int np, int jumppower) {
	vj += jumpa;
	j += vj*jdt;
	if (j <= 0) {//地面についた時の条件
		act = 0;
		vj = jumppower;
		j = 0;
	}
	for (int i(0); i < np; i++) {//ジャンプで台に乗る
		if (b[i].px1 < xx && b[i].px2>xx && j < b[i].high&& j>b[i].low + 2) {
			j = b[i].high;
			act = 0;
			vj = jumppower;
		}
	}
}

void drop() {//穴から落ちるときに呼び出される関数
	dvj += jumpa;
	j += dvj*dt;
	if (j <= -100) {
		act = 100;
	}
}

void condition(plate p[], hole h[], enemy e[], star s[], double goalline, int np, int nh, int ne, int ns) {
	for (int i(0); i < nh; i++) {//穴に落ちるかの判定
		if (j == 0 && h[i].hx1 < xx && h[i].hx2>xx) {
			act = 2;
		}
	}

	for (int i(0); i < np; i++) {//台からふんわり降りる
		if (j == p[i].high&&xx >= p[i].px2&&xx <= p[i].px2 + 1) {
			act = 1;
			vj = 0;
		}
	}

	for (int i(0); i < np; i++) {//台に下らぶつかると跳ね返る
		if (p[i].px1 < xx && p[i].px2>xx && p[i].low <= j + 20 && p[i].low > j + 19) {
			vj = -vj;
		}
	}

	for (int i(0); i < np; i++) {//ジャンプで壁に当たるか判定
		if (p[i].px1 == -350 && j <= p[i].high && j + 20 >= p[i].low) {
			act = 100;
		}
	}

	for (int i(0); i < ne; i++) {//敵に当たるかどうかの判定
		if ((e[i].ex + 360)*(e[i].ex + 360) + (5 + j)*(5 + j) <= 400) {
			act = 100;
		}
	}

	for (int i(0); i < ns; i++) {//星に当たったら星が消え得点アップ
		if ((s[i].sx + 360)*(s[i].sx + 360) + (s[i].sy + 290 - j)*(s[i].sy + 290 - j) < 169) {
			s[i].sy += 700;
			point++;
		}
	}

	if (act == 1) {//Jを押したらジャンプ
		jampup(p, np, jumppower1);
	}
	else if (act == 2) {//穴に落ちる場合
		drop();
	}
	else if (act == 100) {//ゲームオーバーになる条件
		stringstream gameover1,gameover2,gameover3;
		gameover1 <<"GAME OVER!  Please press R to chosse other course.";
		gameover2 << "GAME OVER! \n Please press R to do last course.";
		gameover3 << "GAME OVER! \n You did all courses, and your total point is " << point;
		SetColor(1.0, 0.0, 0.0);
		if (e1 + e2 + e3 == 1)DrawString(-250, 0, gameover1.str());
		if (e1 + e2 + e3 == 2)DrawString(-250, 0, gameover2.str());
		if (e1 + e2 + e3 == 3)DrawString(-250, 0, gameover3.str());
		dt = 0;
		n = 0;
	}

	if (n >= goalline) {//ゴールで止まる条件
		stringstream goal1,goal2;
		goal1 << "GAME CLEAR! Press R to restart";
		goal2 << "GAME CLEAR! Your total point is " << point;
		SetColor(1.0, 0.0, 0.0);
		if (e1 + e2 + e3 <= 2)DrawString(-250, 0, goal1.str());
		if(e1 + e2 + e3 == 3)DrawString(-250, 0, goal2.str());
		dt = 0;
		if (goaldown > -450) {
			goaldown -= 3;
		}
	}
}

void displayfunc() {
	using namespace SDGLibF;
	Before(); n += dt*vp;//全体の座標

	//画面全体の塗りつぶし
	if (situation == 1) {
		SetColor(1, 1, 0.9);
	}
	else if (situation == 2) {
		SetColor(1, 0.9, 1);
	}
	else if (situation == 3) {
		SetColor(0.8, 1, 1);
	}
	else if (situation == 0) {
		SetColor(1, 1, 1);
	}
	for		(int i(0); i < 700; i++) {
		DrawLine(1, -400, -400 + i, 400, -400 + i);
	}

	//ゲームが始まった時の画面の描写
	if (situation == 0) {
		SetColor(1, 1, 1);
		stringstream game, startword1, startword2, startword3;
		game << "GAME START";
		if (e1 == 0)startword1 << "- Course1 -  difficulty : 3   Press '1' ";
		if (e2 == 0)startword2 << "- Course2 -  difficulty : 5   Press '2' ";
		if (e3 == 0)startword3 << "- Course3 -  difficulty : 7   Press '3' ";
		SetColor(1, 0, 0);
		DrawString(-90, 0, game.str());
		DrawString(-160, -150, startword1.str());
		DrawString(-160, -200, startword2.str());
		DrawString(-160, -250, startword3.str());
	}

	//ボールの位置の計算
	xx = -360;
	yy = -290 + j;

	//背景を描写する関数を呼び出す
	Drawscene();

	//スターを取った時の関数を呼び出す
	DrawScore();

	//変数situationの値によって呼び出すオブジェクトを変える
	if (situation == 1) {
		Draw(platen, holen, enemyn, starn, goalline1, nump, numh, nume, nums);//すべてのオブジェクトの描写
		condition(platen, holen, enemyn, starn, goalline1, nump, numh, nume, nums);
		e1 = 1;
	}

	if (situation == 2) {
		Draw(platen2, holen2, enemyn2, starn2, goalline2, nump2, numh2, nume2, nums2);//すべてのオブジェクトの描写
		condition(platen2, holen2, enemyn2, starn2, goalline2, nump2, numh2, nume2, nums2);
		e2 = 1;
	}

	if (situation == 3) {
		Draw(platen3, holen3, enemyn3, starn3, goalline3, nump3, numh3, nume3, nums3);//すべてのオブジェクトの描写
		condition(platen3, holen3, enemyn3, starn3, goalline3, nump3, numh3, nume3, nums3);
		e3 = 1;
	}

	//人間を描く関数を呼び出す
	DrawHuman();

	After();
}

void sim(void) {
	ReDraw();
}

void keyboardfunc(unsigned char k, int x, int y) {
	switch (k) {
	case 's':
		if (situation != 0) {
			dt = 0.02;
			IdleFunc(sim);
		}
		break;
	case 'j':
		//ジャンプする
		if (act != 100 && dt == 0.02) {
			act = 1;
		}
		break;
	case 'r':
		//リスタートする
		if (e1 == 0 || e2 == 0 || e3 == 0) {
			situation = 0;
			act = 0;
			j = 0;
			n = 0;
			vj = jumppower1;
			goaldown = 0;
			break;
		}
		
	case 27:	/* Escape */
		exit(0);
		break;

	//数字１、２、３、によってコースを選ぶ
	case '1':
		if (situation == 0 && e1 == 0) {
			situation = 1;
			break;
		}
	case '2':
		if (situation == 0 && e2 == 0) {
			situation = 2;
			break;
		}
	case '3':
		if (situation == 0 && e3 == 0) {
			situation = 3;
			break;
		}
	default:
		return;
	}
	ReDraw();
}

int main(void)
{
	cout << "---操作説明---\n初めにコースを１〜３の中から選びます。そのあとにｓを押すとゲームがスタートします。ｊを押すとジャンプできます。\nゲームオーバーになる条件\n・ジャンプが届かずに壁に当たる\n・敵にぶつかる\n・穴に落ちる\n\n各コース１〜３それぞれ１回のみチャレンジできます。\nチャレンジする順番は自由です。\n３コースの合計で集めたスターの数が最後に表示されます。スターを集めながらゴールを目指しましょう！！\n\n";
	//オブジェクトを定義した配列の個数をsizeif関数を使って数えることによって、オブジェクトを増やしてもほかの部分を変えなくてもいいようにした。
	nump = sizeof platen / sizeof platen[0];
	nume = sizeof enemyn / sizeof enemyn[0];
	nums = sizeof starn / sizeof starn[0];
	numh = sizeof holen / sizeof holen[0];
	nump2 = sizeof platen2 / sizeof platen2[0];
	nume2 = sizeof enemyn2 / sizeof enemyn2[0];
	nums2 = sizeof starn2 / sizeof starn2[0];
	numh2 = sizeof holen2 / sizeof holen2[0];
	nump3 = sizeof platen3 / sizeof platen3[0];
	nume3 = sizeof enemyn3 / sizeof enemyn3[0];
	nums3 = sizeof starn3 / sizeof starn3[0];
	numh3 = sizeof holen3 / sizeof holen3[0];
	SDGLib mygraphic(800, 700, "- 2D Graphics - (61614901 Masato Nohara)", -400.0, 400.0, -400.0, 300.0);
	mygraphic.SetCursor(GLUT_CURSOR_WAIT);
	mygraphic.Display(displayfunc);
	mygraphic.Keyboard(keyboardfunc);
	mygraphic.Start();
	return 0;
}
