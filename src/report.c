#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "myShape.h"

#define KEY_SPACE 32
#define KEY_ESC 27
#define SCREEN_W 700
#define SCREEN_H 500
#define SCREEN_Y -1

// text
#define TXT_W 800
#define TXT_H 800
#define TXT_SCORE_Y 750
#define TXT_SCORE_W 16

#define END_RANGE 25

// ground
#define GROUND_W 200.0
#define GROUND_H 20.0

// player
#define GRAVITY 0.0003
#define PLAYER_JUMP_SPD 0.04
#define PLAYER_ROLL_SPD -1.4
#define PLAYER_RAD 0.5
#define PlAYER_BTN_VOL 3

// enemy
#define ENEMY_SPD 0.03
#define ENEMY_ROLL_RATE 60.0
#define ENEMY_VOL 4
#define ENEMY_RAD 1
#define ENEMY_SPINE_VOL 12

// star
#define STAR_VOL 50

// rocket
#define ROCKET_SPD 0.001
#define ROCKET_WING_VOL 6

// wall
#define WALL_SPD 0.08

void restart(void);
void polarView(void);

void setScore(int scoreArg, int posXArg);
void setGround(void);
void setPlayer(void);
void setEnemy(int enemyNum);
void setStar(int StarNum);
void setRocket(void);
void setWall(void);

int scene = 1; // 1:スタート画面　2:ゲーム画面
int nextScene = 1;
float score;
int maxScore = 0;
float theta;

float elevation;
float azimuth;
float distance;

// player
float playerSpeedY;
float playerPosX;
float playerPosY;
float playerTheta;
int isJump;

// enemy
float enemyPosX[ENEMY_VOL];
float enemyPosY[ENEMY_VOL];

// star
float starPosX[STAR_VOL];
float starPosY[STAR_VOL];
float starPosZ[STAR_VOL];

// rocket
float rocketPosX;
float rocketPosY;
float rocketSpeed;
int isMoveRocket = 0;

// wall
float wallPosX = 100;
int isChangeScene = 0;

// マテリアル
float diffuseRed[] = { 1.0, 0.0, 0.0, 1.0 };
float diffuseDarkRed[] = { 0.6, 0.0, 0.0, 1.0 };
float diffuseBlue[] = { 0.6, 0.6, 1.0, 1.0 };
float diffuseLightBlue[] = { 0.8, 0.8, 1.0, 1.0 };
float diffuseBlack[] = { 0.15, 0.15, 0.15, 1.0 };
float diffuseBrown[] = { 0.3, 0.15, 0.15, 1.0 };
float diffuseSilver[] = { 0.6, 0.6, 0.6, 1.0 };
float diffuseDarkSilver[] = { 0.55, 0.55, 0.55, 1.0 };

float specular[] = { 0.8, 0.8, 0.8, 1.0 };
float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
float shininess = 124.0;

DrawString(char* str, float x0, float y0, double w, double h)
{
	int len;

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		glLoadIdentity();
		gluOrtho2D(0.0, w, 0.0, h);
		len = strlen(str);
		for (int i = 0; i < len; i++)
		{
			glRasterPos2f(x0 + 20 * i, y0);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
			str++;
		}
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

void restart(void)
{
	distance = 20.0;
	elevation = 0.0;
	azimuth = 0.0;

	if (maxScore < score)
	{
		maxScore = score;
	}
	score = 0.0;
	theta = 0.0;

	// player
	playerSpeedY = 0.0;
	playerPosX = -2.0;
	playerPosY = PLAYER_RAD;
	playerTheta = 0.0;
	isJump = 1;

	// enemy
	for (int i = 0; i < ENEMY_VOL; i++)
	{
		enemyPosX[i] = 20 * (i + 2);
	}

	// rocket
	rocketPosX = -2.0;
	rocketPosY = 3.0;
	rocketSpeed = 0.0;
}

void polarView(void)
{
	glTranslatef(0.0, SCREEN_Y, -distance);
	glRotatef(-elevation, 1.0, 0.0, 0.0);
	glRotatef(azimuth, 0.0, 1.0, 0.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// テキスト
	glPushMatrix();
		DrawString("MAX", 50, TXT_SCORE_Y, TXT_W, TXT_H);
		setScore(maxScore, 200);

		if (scene == 1)
		{
			DrawString("Push to Start!", 260, 200 + sin(theta), TXT_W, TXT_H);
			DrawString("<SpaceKey>", 295, 140 + sin(theta), TXT_W, TXT_H);
		}
		if (scene == 2)
		{
			setScore(score, 750);
		}
	glPopMatrix();

	// モデリング
	glPushMatrix();
		polarView();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHT0);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		glShadeModel(GL_SMOOTH);

		if (scene == 2)
		{
			// スコア加算
			if (nextScene == 2)
			{
				score += ENEMY_SPD;
			}

			setGround();
			if (isChangeScene == 0)
			{
				setPlayer();
				for (int i = 0; i < ENEMY_VOL; i++)
				{
					setEnemy(i);
				}
			}
		}

		for (int i = 0; i < STAR_VOL; i++)
		{
			setStar(i);
		}

		setRocket();
		setWall();

		glDisable(GL_DEPTH_TEST);
	glPopMatrix();

	glFlush();
}

void idle(void)
{
	theta = fmod(theta + 0.01, 360.0);
	glutPostRedisplay();
}

void mySkey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			if (scene == 1)
			{
				if (isMoveRocket == 0)
				{
					isMoveRocket = 1;
					rocketSpeed = -0.1;
				}
			}

			if (isJump == 0)
			{
				isJump = 1;
				playerSpeedY = PLAYER_JUMP_SPD;
				playerPosY += playerSpeedY;
			}
			break;
	}
	glutPostRedisplay();
}

