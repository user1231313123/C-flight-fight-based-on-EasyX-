#define _CRT_SECURE_NO_WARNINGS
#define EnemySpeed 1
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<vector>
#include<iostream>
#include<ctime>
#include<string>
#include<fstream>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib ")
//播放主菜单BGM进入游戏后关闭
void PlayBGM()
{
	mciSendString("open ./S1.wma alias bgm", NULL, 0, NULL);
	mciSendString("play bgm repeat", NULL, 0, NULL);
}
void StopBGM()
{
	mciSendString("close bgm", NULL, 0, NULL);
}

//播放战斗音乐
void PlayBGM2()
{
	mciSendString("open ./fightmusic.wma alias bgm2", NULL, 0, NULL);
	mciSendString("play bgm2 repeat", NULL, 0, NULL);
}
void StopBGM2()
{
	mciSendString("close bgm2", NULL, 0, NULL);
}
using namespace std;
int num = 0;
//导入Enemy爆炸动画图片
IMAGE EnemyBoom[4];
//导入Player爆炸动画图片
IMAGE PlayerBoom[4];
//导入StrongEnemy爆炸动画图片
IMAGE StrongEnemyBoom[4];
IMAGE heroimg, enemyimg, bkimg, bulletimg;
IMAGE StrongEnemyimg;


void LoadImg()
{

	loadimage(&StrongEnemyimg, "./images/enemy3_hit.png");
	loadimage(&heroimg, "./images/me1.png", 51, 63);
	loadimage(&enemyimg, "./images/enemy1.png");
	loadimage(&bkimg, "./images/bk2.png", 600, 2400);
	loadimage(&bulletimg, "./images/bullet1.png");
	//Enemy爆炸动画图片加载
	loadimage(&EnemyBoom[0], "./images/enemy1_down1.png");
	loadimage(&EnemyBoom[1], "./images/enemy1_down2.png");
	loadimage(&EnemyBoom[2], "./images/enemy1_down3.png");
	loadimage(&EnemyBoom[3], "./images/enemy1_down4.png");
	//Player爆炸动画图片加载
	loadimage(&PlayerBoom[0], "./images/me_destroy_1.png", 51, 63);
	loadimage(&PlayerBoom[1], "./images/me_destroy_2.png", 51, 63);
	loadimage(&PlayerBoom[2], "./images/me_destroy_3.png", 51, 63);
	loadimage(&PlayerBoom[3], "./images/me_destroy_4.png", 51, 63);
	//StrongEnemy爆炸动画图片加载
	loadimage(&StrongEnemyBoom[0], "./images/enemy3_down1.png");
	loadimage(&StrongEnemyBoom[1], "./images/enemy3_down2.png");
	loadimage(&StrongEnemyBoom[2], "./images/enemy3_down3.png");
	loadimage(&StrongEnemyBoom[3], "./images/enemy3_down4.png");
}
const int screenwidth = 600;
const int screenheight = 1200;
bool is_Area(int x, int y, RECT& r)
{
	return (r.left <= x && r.right >= x && r.top <= y && y <= r.bottom);
}


//声名所有的类
class aircraft;
class Bullet;
class Enemy;
class Hero;
class EnemyBullet;
class StrongEnemy;
class StrongEnemyBullet;
class Bullet
{
public:
	Bullet(IMAGE& img, RECT pr)
		:img(img), Damege(25)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top + img.getheight();
	}
	Bullet(IMAGE& img, RECT pr, int d)
		:img(img), Damege(d)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top + img.getheight();
	}
	virtual bool Show()
	{
		if (rect.bottom <= 0)
		{
			return false;
		}
		rect.top -= 50;
		rect.bottom -= 50;
		putimage(rect.left, rect.top, &img);

		return true;
	}
	RECT& GetRect() { return rect; }
	Bullet& operator=(const Bullet& other)
	{
		if (this != &other)
		{
			img = other.img;
			rect = other.rect;
		}
		return *this;
	}
	int GetDamege() { return Damege; }
protected:
	RECT rect;
	IMAGE& img;
	int Damege;
};

