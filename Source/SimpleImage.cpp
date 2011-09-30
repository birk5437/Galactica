#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include "dxtools.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <math.h>
#include <string>
#include <strstream>
#include <sstream>
#include <fstream>

using namespace std;

int click = 1;

int soundqwe = 1;

int levelCounter = -1;

int volume;

bool checkedScore = false;

bool dispHighs = true;

string hpString, levelString, shieldsUpStr, song, nameStr;

int superWeapon = 3;

int high;

int gameOverTimer = 0;

int collisionCounter = 40;

int guns = 1;

bool gameOver = false;

int nameLength = 0;

int lastInputTaken = 0;

bool inputScore = true;

bool gotHighs = false;

int hitPoints = 100;
int level = 1;

bool cheat = false;
int fireDelay = 200;

int score = 0;

int updateDelay = 0;

int lettersEntered = 0;

vector<string> name;

D3DXVECTOR2 input_translation, image_translation;



LPDIRECT3DTEXTURE8 explosion[20];


LPDIRECT3D8             pD3D = NULL;
LPDIRECT3DDEVICE8       pD3DDevice = NULL;
LPD3DXSPRITE			p3DXSprite = NULL;
D3DPRESENT_PARAMETERS	d3dpp;

ifstream f;
ofstream o;
RECT lev;
RECT hi;
RECT sup;
RECT scr;
RECT char_display;

LPD3DXFONT font;

RECT hitP;

string gameOverStr = "Game Over:  F12 to exit, spacebar to restart";
CDInput dinput;
CDMusic dmusic;
CDShow dshow;
CDMusic dmusic3;

CSound sound, Expl;

LPDIRECT3DTEXTURE8		pTexture, pTexture2, pTexture3, blank, bg, pTexture9, input_line, new_high, enter_name, highScoresImg, white_box = NULL;

D3DXVECTOR2 bgt;

/*
A DirectGraphics program that loads and displays image files

To compile:  Either use the workspace provided online, or add the following libraries to your own
project's build settings (Project->Settings, "Link" tab): 

winmm.lib d3d8.lib d3dx8.lib
*/


class whiteBox
{
public:

	D3DXVECTOR2 translation;
	int xChange;
	int yChange;

	whiteBox(int x, int y, int xc, int yc)
	{
		translation.x = x;
		translation.y = y;

		xChange = xc;
		yChange = yc;
	}

	void updatePos()
	{
		if (translation.x >= 1024 || translation.x <= 0)
			xChange *= -1;

		if (translation.y >= 768 || translation.y <= 0)
			yChange *= -1;

		translation.x += xChange;
		translation.y += yChange;
	}

	void disp(LPD3DXSPRITE sprite)
	{
		sprite->Draw(white_box, NULL, NULL, NULL, 
			0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
};

list<whiteBox> boxes;





class explode
{
public:

	int xPos, yPos, frame;
	int delayTimer;

	D3DXVECTOR2 translation, scale;

	explode(int x, int y, int scaleX, int scaleY)
	{
		frame = 1;
		xPos = x;
		yPos = y;
		translation.x = xPos - 64;
		translation.y = yPos - 64;
		delayTimer = 0;
		scale.x = scaleX;
		scale.y = scaleY;

	}

	
	void disp(LPD3DXSPRITE sprite)
	{
		delayTimer++;
		sprite->Draw(explosion[frame], NULL, &scale, NULL, 
			0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));

		if((delayTimer % 13) == 0)
			frame++;
	}



};

list<explode> explodes;






		



class eMissle
{
public:

	D3DXVECTOR2 translation;
	int yMove;

	eMissle(int xPos, int yPos)
	{
		translation.x = xPos;
		yMove = 8;
		translation.y = yPos;
	}

	void disp(LPDIRECT3DTEXTURE8 texture, LPD3DXSPRITE sprite)
	{
		translation.y += yMove;
		
		sprite->Draw(texture, NULL, NULL, NULL, 
			0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));

	}



};

list<eMissle> eMissles;





class enemy2
{
public:
	
	D3DXVECTOR2 translation;
	int xPos, yPos, xMove, yMove, hit;
	bool attack;

	enemy2()
	{
		
		attack = false;
		hit = 4;
		xMove = rand()%5 + 1;
		yMove = 0;
		xPos = rand()%953;
		yPos = rand()%80 + 20;
		translation.x = xPos;
		translation.y = yPos;
	}


