#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>
#include<fstream>
#include<sstream>
#include <GL/glut.h>
#include "sdglib.h"
#define MAX 100000

using namespace std;
ofstream outfile;
ifstream infile;

/*
変数の意味
x1,x2,v1,v2,a1,a2はマスの位置の位置、速度、加速度、添え字の１が上のおもり、２が下の重り
Tはdt=0.05を加えていき、全体の時間を表す（T＊２０がdisplayfuncが呼び出された回数）
sitはシフトSを押したときにSAVE DATAと表示する変数
ｈは外乱を入れるための変数
マスの位置を配列xl1[n],xl2[n]に入れる
dniは外乱の配列をひとつづつ呼び出す変数
*/
static double x1(0), x2(0), v1(0), v2(0), a1, a2, T(0), TT(0), sit(0), h(0);
static int n(0), dni(0), dd(0), first(0);
const double k1(20), k2(70), c(50), m1(5), m2(300), dt(0.05), w(0.3), N(6);//Nは振幅を表示するるときに拡張を何倍したかを表す
double d;
static double xl1[MAX], xl2[MAX], din[MAX];//din[]は外乱の応答を読み込む配列
string ds;

void displayfunc() {
	using namespace SDGLibF;
	Before();

	//右上の時間、位置と座標の名前の描写のために読み込む
	stringstream P1, P2, Time, TI, XX;
	P1 << "X1=" << x1;
	P2 << "X2=" << x2;
	Time << "Time=" << T;
	TI << "t";
	XX << "x";

	//座標系を書く
	SetColor(0.0, 0.0, 0.0);
	DrawLine(1.0, -180, 0, 180, 0);
	DrawLine(1.0, -180, 50, -180, -50);
	DrawString(180, -20, TI.str());
	DrawString(-170, 50, XX.str());

	//入力の計算
	if (h == 0) {
		d = 2 * sin(w*T);
	}
	else if (h == 1) {
		dni++;
		d = din[dni];
	}

	//入力の描写（点を縦に動かす、それを微妙にずらして横に広げる）
	if (h == 0) {
		DrawCircle(1, 0, -350 + d*N, 1);
		for (int i(-200); i <= 200; i++) {
			DrawPoint(1.5, i, -350 + 2 * N*sin(T*w + i*0.1));
		}
	}
	else if (h == 1) {
		dd++;
		for (int i(-200); i <= 200; i++) {
			DrawPoint(1.5, i, -350 + din[dd + i] * N);
		}
	}

	//入力を受ける位置
	SetColor(0, 1, 0);
	DrawCircle(2, 0, -350 + d*N, 1);

	//上ばね周辺の棒
	SetColor(0, 0, 0);
	DrawLine(1, 0, x2*N - 250, 0, x2*N - 240);
	DrawLine(1, -30, x2*N - 240, 30, x2*N - 240);

	//上ばねの上下の棒
	DrawLine(1, -30, x2*N - 240, -30, x2*N - 230);
	DrawLine(1, -30, x1*N - 110, -30, x1*N - 120);
	DrawLine(1, 0, x1*N - 100, 0, x1*N - 110);
	DrawLine(1, -30, x1*N - 110, 30, x1*N - 110);

	//ダンパの描写
	DrawLine(1, 30, x1*N - 110, 30, x1*N - 170);
	DrawLine(1, 30, x2*N - 240, 30, x2*N - 190);
	DrawLine(1, 15, x2*N - 190, 45, x2*N - 190);
	DrawLine(1, 15, x2*N - 190, 15, x2*N - 160);
	DrawLine(1, 45, x2*N - 190, 45, x2*N - 160);
	DrawLine(1, 15, x1*N - 170, 45, x1*N - 170);

	//上ばねの描写
	double kk1 = (N*(x1 - x2) + 110) / 12;
	DrawLine(1, -30, x2*N - 230, -10, kk1 + x2*N - 230);
	DrawLine(1, -10, kk1 + x2*N - 230, -50, 3 * kk1 + x2*N - 230);
	DrawLine(1, -50, 3 * kk1 + x2*N - 230, -10, 5 * kk1 + x2*N - 230);
	DrawLine(1, -10, 5 * kk1 + x2*N - 230, -50, 7 * kk1 + x2*N - 230);
	DrawLine(1, -50, 7 * kk1 + x2*N - 230, -10, 9 * kk1 + x2*N - 230);
	DrawLine(1, -10, 9 * kk1 + x2*N - 230, -50, 11 * kk1 + x2*N - 230);
	DrawLine(1, -50, 11 * kk1 + x2*N - 230, -30, 12 * kk1 + x2*N - 230);

	//下ばねの上下の棒
	DrawLine(1, 0, -350 + d*N, 0, -350 + d*N + 10);
	DrawLine(1, 0, x2*N - 250, 0, x2*N - 260);
	double kk2 = ((x2 - d)*N + 80) / 12;

	//下ばね本体
	DrawLine(1, 0, d*N - 340, 20, kk2 + d*N - 340);
	DrawLine(1, 20, kk2 + d*N - 340, -20, 3 * kk2 + d*N - 340);
	DrawLine(1, -20, 3 * kk2 + d*N - 340, 20, 5 * kk2 + d*N - 340);
	DrawLine(1, 20, 5 * kk2 + d*N - 340, -20, 7 * kk2 + d*N - 340);
	DrawLine(1, -20, 7 * kk2 + d*N - 340, 20, 9 * kk2 + d*N - 340);
	DrawLine(1, 20, 9 * kk2 + d*N - 340, -20, 11 * kk2 + d*N - 340);
	DrawLine(1, -20, 11 * kk2 + d*N - 340, 0, 12 * kk2 + d*N - 340);

	//外乱からの位置の計算
	a1 = -1 / m1*k1*(x1 - d) + k2 / m1*(x2 - x1) + c / m1*(v2 - v1);
	a2 = -k2 / m2*(x2 - x1) - c / m2*(v2 - v1);
	v1 += a1*dt;
	v2 += a2*dt;
	x1 += v1*dt;
	x2 += v2*dt;

	//一番右にたどり着いたときに原点に戻るための計算
	TT += dt;
	if (TT > 360) {//TとTTを分けることによって入力の位相を突然変えずに初期位置に戻した
		TT -= 360;
		first += 360 / dt;
	}

	//動くボール1、軌跡の描写
	SetColor(1.0, 0.0, 0.0);
	DrawCircle(1.0, TT - 180, x1 * N, 5);
	n++;
	xl1[n] = x1;
	for (int i(first); i <= n; i++) {
		DrawLine(0.1, -180 + (i - 1)*dt - first*dt, xl1[i - 1] * N, -180 + i*dt - first*dt, xl1[i] * N);
	}
	DrawString(30, 150, P1.str());
	DrawLine(5, -30, x1*N - 100, 30, x1*N - 100);//（マス）

	//動く2ボール、軌跡の描写
	SetColor(0.0, 0.0, 1.0);
	DrawCircle(1.0, TT - 180, x2 * N, 5);
	xl2[n] = x2;
	for (int i(first); i <= n; i++) {
		DrawLine(0.1, -180 + (i - 1)*dt - first*dt, xl2[i - 1] * N, -180 + i*dt - first*dt, xl2[i] * N);
	}
	DrawString(30, 120, P2.str());
	DrawLine(5, -30, x2*N - 250, 30, x2*N - 250);//（マス）

	//時間の計算と描写
	T += dt;
	SetColor(0.0, 0.0, 0.0);
	DrawString(30, 90, Time.str());

	//SAVE DATAの描写
	if (sit == 1) {
		char str[] = "SAVE DATA!";
		SetColor(1.0, 0.0, 0.0);
		DrawString(50, -50, str);
	}

	//座標の位置をファイルに書き込む
	outfile << T - 180 << "," << x1 << "," << T - 180 << "," << x2 << endl;

	After();
}

void sim(void) {
	using namespace SDGLibF;
	ReDraw();
}


void keyboardfunc(unsigned char k, int x, int y) {
	using namespace SDGLibF;
	switch (k) {
	case 's':
		IdleFunc(sim);
		break;
	case 'S':
		sit++;
		IdleFunc(NULL);
		ReDraw();
		break;
	case 'd':
		h = 1;
		IdleFunc(sim);
		break;
	case 27:	/* Escape */
		exit(0);
		break;
	default:
		return;
	}
	ReDraw();
}

int main(void)
{
	outfile.open("file.csv");
	infile.open("file1.csv");

	//dを押したときのファイルを配列に読み込んだ
	int i(0);
	while (getline(infile, ds)) {
		stringstream ss;
		ss << ds;
		ss >> din[i];
		i++;
	}

	SDGLib mygraphic(400, 600, "- 2D Graphics - (61614901 Masato Nohara)", -200.0, 200.0, -400.0, 200.0);
	mygraphic.SetCursor(GLUT_CURSOR_WAIT);
	mygraphic.Display(displayfunc);
	mygraphic.Keyboard(keyboardfunc);
	mygraphic.Start();
	outfile.close();
	return 0;
}
