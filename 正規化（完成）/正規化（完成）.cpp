#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<GL/glut.h>
#define QP 9
#define AP 6
#define BP 4

void drawCircle(double x, double y, double radius)
{
	int i;	double PI = 3.14159;	int CIRCLE_REC = 60;

	glBegin(GL_POLYGON);
	for (i = 0; i < CIRCLE_REC; i++)
		glVertex2f(x + radius*cos((2.*PI*i) / CIRCLE_REC),
		y + radius*sin((2.*PI*i) / CIRCLE_REC));
	glEnd();
}

//図形の座標番号A
void render_stringA(float x, float y, const char* string)
{
	float z = -1.0f;
	glRasterPos3f(x, y, z);
	char*p = (char*)string;
	while (*p != '\0')glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
}

//図形の座標番号B
void render_stringB(float x, float y, const char* string)
{
	float z = -1.0f;
	glRasterPos3f(x, y, z);
	char*p = (char*)string;
	while (*p != '\0')glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
}


void display()
{
	int dummy, i = 0, j = 0;
	int cn1[AP - 1], cnn1 = 0, nan1 = 0, maxAxx = 0, minAxx = 0, maxAyy = 0, minAyy = 0;
	int cn2[BP - 1], cnn2 = 0, nan2 = 0, maxBxx = 0, minBxx = 0, maxByy = 0, minByy = 0;
	int gcn2[BP - 1], gcng2 = 0, ggg = 0;//近接用
	int norm1 = 0, norm2 = 0;	//正規化スイッチ
	double x[4], y[4];//基準線
	double Qx[QP], Qy[QP];	//シルエット座標
	double ax[AP], ay[AP];	//五角形座標
	double bx[BP], by[BP];  //三角形座標
	double rrA[AP - 1], rrB[BP - 1];	//三平方の定理
	double thetaA = 0, thetaB = 0;	//角度の初期パラメータ
	double thetaAC, thetaAD[AP - 1], thetaBC, thetaBD[BP - 1];
	double disAx[AP - 1], disAy[AP - 1], disBx[BP - 1], disBy[BP - 1];	//各点の座標距離
	double theta1[AP], theta2[BP];	//ピースの図形角度

	double abAx, abAy;	//鉛直線座標
	double cdAx = 0, cdAy = 0;	//交差座標
	double LineAx[BP], LineAy[BP], CroAx[BP], CroAy[BP];	//線分計算
	double maxAx, minAx, maxAy, minAy;	//内部三角形の大小値
	double saAx, saAy; //内部差
	double abBx, abBy;	//鉛直線座標
	double cdBx = 0, cdBy = 0;	//交差座標
	double LineBx[AP], LineBy[AP], CroBx[AP], CroBy[AP];	//線分計算
	double maxBx, minBx, maxBy, minBy;	//内部三角形の大小値
	double saBx, saBy; //内部差
	double gAx = 0, gAy = 0, gBx = 0, gBy = 0;	//重心点
	double gdisAx[AP], ggisAy[AP], gdisBx[BP], gdisBy[BP];	//重心との座標距離

	double QminAx, QminAy, QminBx, QminBy;	//正規化点
	double axx[AP], ayy[AP], bxx[BP], byy[BP];	//正規化のための座標保存

	double ran[4], ram[2];	//乱数用
	double r = 0.1;
	char str[256];	//座標番号

	srand((unsigned int)time(NULL));
	dummy = rand();
	dummy = rand();

	glClearColor(1.0, 1.0, 1.0, 1.0);

	//基準線
	x[0] = -5; y[0] = 0;
	x[1] = 5; y[1] = 0;
	x[2] = 0; y[2] = -5;
	x[3] = 0; y[3] = 5;

	//問題シルエット座標
	Qx[0] = 1.0; Qy[0] = 0.0;
	Qx[1] = 2.0; Qy[1] = 1.1;
	Qx[2] = 2.0; Qy[2] = 2.0;
	Qx[3] = 1.5; Qy[3] = 2.0;
	Qx[4] = 1.5; Qy[4] = 3.1;
	Qx[5] = 0.5; Qy[5] = 2.0;
	Qx[6] = 0.0; Qy[6] = 2.0;
	Qx[7] = 0.0; Qy[7] = 0.0;
	Qx[8] = 1.0; Qy[8] = 0.0;

	//四角形-pieace1
	ax[0] = -1.0; ay[0] = -1.0;
	ax[1] = 0.0; ay[1] = -1.0;
	ax[2] = 1.0; ay[2] = 0.1;
	ax[3] = 1.0; ay[3] = 1.0;
	ax[4] = -1.0; ay[4] = 1.0;
	ax[5] = -1.0; ay[5] = -1.0;

	//三角形-pieace2
	bx[0] = 0.0; by[0] = -1.0;
	bx[1] = 1.0; by[1] = -1.0;
	bx[2] = 1.0; by[2] = 0.1;
	bx[3] = 0.0; by[3] = -1.0;

	//初期角度乱数
	ran[0] = 0;	ran[1] = 0;
	ran[2] = 0; ran[3] = 0;

	//平行移動スイッチ
	ram[0] = 0; ram[1] = 0;

	//以下アニメーションの描画
	for (int iiii = 1; iiii <= 1000; iiii++){

		for (i = 0; i < AP - 2; i++) {
			for (j = i + 1; j < AP - 1; j++) {
				if (fabs(ax[i] - ax[j]) < 0.000001){
					ax[j] += 0.000001;
				}
				if (fabs(ay[i] - ay[j]) < 0.000001){
					ay[j] += 0.000001;
				}
			}
		}
		ax[AP - 1] = ax[0];
		ay[AP - 1] = ay[0];

		for (i = 0; i < BP - 2; i++) {
			for (j = i + 1; j < BP - 1; j++) {
				if (fabs(bx[i] - bx[j]) < 0.000001){
					bx[j] += 0.000001;
				}
				if (fabs(by[i] - by[j]) < 0.000001){
					by[j] += 0.000001;
				}
			}
		}
		bx[BP - 1] = bx[0];
		by[BP - 1] = by[0];

		double time1 = (double)clock() / CLOCKS_PER_SEC;
		for (;;){
			double time2 = (double)clock() / CLOCKS_PER_SEC;
			if (time2 - time1 > 0.5) break;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		//getchar();

		//問題シルエット描画	←順番変えれない
		glBegin(GL_POLYGON);
		glColor3f(1.0, 0.8, 1.0);
		for (i = 0; i < QP; i++)
			glVertex2f(Qx[i], Qy[i]);
		glEnd();

		//基準線	←順番変えれない
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		for (i = 0; i < 4; i++)
			glVertex2f(x[i], y[i]);
		glEnd();


		//pieace1角度
		thetaA = ran[0] * (3.14 / 180);

		//piace1の距離計算
		for (i = 0; i < AP - 1; i++){
			disAx[i] = fabs(ax[0] - ax[i + 1]);
			disAy[i] = fabs(ay[0] - ay[i + 1]);
		}
		for (i = 0; i < 4; i++){
			rrA[i] = sqrt(disAx[i] * disAx[i] + disAy[i] * disAy[i]);
		}

		//pieace1の角度計算
		if (iiii == 1){
			theta1[0] = atan(disAy[0] / disAx[0]);
			if (ax[0]>ax[1] && ay[0] < ay[1]) {  //左上にある
				theta1[0] = 3.14 - theta1[0];
			}
			if (ax[0]>ax[1] && ay[0] > ay[1]){ //左下にある
				theta1[0] += 3.14;
			}
			if (ax[0]<ax[1] && ay[0] > ay[1]) {	//右下にある
				theta1[0] = 2 * 3.14 - theta1[0];
			}

			for (i = 1; i < AP - 1; i++){
				theta1[i] = atan(disAy[i] / disAx[i]);
				if (ax[0]>ax[i + 1] && ay[0] < ay[i + 1]) {  //左上にある
					theta1[i] = 3.14 - theta1[i];
				}
				if (ax[0]>ax[i + 1] && ay[0] > ay[i + 1])  theta1[i] += 3.14;  //左下にある
				if (ax[0]<ax[i + 1] && ay[0] > ay[i + 1])  theta1[i] = 2 * 3.14 - theta1[i];  //右下にある
			}
		}
		else{
			theta1[0] = thetaAC;
			for (i = 1; i < AP - 2; i++){
				theta1[i] = thetaAD[i];
			}
		}

		thetaAC = theta1[0] + thetaA;
		if (thetaAC < 0)  thetaAC += 2 * 3.14;
		for (i = 1; i < AP - 1; i++){
			thetaAD[i] = theta1[i] + thetaA;
			if (thetaAD[i] < 0)  thetaAD[i] += 2 * 3.14;
		}

		//pieace1の角度動作
		ax[1] = ax[0] + rrA[0] * cos(thetaAC);
		ay[1] = ay[0] + rrA[0] * sin(thetaAC);
		for (i = 1; i < AP - 2; i++){
			ax[i + 1] = ax[0] + rrA[i] * cos(thetaAD[i]);
			ay[i + 1] = ay[0] + rrA[i] * sin(thetaAD[i]);
		}
		//pieace1の平行移動
		for (i = 0; i < AP; i++){
			if (ram[0] == 0) ax[i] -= ran[2];
			else ay[i] -= ran[2];

			norm1 += 1; //正規化スイッチ
		}

		//pieace2とpieace1の内外判定
		//※図形の境界線上は図形外としている。これは境界線上が図形の大きさに含まれないためである。
		for (i = 0; i < BP - 1; i++){
			for (j = 0; j < AP - 1; j++){
				if (((ax[j] - 5.0) * (by[i] - ay[j]) + (ay[j] - ay[j]) * (ax[j] - bx[i]))
					* ((ax[j] - 5.0) * (by[i + 1] - ay[j]) + (ay[j] - ay[j]) * (ax[j] - bx[i + 1])) < 0){

					if (((bx[i] - bx[i + 1]) * (ay[j] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[j]))
						* ((bx[i] - bx[i - 1]) * (ay[j] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - 5.0)) < 0){
						cn1[j] += 1;
					}
				}
			}
		}

		//pieace1の重心を求める
		for (i = 0; i < AP - 1; i++){
			gAx += ax[i];
			gAy += ay[i];
		}
		gAx = gAx / (AP - 1);
		gAy = gAy / (AP - 1);

		//pieace1-内部座標の個数確認
		for (i = 0; i < AP; i++){
			if (cn1[i] % 2 >= 1){
				cnn1 += 1;
			}
		}

		//pieace1が内部に入っている場合の計算
		minAy = 5.0;	maxAy = -5.0;
		minAx = 5.0;	maxAx = -5.0;
		for (i = 0; i < AP - 1; i++){
			if (cn1[i] % 2 >= 1){
				if (ax[i] > maxAx){ //四角形の一番右の座標を特定
					maxAx = ax[i];
					maxAxx = i;
				}
				if (ay[i] > maxAy){ //四角形の一番上の座標を特定
					maxAy = ay[i];
					maxAyy = i;
				}
				if (ax[i] < minAx){ //四角形の一番左の座標を特定
					minAx = ax[i];
					minAxx = i;
				}
				if (ay[i] < minAy){ //四角形の一番下の座標を特定
					minAy = ay[i];
					minAyy = i;
				}
			}
		}

		//移動座標乱数
		nan1 = rand() % 4;


		//pieace1-図形内部の場合の移動処理1
		if (cnn1 == 1){//内部座標が一個の場合
			for (int ii = 0; ii < AP - 1; ii++){
				if (cn1[ii] % 2 >= 1){
					if (nan == 0){	//上に移動
						abAx = ax[ii];	abAy = 5.0;
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[ii] - abAx) * (by[i] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i]))
									* ((ax[ii] - abAx) * (by[i + 1] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[ii] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[ii]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAy = cdAy - ay[ii];
										for (j = 0; j < AP; j++){
											ay[j] = ay[j] + saAy;
										}
									}
								}
							}
						}
					}
					else if (nan1 == 1){	//右に移動
						abAx = 5.0;	abAy = ay[ii];
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[ii] - abAx) * (by[i] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i]))
									* ((ax[ii] - abAx) * (by[i + 1] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[ii] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[ii]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAx = cdAx - ax[ii];
										for (j = 0; j < AP; j++){
											ax[j] = ax[j] + saAx;
										}
									}
								}
							}
						}
					}
					else if (nan1 == 2){	//下に移動
						abAx = ax[ii];	abAy = -5.0;
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[ii] - abAx) * (by[i] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i]))
									* ((ax[ii] - abAx) * (by[i + 1] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[ii] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[ii]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAy = cdAy - ay[ii];
										for (j = 0; j < AP; j++){
											ay[j] = ay[j] + saAy;
										}
									}
								}
							}
						}
					}
					else if (nan1 == 3){	//左に移動
						abAx = -5.0;	abAy = ay[ii];
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[ii])) - (((ay[ii] * abAx) - (ax[ii] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[ii])*(bx[i] - bx[i + 1])) - ((abAx - ax[ii])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[ii] - abAx) * (by[i] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i]))
									* ((ax[ii] - abAx) * (by[i + 1] - ay[ii]) + (ay[ii] - abAy) * (ax[ii] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[ii] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[ii]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAx = cdAx - ax[ii];
										for (j = 0; j < AP; j++){
											ax[j] = ax[j] + saAx;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//pieace1-図形内部の場合の移動処理2
		else if (cnn1 >= 2){//内部座標が複数個の場合
			for (int ii = 0; ii < BP - 1; ii++){
				if (cn1[ii] % 2 >= 1){
					if (nan == 0){	//上に移動
						abAx = ax[minAyy];	abAy = 5.0;
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[minAyy])) - (((ay[minAyy] * abAx) - (ax[minAyy] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[minAyy])*(bx[i] - bx[i + 1])) - ((abAx - ax[minAyy])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[minAyy])) - (((ay[minAyy] * abAx) - (ax[minAyy] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[minAyy])*(bx[i] - bx[i + 1])) - ((abAx - ax[minAyy])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[minAyy] - abAx) * (by[i] - ay[minAyy]) + (ay[minAyy] - abAy) * (ax[minAyy] - bx[i]))
									* ((ax[minAyy] - abAx) * (by[i + 1] - ay[minAyy]) + (ay[minAyy] - abAy) * (ax[minAyy] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[minAyy] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[minAyy]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAy = cdAy - ay[minAyy];
										for (j = 0; j < AP; j++){
											ay[j] = ay[j] + saAy;
										}
									}
								}
							}
						}
					}
					else if (nan1 == 1){	//右に移動
						abAx = 5.0;	abAy = ay[minAxx];
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[minAxx])) - (((ay[minAxx] * abAx) - (ax[minAxx] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[minAxx])*(bx[i] - bx[i + 1])) - ((abAx - ax[minAxx])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[minAxx])) - (((ay[minAxx] * abAx) - (ax[minAxx] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[minAxx])*(bx[i] - bx[i + 1])) - ((abAx - ax[minAxx])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[minAxx] - abAx) * (by[i] - ay[minAxx]) + (ay[minAxx] - abAy) * (ax[minAxx] - bx[i]))
									* ((ax[minAxx] - abAx) * (by[i + 1] - ay[minAxx]) + (ay[minAxx] - abAy) * (ax[minAxx] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[minAxx] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[minAxx]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAx = cdAx - ax[minAxx];
										for (j = 0; j < AP; j++){
											ax[j] = ax[j] + saAx;
										}
									}
								}
							}
						}
					}
					else if (nan1 == 2){	//下に移動
						abAx = ax[maxAyy];	abAy = -5.0;
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[maxAyy])) - (((ay[maxAyy] * abAx) - (ax[maxAyy] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[maxAyy])*(bx[i] - bx[i + 1])) - ((abAx - ax[maxAyy])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[maxAyy])) - (((ay[maxAyy] * abAx) - (ax[maxAyy] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[maxAyy])*(bx[i] - bx[i + 1])) - ((abAx - ax[maxAyy])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[maxAyy] - abAx) * (by[i] - ay[maxAyy]) + (ay[maxAyy] - abAy) * (ax[maxAyy] - bx[i]))
									* ((ax[maxAyy] - abAx) * (by[i + 1] - ay[maxAyy]) + (ay[maxAyy] - abAy) * (ax[maxAyy] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[maxAyy] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[maxAyy]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAy = cdAy - ay[maxAyy];
										for (j = 0; j < AP; j++){
											ay[j] = ay[j] + saAy;
										}
									}
								}
							}
						}
					}
					else if (nan1 == 3){	//左に移動
						abAx = -5.0;	abAy = by[maxAxx];
						//線分交差判定
						for (i = 0; i < BP - 1; i++){
							LineAx[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAx - ax[maxAxx])) - (((ay[maxAxx] * abAx) - (ax[maxAxx] * abAy))*(bx[i] - bx[i + 1])));
							CroAx[i] = (((abAy - ay[maxAxx])*(bx[i] - bx[i + 1])) - ((abAx - ax[maxAxx])*(by[i] - by[i + 1])));
							LineAy[i] = ((((by[i + 1] * bx[i]) - (bx[i + 1] * by[i]))*(abAy - ay[maxAxx])) - (((ay[maxAxx] * abAx) - (ax[maxAxx] * abAy))*(by[i] - by[i + 1])));
							CroAy[i] = (((abAy - ay[maxAxx])*(bx[i] - bx[i + 1])) - ((abAx - ax[maxAxx])*(by[i] - by[i + 1])));
						}
						for (i = 0; i < BP - 1; i++){
							if (LineAx[i] != 0 && CroAx[i] != 0 || LineAy[i] != 0 && CroAy[i] != 0){
								if (((ax[maxAxx] - abAx) * (by[i] - ay[maxAxx]) + (ay[maxAxx] - abAy) * (ax[maxAxx] - bx[i]))
									* ((ax[maxAxx] - abAx) * (by[i + 1] - ay[maxAxx]) + (ay[maxAxx] - abAy) * (ax[maxAxx] - bx[i + 1])) < 0){
									if (((bx[i] - bx[i + 1]) * (ay[maxAxx] - by[i]) + (by[i] - by[i + 1]) * (bx[i] - ax[maxAxx]))
										* ((bx[i] - bx[i + 1]) * (abAy - by[i]) + (by[i] - by[i + 1]) * (bx[i] - abAx)) < 0){

										cdAx = LineAx[i] / CroAx[i];
										cdAy = LineAy[i] / CroAy[i];

										saAx = cdAx - ax[maxAxx];
										for (j = 0; j < AP; j++){
											ax[j] = ax[j] + saAx;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		//ラベルの番号表示
		glColor3f(0.8, 0.0, 1.0);
		for (i = 0; i < AP - 1; i++){
			sprintf_s(str, "%d", i);
			render_stringA(ax[i], ay[i], str);
		}

		//pieace1の重心確認用
		gAx = 0; gAy = 0;
		for (i = 0; i < AP - 1; i++){
			gAx += ax[i];
			gAy += ay[i];
		}
		gAx = gAx / (AP - 1);
		gAy = gAy / (AP - 1);
		glColor3f(1.0, 0.0, 0.0);
		drawCircle(gAx, gAy, r);


		//peace1-正規化のための座標保存
		for (i = 0; i < AP; i++){
			axx[i] = ax[i];
			ayy[i] = ay[i];
		}

		//peace1の正規化点 
		QminAx = ax[0];
		QminAy = ay[0];
		for (i = 1; i < AP-1; i++){
			if (QminAx > ax[i]){
				QminAx = ax[i];
			}
		}
		for (i = 1; i < AP-1; i++){
			if (QminAy > ay[i]){
				QminAy = ay[i];
			}
		}

		if (norm1 == AP){
			//正規化実行
			for (i = 0; i < AP; i++){
				ax[i] -= QminAx;
				ay[i] -= QminAy;
			}
			//pieace1(四角形)
			glBegin(GL_LINE_LOOP);
			glColor3f(0.0, 1.0, 0.0);
			for (i = 0; i < AP; i++)
				glVertex2f(ax[i], ay[i]);
			glEnd();

			//正規化後の座標戻し
			for (i = 0; i < AP; i++){
				ax[i] = axx[i];
				ay[i] = ayy[i];
				norm1 = 0;
			}
		}

		//pieace1(四角形)
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		for (i = 0; i < AP - 1; i++){
			glVertex2f(ax[i], ay[i]);
			glVertex2f(ax[i + 1], ay[i + 1]);
		}
		glEnd();

		//pieace2角度
		thetaB = ran[1] * (3.14 / 180);

		//pieace2の距離計算
		for (i = 0; i < BP - 1; i++){
			disBx[i] = fabs(bx[0] - bx[i + 1]);
			disBy[i] = fabs(by[0] - by[i + 1]);
		}
		for (i = 0; i < BP - 1; i++){
			rrB[i] = sqrt(disBx[i] * disBx[i] + disBy[i] * disBy[i]);
		}

		//pieace2の角度計算
		if (iiii == 1){
			theta2[0] = atan(disBy[0] / disBx[0]);
			if (bx[0]>bx[1] && by[0]<by[1]) {  //左上にある
				theta2[0] = 3.14 - theta2[0];
			}
			if (bx[0]>bx[1] && by[0] > by[1]){ //左下にある
				theta2[0] += 3.14;
			}
			if (bx[0]<bx[1] && by[0]>by[1]) {	//右下にある
				theta2[0] = 2 * 3.14 - theta2[0];
			}

			for (i = 1; i < BP - 1; i++){
				theta2[i] = atan(disBy[i] / disBx[i]);
				if (bx[0]>bx[i + 1] && by[0]<by[i + 1]) {  //左上にある
					theta2[i] = 3.14 - theta2[i];
				}
				if (bx[0]>bx[i + 1] && by[0] > by[i + 1])  theta2[i] += 3.14;  //左下にある
				if (bx[0]<bx[i + 1] && by[0]>by[i + 1])  theta2[i] = 2 * 3.14 - theta2[i];  //右下にある
			}
		}
		else{
			theta2[0] = thetaBC;
			for (i = 1; i < BP - 2; i++){
				theta2[i] = thetaBD[i];
			}
		}

		thetaBC = theta2[0] + thetaB;
		if (thetaBC < 0)  thetaBC += 2 * 3.14;
		for (i = 1; i < BP - 1; i++){
			thetaBD[i] = theta2[i] + thetaB;
			if (thetaBD[i] < 0)  thetaBD[i] += 2 * 3.14;
		}

		//pieace2の角度動作
		bx[1] = bx[0] + rrB[0] * cos(thetaBC);
		by[1] = by[0] + rrB[0] * sin(thetaBC);
		for (i = 1; i < BP - 2; i++){
			bx[i + 1] = bx[0] + rrB[i] * cos(thetaBD[i]);
			by[i + 1] = by[0] + rrB[i] * sin(thetaBD[i]);
		}

		//pieace2の平行移動
		for (i = 0; i < BP; i++){
			if (ram[1] == 0) bx[i] += ran[3];
			else by[i] += ran[3];

			norm2 += 1; //正規化スイッチ
		}

		//pieace2とpieace1の内外判定（近接用）
		//※図形の境界線上は図形外としている。これは境界線上が図形の大きさに含まれないためである。
		for (i = 0; i < AP - 1; i++){
			for (j = 0; j < BP - 1; j++){
				if (((bx[j] - 5.0) * (ay[i] - by[j]) + (by[j] - by[j]) * (bx[j] - ax[i]))
					* ((bx[j] - 5.0) * (ay[i + 1] - by[j]) + (by[j] - by[j]) * (bx[j] - ax[i + 1])) < 0){
					if (((ax[i] - ax[i + 1]) * (by[j] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[j]))
						* ((ax[i] - ax[i - 1]) * (by[j] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - 5.0)) < 0){
						gcn2[j] += 1;
					}
				}
			}
		}

		//pieace2の重心を求める
		for (i = 0; i < BP - 1; i++){
			gBx += bx[i];
			gBy += by[i];
		}
		gBx = gBx / (BP - 1);
		gBy = gBy / (BP - 1);


		//pieace2-内部座標の個数確認（近接用）
		for (i = 0; i < BP; i++){
			if (gcn2[i] % 2 >= 1){
				gcng2 += 1;
			}
		}printf("gcng2=%d\n", gcng2);

		//pieace2が離れている場合、四角形内部に入れる処理
		if (iiii>1){
			if (gcng2 == 0){
				//pieace2の距離計算
				for (i = 0; i < BP; i++){
					gdisBx[i] = -(gBx - bx[i]);
					gdisBy[i] = -(gBy - by[i]);
				}
				//pieace2の移動
				for (i = 0; i < BP; i++){
					gBx = gAx; gBy = gAy;
					bx[i] = gBx + gdisBx[i];
					by[i] = gBy + gdisBy[i];
				}
				ggg+=1;
				printf("重心まで移動%d\n", ggg);
			}
		}

		//pieace2とpieace1の内外判定（排出用）
		//※図形の境界線上は図形外としている。これは境界線上が図形の大きさに含まれないためである。
		for (i = 0; i < AP - 1; i++){
			for (j = 0; j < BP - 1; j++){
				if (((bx[j] - 5.0) * (ay[i] - by[j]) + (by[j] - by[j]) * (bx[j] - ax[i]))
					* ((bx[j] - 5.0) * (ay[i + 1] - by[j]) + (by[j] - by[j]) * (bx[j] - ax[i + 1])) < 0){
					if (((ax[i] - ax[i + 1]) * (by[j] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[j]))
						* ((ax[i] - ax[i - 1]) * (by[j] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - 5.0)) < 0){
						cn2[j] += 1;
					}
				}
			}
		}

		//pieace2-内部座標の個数確認（排出用）
		for (i = 0; i < BP; i++){
			if (cn2[i] % 2 >= 1){
				cnn2 += 1;
			}
		}printf("cnn2=%d\n\n", cnn2);

		//pieace2が内部に入っている場合の計算
		minBy = 5.0;	maxBy = -5.0;
		minBx = 5.0;	maxBx = -5.0;
		for (i = 0; i < BP - 1; i++){
			if (cn2[i] % 2 >= 1){
				if (bx[i] > maxBx){ //三角形の一番右の座標を特定
					maxBx = bx[i];
					maxBxx = i;
				}
				if (by[i] > maxBy){ //三角形の一番上の座標を特定
					maxBy = by[i];
					maxByy = i;
				}
				if (bx[i] < minBx){ //三角形の一番左の座標を特定
					minBx = bx[i];
					minBxx = i;
				}
				if (by[i] < minBy){ //三角形の一番下の座標を特定
					minBy = by[i];
					minByy = i;
				}
			}
		}

		//pieace2-移動座標乱数
		nan2 = rand() % 4;

		//pieace2-図形内部の場合の移動処理1
		if (cnn2 == 1){//内部座標が一個の場合
			for (int ii = 0; ii < BP - 1; ii++){
				if (cn2[ii] % 2 >= 1){
					if (nan == 0){	//上に移動
						abBx = bx[ii];	abBy = 5.0;
						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[ii] - abBx) * (ay[i] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i]))
									* ((bx[ii] - abBx) * (ay[i + 1] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[ii] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[ii]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBy = cdBy - by[ii];
										for (j = 0; j < BP; j++){
											by[j] = by[j] + saBy;
										}
									}
								}
							}
						}
					}
					else if (nan2 == 1){	//右に移動
						abBx = 5.0;	abBy = by[ii];
						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[ii] - abBx) * (ay[i] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i]))
									* ((bx[ii] - abBx) * (ay[i + 1] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[ii] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[ii]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBx = cdBx - bx[ii];
										for (j = 0; j < BP; j++){
											bx[j] = bx[j] + saBx;
										}
									}
								}
							}
						}
					}
					else if (nan2 == 2){	//下に移動
						abBx = bx[ii];	abBy = -5.0;
						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[ii] - abBx) * (ay[i] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i]))
									* ((bx[ii] - abBx) * (ay[i + 1] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[ii] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[ii]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBy = cdBy - by[ii];
										for (j = 0; j < BP; j++){
											by[j] = by[j] + saBy;
										}
									}
								}
							}
						}
					}
					else if (nan2 == 3){	//左に移動
						abBx = -5.0;	abBy = by[ii];
						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[ii])) - (((by[ii] * abBx) - (bx[ii] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[ii])*(ax[i] - ax[i + 1])) - ((abBx - bx[ii])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[ii] - abBx) * (ay[i] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i]))
									* ((bx[ii] - abBx) * (ay[i + 1] - by[ii]) + (by[ii] - abBy) * (bx[ii] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[ii] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[ii]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBx = cdBx - bx[ii];
										for (j = 0; j < BP; j++){
											bx[j] = bx[j] + saBx;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//piace2-図形内部の場合の移動処理2
		else if (cnn2 >= 2 || ggg==1){//内部座標が複数個の場合
			for (int ii = 0; ii < BP - 1; ii++){
				if (cn2[ii] % 2 >= 1){
					if (nan == 0){	//上に移動
						abBx = bx[minByy];	abBy = 5.0;

						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[minByy])) - (((by[minByy] * abBx) - (bx[minByy] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[minByy])*(ax[i] - ax[i + 1])) - ((abBx - bx[minByy])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[minByy])) - (((by[minByy] * abBx) - (bx[minByy] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[minByy])*(ax[i] - ax[i + 1])) - ((abBx - bx[minByy])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[minByy] - abBx) * (ay[i] - by[minByy]) + (by[minByy] - abBy) * (bx[minByy] - ax[i]))
									* ((bx[minByy] - abBx) * (ay[i + 1] - by[minByy]) + (by[minByy] - abBy) * (bx[minByy] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[minByy] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[minByy]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBy = cdBy - by[minByy];
										for (j = 0; j < BP; j++){
											by[j] = by[j] + saBy;
										}
									}
								}
							}
						}
					}
					else if (nan2 == 1){	//右に移動
						abBx = 5.0;	abBy = by[minBxx];
						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[minBxx])) - (((by[minBxx] * abBx) - (bx[minBxx] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[minBxx])*(ax[i] - ax[i + 1])) - ((abBx - bx[minBxx])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[minBxx])) - (((by[minBxx] * abBx) - (bx[minBxx] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[minBxx])*(ax[i] - ax[i + 1])) - ((abBx - bx[minBxx])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[minBxx] - abBx) * (ay[i] - by[minBxx]) + (by[minBxx] - abBy) * (bx[minBxx] - ax[i]))
									* ((bx[minBxx] - abBx) * (ay[i + 1] - by[minBxx]) + (by[minBxx] - abBy) * (bx[minBxx] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[minBxx] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[minBxx]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBx = cdBx - bx[minBxx];
										for (j = 0; j < BP; j++){
											bx[j] = bx[j] + saBx;
										}
									}
								}
							}
						}
					}
					else if (nan2 == 2){	//下に移動
						abBx = bx[maxByy];	abBy = -5.0;
						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[maxByy])) - (((by[maxByy] * abBx) - (bx[maxByy] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[maxByy])*(ax[i] - ax[i + 1])) - ((abBx - bx[maxByy])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[maxByy])) - (((by[maxByy] * abBx) - (bx[maxByy] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[maxByy])*(ax[i] - ax[i + 1])) - ((abBx - bx[maxByy])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[maxByy] - abBx) * (ay[i] - by[maxByy]) + (by[maxByy] - abBy) * (bx[maxByy] - ax[i]))
									* ((bx[maxByy] - abBx) * (ay[i + 1] - by[maxByy]) + (by[maxByy] - abBy) * (bx[maxByy] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[maxByy] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[maxByy]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBy = cdBy - by[maxByy];
										for (j = 0; j < BP; j++){
											by[j] = by[j] + saBy;
										}
									}
								}
							}
						}
					}
					else if (nan2 == 3){	//左に移動
						abBx = -5.0;	abBy = by[maxBxx];
						//線分交差判定
						for (i = 0; i < AP - 1; i++){
							LineBx[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBx - bx[maxBxx])) - (((by[maxBxx] * abBx) - (bx[maxBxx] * abBy))*(ax[i] - ax[i + 1])));
							CroBx[i] = (((abBy - by[maxBxx])*(ax[i] - ax[i + 1])) - ((abBx - bx[maxBxx])*(ay[i] - ay[i + 1])));
							LineBy[i] = ((((ay[i + 1] * ax[i]) - (ax[i + 1] * ay[i]))*(abBy - by[maxBxx])) - (((by[maxBxx] * abBx) - (bx[maxBxx] * abBy))*(ay[i] - ay[i + 1])));
							CroBy[i] = (((abBy - by[maxBxx])*(ax[i] - ax[i + 1])) - ((abBx - bx[maxBxx])*(ay[i] - ay[i + 1])));
						}
						for (i = 0; i < AP - 1; i++){
							if (LineBx[i] != 0 && CroBx[i] != 0 || LineBy[i] != 0 && CroBy[i] != 0){
								if (((bx[maxBxx] - abBx) * (ay[i] - by[maxBxx]) + (by[maxBxx] - abBy) * (bx[maxBxx] - ax[i]))
									* ((bx[maxBxx] - abBx) * (ay[i + 1] - by[maxBxx]) + (by[maxBxx] - abBy) * (bx[maxBxx] - ax[i + 1])) < 0){
									if (((ax[i] - ax[i + 1]) * (by[maxBxx] - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - bx[maxBxx]))
										* ((ax[i] - ax[i + 1]) * (abBy - ay[i]) + (ay[i] - ay[i + 1]) * (ax[i] - abBx)) < 0){

										cdBx = LineBx[i] / CroBx[i];
										cdBy = LineBy[i] / CroBy[i];

										saBx = cdBx - bx[maxBxx];
										for (j = 0; j < BP; j++){
											bx[j] = bx[j] + saBx;
										}
									}
								}
							}
						}
					}
				}
			}
		}


		//ラベルの番号表示
		glColor3f(0.0, 0.0, 1.0);
		for (i = 0; i < BP - 1; i++){
			sprintf_s(str, "%d", i);
			render_stringA(bx[i], by[i], str);
		}

		//pieace2(三角形)
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		for (i = 0; i < BP - 1; i++){
			glVertex2f(bx[i], by[i]);
			glVertex2f(bx[i + 1], by[i + 1]);
		}
		glEnd();

		//pieace2の重心確認用
		gBx = 0; gBy = 0;
		for (i = 0; i < BP - 1; i++){
			gBx += bx[i];
			gBy += by[i];
		}
		gBx = gBx / (BP - 1);
		gBy = gBy / (BP - 1);
		glColor3f(0.0, 0.0, 1.0);
		drawCircle(gBx, gBy, r);


		//peace2-正規化のための座標保存
		for (i = 0; i < BP; i++){
			bxx[i] = bx[i];
			byy[i] = by[i];
		}

		if (norm2 == BP){
			//正規化実行
			for (i = 0; i < BP; i++){
				bx[i] -= QminAx;
				by[i] -= QminAy;
			}
			//pieace2(三角形)
			glBegin(GL_LINE_LOOP);
			glColor3f(0.0, 1.0, 0.0);
			for (i = 0; i < BP; i++)
				glVertex2f(bx[i], by[i]);
			glEnd();

			//正規化後の座標戻し
			for (i = 0; i < BP; i++){
				bx[i] = bxx[i];
				by[i] = byy[i];
				norm2 = 0;
			}
		}

		glFlush();

		//角度乱数	※図形が離れていくのはSAで調整
		ran[0] = (double)(rand() % 21 - 10);
		ran[1] = (double)(rand() % 21 - 10);
		//平行移動乱数
		ran[2] = (double)(rand() % 2) / 10;
		ran[3] = (double)(rand() % 2) / 10;
		if (ran[2] == 0) ran[2] -= 0.1;	//0の時は-0.1にする
		if (ran[3] == 0) ran[3] -= 0.1;	//0の時は-0.1にする
		//平行移動スイッチ
		ram[0] = (double)(rand() % 2);
		ram[1] = (double)(rand() % 2);

		//内外判定の初期化
		for (i = 0; i < AP - 1; i++){
			cn1[i] = 0;
		}
		nan1 = 0; cnn1 = 0;
		for (i = 0; i < BP - 1; i++){
			cn2[i] = 0;
		}
		nan2 = 0; cnn2 = 0;

		//近接処理の初期化
		for (i = 0; i < BP - 1; i++){
			gcn2[i] = 0;
		}
		gcng2 = 0;

		//内部三角形の最大最小の初期化
		maxAxx = 0; minAxx = 0;
		maxAyy = 0; minAyy = 0;
		maxBxx = 0; minBxx = 0;
		maxByy = 0; minByy = 0;

		//重心の初期化
		gAx = 0; gAy = 0; gBx = 0; gBy = 0;
		ggg = 0;


		//getchar();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(400, 400);  // ウィンドウサイズは400x400ドット
	glutInitWindowPosition(600, 300);  // ウィンドウサイズの表示位置の指定
	glutCreateWindow("simple");
	glOrtho(-5.0, 5.0, -5.0, 5.0, -1.0, 1.0);

	printf("開始します。何かキーを押してください。");  getchar();

	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}