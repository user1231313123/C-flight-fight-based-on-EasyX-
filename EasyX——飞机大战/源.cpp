#define _CRT_SECURE_NO_WARNINGS
#define EnemySpeed 2
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<vector>
#include<iostream>
#include<ctime>
#include<string>
using namespace std;

const auto screenwidth = 600;
const auto screenheight = 1200;
bool is_Area(int x, int y, RECT& r)
{
	return (r.left <= x && r.right >= x && r.top <= y && y <= r.bottom);
}
//void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color)
//{
//	DWORD* src = GetImageBuffer(img);
//	DWORD* dst = GetImageBuffer(GetWorkingImage());
//	int w1 = img->getwidth(), h1 = img->getheight(), w2;
//	if (GetWorkingImage() == NULL)
//		w2 = getwidth();
//	else
//		w2 = GetWorkingImage()->getwidth();
//	for (int i = 0; i < w1; i++)
//		for (int j = 0; j < h1; j++)
//		{
//			if (src[j * w1 + i] != color) //显存中像素角标为y*w+x
//				dst[(j + dsty) * w2 + i + dstx] = src[j * w1 + i];
//		}
//	if (!GetWorkingImage()) //如果目标是窗口
//		FlushBatchDraw();
//}

//bool Crash(RECT &r1,RECT&r2)
//{
//	RECT r;
//	r.left = r1.left - (r2.right - r2.left);
//	r.right = r1.right;
//	r.top = r1.top - (r2.bottom - r2.top);
//	r.bottom = r1.bottom;
//	return (r.left<r2.left&& r.right>r2.left && r.bottom > r2.top && r.top < r2.bottom);
//}

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

void welcome()
{
	LPCTSTR Title = "飞机大战";
	LPCTSTR textPlay = "开始游戏";
	LPCTSTR textExit = "退出游戏";

	RECT Rplay, Rexit;

	BeginBatchDraw();

	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, "黑体");
	settextcolor(BLACK);
	outtextxy(screenwidth / 2 - textwidth(Title) / 2, screenheight / 6, Title);
	
	settextstyle(40, 0, "楷体");

	Rplay.left = (screenwidth / 2 - textwidth(textPlay) / 2);
	Rplay.right = Rplay.left + textwidth(textPlay);
	Rplay.top = screenheight / 4;
	Rplay.bottom = screenheight / 4+textheight(textPlay);

	Rexit.left = (screenwidth / 2 - textwidth(textExit) / 2);
	Rexit.right = Rplay.left + textwidth(textExit);
	Rexit.top = screenheight / 3;
	Rexit.bottom = Rexit.top + textheight(textExit);

	outtextxy(Rplay.left, Rplay.top, textPlay);
	outtextxy(Rexit.left, Rexit.top, textExit);
	EndBatchDraw();
	while (1)
	{
		ExMessage me;
		getmessage(&me, EM_MOUSE);
		if (me.lbutton)
		{
			if (is_Area(me.x, me.y, Rplay))
				return;
			else if (is_Area(me.x, me.y, Rexit))
				exit(0);
		}
	}

	
}



class Bullet
{
public:
	Bullet(IMAGE& img, RECT pr)
		:img(img),Damege(25)
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
		rect.top -= 9;
		rect.bottom -= 9;
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
	EnemyBullet(IMAGE& img, RECT pr) :Bullet(img, pr){}
	virtual bool Show()
	{
		if (rect.top >= screenheight)
		{
			return false;
		}
		rect.top += 5;
		rect.bottom += 5;
		putimage(rect.left, rect.top, &img);

		return true;
	}
	EnemyBullet& operator=(const EnemyBullet & other)
	{
		if (this != &other)
		{
			Bullet::operator=(other);
		}
		return *this;
	}

};

class BK
{
public:
	BK(IMAGE& i) :img(i),h(-screenheight) {}
	~BK(){}
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

};