void myKbd(unsigned char key, int x, int y)
{
	switch (key)
	{
		case KEY_SPACE:
			if (scene == 1)
			{
				if (isMoveRocket == 0)
				{
					isMoveRocket = 1;
					rocketSpeed = -0.1;
				}
			}

			if (isJump == 0)
			{
				isJump = 1;
				playerSpeedY = PLAYER_JUMP_SPD;
				playerPosY += playerSpeedY;
			}
			break;
		case KEY_ESC:
			exit(0);

	}
	glutPostRedisplay();
}

void setScore(int scoreArg, int posXArg)
{
	int cnt = 0;
	int numTmp;
	DrawString("m", posXArg + TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);

	do
	{
		numTmp = scoreArg % 10;

		switch (numTmp)
		{
			case (0):
				DrawString("0", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (1):
				DrawString("1", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (2):
				DrawString("2", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (3):
				DrawString("3", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (4):
				DrawString("4", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (5):
				DrawString("5", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (6):
				DrawString("6", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (7):
				DrawString("7", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (8):
				DrawString("8", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
			case (9):
				DrawString("9", posXArg - cnt * TXT_SCORE_W, TXT_SCORE_Y, TXT_W, TXT_H);
				break;
		}
		cnt++;
		scoreArg /= 10;
	} while (scoreArg != 0);
}

void setGround(void)
{
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseDarkSilver);

		glTranslatef(0.0, -GROUND_H /2.0 - PLAYER_RAD, 0.0);
		glScalef(GROUND_W, GROUND_H, 3.0);

		glEnable(GL_LIGHTING);
		glutSolidCube(1.0);
		glDisable(GL_LIGHTING);
	glPopMatrix();
}

void setPlayer(void)
{
	playerSpeedY -= GRAVITY;

	// 着地判定
	if (playerPosY <= 0.0)
	{
		isJump = 0;
		playerTheta = 0.0;
	}

	if (isJump == 1)
	{
		playerPosY += playerSpeedY;
		playerTheta += PLAYER_ROLL_SPD;
	}

	glPushMatrix();
		glTranslatef(playerPosX, playerPosY, 0.0);
		glRotatef(playerTheta, 0, 0, 1);
		if (isJump == 0)
		{
			glRotatef((int)(sin(theta * 2) * 10 - 5), 0, 0, 1); // 揺らすアニメーション
		}
		glRotatef(50, 0, 1, 0);

		// 1段目の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseLightBlue);

			glEnable(GL_LIGHTING);
			glutSolidSphere(PLAYER_RAD, 36, 36);
			glDisable(GL_LIGHTING);

			// 右腕の描画
			glPushMatrix();
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBrown);

				glTranslatef(0.4, 0.2, 0.0);
				glRotatef(-60, 0, 0, 1);

				glEnable(GL_LIGHTING);
				mySolidCylinder(0.05, 0.7, 36);
				glDisable(GL_LIGHTING);
			glPopMatrix();

			// 左腕の描画
			glPushMatrix();
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBrown);

				glTranslatef(-0.4, 0.2, 0.0);
				glRotatef(60, 0, 0, 1);

				glEnable(GL_LIGHTING);
				mySolidCylinder(0.05, 0.7, 36);
				glDisable(GL_LIGHTING);
			glPopMatrix();

			// ボタンの描画
			glPushMatrix();
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBrown);
				for (int i = 0; i < PlAYER_BTN_VOL; i++)
				{
					glPushMatrix();
						glRotatef(20 * i - 21, 1, 0, 0);
						glTranslatef(0.0, 0.0, 0.42);

						glEnable(GL_LIGHTING);
						glutSolidSphere(0.1, 36, 36);
						glDisable(GL_LIGHTING);
					glPopMatrix();
				}
			glPopMatrix();
		glPopMatrix();

		// 2段目の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseLightBlue);

			glTranslatef(0.0, PLAYER_RAD * 1.3, 0.0);

			glEnable(GL_LIGHTING);
			glutSolidSphere(PLAYER_RAD * 0.85, 36, 36);
			glDisable(GL_LIGHTING);

			// 右目の描画
			glPushMatrix();
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBlack);

				glTranslatef(0.1, 0.1, 0.3);

				glEnable(GL_LIGHTING);
				glutSolidSphere(PLAYER_RAD * 0.22, 36, 36);
				glDisable(GL_LIGHTING);
			glPopMatrix();

			// 左目の描画
			glPushMatrix();
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBlack);

				glTranslatef(-0.1, 0.1, 0.3);

				glEnable(GL_LIGHTING);
				glutSolidSphere(PLAYER_RAD * 0.22, 36, 36);
				glDisable(GL_LIGHTING);
			glPopMatrix();

			// 鼻の描画
			glPushMatrix();
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseRed);

				glTranslatef(0.0, -0.1, 0.2);
				glRotatef(-20, 1, 0, 0);

				glEnable(GL_LIGHTING);
				glutSolidCone(0.1, 0.5, 36, 36);
				glDisable(GL_LIGHTING);
			glPopMatrix();
		glPopMatrix();
		
	glPopMatrix();
}

void setEnemy(int enemyNum)
{
	float playerDis;

	playerDis = sqrt(pow(enemyPosX[enemyNum] - playerPosX, 2) + pow(enemyPosY[enemyNum] - playerPosY, 2));
	enemyPosX[enemyNum] -= ENEMY_SPD;
	enemyPosY[enemyNum] = -PLAYER_RAD;

	// 衝突判定
	if (playerDis < ENEMY_RAD + PLAYER_RAD)
	{
		nextScene = 1;
	}

	// 再配置
	if (enemyPosX[enemyNum] < -END_RANGE)
	{
		enemyPosX[enemyNum] = rand() % END_RANGE + END_RANGE;
	}

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBlack);

		glTranslatef(enemyPosX[enemyNum], enemyPosY[enemyNum], 0.0);
		glScalef(1.0, 1.0, 0.95);
		glRotatef(theta * ENEMY_ROLL_RATE, 0, 0, 1);

		glEnable(GL_LIGHTING);
		glutSolidSphere(ENEMY_RAD, 36, 36);
		glDisable(GL_LIGHTING);

		// 軸の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseSilver);

			glTranslatef(0.0, 0.0, ENEMY_RAD * 0.9);
			glScalef(0.5, 0.5, 1.5);

			glEnable(GL_LIGHTING);
			glutSolidSphere(0.3, 36, 36);
			glDisable(GL_LIGHTING);
		glPopMatrix();

		// 棘の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseSilver);

			for (int i = 0; i < ENEMY_SPINE_VOL; i++)
			{
				glRotatef(360.0 / ENEMY_SPINE_VOL, 0, 0, 1);
				glPushMatrix();
					glTranslatef(0.0, 0.5, 0.0);
					glRotatef(90, 1, 0, 0);
					glRotatef(180, 1, 0, 0);

					glEnable(GL_LIGHTING);
					glutSolidCone(0.5, 1.0, 12, 3);
					glDisable(GL_LIGHTING);
				glPopMatrix();
			}
		glPopMatrix();
	glPopMatrix();
}

void setStar(int starNum)
{
	starPosX[starNum] -= ENEMY_SPD * (5.03 - ((scene - 1) * 5));

	// 再配置
	if (starPosX[starNum] < -END_RANGE)
	{
		starPosX[starNum] = rand() % END_RANGE + END_RANGE;
		starPosY[starNum] = rand() % 35 - 10;
		starPosZ[starNum] = rand() % 25 - 25;
	}

	glPushMatrix();
		glTranslatef(starPosX[starNum], starPosY[starNum], starPosZ[starNum]);
		glScalef(0.1, 0.1, 0.1);

		glEnable(GL_LIGHTING);
		glutSolidSphere(1.0, 12.0, 12.0);
		glDisable(GL_LIGHTING);
	glPopMatrix();
}

void setRocket(void)
{
	if (isMoveRocket == 1)
	{
		rocketSpeed += ROCKET_SPD;
		if (END_RANGE < rocketPosX)
		{
			nextScene = 2;
			isMoveRocket = 0;
		}
	}
	rocketPosX += rocketSpeed;
	rocketPosY = sin(theta / 5) / 2;

	glPushMatrix();
		if (scene == 1)
		{
			glTranslatef(rocketPosX, rocketPosY - SCREEN_Y, -20.0);
			glRotatef(-90, 0, 0, 1);
		}
		if (scene == 2)
		{
			glTranslatef(0, 0, 10);
			glRotatef(theta * 3, 1, 0, 0);
			glRotatef(180, 0, 0, 1);
			glTranslatef(0, 0, 30);
		}
		glRotatef(theta * 3, 0, 1, 0);
		glScalef(2, 3, 2);
		
		// 先端の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseRed);

			glTranslatef(0.0, 2.0, 0.0);
			glRotatef(-90.0, 1.0, 0.0, 0.0);

			glEnable(GL_LIGHTING);
			glutSolidCone(1.0, 2.0, 12, 3);
			glDisable(GL_LIGHTING);
		glPopMatrix();

		// 2段目の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseSilver);

			glTranslatef(0.0, 1.0, 0.0);

			glEnable(GL_LIGHTING);
			mySolidCylinder(1.0, 2.0, 12);
			glDisable(GL_LIGHTING);
		glPopMatrix();

		// 1段目の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseDarkSilver);

			glTranslatef(0.0, -1.0, 0.0);

			glEnable(GL_LIGHTING);
			mySolidCylinder(1.0, 2.0, 12);
			glDisable(GL_LIGHTING);
		glPopMatrix();

		// 噴射口の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBlack);

			glTranslatef(0.0, -2.5, 0.0);
			glRotatef(-90.0, 1.0, 0.0, 0.0);

			glEnable(GL_LIGHTING);
			glutSolidCone(0.5, 1.0, 12, 3);
			glDisable(GL_LIGHTING);
		glPopMatrix();

		// 羽の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseDarkRed);

			for (int i = 0; i < ROCKET_WING_VOL; i++)
			{
				glRotatef(-360 / ROCKET_WING_VOL, 0, 1, 0);
				glPushMatrix();
					glTranslatef(1.0, -2.0, 0.0);
					glScalef(1.5, 2.0, 0.5);
					glRotatef(-90.0, 1, 0, 0);

					glEnable(GL_LIGHTING);
					glutSolidCone(0.5, 1.0, 12, 3);
					glDisable(GL_LIGHTING);
				glPopMatrix();
			}
		glPopMatrix();

		// 窓の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBlue);

			glTranslatef(0.0, 1.0, 0.8);
			glScalef(0.6, 0.4, 0.5);

			glEnable(GL_LIGHTING);
			glutSolidSphere(1.0, 24.0, 15.0);
			glDisable(GL_LIGHTING);
		glPopMatrix();

		// 窓枠の描画
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseBlack);

			glTranslatef(0.0, 1.0, 0.57);
			glScalef(0.6, 0.4, 1.0);
			glRotatef(-90, 1, 0, 0);

			glEnable(GL_LIGHTING);
			mySolidCylinder(1.0, 1.0, 24.0);
			glDisable(GL_LIGHTING);
		glPopMatrix();
	glPopMatrix();
}

void setWall(void)
{
	if (scene != nextScene)
	{
		if (isChangeScene == 0)
		{
			isChangeScene = 1;
			wallPosX = -20;
		}
		else if (0 < wallPosX)
		{
			isChangeScene = 0;
			scene = nextScene;
			if (scene == 1)
			{
				restart();
			}
		}
	}
	wallPosX += WALL_SPD;

	glPushMatrix();
		glTranslatef(wallPosX, 0.0, 1.0);
		glScalef(20.0, 50, 2.0);

		glEnable(GL_LIGHTING);
		glutSolidCube(1.0);
		glDisable(GL_LIGHTING);
	glPopMatrix();
}

void myInit(char* progname)
{
	float aspect = (float)SCREEN_W / (float)SCREEN_H;

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(progname);
	glClearColor(0.0, 0.0, 0.3, 1.0);

	glutSpecialFunc(mySkey);
	glutKeyboardFunc(myKbd);
	restart();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, aspect, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	glutInit(&argc,argv);
	myInit(argv[0]);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
	return(0);
}