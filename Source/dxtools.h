/*
Direct X Wrapper Class for Win32 Applications
By Daniel Christensen

Use these libraries: dxguid.lib dinput8.lib winmm.lib d3d8.lib d3dx8.lib strmiids.lib

To convert a .3ds file to a .x:			conv3ds -f -s-1 mesh.3ds
*/

#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800


/*
Useful load of header files for game programming...
*/
#include <windows.h>
#include <windowsx.h>
#include <winbase.h>
#include <mmsystem.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <dinput.h>
#include <dmusici.h>
#include <dshow.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <D3d8types.h>
#include <tchar.h>
#include <list>
#include <algorithm>
#include <fstream>
using namespace std;

const float PI = 3.1415926535f;

/*
logthis is a file logger function.  Call resetlog at the beginning of your program to purge the text
file (log.txt by default), and then all logthis() calls will write an entry to the file...
*/
template<class T>void logthis(T message);
void logthis(int message);
void resetlog();

/*
Simple sound class: use for wave and midi files
Can load stuff into memory, but is limited in file format capabilities.

Use CSound objects to store information for the CDMusic class.
*/
class CSound
{
	friend class CDMusic;
public:
	CSound();
	void Release();
private:
	IDirectMusicSegment8*     g_pSegment;
	unsigned int lastStartTime;
};

/*
Initialize CDInput once at program input to begin.

Call UpdateMouse to get new information from the mouse.

KeyDown and MouseButtonDown will retrieve input device states at the time they're called.
*/
class CDInput
{
public:
	CDInput();

	bool Initialize( HWND hWnd );
	void Release();

	void UpdateMouse();

	int GetMouseXDif();
	int GetMouseYDif();
	int GetMouseZDif();
	int MouseButtonDown(int button);
	bool KeyDown(int key);

private:
	LPDIRECTINPUT8       g_pDI;
	LPDIRECTINPUTDEVICE8 g_pdidMouse;
	LPDIRECTINPUTDEVICE8 g_pdidKeyboard;

	DIMOUSESTATE dims;
};

/*
Call initialize to begin, release when you're done.

Send sound objects and a file name path to LoadSound to load a sound, 
and then send the initialized sound objects to PlaySound to play them.
*/
class CDMusic
{
public:

	CDMusic();

	bool Initialize();
	void Release();

	bool LoadSound(CSound* s, const char* filename);
	void PlaySound(CSound* s, int numRepeats);

private:

	static void ReleaseSound(CSound* s);

	IDirectMusicLoader8*      g_pLoader;
	IDirectMusicPerformance8* g_pPerformance;

	list<CSound*> sounds;

};

/*
PlayMovie and PlayMusic play streaming media analogously to media player.

The Playing() function queries whether media is playing.  Check this periodically to restart
or change sound tracks when they finish.
*/
class CDShow
{
public:

	bool Initialize();
	void Release();
	bool PlayMusic(const char* filename);
	bool PlayMovie(char* filename, HWND hWnd, int xres, int yres);

	bool Playing();
	void SetVolume(int percent);

	void Stop();

private:
	IGraphBuilder*	pGraph;
	IMediaControl*	pMediaControl;
	IMediaEvent*	pEvent;
	IMediaSeeking*	pSeeking;
	IBasicAudio*	pVolume;

	IVideoWindow*	pVidWin;
};