	void disp(LPDIRECT3DTEXTURE8 texture, LPD3DXSPRITE sprite)
	{
		
		if(rand()%400 == 25 && attack == false)
		{
			attack = true;
			yMove += rand()%3 + 1;
		}
		
		xPos += xMove;
		translation.x = xPos;
		yPos += yMove;
		translation.y = yPos;

		if (xPos <= 0 || xPos >= 954)
			xMove *= -1;
		
		sprite->Draw(texture, NULL, NULL, NULL, 
			0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	void fire()
	{
		eMissle eMissle1(xPos + 16, yPos + 35);
		eMissles.push_back(eMissle1);
		eMissle eMissleTwo(xPos + 48, yPos + 35);
		eMissles.push_back(eMissleTwo);
	}
};

list<enemy2> enemy2s;






class enemy
{
public:

	int xPos, yPos, xMove, yMove, hit;
	bool attack;

	D3DXVECTOR2 translation;

	enemy()
	{
		attack = false;
		hit = 2;
		xMove = rand()%5 + 1;
		yMove = 0;
		xPos = rand()%953;
		yPos = rand()%80 + 20;
		translation.x = xPos;
		translation.y = yPos;
	}

	void disp(LPDIRECT3DTEXTURE8 texture, LPD3DXSPRITE sprite)
	{
		
		if(rand()%500 == 25 && attack == false)
		{
			attack = true;
			yMove += rand()%3 + 1;
		}
		
		xPos += xMove;
		translation.x = xPos;
		yPos += yMove;
		translation.y = yPos;

		if (xPos <= 0 || xPos >= 954)
			xMove *= -1;
		
		sprite->Draw(texture, NULL, NULL, NULL, 
			0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	
	void fire()
	{
		eMissle eMissle(xPos + 35, yPos + 64);
		eMissles.push_back(eMissle);
	}

};

list<enemy> enemies;




class ship
{
public:


	bool visible;
	int xPos;
	
	D3DXVECTOR2 translation;

	ship()
	{
		xPos = 512;
		translation.y = 650;
		visible = true;
	}

	void disp(LPDIRECT3DTEXTURE8 texture, LPD3DXSPRITE sprite)
	{
		if (visible)
		{
			translation.x = xPos;
			sprite->Draw(texture, NULL, NULL, NULL, 
				0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		else
		{
			sprite->Draw(blank, NULL, NULL, NULL, 0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}


	void destroy()
	{
		explode explode1(translation.x, translation.y, 1, 1);
		explodes.push_back(explode1);
		dmusic3.PlaySound(&Expl, 0);
		visible = false;
	}
};





ship ship1;




class missle
{
public:

	D3DXVECTOR2 translation;
	int xPos, yPos, yMove;
	

	missle(int x, int y)
	{
		
		xPos = x;     //ship1.xPos + 32;
		yPos = y;     //ship1.translation.y;
		yMove = -8;
		translation.x = xPos;
		translation.y = yPos;
	}

	void disp(LPDIRECT3DTEXTURE8 texture, LPD3DXSPRITE sprite)
	{
		
		yPos += yMove;
		translation.y = yPos;

		sprite->Draw(texture, NULL, NULL, NULL, 
			0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
};


list<missle> missles;



struct hScore
{
	string fName, lName;
	int score;
};

vector<hScore>hScores;

void getHighs()
{
	string str1;
	int i;
	ifstream in;
	in.open("highs.dat");
	logthis("opened highs.dat...entering read for loop");

	for (int hs = 0; hs < 10; hs++)
	{
		hScore h;
		
		in >> str1;
		h.fName = str1;
		logthis("assigned fName");
		in >> str1;
		h.lName = str1;
		in >> i;
		h.score = i;
		
		hScores.push_back(h);
	}

}







void setHighs(vector<string> vec, int scoreNumber)
{
	ifstream in;
	ofstream out;

	string fName[10];
	string lName[10];
	int s[10];

	bool hasSpace = false;

	in.open("temp.txt");
	out.open("highs.dat");

	for (int fCounter = 0; fCounter < 10; fCounter++)
	{
		in >> fName[fCounter];
		in >> lName[fCounter];
		in >> s[fCounter];
	}
	in.close();

	if (scoreNumber > 0)
	{

		for (fCounter = 0; fCounter < scoreNumber; fCounter++)
		{
			out << fName[fCounter];
			out << " ";
			out << lName[fCounter];
			out << " " << s[fCounter] << endl;
		}
	}

	for (int vCounter = 0; vCounter < vec.size(); vCounter++)
	{
		if (vec[vCounter] == " ")
			hasSpace = true;

		out << vec[vCounter];
	}

	if (!hasSpace)
	{
		out << " " << "*~&";
	}

	out << " " << score << endl;

	for (fCounter = scoreNumber; fCounter < 9; fCounter++)
	{
		out << fName[fCounter] << " ";
		out << lName[fCounter] << " ";
		out << s[fCounter] << endl;
	}

	out.close();


}





int checkHigh(int score)
{
	ifstream in;
	ofstream out;
	in.open("highs.dat");
	out.open("temp.txt");
	string name;
	int pScore = 0;
	int highNum = -1;
	bool gotHigh = false;

	logthis("checking highs");

	for (int fileCounter = 0; fileCounter < 10; fileCounter++)
	{
		in >> name;
		out << name << " ";
		in >> name;
		out << name << " ";
		in >> pScore;
		out << pScore << endl;
		if (pScore < score && gotHigh == false)
		{
			logthis("file counter:");
			logthis(fileCounter);
			highNum = fileCounter;
			gotHigh = true;
		}
	}

	in.close();
	out.close();

	logthis(pScore);
	
	return highNum;
}




void resetGame()
{
	level = 1;
	
	superWeapon = 3;

	guns = 1;

	score = 0;

	cheat = false;

	enemies.clear();
	missles.clear();
	eMissles.clear();
	enemy2s.clear();
	for (int r = 0; r < 2; r++)
	{

		{
			enemy enemy1;
			enemies.push_back(enemy1);
		}
	}
	

	hitPoints = 100;
	gameOver = false;
}



void ReleaseAll()
{
	if(p3DXSprite != NULL)
		p3DXSprite->Release();

	if(pD3DDevice != NULL)
		p3DXSprite->Release();

	if(pD3D != NULL)
		pD3D->Release();

	if(pTexture != NULL)
		pTexture->Release();

	if(pTexture2 != NULL)
		pTexture2->Release();

	if(pTexture3 != NULL)
		pTexture3->Release();


	f.close();
	
	dmusic.Release();
	dshow.Release();

	dinput.Release();

	CoUninitialize();
}

long WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SETCURSOR:
		SetCursor(NULL);
		return true;

	case WM_KEYDOWN:
		
		switch ( wParam )
		{
		case VK_F12:
			//ReleaseAll();
			PostMessage( hWnd, WM_CLOSE, 0, 0 );
			break;


		case VK_LEFT:
			ship1.xPos-=8;
			break;
		case VK_RIGHT:
			ship1.xPos+=8;
			break;

		/*case VK_SPACE:
			
			if (gameOver)
				resetGame();
			
			break;*/


		case VK_F1:

			cheat = true;

			if (fireDelay == 0)
				fireDelay = 200;
			else
				fireDelay = 0;

			break;

		case VK_TAB:
			superWeapon++;
			hitPoints -= 25;
			break;


		case VK_F3:
			cheat = true;
			if (guns == 1)
				guns = 2;
			else if (guns == 2)
				guns = 3;
			else if (guns == 3)
				guns = 4;
			else if (guns == 4)
				guns = 5;
			else if (guns == 5)
				guns = 1;
			
			score = 0;

			break;

		case VK_F4:
			cheat = true;
			hitPoints += 10;
			break;
			
		case VK_F5:
			cheat = true;
			superWeapon += 5;
			break;

		
		case VK_F2:
			enemy2 enemyTwo;
			enemy2s.push_back(enemyTwo);
			break;



		
			
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SetUpDirectGraphics(int xResolution, int yResolution, HWND hwnd)
{
	pD3D = NULL;
	pD3DDevice = NULL;

	logthis(D3D_SDK_VERSION);

	pD3D = Direct3DCreate8(D3D_SDK_VERSION);
	if(pD3D == NULL)
	{
		logthis("Failed to initialize the direct3d object - make sure this system has a compatible version of DirectX");
		return false;
	}
	logthis("Created the direct3D object");

	D3DDISPLAYMODE d3ddm;
	if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		logthis("Failed to retrieve the adapter display mode");
		return false;
	}
	logthis("Retrieved the adapter display settings");

	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.hDeviceWindow    = hwnd;
    d3dpp.BackBufferWidth  = xResolution;
    d3dpp.BackBufferHeight = yResolution;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT ret = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &d3dpp, &pD3DDevice);
	
	if( FAILED( ret ) )
	{
		logthis("failed to create the direct3d device - check your video card settings.");
		
		switch(ret)
		{
		case D3DERR_INVALIDCALL:
			logthis("D3DERR_INVALIDCALL");
			break;

		case D3DERR_NOTAVAILABLE:
			logthis("D3DERR_NOTAVAILABLE");
			break;

		case D3DERR_OUTOFVIDEOMEMORY:
			logthis("D3DERR_OUTOFVIDEOMEMORY");
			break;

		default:
			logthis("D3DERR_WTF");
			break;
		}
		
		return false;
	}
	logthis("Created the direct3d device.");

	if( FAILED( D3DXCreateSprite(pD3DDevice, &p3DXSprite) ))
	{
		logthis("Failed to create the d3dx sprite interface.");
		return false;
	}
	logthis("Created the d3dx sprite interface");

	return true;
}

void BeginDrawing(int clearR, int clearG, int clearB)
{
	pD3DDevice->BeginScene();
	p3DXSprite->Begin();

	pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(clearR, clearG, clearB),
		1.0f, 0 );
}

void EndDrawing()
{
	p3DXSprite->End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );
	pD3DDevice->EndScene();
}








void Update()
{
	
	



	if (cheat)
		score = 0;
	
	ostrstream oss;
	ostrstream oss2;
	ostrstream oss3;
	ostrstream oss4;
	string laserStr;
	ostrstream oss5;
	string scoreStr;
	ostrstream oss6;
	string highScore;
	
	ostrstream oss7;
	string gunsUpStr;

	BeginDrawing(0, 0, 0);

	if (gameOverTimer < 200)
	{


		lev.top = 382;
		lev.left = 500;

		if (gameOver)
		{
			gameOverTimer++;
			
			if (ship1.visible)
				ship1.destroy();

			font->DrawTextA(gameOverStr.c_str(), 44, &lev, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
		}

		
		static int lastFireTime = timeGetTime();

	
		oss7 << "Lasers Upgraded." << "   ";
		gunsUpStr = oss7.str();

		oss6 << "High: " << high << "    ";
		highScore = oss6.str();


		oss5 << "Score: " << score << "     ";
		scoreStr = oss5.str();

		oss4 << "Hyper Lasers: " << superWeapon << "     ";
		laserStr = oss4.str();


		oss2 << "Level: " << level << "    ";
		levelString = oss2.str();

		oss3 << "Shields +5%" << 0;
		shieldsUpStr = oss3.str();

		oss << "Shields: " << hitPoints << "%   ";
		hpString = oss.str();

		
		sup.top = 0;
		sup.left = 300;

		scr.top = 0;
		scr.left = 600;

		hi.top = 0;
		hi.left = 800;

		hitP.top = 0;
		hitP.left = 0;
		hitP.right = 500;

		


		font->DrawTextA(hpString.c_str(), 13, &hitP, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
		font->DrawTextA(laserStr.c_str(), 17, &sup, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
		font->DrawTextA(scoreStr.c_str(), 11, &scr, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
		font->DrawTextA(highScore.c_str(), 10, &hi, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));

		dinput.UpdateMouse();

		if (ship1.xPos < -70)
			ship1.xPos = -70;
		
		else if (ship1.translation.y > 704)
			ship1.translation.y = 704;

		else if (ship1.xPos > 1088)
			ship1.xPos = 1088;

		else
		{
			ship1.xPos += dinput.GetMouseXDif();
			ship1.translation.y += dinput.GetMouseYDif();
		}
	

	
		p3DXSprite->Draw(bg, NULL, NULL, NULL, 
			0, &bgt, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	
		if(dinput.MouseButtonDown(1) && ((timeGetTime() - lastFireTime) > fireDelay))
		{

			if (superWeapon > 0)
			{
				for (int v = 1; v < 1030; v += 10)
				{
					missle missle1(v, ship1.translation.y);
					missles.push_back(missle1);
					lastFireTime = timeGetTime();
					dmusic.PlaySound(&sound, 0);
				}
			superWeapon--;
			}
		}
			
	
		if(dinput.MouseButtonDown(0))
		{
			
			if((timeGetTime() - lastFireTime) > fireDelay)									//RATE OF FIRE
			{

				if (guns >= 2)
				{
					missle missleONE(ship1.xPos + 16, ship1.translation.y + 10);
					missle missleTWO(ship1.xPos + 48, ship1.translation.y + 10);
					missles.push_back(missleTWO);
					missles.push_back(missleONE);
					
					if (guns == 3 || guns >= 5)
					{
						missle missle1(ship1.xPos + 32, ship1.translation.y);
						missles.push_back(missle1);
					}

					if (guns >= 4)
					{
						missle missle3(ship1.xPos, ship1.translation.y + 20);
						missle missle4(ship1.xPos + 64, ship1.translation.y + 20);
						missles.push_back(missle3);
						missles.push_back(missle4);
					}

				}


				else
				{
					missle missle1(ship1.xPos + 32, ship1.translation.y);
					missles.push_back(missle1);
				}
					
					lastFireTime = timeGetTime();
					dmusic.PlaySound(&sound, 0);
			}

		}

		for(list<enemy>::iterator k = enemies.begin(); k != enemies.end(); k++)
		{


			if(k->xPos >= ship1.translation.x - 1 && k->xPos <= ship1.translation.x + 71 && k->yPos >= ship1.translation.y && k->yPos - 1 <= ship1.translation.y + 71)
			{


				explode explodeONE(k->translation.x, k->translation.y, 1, 1);
				explodes.push_back(explodeONE);
				dmusic3.PlaySound(&Expl, 0);
				list<enemy>::iterator y = k;
				k--;
				enemies.erase(y);
				hitPoints -= 20;

			}	




			if(k->yPos >= 768)
				k-> yPos = 20;

			k->disp(pTexture3, p3DXSprite);
			int t = int(rand()%60);
			if(t == 2)
			{
				k->fire();
			}
		}



		for(list<enemy2>::iterator z = enemy2s.begin(); z != enemy2s.end(); z++)
		{


			if(z->xPos >= ship1.translation.x - 1 && z->xPos <= ship1.translation.x + 71 && z->yPos >= ship1.translation.y && z->yPos - 1 <= ship1.translation.y + 71)
			{
				explode explodeONE(z->translation.x, z->translation.y, 1, 1);
				explodes.push_back(explodeONE);
				dmusic3.PlaySound(&Expl, 0);
				list<enemy2>::iterator y = z;
				z--;
				enemy2s.erase(y);
				hitPoints -= 20;
				
			}	




			if(z->yPos >= 768)
				z-> yPos = 20;

			z->disp(pTexture9, p3DXSprite);
			int t = int(rand()%60);
			if(t == 2)
			{
				z->fire();
			}
		}



		for(list<missle>::iterator i = missles.begin(); i != missles.end(); i++)
		{
			i->disp(pTexture2, p3DXSprite);

			if(i->yPos <= 0)
			{
				list<missle>::iterator j = i;
				i--;
				missles.erase(j);
			}
	
		
			for(list<enemy>::iterator q = enemies.begin(); q != enemies.end(); q++)
			{
		
				if(i->yPos <= (q->yPos + 64) && i->yPos >= (q->yPos - 10) && i->xPos >= (q->xPos - 3) && i->xPos <= (q->xPos + 64))
				{
					list<missle>::iterator j = i;
					i--;
					missles.erase(j);
					q->hit--;
					if (q->hit == 0)
					{

						score += 1;
						list<enemy>::iterator l = q;
						q--;
						explode explodeONE(l->translation.x, l->translation.y, 1, 1);
						explodes.push_back(explodeONE);
						dmusic3.PlaySound(&Expl, 0);
						enemies.erase(l);
					}	

				}

			
			}

		

			for(list<enemy2>::iterator p = enemy2s.begin(); p != enemy2s.end(); p++)
			{
		
				if(i->yPos <= (p->yPos + 64) && i->yPos >= (p->yPos - 10) && i->xPos >= (p->xPos - 3) && i->xPos <= (p->xPos + 64))
				{
					list<missle>::iterator j = i;
					i--;
					missles.erase(j);
					p->hit--;
					if (p->hit == 0)
					{
						score += 5;
						list<enemy2>::iterator l = p;
						p--;
						explode explodeONE(l->translation.x, l->translation.y, 1, 1);
						explodes.push_back(explodeONE);
						dmusic3.PlaySound(&Expl, 0);
						enemy2s.erase(l);
					}	

				}

			
			}




	

		}

		for(list<eMissle>::iterator v = eMissles.begin(); v != eMissles.end(); v++)
		{
		
			v->disp(pTexture2, p3DXSprite);
		
			if (v->translation.y >= ship1.translation.y && v->translation.y <= ship1.translation.y + 64)
			{
				if (v->translation.x >= ship1.translation.x && v->translation.x <= ship1.translation.x + 64)
				{
					list<eMissle>::iterator w = v;
					v--;
					eMissles.erase(w);
					hitPoints -= 5;
				}
			}
		
			if (v->translation.y >= 765)
			{
				list<eMissle>::iterator w = v;
				v--;
				eMissles.erase(w);
			}
		}
	
	
		for(list<explode>::iterator w = explodes.begin(); w != explodes.end(); w++)
		{
			w->disp(p3DXSprite);
			w->frame++;
			if(w->frame >= 20)
			{
				list<explode>::iterator m = w;
				w--;
				explodes.erase(m);
			}
		}

		ship1.disp(pTexture, p3DXSprite);

	  /*p3DXSprite->Draw(pTexture, NULL, NULL, NULL,
		0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));*/


	/*
	Draw( 
		a valid texture (LPDIRECT3DTEXTURE8) - invalid textures will crash your program,
		pointer to a clipping rectangle (RECT*) or NULL if no clipping,
		pointer to a vector defining the scale change (D3DXVECTOR2*) or NULL for (1,1),
		pointer to a vector defining the center of any rotation (D3DXVECTOR2*) or NULL for (0,0),
		the value of the rotation in radians,
		pointer to a vector defining the position (D3DXVECTOR2*),
		the color modulation: D3DCOLOR_ARGB(alpha, red, green, blue), 0 - 255 as integers
		);
	*/

		if(levelCounter == 1)
		{
			for(int b = 0; b < level; b++)
			{
				for(int a = 0; a < 2; a++)
				{	
					
					if (rand()%5 == 2)
					{
						enemy2 enemyTwo;
						enemy2s.push_back(enemyTwo);
					}

					else
					{
						enemy enemy;
						enemies.push_back(enemy);
					}

				}
			}
			levelCounter = -1;
		}
	
		if (enemies.size() == 0 && enemy2s.size() == 0 && levelCounter < 0)
		{
		
			levelCounter = 250;
			
			
			level++;

		}
	
		if (levelCounter > 125)
			font->DrawTextA(levelString.c_str(), 9, &lev, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));

		if (levelCounter <= 125 && levelCounter > 0)
		{

			if (level == 10 || level == 15 || level == 20 || level == 25)
			{
				font->DrawTextA(gunsUpStr.c_str(), 17, &lev, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
				guns = level / 5;
			}
			else
			{
				font->DrawTextA(shieldsUpStr.c_str(), 11, &lev, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
			}
			
		
			if (levelCounter == 125)
				hitPoints += 5;
		}

		if(hitPoints <= 0)
		{
			gameOver = true;
		}

		levelCounter--;
		

	
	}
	else
	{
		if (!checkedScore)
		{
			high = checkHigh(score);
			logthis("high = ");
			logthis(high);
			checkedScore = true;
		}

		dshow.Stop();
		
		/*
		hitPoints = 0;
		oss << "Shields: " << hitPoints << "%" << "    ";
		hpString = oss.str();
		lev.left = 225;
		
		font->DrawTextA(hpString.c_str(), 13, &hitP, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
		*/	
	
		if (high != -1)
			inputScore = true;
		else
			dispHighs = true;


		if (inputScore && !dispHighs)    //input player's name
		{

			
			int inputDelay = 70;


			ostrstream highChar;

			input_translation.x = 512;
			input_translation.y = 384;

			char_display.bottom = 383;
			char_display.top = 344;
			char_display.left = 500;


			if (name.size() > 0)
			{
			
				for (int nameCounter = 0; nameCounter < 20 - lettersEntered; nameCounter++)
				{
					name.pop_back();
				}
			
			}
			
		
			
			//check letters

			if (lettersEntered < 20)
			{
				if (dinput.KeyDown(DIK_A))
				{
					
	
					while (dinput.KeyDown(DIK_A))
					{
					}
	
					name.push_back("A");
					lettersEntered++;
				}
	
				if (dinput.KeyDown(DIK_B))
				{
					while (dinput.KeyDown(DIK_B))
					{
					}
	
					name.push_back("B");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_C))
				{
					while (dinput.KeyDown(DIK_C))
					{
					}
	
					name.push_back("C");
					lettersEntered++;
				}
				
				
	
				if (dinput.KeyDown(DIK_D))
				{
					while (dinput.KeyDown(DIK_D))
					{
					}
	
					name.push_back("D");
					lettersEntered++;
				}
	
				if (dinput.KeyDown(DIK_E))
				{
					while (dinput.KeyDown(DIK_E))
					{
					}
	
					name.push_back("E");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_F))
				{
					while (dinput.KeyDown(DIK_F))
					{
					}
	
					name.push_back("F");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_G))
				{
					while (dinput.KeyDown(DIK_G))
					{
					}
	
					name.push_back("G");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_H))
				{
					while (dinput.KeyDown(DIK_H))
					{
					}
	
					name.push_back("H");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_I))
				{
					while (dinput.KeyDown(DIK_I))
					{
					}
	
					name.push_back("I");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_J))
				{
					while (dinput.KeyDown(DIK_J))
					{
					}
	
					name.push_back("J");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_K))
				{
					while (dinput.KeyDown(DIK_K))
					{
					}
	
					name.push_back("K");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_L))
				{
					while (dinput.KeyDown(DIK_L))
					{
					}
	
					name.push_back("L");
					lettersEntered++;
				}
	
				
				if (dinput.KeyDown(DIK_M))
				{
					while (dinput.KeyDown(DIK_M))
					{
					}
	
					name.push_back("M");
					lettersEntered++;
				}
	
				
				if (dinput.KeyDown(DIK_N))
				{
					while (dinput.KeyDown(DIK_N))
					{
					}
	
					name.push_back("N");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_O))
				{
					while (dinput.KeyDown(DIK_O))
					{
					}
	
					name.push_back("O");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_P))
				{
					while (dinput.KeyDown(DIK_P))
					{
					}
	
					name.push_back("P");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_Q))
				{
					while (dinput.KeyDown(DIK_Q))
					{
					}
	
					name.push_back("Q");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_R))
				{
					while (dinput.KeyDown(DIK_R))
					{
					}
	
					name.push_back("R");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_S))
				{
					while (dinput.KeyDown(DIK_S))
					{
					}
	
					name.push_back("S");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_T))
				{
					while (dinput.KeyDown(DIK_T))
					{
					}
	
					name.push_back("T");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_U))
				{
					while (dinput.KeyDown(DIK_U))
					{
					}
	
					name.push_back("U");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_V))
				{
					while (dinput.KeyDown(DIK_V))
					{
					}
	
					name.push_back("V");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_W))
				{
					while (dinput.KeyDown(DIK_W))
					{
					}
	
					name.push_back("W");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_X))
				{
					while (dinput.KeyDown(DIK_X))
					{
					}
	
					name.push_back("X");
					lettersEntered++;
				}
	
	
				if (dinput.KeyDown(DIK_Y))
				{
					while (dinput.KeyDown(DIK_Y))
					{
					}
	
					name.push_back("Y");
					lettersEntered++;
				}
	
	
	
				if (dinput.KeyDown(DIK_Z))
				{
					while (dinput.KeyDown(DIK_Z))
					{
					}
	
					name.push_back("Z");
					lettersEntered++;
				}


				if (dinput.KeyDown(DIK_SPACE))
				{
					while (dinput.KeyDown(DIK_SPACE))
					{
					}
	
					name.push_back(" ");
					lettersEntered++;
				}

				if (dinput.KeyDown(DIK_RETURN))
				{
					while (dinput.KeyDown(DIK_RETURN))
					{
					}
					
					setHighs(name, high);
					dispHighs = true;
				}



			}

	


			if (dinput.KeyDown(DIK_BACKSPACE) && name.size() > 0)
			{
				if (timeGetTime() - lastInputTaken > inputDelay)
				{
					name.pop_back();
					lettersEntered--;
					lastInputTaken = timeGetTime();
				}
			}



			for (int nameCount = 0; nameCount < 20 - lettersEntered; nameCount++)
			{
				name.push_back(" ");
			}





			nameStr.erase();

			
			for (int strCounter = 0; strCounter < name.size(); strCounter++)
			{
				nameStr.append(name[strCounter]);
			}

			
			image_translation.x = 270;
			image_translation.y = 315;

			p3DXSprite->Draw(enter_name, NULL, NULL, NULL, 
				0, &image_translation, D3DCOLOR_ARGB(255, 255, 255, 255));			
			
			font->DrawTextA(nameStr.c_str(), 20, &char_display, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));

				
			input_translation.x = (lettersEntered * 12 * 1.5) + 503;
			input_translation.y = 362;

			p3DXSprite->Draw(input_line, NULL, NULL, NULL, 
				0, &input_translation, D3DCOLOR_ARGB(255, 255, 255, 255));

			
			image_translation.x = 256;
			image_translation.y = 100;

			p3DXSprite->Draw(new_high, NULL, NULL, NULL, 
				0, &image_translation, D3DCOLOR_ARGB(255, 255, 255, 255));
			


		}

		if (dispHighs)
		{

			p3DXSprite->Draw(highScoresImg, NULL, NULL, NULL, 
				0, &image_translation, D3DCOLOR_ARGB(255, 255, 255, 255));			
			
			if (!gotHighs)
			{

				image_translation.x = 256;
				image_translation.y = 50;

				int temp = 1;
				int temp2 = 1;
				for (int i = 0; i < 300; i++)
				{
					if (rand() % 2 == 1)
						temp = 1;
					else
						temp = -1;

					if (rand() % 2 == 1)
						temp2 = 1;
					else
						temp2 = -1;

					whiteBox box((rand() % 1000) + 1, (rand() % 750) + 1, temp, temp2);
					boxes.push_back(box);
				}
				getHighs();
				gotHighs = true;
			}

			for (list<whiteBox>::iterator iter = boxes.begin(); iter != boxes.end(); iter++)
			{
				iter->updatePos();
				iter->disp(p3DXSprite);
			}

			int x, y;
			string str1;

			x = 300;
			y = 256;

			for (int count = 0; count < 10; count++)
			{
				str1 = hScores[count].fName;

				if (hScores[count].lName != "*~&")
				{
					str1 += " ";
					str1 += hScores[count].lName;
				}

				char_display.top = y;
				char_display.bottom = (y + 10);
				char_display.left = x;

				for (int sCtr = str1.length(); sCtr < 20; sCtr++)
				{
					str1 += " ";
				}

				font->DrawTextA(str1.c_str(), 20, &char_display, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
				
				x = 710;
				char_display.left = x;
				
				stringstream ss;
				ss << hScores[count].score;
				ss >> str1;

				
				for (sCtr = str1.length(); sCtr < 5; sCtr++)
				{
					str1 += " ";
				}

				font->DrawTextA(str1.c_str(), 5, &char_display, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 0, 0));
				
				x = 300;
				y += 40;

			}
			

		}
			

	}

	EndDrawing();

}