class EnemyBullet :public Bullet
{
public:
	EnemyBullet(IMAGE& img, RECT pr) :Bullet(img, pr) {}
	virtual bool Show()
	{
		if (rect.top >= screenheight)
		{
			return false;
		}
		rect.top += 4;
		rect.bottom += 4;
		putimage(rect.left, rect.top, &img);

		return true;
	}
	EnemyBullet& operator=(const EnemyBullet& other)
	{
		if (this != &other)
		{
			Bullet::operator=(other);
		}
		return *this;
	}
};
class StrongEnemyBullet :public Bullet
{
public:
	StrongEnemyBullet(IMAGE& img, RECT pr, int of) :Bullet(img, pr, 100), offset(of) {}
	virtual bool Show()
	{
		if (rect.top >= screenheight)
		{
			return false;
		}

		rect.top += 4;
		rect.bottom += 4;
		rect.left += offset;
		rect.right += offset;
		//cout <<"StrongEnemyBullet:"<< rect.top << " " << rect.bottom << " " << rect.left << " " << rect.right << endl;
		putimage(rect.left, rect.top, &img);
		return true;
	}
protected:
	int offset;//偏移量
};

class BK
{
public:
	BK(IMAGE& i) :img(i), h(-screenheight) {}
	~BK() {}
	void show()
	{
		if (h >= 0) { h = -screenheight; }
		h += 1;
		putimage(0, h, &img);
	}
private:
	IMAGE& img;
	double h;//高度
};

class aircraft
{
public:
	aircraft(IMAGE& i) :img(i) {}
	aircraft(IMAGE& i, int hp) : img(i), HP(hp)
	{
		rect.left = 0;
		rect.top = 0;
		rect.right = 0;
		rect.bottom = 0;
	}

	virtual ~aircraft() {}

	int GetHP() { return HP; }
	void SetHP(int x) { HP = x; }
	bool is_die() { return HP <= 0; }
	void Attacked(int Damege) { HP -= Damege; }

	RECT& GetRect()
	{
		return rect;
	}
	virtual void SetRect() {}
	virtual void show() {}
protected:
	int HP;

	IMAGE& img;
	RECT rect;
};

class Hero : public aircraft
{
public:
	virtual void SetRect()
	{
		rect.left = screenwidth / 2 - img.getwidth() / 2;
		rect.top = screenheight - img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = screenheight;
	}
	Hero(IMAGE& i) : aircraft(i, 10000)
	{
		rect.left = screenwidth / 2 - img.getwidth() / 2;
		rect.top = screenheight - img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = screenheight;
	}
	virtual void show()
	{
		//putimage(pMine[i]->x, pMine[i]->y, &img[0], SRCAND);
		//putimage(pMine[i]->x, pMine[i]->y, &img[1], SRCPAINT);
		setbkmode(TRANSPARENT);
		string s;
		s = to_string(HP);
		string str = "HP: " + s;
		settextcolor(BLACK);
		settextstyle(15, 0, "黑体");
		outtextxy(rect.left - (rect.right - rect.left) / 2 + textwidth(str.c_str()) / 2, rect.top - textheight(str.c_str()), str.c_str());
		putimage(rect.left, rect.top, &img);
	}
	void control()
	{
		ExMessage mess;
		if (peekmessage(&mess, EM_MOUSE))
		{
			rect.left = mess.x - img.getwidth() / 2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.left + img.getwidth();
			rect.bottom = rect.top + img.getheight();
			//cout << rect.left << endl << rect.right << endl << rect.bottom << endl << rect.top<<endl;
		}
	}
	void Shoot(vector<Bullet>& bullets, IMAGE& bulletimg)
	{
		RECT heroRect = GetRect();

		int bulletOffsetX[] = { -img.getwidth() / 4, 0, img.getwidth() / 4 };

		for (int i = 0; i < sizeof(bulletOffsetX) / sizeof(int); ++i)
		{
			RECT bulletRect;
			bulletRect.left = heroRect.left + (heroRect.right - heroRect.left) / 2 - bulletimg.getwidth() / 2 + bulletOffsetX[i];
			bulletRect.top = heroRect.top - bulletimg.getheight();
			bulletRect.right = bulletRect.left + bulletimg.getwidth();
			bulletRect.bottom = bulletRect.top + bulletimg.getheight();

			bullets.push_back(Bullet(bulletimg, bulletRect, Damege));
		}
	}
	void SetDamege(int D) { Damege = D; }
	int GetDamege() { return Damege; }
	//Hero死亡动画播放
	void HeroDead()
	{
		//播放死亡动画
		putimage(rect.left, rect.top, &PlayerBoom[0]);
		cout << "1" << endl;
		Sleep(100);
		putimage(rect.left, rect.top, &PlayerBoom[1]);
		cout << "2" << endl;
		Sleep(100);
		putimage(rect.left, rect.top, &PlayerBoom[2]);
		cout << "3" << endl;
		Sleep(100);
		putimage(rect.left, rect.top, &PlayerBoom[3]);
		cout << "4" << endl;
		Sleep(100);

	}
private:
	int Damege = 25;

};