class Hero
{
public:
	Hero(IMAGE& i) :img(i),HP(400)
	{ 
		rect.left = screenwidth / 2 - img.getwidth() / 2;
		rect.top = screenheight - img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = screenheight;
	}
	void show()
	{
		//putimage(pMine[i]->x, pMine[i]->y, &img[0], SRCAND);
		//putimage(pMine[i]->x, pMine[i]->y, &img[1], SRCPAINT);
		setbkmode(TRANSPARENT);
		string s;
		s = to_string(HP);
		string str = "HP: " + s;
		settextcolor(BLACK);
		settextstyle(20, 0, "黑体");
		outtextxy(rect.left - (rect.right - rect.left) / 2 + textwidth(str.c_str()),rect.top-textheight(str.c_str()), str.c_str());
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
	RECT& GetRect()
	{
		return rect;
	}
	~Hero(){}

	int GetHP() { return HP; }
	void SetHP(int x) { HP = x; }
	bool is_die() { return HP <= 0; }
	void Attacked(int Damege) { HP -= Damege; }

private:
	int HP;
	IMAGE& img; 
	RECT rect;
};



class Enemy
{
public:
	Enemy(IMAGE&i,int X):img(i),HP(100)
	{
		rect.left = X;
		rect.top = -img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = rect.top + img.getheight();
		putimage(rect.left, rect.top, &img);
	}
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
	int GetHP() { return HP; }
	void SetHP(int x) { HP = x; }
	bool is_die() { return HP <= 0; }
	void Attacked(int Damege) { HP -= Damege; }
	RECT& GetRect()
	{
		return rect;
	}
	bool Show()
	{
		if (rect.top >= screenheight)
		{
			return false;
		}
		rect.top += EnemySpeed;
		rect.bottom += EnemySpeed;
		putimage(rect.left, rect.top, &img);
		return true;
	}
	~Enemy(){}

private:
	int HP;
	IMAGE& img;
	RECT rect;
	int x, y;
};


bool addenemy;

void timefunc()
{
	addenemy = true;
}
bool play()
{
	int EnemyKillCount = 0;
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = true;
	IMAGE heroimg, enemyimg, bkimg, bulletimg;
	loadimage(&heroimg, "./images/me1.png");
	loadimage(&enemyimg,"./images/enemy1.png");
	loadimage(&bkimg,"./images/bk2.png",600,2400); 
	loadimage(&bulletimg,"./images/bullet1.png");

	BK backgrand(bkimg);
	Hero H(heroimg);

	clock_t lastEnemyTime = clock();
	clock_t BulletLastEnemyTime = clock();
	clock_t EnemyBulletLastEnemyTime = clock();

	vector<Enemy> es;
	vector<Bullet> HeroBullet;
	vector<EnemyBullet> EBullet;
	//for (int i = 0; i < 4; i++)
	//{
	//	es.push_back(Enemy(enemyimg, abs(rand() % (screenwidth - enemyimg.getheight()))));
	//}

	while (is_play)
	{
		BeginBatchDraw();
		Sleep(2);
		backgrand.show();
		//H.control();
		//H.show();

		clock_t currentTime = clock();
		double elapsedTime = static_cast<double>(currentTime - lastEnemyTime) / CLOCKS_PER_SEC;

		if (elapsedTime >= 1.5&&es.size()<6)  // 每秒生成一个敌人
		{
			es.push_back(Enemy(enemyimg, abs(rand() % (screenwidth - enemyimg.getheight()))));
			lastEnemyTime = currentTime;
		}





		clock_t BulletCurrentTime = clock();
		double BulletElapsedTime = static_cast<double>(BulletCurrentTime - BulletLastEnemyTime) / CLOCKS_PER_SEC;
		
		if (BulletElapsedTime >= 0.05)
		{
			HeroBullet.push_back(Bullet(bulletimg, H.GetRect()));
			BulletLastEnemyTime = BulletCurrentTime;
		}


		clock_t EnemycurrentTime = clock();
		double EnemyelapsedTime = static_cast<double>(EnemycurrentTime - EnemyBulletLastEnemyTime) / CLOCKS_PER_SEC;
		auto Ebit = es.begin();
		while (Ebit != es.end()&& EnemyelapsedTime >= 1.3)
		{
			EBullet.push_back(EnemyBullet(bulletimg, Ebit->GetRect()));
			EnemyBulletLastEnemyTime = EnemycurrentTime;
			Ebit++;
		}


		auto it = es.begin();
		while (it!=es.end())
		{
			if (Crash(it->GetRect(), H.GetRect()))//Hero与敌机撞击判定
			{
				is_play=false;
				//EnemyKillShow(EnemyKillCount);
			}
			cout << "0" << endl;
			auto bit= HeroBullet.begin();//Hero子弹与敌机的撞击判定
			while (bit!=HeroBullet.end()&& it != es.end())
			{
				cout << "0.4" << endl;
				if (Crash(bit->GetRect(), it->GetRect()))
				{
					cout << "0.5" << endl;
					it->Attacked(bit->GetDamege());
					bit = HeroBullet.erase(bit);
					if (it->is_die())
					{
						cout << "0.6" << endl;
						EnemyKillCount++;
						it = es.erase(it);
					}
					cout << "0.6" << endl;
				}
				else { bit++; }
				cout << "2.4" << endl;
			}
			cout << "2.5" << endl;

			//剩余敌机show
			if (it != es.end() && !(*it).Show())
			{
				cout << "3" << endl;
				it = es.erase(it);
			}
			if (it != es.end()) {
				cout << "4" << endl;
				it++;
			}
		}

		//Hero受击判定
		auto EBit = EBullet.begin();
		while (EBit !=EBullet.end())
		{
			if (Crash(EBit->GetRect(), H.GetRect()))//Hero与敌机撞击判定
			{
				H.Attacked(EBit->GetDamege());
				if (H.is_die())
				{
					is_play = false;
					break;
				}
				EBit = EBullet.erase(EBit);
			}
			if (EBit != EBullet.end())
			{
				EBit++;
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
				cout << "5" << endl;
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

		EnemyKillShow(EnemyKillCount);


		flushmessage();


		EndBatchDraw();
	}

	//结束画面
	LPCTSTR over = "GAME OVER!";
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, "黑体");
	settextcolor(BLACK);
	outtextxy(screenwidth / 2 - textwidth(over) / 2, 300, over);
	Sleep(3000);
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
	}
}