/*
This is a packaged function that creates a message processing loop and trades control between the 
update function and the WndProc.
*/
void CreateUpdateLoop(unsigned int MinUpdateTime)
{
	unsigned int frametimebuffer = 0;
	MSG msg;

	while(1)
    {
		/*
		If there is a message waiting, and it can be acquired, then send it to the WndProc function.
		*/
		MinUpdateTime = updateDelay;
        if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
			/*
			If the program can't access a message, this is generally a very bad sign, and it's
			time to call it a day.
			*/
            if ( !GetMessage( &msg, NULL, 0, 0 ) )
                return;

            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else if((timeGetTime() - frametimebuffer) > MinUpdateTime)
		{
			/*
			If there were no messages waiting, and if sufficient time has elapsed, then call an
			update function.
			*/

			frametimebuffer = timeGetTime();
			
			Update();
		}
    }
	ReleaseAll();

	return;
}




void LoadFont(LPDIRECT3DDEVICE8 d3d, LPD3DXFONT* f, const string& fontname, int width, int height)
{
	LOGFONT lfont;
	lfont.lfWidth = width;
	lfont.lfHeight = height;
	lfont.lfEscapement = 0;
	lfont.lfOrientation = 0;
	lfont.lfWeight = FW_NORMAL;
	lfont.lfItalic = false;
	lfont.lfUnderline = false;
	lfont.lfStrikeOut = false;
	lfont.lfCharSet = ANSI_CHARSET;
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfont.lfQuality = DRAFT_QUALITY;
	lfont.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(lfont.lfFaceName, fontname.c_str());

	D3DXCreateFontIndirect(d3d, &lfont , f);
}