class Enemy : public aircraft
{
public:
	Enemy(IMAGE& i) :aircraft(i) {}
	Enemy(IMAGE& i, int X) : aircraft(i, 500), boomsum(0)
	{
		rect.left = X;
		rect.top = -img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = rect.top + img.getheight();
		//putimage(rect.left, rect.top, &img);
	}
	// 在此处添加特定于Enemy的方法
	Enemy& operator=(const Enemy& other)
	{
		if (this != &other)
		{
			img = other.img;
			rect = other.rect;
			x = other.x;
			y = other.y;
			HP = other.HP;
		}
		return *this;
	}
	virtual bool Show()
	{
		if (is_die())
		{
			boomsum = (int)timer;
			//cout<<"boomsum:"<<boomsum<<endl;
			if (boomsum >= 4)
			{
				//cout << "!!!!!" << endl;
				boomsum = 0;
				timer = 0;
				//HP=500;



				return false;
			}
			putimage(rect.left, rect.top, &EnemyBoom[boomsum]);
			//cout<<"timer:"<<timer<<endl;
			timer += 0.1;
			return true;
		}
		else
		{
			//cout<<"HP:"<<HP<<endl;
			
			if (rect.top >= screenheight)
			{
				return false;
			}
			//rect.left = x;
			//rect.top = y;
			//rect.right = rect.left + img.getwidth();
			//rect.bottom = rect.top + img.getheight();
			rect.top += EnemySpeed;
			rect.bottom += EnemySpeed;
			x = rect.left;
			y = rect.top;
			putimage(rect.left, rect.top, &img);
			return true;
		}
	}
	virtual bool is_die()
	{
		if (HP <= 0)
		{
			//cout << "DIE" << endl;
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual void SetRect(int X, int Y)
	{
		rect.left = X;
		rect.top = Y;
		rect.right = rect.left + img.getwidth();
		rect.bottom = rect.top + img.getheight();
	}
	int GetFlag() { return flag; }
	void SetFlag(int f) { flag = f; }
	int GetX() { return x; }
	void SetX(int X) { x = X; }
	int GetY() { return y; }
	void SetY(int Y) { y = Y; }
	int Getboomsum() { return boomsum; }
	void Setboomsum(int b) { boomsum = b; }
	double GetTimer() { return timer; }
	void SetTimer(double t) { timer = t; }
	~Enemy()
	{
		//cout<<"flag:"<<flag<<endl;
		//cout<<"timer:"<<timer<<endl;
		//cout<<"boomsum:"<<boomsum<<endl;
	}
public:
	int flag = 0;
protected:
	int boomsum = 0;
	int x, y;
	double timer = 0;
};
class StrongEnemy :public aircraft
{
public:
	StrongEnemy(IMAGE& i) :aircraft(i) {}
	StrongEnemy(IMAGE& i, int x) :aircraft(i, 10000)
	{
		rect.left = x;
		rect.top = -img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = rect.top + img.getheight();
		offset = 3;
		//putimage(rect.left, rect.top, &img);
	}
	StrongEnemy& operator=(const StrongEnemy& other)
	{
		if (this != &other)
		{
			img = other.img;
			rect = other.rect;
			x = other.x;
			y = other.y;
			HP = other.HP;
		}
		return *this;
	}
	virtual bool Show()
	{
		if (aircraft::is_die())
		{
			boomsum = (int)timer;
			if (boomsum >= 4)
			{
				boomsum = 0;
				timer = 0;
				HP = 10000;

				return false;
			}
			putimage(rect.left, rect.top, &StrongEnemyBoom[boomsum]);
			timer += 0.1;
			return true;
		}
		else
		{
			if (rect.top >= screenheight)
			{
				return false;
			}
			rect.top = 0;
			rect.bottom = rect.top + img.getheight();
			x = rect.left;
			y = rect.top;
			
			putimage(rect.left, rect.top, &img);
			return true;
		}

	}
	void Shoot(vector<StrongEnemyBullet>& bullets, IMAGE& bulletimg)
	{
		RECT heroRect = GetRect();
		//cout << "StrongEnemyRectBullet:" << heroRect.top << " " << heroRect.bottom << " " << heroRect.left << " " << heroRect.right << endl;

		int bulletOffsetX[] = { -img.getwidth() / 4, 0, img.getwidth() / 4 };
		int j = -offset;
		for (int i = 0; i < sizeof(bulletOffsetX) / sizeof(int); ++i)
		{
			RECT bulletRect;
			bulletRect.left = heroRect.left + (heroRect.right - heroRect.left) / 2 - bulletimg.getwidth() / 2 + bulletOffsetX[i];
			bulletRect.top = heroRect.top - bulletimg.getheight();
			bulletRect.right = bulletRect.left + bulletimg.getwidth();
			bulletRect.bottom = bulletRect.top + bulletimg.getheight();
			//cout << "StrongBullet:" << bulletRect.top << " " << bulletRect.bottom << " " << bulletRect.left << " " << bulletRect.right << endl;


			bullets.push_back(StrongEnemyBullet(bulletimg, bulletRect, j));
			j += offset;
		}
		offset = rand() % 4;
	}
	virtual void SetRect(int x,int y)
	{
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + img.getwidth();
		rect.bottom = rect.top + img.getheight();
	}
	int GetFlag() { return flag; }
	void SetFlag(int f) { flag = f; }
	int GetX() { return x; }
	void SetX(int X) { x = X; }
	int GetY() { return y; }
	void SetY(int Y) { y = Y; }
	int Getboomsum() { return boomsum; }
	void Setboomsum(int b) { boomsum = b; }
	double GetTimer() { return timer; }
	void SetTimer(double t) { timer = t; }
	int GetOffset() { return offset; }
	void SetOffset(int o) { offset = o; }
public:
	int flag = 0;
protected:
	int boomsum = 0;
	int x, y;
	int offset;//偏移量
	double timer = 0;
};

//声明所有的函数
void LoadImg();
void Pause(vector<Enemy>& es, vector<StrongEnemy>& SE, vector<Bullet>& HeroBullet, vector<EnemyBullet>& EBullet, vector<StrongEnemyBullet>& SBullet,Hero&H,int&kill);
void EnemyKillShow(int EnemyKillCount);
bool Crash(const RECT& r1, const RECT& r2);
void welcome();
void SaveGame(vector<Enemy>& es, vector<StrongEnemy>& SE, vector<Bullet>& HeroBullet, vector<EnemyBullet>& EBullet, vector<StrongEnemyBullet>& SBullet,Hero&H,int&kill);
void LoadGame(vector<Enemy>& es, vector<StrongEnemy>& SE, vector<Bullet>& HeroBullet, vector<EnemyBullet>& EBullet, vector<StrongEnemyBullet>& SBullet,Hero&H,int&kill);
void LoadHero(Hero&H,int&kill);




void Pause(vector<Enemy>& es, vector<StrongEnemy>& SE, vector<Bullet>& HeroBullet, vector<EnemyBullet>& EBullet, vector<StrongEnemyBullet>& SBullet,Hero&H,int&kill)
{
	IMAGE img;
	loadimage(&img, "./images/pause_nor.png");
	putimage(screenwidth / 2-img.getwidth()/2, screenheight / 2-img.getheight()/2, &img);
	//居中显示文字：“按P继续游戏”
	settextcolor(BLACK);
	settextstyle(20, 0, "黑体");
	outtextxy(screenwidth / 2 - textwidth("按P继续游戏") / 2, screenheight / 2 + img.getheight() / 2, "按S继续游戏");
	while (1)
	{
		if (GetAsyncKeyState('Q'))
		{
			break;
		}
		//按下S存档游戏
		else if (GetAsyncKeyState('S'))
		{
			SaveGame(es,SE,HeroBullet,EBullet,SBullet,H,kill);
		}
		else if (GetAsyncKeyState('L'))
		{
			LoadGame(es, SE, HeroBullet, EBullet, SBullet,H,kill);
		}
	}
}

void SaveGame(vector<Enemy>& es, vector<StrongEnemy>& SE, vector<Bullet>& HeroBullet, vector<EnemyBullet>& EBullet, vector<StrongEnemyBullet>& SBullet,Hero&H,int&kill)
{
	//创建文件:Enemy 存储Enemy数据
	ofstream outputFile("./Enemy.txt");
	for ( vector<Enemy>::iterator i=es.begin();i!=es.end();i++)
	{
		outputFile <<i->GetFlag() << " "
			<< i->GetX() << " "
			<< i->GetY() << " "
			<< i->Getboomsum() << " "
			<< i->GetTimer() << " "
			<< i->GetHP() << "\n";
	}
	outputFile.close();
	//创建文件:StrongEnemy 存储StrongEnemy数据
	outputFile.open("./StrongEnemy.txt");
	for (vector<StrongEnemy>::iterator Senemy = SE.begin();  Senemy != SE.end(); Senemy++)
	{
		outputFile << Senemy->GetFlag() << " "
			<< Senemy->GetX() << " "
			<< Senemy->GetY() << " "
			<< Senemy->Getboomsum() << " "
			<< Senemy->GetTimer() <<" "
			<< Senemy->GetHP() <<" "
			<< Senemy->GetOffset() <<"\n";
	}
	outputFile.close();
	outputFile.open("./Hero.txt");
	outputFile << H.GetHP() << " " << kill;
	outputFile.close();

	//创建文件:Bullet 存储Bullet数据

	//创建文件:EnemyBullet 存储EnemyBullet数据

	//创建文件:StrongEnemyBullet 存储StrongEnemyBullet数据

}
void LoadStrongEnemy(vector<StrongEnemy>& SE)
{
	ifstream inputFile("./StrongEnemy.txt");
	int flag, x, y, boomsum, HP, offset;
	double	timer;
	while (inputFile >> flag >> x >> y >> boomsum >> timer >> HP >> offset)
	{
		StrongEnemy Senemy(StrongEnemyimg);
		Senemy.SetFlag(flag);
		Senemy.SetRect(x,y);
		Senemy.Setboomsum(boomsum);
		Senemy.SetTimer(timer);
		Senemy.SetHP(HP);
		Senemy.SetOffset(offset);
		SE.push_back(Senemy);
	}
	inputFile.close();
}
void LoadEnemy(vector<Enemy>& es)
{
	ifstream inputFile("./Enemy.txt");
	int flag, x, y, boomsum, HP;
	double	timer;
	while (inputFile >> flag >> x >> y >> boomsum >> timer >> HP)
	{
		Enemy enemy(enemyimg);
		enemy.SetFlag(flag);
		enemy.SetX(x);
		enemy.SetY(y);
		enemy.SetRect(x, y);
		enemy.Setboomsum(boomsum);
		enemy.SetTimer(timer);
		enemy.SetHP(HP);
		es.push_back(enemy);
	}
	inputFile.close();
}
void LoadHero(Hero& H,int&kill)
{
	ifstream inputFile("./Hero.txt");
	int HP;
	inputFile >> HP>>kill;
	H.SetHP(HP);
	inputFile.close();
}
void LoadGame(vector<Enemy>& es, vector<StrongEnemy>& SE, vector<Bullet>& HeroBullet, vector<EnemyBullet>& EBullet, vector<StrongEnemyBullet>& SBullet,Hero&H,int&kill)
{
	es.clear();
	SE.clear();
	HeroBullet.clear();
	EBullet.clear();
	SBullet.clear();

	LoadEnemy(es);
	LoadStrongEnemy(SE);
	LoadHero(H,kill);
	//LoadBullet(HeroBullet);
	//LoadEnemyBullet(EBullet);
	//LoadStrongEnemyBullet(SBullet);

}

void EnemyKillShow(int EnemyKillCount)
{
	setbkmode(TRANSPARENT);
	string s;
	s = to_string(EnemyKillCount);
	string str = "击杀数: " + s;
	settextcolor(BLACK);
	settextstyle(20, 0, "黑体");
	outtextxy(10, 10, str.c_str());
}


bool Crash(const RECT& r1, const RECT& r2) {
	return (r1.left < r2.right&& r1.right > r2.left && r1.bottom > r2.top && r1.top < r2.bottom);
}

//提示界面
void Tips()
{
	cleardevice();
	BeginBatchDraw();
	//加载Tips图片
	IMAGE Tipsimg;
	loadimage(&Tipsimg, "./images/Tips.png",screenwidth,413);
	putimage(0, 0, &Tipsimg);
	//显示返回文字

	settextcolor(BLACK);
	settextstyle(40, 0, "黑体");
	//判断鼠标是否点击返回
	LPCTSTR textBack = "返回";
	RECT Rback;
	Rback.left = screenwidth / 2 - textwidth(textBack) / 2;
	Rback.right = screenwidth / 2 + textwidth(textBack) / 2;
	Rback.top = screenheight / 2 + 200 - textheight(textBack) / 2;
	Rback.bottom = screenheight / 2 + 200 + textheight(textBack) / 2;
	outtextxy(Rback.left, Rback.top, textBack);
	FlushBatchDraw();
	EndBatchDraw();

	ExMessage me;
	getmessage(&me, EM_MOUSE);
	while (1)
	{
		if (me.lbutton)
		{
			if (is_Area(me.x, me.y, Rback))
			{
				return;
			}
		}
	}
	return;
}

void welcome()
{
	LPCTSTR Title = "飞机大战";
	LPCTSTR textPlay = "开始游戏";
	LPCTSTR textExit = "退出游戏";
	LPCTSTR textTips = "提示(长按查看)";

	RECT Rplay, Rexit,Rtips;
	//播放背景音乐
	PlayBGM();


	while (1)
	{
		BeginBatchDraw();

		setbkcolor(WHITE);
		cleardevice();
		settextstyle(60, 0, "黑体");
		settextcolor(BLACK);
		outtextxy(screenwidth / 2 - textwidth(Title) / 2, screenheight / 6, Title);

		settextstyle(40, 0, "楷体");

		Rtips.left = (screenwidth / 2 - textwidth(textPlay) / 2);
		Rtips.right = Rtips.left + textwidth(textTips);
		Rtips.top = screenheight / 2.5;
		Rtips.bottom = Rtips.top + textheight(textTips);

		Rplay.left = (screenwidth / 2 - textwidth(textPlay) / 2);
		Rplay.right = Rplay.left + textwidth(textPlay);
		Rplay.top = screenheight / 4;
		Rplay.bottom = screenheight / 4 + textheight(textPlay);

		Rexit.left = (screenwidth / 2 - textwidth(textExit) / 2);
		Rexit.right = Rplay.left + textwidth(textExit);
		Rexit.top = screenheight / 3;
		Rexit.bottom = Rexit.top + textheight(textExit);

		outtextxy(Rplay.left, Rplay.top, textPlay);
		outtextxy(Rexit.left, Rexit.top, textExit);
		outtextxy(Rtips.left, Rtips.top, textTips);
		EndBatchDraw();
		ExMessage me;
		getmessage(&me, EM_MOUSE);
		if (me.lbutton)
		{
			if (is_Area(me.x, me.y, Rplay)) {
				StopBGM();
				PlayBGM2();
				return;
			}
			else if (is_Area(me.x, me.y, Rexit)) {
				exit(0);
			}
			else if (is_Area(me.x, me.y, Rtips)) {
				Tips();
			}
		}
	}

	
}




 

bool addenemy;





void timefunc()
{
	addenemy = true;
}
bool play()
{
	
	int EnemyKillCount = 0;
	int StrongCount = 0;
	int flag = 0;
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = true;


	LoadImg();

	BK backgrand(bkimg);
	Hero H(heroimg);

	clock_t lastEnemyTime = clock();
	clock_t BulletLastEnemyTime = clock();
	clock_t EnemyBulletLastEnemyTime = clock();
	clock_t StrongBulletEnemyLastTime = clock();

	vector<Enemy> es;
	vector<StrongEnemy> SE;
	vector<Bullet> HeroBullet;
	vector<EnemyBullet> EBullet;
	vector<StrongEnemyBullet> SBullet;
	//for (int i = 0; i < 4; i++)
	//{
	//	es.push_back(Enemy(enemyimg, abs(rand() % (screenwidth - enemyimg.getheight()))));
	//}

	while (is_play)
	{
		Sleep(3);
		BeginBatchDraw();
		if (GetAsyncKeyState('P'))
		{
			FlushBatchDraw();
			Pause(es,SE,HeroBullet,EBullet,SBullet,H,EnemyKillCount);
		}
		EndBatchDraw();
		BeginBatchDraw();
		Sleep(2);
		backgrand.show();
		//H.control();
		//H.show();


		clock_t currentTime = clock();
		double elapsedTime = static_cast<double>(currentTime - lastEnemyTime) / CLOCKS_PER_SEC;

		//if (elapsedTime >= 1&&es.size()<6)  // 每秒生成一个敌人
		if (elapsedTime >= 1&&es.size()<6)  // 每秒生成一个敌人
		{
			es.push_back(Enemy(enemyimg, abs(rand() % (screenwidth - enemyimg.getheight()))));
			num++;
			cout << es.size() << endl;
			cout<<"num:"<<num<<endl;
			lastEnemyTime = currentTime;
		}


		//cout << "ES:" << es.size() << endl;


		clock_t BulletCurrentTime = clock();
		double BulletElapsedTime = static_cast<double>(BulletCurrentTime - BulletLastEnemyTime) / CLOCKS_PER_SEC;
		
		if (BulletElapsedTime >= 0.05)
		{
			if (EnemyKillCount >= 15)
			{
				H.Shoot(HeroBullet, bulletimg);
				H.SetDamege(25+ EnemyKillCount%3);
				BulletLastEnemyTime = BulletCurrentTime;
			}
			else
			{
				HeroBullet.push_back(Bullet(bulletimg, H.GetRect(),H.GetDamege()));
				BulletLastEnemyTime = BulletCurrentTime;
			}
		}


		clock_t EnemycurrentTime = clock();
		double EnemyelapsedTime = static_cast<double>(EnemycurrentTime - EnemyBulletLastEnemyTime) / CLOCKS_PER_SEC;
		auto Ebit = es.begin();
		while (Ebit != es.end()&& EnemyelapsedTime >= 1)
		{
			EBullet.push_back(EnemyBullet(bulletimg, Ebit->GetRect()));
			EnemyBulletLastEnemyTime = EnemycurrentTime;
			Ebit++;
		}


		clock_t StrongEnemycurrentTime = clock();
		double StrongEnemyelapsedTime = static_cast<double>(StrongEnemycurrentTime - StrongBulletEnemyLastTime) / CLOCKS_PER_SEC;
		auto SEit = SE.begin();
		while (SEit != SE.end() && StrongEnemyelapsedTime >= 0.5)
		{
			SEit->Shoot(SBullet, bulletimg);
			StrongBulletEnemyLastTime = StrongEnemycurrentTime;
			SEit++;
		}
		if (StrongCount >= 15)
		{
			H.SetDamege(25+ EnemyKillCount % 3);
			SE.push_back(StrongEnemy(StrongEnemyimg, abs(rand() % (screenwidth-StrongEnemyimg.getwidth()) - StrongEnemyimg.getwidth())));
			StrongCount = 0;
		}


		auto it = es.begin();
		while (it!=es.end())
		{
			if (Crash(it->GetRect(), H.GetRect()))//Hero与敌机撞击判定
			{
				is_play=false;
				EndBatchDraw();
				H.HeroDead();
				break;

				//EnemyKillShow(EnemyKillCount);
			}
			//cout << "0" << endl;
			auto bit= HeroBullet.begin();//Hero子弹与敌机的撞击判定
			while (bit!=HeroBullet.end()&& it != es.end())
			{
				//cout << "0.4" << endl;
				if (Crash(bit->GetRect(), it->GetRect())&&!it->is_die())
				{
					//cout << "0.5" << endl;
					it->Attacked(bit->GetDamege());
					bit = HeroBullet.erase(bit);
					cout << "HP:" << it->GetHP() << endl;

					//if (it->is_die()&&it->flag==0)
					if (it->is_die())
					{
						it->flag = 1;
						EnemyKillCount++;
						StrongCount++;
						break;
						//it = es.erase(it);
					}
				}
				else { bit++; }
			}
			//剩余敌机show
			if (it != es.end() && !(*it).Show())
			{
				cout<<"flag: "<<it->flag<<"  Erased!"<<endl;
				it = es.erase(it);

			}
			if (it != es.end()) {

				it++;
			}
		}
		if (!is_play) { break; }




		//BOSS
		auto Sit = SE.begin();
		while (Sit != SE.end())
		{
			if (Crash(Sit->GetRect(), H.GetRect()))//Hero与BOSS撞击判定
			{
				is_play = false;
				EndBatchDraw();
				H.HeroDead();
				break;

			}

			auto bit = HeroBullet.begin();//Hero子弹与BOSS的撞击判定
			while (bit != HeroBullet.end() && Sit != SE.end())
			{

				if (Crash(bit->GetRect(), Sit->GetRect()))
				{

					Sit->Attacked(bit->GetDamege());
					bit = HeroBullet.erase(bit);
					if (Sit->is_die()&&Sit->flag==0)
					{
						Sit->flag = 1;
						EnemyKillCount++;
						StrongCount++;
					}
				}
				else { bit++; }

			}

			if (!is_play) { break; }
			//剩余敌机show
			if (Sit != SE.end() && !(*Sit).Show())
			{
				
				Sit = SE.erase(Sit);
			}
			if (Sit != SE.end()) {

				Sit++;
			}
		}

		//Hero受击判定
		auto EBit = EBullet.begin();
		while (EBit !=EBullet.end())
		{
			if (Crash(EBit->GetRect(), H.GetRect()))
			{
				H.Attacked(EBit->GetDamege());
				if (H.is_die())
				{
					is_play = false;
					EndBatchDraw();
					H.HeroDead();
					break;
				}
				EBit = EBullet.erase(EBit);
			}
			if (EBit != EBullet.end())
			{
				EBit++;
			}
		}
		if (!is_play) { break; }
		auto SBit = SBullet.begin();
		while (SBit != SBullet.end())
		{
			if (Crash(SBit->GetRect(), H.GetRect()))
			{
				H.Attacked(SBit->GetDamege());
				if (H.is_die())
				{
					is_play = false;
					break;
				}
				SBit = SBullet.erase(SBit);
			}
			if (SBit != SBullet.end())
			{
				SBit++;
			}
		}

		H.control();
		H.show();


		//Hero子弹发射与边界判定
		auto buit = HeroBullet.begin();
		while (buit!=HeroBullet.end())
		{
			if (!(*buit).Show())
			{
				buit = HeroBullet.erase(buit);
			}
			if (buit != HeroBullet.end())
				buit++;

		}
		
		//Enemy子弹发射与边界判定
		EBit = EBullet.begin();
		while (EBit != EBullet.end())
		{
			if (!(*EBit).Show())
			{
				EBit = EBullet.erase(EBit);
			}
			if (EBit != EBullet.end())
			{
				EBit++;
			}
		}

		SBit = SBullet.begin();
		while (SBit != SBullet.end())
		{
			if (!(*SBit).Show())
			{
				SBit = SBullet.erase(SBit);
			}
			if (SBit != SBullet.end())
			{
				SBit++;
			}
		}

		EnemyKillShow(EnemyKillCount);


		flushmessage();


		EndBatchDraw();
		//弹药升级
		if (EnemyKillCount == 15&&flag==0)
		{
			LPCTSTR pow = "POWER UP!!";
			settextstyle(60, 0, "黑体");
			settextcolor(RGB(220, 20, 60));
			outtextxy(screenwidth / 2 - textwidth(pow) / 2, 300, pow);
			Sleep(1000);
			flag++;
		}
	}

	//结束画面
	LPCTSTR over = "GAME OVER!";
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, "黑体");
	settextcolor(BLACK);
	outtextxy(screenwidth / 2 - textwidth(over) / 2, 300, over);
	Sleep(2000);

	return true;
}

int main()
{
	initgraph(screenwidth, screenheight, EW_NOMINIMIZE | SHOWCONSOLE);
	bool is_run = true;
	while (is_run)
	{
		welcome();
		is_run=play();
		StopBGM2();
	}
	return 0;
}