int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
	resetlog();

	logthis("beginning Galacta log file");
	

	f.open("high");

	f >> high;

	f.close();

	ifstream istr;
	istr.open("music.txt");
	istr >> song;
	istr >> volume;
	istr.close();


	
	bgt.x = 0;
	bgt.y = 20;
	
	
	for(int a = 0; a < 2; a++)
	{
		enemy enemy;
		enemies.push_back(enemy);
	}

	

	srand((unsigned)time(NULL));
	WNDCLASS WindowClass;
	HWND hWnd;

	char* name = "Galacta";
	
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = hInstance;
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = hInstance;
	WindowClass.hIcon = LoadIcon( hInstance, IDI_APPLICATION );
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = name;
	WindowClass.lpszClassName = name;

	RegisterClass(&WindowClass);

	hWnd = CreateWindow(name, name,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			NULL, NULL, hInstance, NULL);

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	logthis("Created and showed window");

	if(FAILED(CoInitialize(NULL)))
		return 0;

	logthis("Initialized COM");
	
	dmusic.Initialize();
	dshow.Initialize();
	dmusic3.Initialize();
	dinput.Initialize(hWnd);





	logthis("Entering d3d initialization function");

	if(SetUpDirectGraphics(1024, 768, hWnd) == false)
		return 0;

	logthis("d3d initialized");


	
	LoadFont(pD3DDevice, &font, "System", 12, 18);
	logthis("Created font object from system font");
	
	dmusic.LoadSound(&sound, "laser.wav");
	
	dshow.PlayMusic(song.c_str());
	dshow.SetVolume(volume);
	
	dmusic3.LoadSound(&Expl, "smexp.wav");
	logthis("Loaded sound files");


	D3DXCreateTextureFromFileEx(pD3DDevice, "ship.tga", D3DX_DEFAULT, D3DX_DEFAULT,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
			D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &pTexture);

	D3DXCreateTextureFromFileEx(pD3DDevice, "missle.tga", D3DX_DEFAULT, D3DX_DEFAULT,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
			D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &pTexture2);

	D3DXCreateTextureFromFileEx(pD3DDevice, "enemy.tga", D3DX_DEFAULT, D3DX_DEFAULT,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
			D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &pTexture3);

	D3DXCreateTextureFromFileEx(pD3DDevice, "stars.jpg", D3DX_DEFAULT, D3DX_DEFAULT,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
			D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &bg);

	D3DXCreateTextureFromFileEx(pD3DDevice, "enemy2.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &pTexture9);


	D3DXCreateTextureFromFileEx(pD3DDevice, "input_line.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &input_line);


	D3DXCreateTextureFromFileEx(pD3DDevice, "n_high.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &new_high);

	D3DXCreateTextureFromFileEx(pD3DDevice, "enter_name.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &enter_name);
		
	D3DXCreateTextureFromFileEx(pD3DDevice, "highScores.bmp", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(255,255,255,255), NULL, NULL, &highScoresImg);


	D3DXCreateTextureFromFileEx(pD3DDevice, "white_box.bmp", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &white_box);


	/*
	Every file loading option you could ever possibly want "conveniently" placed in one
	giant function call.  The parameters with an ! in front are the ones I actually find useful.

	D3DXCreateTextureFromFileEx(
		!	an initialized d3d device (LPDIRECT3DDEVICE8),
		!	a file name (char*),
			the width (int), or D3DX_DEFAULT to use the dimensions of the file,
			the height (int), or D3DX_DEFAULT to use the dimensions of the file,
			number of MIP map levels - just the initial 1 for 2d apps,
			the "usage" - just keep it 0 trust me,
			the pixel format - D3DFMT_UNKNOWN reads it in from the file,
			the memory management options - probably want managed,
		!	the filtering algorithm - D3DX_FILTER_NONE or D3DX_FILTER_LINEAR,
			the MIP map filtering algorithm - D3DX_FILTER_NONE,
			the color key - will render this color as transparent,
			source info to describe texture in advance - keep NULL,
			a palette entry to fill in - keep NULL,
		!	finally - the pointer to the actual texture
			};
	*/

		
	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0000.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[0]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0005.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[1]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0010.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[2]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0015.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[3]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0020.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[4]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0025.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[5]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0030.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[6]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0035.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[7]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0040.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[8]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0045.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[9]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0050.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[10]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0055.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[11]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0060.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[12]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0065.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[13]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0070.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[14]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0075.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[15]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0080.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[16]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0085.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[17]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0090.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[18]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0095.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[19]);

	D3DXCreateTextureFromFileEx(pD3DDevice, "exp0100.tga", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
		D3DCOLOR_ARGB(0,255,255,255), NULL, NULL, &explosion[20]);

	logthis("Loaded all textures, entering update loop at 20/1000 Hz");


	

	CreateUpdateLoop(20);

	return 0;
}
