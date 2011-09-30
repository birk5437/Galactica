
#include "dxtools.h"

#define INIT_DXSTRUCT(dxstruct) { ZeroMemory(&dxstruct, sizeof(dxstruct)); dxstruct.dwSize = sizeof(dxstruct); }

template<class T>void logthis(T message)
{
	ofstream file;
	file.open("log.txt", ios::app);
	file << message << endl;
	file.close();
	return;
}

void logthis(int message)
{
	ofstream file;
	file.open("log.txt", ios::app);
	char output[32];
	sprintf(output, "%d", message);
	file << output << endl;
	file.close();
	return;
}

void logthis(float message)
{
	ofstream file;
	file.open("log.txt", ios::app);
	file << message << endl;
	file.close();
	return;
}

void logthis(const char* message)
{
	ofstream file;
	file.open("log.txt", ios::app);
	file << message << endl;
	file.close();
	return;
}

void logthis(RECT stuff)
{
	ofstream file;
	file.open("log.txt", ios::app);
	file << "<RECT> left = " << stuff.left << ", top = " << stuff.top
		<< ", right = " << stuff.right << ", bottom = " << stuff.bottom 
		<< " <END RECT>\n";
	file.close();
	return;
}

void resetlog()
{
	ofstream file;
	file.open("log.txt");
	file.clear();
	file.close();
	return;
}

CDInput::CDInput()
{

}

bool CDInput::Initialize( HWND hWnd )
{
	logthis("Beginning DirectInput initialization");

	HINSTANCE hInst = (HINSTANCE)GetWindowLong( hWnd, GWL_HINSTANCE );
	HRESULT   hr;

	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use
	if( FAILED( hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDI,
										NULL) ) )
	{
		logthis("*** DirectInputCreate() failed.");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Created DirectInput object");
	#endif

	// Obtain an interface to the system mouse device
	if( FAILED( hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pdidMouse,
										  NULL ) ) )
	{
		logthis("*** Direct Input CreateDevice() failed (mouse).");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Created system mouse device.");
	#endif

	// Set the data format to "mouse format". A data format specifies which
	// controls on a device we are interested in, and how they should be
	// reported. This tells DirectInput that we will be passing a
	// DIMOUSESTATE structure to IDirectInputDevice::GetDeviceState.
	if( FAILED( hr = g_pdidMouse->SetDataFormat( &c_dfDIMouse ) ) )
	{
		logthis("*** Direct Input SetDataFormat() failed (mouse).");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Set mouse to mouse data format.");
	#endif

	//NONEXLUSIVE : not interfere with other apps
	//BACKGROUND : use input even if not in foreground
	if( FAILED( hr = g_pdidMouse->SetCooperativeLevel( hWnd,
									DISCL_NONEXCLUSIVE | DISCL_BACKGROUND ) ) )
	{
		logthis("*** Direct Input SetCooperativeLevel() failed (mouse).");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Set mouse cooperative level.");
	#endif

	if( FAILED( hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_pdidKeyboard,
                                      NULL ) ) )
	{
		logthis("*** CreateDevice() failed (keyboard).");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Created system keyboard object.");
	#endif

	if( FAILED( hr = g_pdidKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		logthis("*** SetDataFormat() failed (keyboard).");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Set keyboard to keyboard data format");
	#endif

	if( FAILED( hr = g_pdidKeyboard->SetCooperativeLevel( hWnd,
									DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) ) )
	{
		logthis("*** SetCooperativeLevel() keyboard failed");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Set keyboard cooperative level.");
	#endif

	return true;
}

void CDInput::Release()
{
	logthis("Beginning DirectInput release.");

	// Destroy the mouse object
	if( g_pdidMouse )
	{
		// Unacquire the device (just in case) before exiting.
		g_pdidMouse->Unacquire();

		g_pdidMouse->Release();
		g_pdidMouse = NULL;

		#ifdef CDX_DEBUG
		logthis("DirectInput mouse device released.");	
		#endif
	}

	// Destroy the mouse object
	if( g_pdidKeyboard )
	{
		// Unacquire the device (just in case) before exiting.
		g_pdidKeyboard->Unacquire();

		g_pdidKeyboard->Release();
		g_pdidKeyboard = NULL;

		#ifdef CDX_DEBUG
		logthis("DirectInput keyboard device released.");
		#endif
	}

	// Destroy the DInput object
	if( g_pDI )
	{
		g_pDI->Release();
		g_pDI = NULL;

		#ifdef CDX_DEBUG
		logthis("DirectInput object released.");
		#endif	
	}

	logthis("DirectInput released successfully.");

	return;
}

void CDInput::UpdateMouse()
{
	if(g_pdidMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims ) != DI_OK)
	{
		g_pdidMouse->Acquire();

		#ifdef CDX_DEBUG
		logthis("Acquiring mouse.");
		#endif
	}
	return;
}

int CDInput::GetMouseXDif()
{
	return dims.lX;
}

int CDInput::GetMouseYDif()
{
	return dims.lY;
}

int CDInput::GetMouseZDif()
{
	return dims.lZ;
}

int CDInput::MouseButtonDown(int button)
{
	return (dims.rgbButtons[button] & 0x80);
}

bool CDInput::KeyDown(int key)
{
	BYTE  diKeys[256];
	
	if (g_pdidKeyboard->GetDeviceState(256, diKeys) == DI_OK) 
	{
		if(diKeys[key] & 0x80)
			return true;
		else
			return false;	
	}
	else
	{
		g_pdidKeyboard->Acquire();
	}

	return 0;
}

	
CDMusic::CDMusic()
{
	g_pLoader      = NULL;
	g_pPerformance = NULL;
}

bool CDMusic::LoadSound(CSound* s, const char* filename)
{
	if (NULL != s->g_pSegment)
    {
        s->g_pSegment->Release();
        s->g_pSegment = NULL;
    }

	sounds.push_front(s);

	//Converting from multibyte to widechar
	char* mbfilename = new char[100];
	strcpy(mbfilename, filename);
	wchar_t* widebuffer = new wchar_t[100];
	mbstowcs(widebuffer, mbfilename, 100);

	if (FAILED(g_pLoader->LoadObjectFromFile(
		CLSID_DirectMusicSegment, // Class identifier.
		IID_IDirectMusicSegment8, // ID of desired interface.
		widebuffer,     // Filename.
		(void**) &s->g_pSegment)))
	{
		logthis("*** Failed to find the sound file:");
		logthis(filename);
		return false;
	}

	s->g_pSegment->Download( g_pPerformance );

	return true;
}

void CDMusic::PlaySound(CSound* s, int numRepeats)
{
	if((timeGetTime() - 50) > s->lastStartTime)
		s->lastStartTime = timeGetTime();
	else
		return;

	if (NULL == s->g_pSegment)
		return;

	s->g_pSegment->SetRepeats(numRepeats);
	g_pPerformance->PlaySegmentEx(s->g_pSegment,NULL,NULL, DMUS_SEGF_NOINVALIDATE ,0,NULL,NULL,NULL);
}

void CDMusic::ReleaseSound(CSound* s)
{
	s->Release();
	s = NULL;
}

void CDMusic::Release()
{
	logthis("Beginning DirectMusic release.");

	// Release all of the sound objects that have been created
	for_each(sounds.begin(), sounds.end(), ReleaseSound);
	sounds.clear();

	#ifdef CDX_DEBUG
	logthis("Sound buffers released");	
	#endif

    // If there is any music playing, stop it.
    if (NULL != g_pPerformance)
    {
        g_pPerformance->Stop( NULL, NULL, 0, 0 );

        // CloseDown and Release the performance object
        g_pPerformance->CloseDown();
        g_pPerformance->Release();
        g_pPerformance = NULL;

		#ifdef CDX_DEBUG
		logthis("DirectMusic performance closed down and released.");	
		#endif
    }

    // Release the loader object
    if (NULL != g_pLoader)
    {
        g_pLoader->Release();
        g_pLoader = NULL;

		#ifdef CDX_DEBUG
		logthis("DirectMusic loader released.");	
		#endif
    }    

	logthis("DirectMusic released successfully.");

    return;
}

bool CDMusic::Initialize()
{
	logthis("Beginning DirectMusic initialization.");

	HRESULT hr;
    BOOL    bUseCurrentWorkingDir = FALSE;

    // Create loader object
    hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
                           IID_IDirectMusicLoader, (void**)&g_pLoader );
    if ( FAILED(hr) )
    {
		logthis("*** Couldn't create loader");
        return false;
	}

	#ifdef CDX_DEBUG
	logthis("Created DirectMusic loader.");
	#endif

    // Create performance object
    hr = CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
		                     IID_IDirectMusicPerformance, (void**)&g_pPerformance );
    if ( FAILED(hr) )
    {
		logthis("*** Couldn't create performance");
        return false;
	}

	#ifdef CDX_DEBUG
	logthis("Created DirectMusic performance.");
	#endif

	g_pPerformance->InitAudio(NULL, NULL, NULL, DMUS_APATH_SHARED_STEREOPLUSREVERB,
		64, DMUS_AUDIOF_ALL, NULL);

	#ifdef CDX_DEBUG
	logthis("Initialized performance audio.");
	#endif

	IDirectMusicAudioPath* g_pAudioPath;
	
	g_pPerformance->GetDefaultAudioPath(&g_pAudioPath);
	g_pAudioPath->SetVolume(-1000, 0);

    return true;
}

CSound::CSound()
{
	g_pSegment = NULL;
	lastStartTime = timeGetTime();		
}

void CSound::Release()
{
	if (NULL != g_pSegment)
	{
		g_pSegment->Release();
		g_pSegment = NULL;
	}
}


bool CDShow::Initialize()
{
	logthis("Beginning DirectShow initialization");

	if(FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                IID_IGraphBuilder, (void **)&pGraph)))
	{
		logthis("*** Couldn't create directshow filter graph object.");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Created DirectShow filter graph.");
	#endif

	if(FAILED(pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl)))
	{
		logthis("*** Couldn't query media control interface.\n");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Queried media control interface.");
	#endif
	
	if(FAILED(pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent)))
	{
		logthis("*** Couldn't query media event interface.\n");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Queried media event interface.");
	#endif

	if (FAILED (pGraph->QueryInterface(IID_IMediaSeeking,(void **)&pSeeking)))
	{
		logthis("*** Couldn't query the media seeking interface.\n");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Queried media seeking interface.");
	#endif

	if (FAILED (pGraph->QueryInterface(IID_IBasicAudio,(void **)&pVolume)))
	{
		logthis("*** Couldn't query the basic audio interface.\n");
		return false;
	}

	#ifdef CDX_DEBUG
	logthis("Queried basic audio interface.");
	#endif

	return true;
}

void CDShow::Release()
{
	logthis("Beginning DirectShow release.");

	Stop();

	if(pMediaControl != NULL)
	{
		pMediaControl->Release();
		pMediaControl = NULL;

		#ifdef CDX_DEBUG
		logthis("Media control object released.");	
		#endif
	}

	if(pEvent != NULL)
	{
		pEvent->Release();
		pEvent = NULL;

		#ifdef CDX_DEBUG
		logthis("Event object released.");	
		#endif
	}

	if(pGraph != NULL)
	{
		pGraph->Release();
		pGraph = NULL;

		#ifdef CDX_DEBUG
		logthis("Graph object released.");	
		#endif
	}

	if(pSeeking != NULL)
	{
		pSeeking->Release();
		pSeeking = NULL;

		#ifdef CDX_DEBUG
		logthis("Seeking object released.");	
		#endif
	}

	if(pVolume != NULL)
	{
		pVolume->Release();
		pVolume = NULL;

		#ifdef CDX_DEBUG
		logthis("Volume object released.");	
		#endif
	}

	if(pVidWin != NULL)
	{
		pVidWin->Release();
		pVidWin = NULL;

		#ifdef CDX_DEBUG
		logthis("Video Window released.");	
		#endif
	}

	logthis("DirectShow released successfully.");
}

bool CDShow::PlayMusic(const char* filename)
{
	Release();
	Initialize();

	DWORD dwAttr = GetFileAttributes(filename);
	if (dwAttr == (DWORD) -1)
	{
		logthis("*** Couldn't find music file.");
		return false;
	}

	//Converting from multibyte to widechar
	char* mbfilename = new char[100];
	strcpy(mbfilename, filename);
	wchar_t* widebuffer = new wchar_t[100];
	mbstowcs(widebuffer, mbfilename, 100);

	pGraph->RenderFile(widebuffer, NULL);
	pMediaControl->Run();

	return true;
}

void CDShow::SetVolume(int percent)
{
	pVolume->put_Volume(-(100 * (100 - percent)));
}

bool CDShow::Playing()
{



	LONGLONG current, stop;
	current = 0;
	stop = 1;
	pSeeking->GetPositions(&current, &stop);
	if(current >= stop)
	{
		current = 0;
		pSeeking->SetPositions(&current, AM_SEEKING_AbsolutePositioning,
			&stop, AM_SEEKING_AbsolutePositioning);
		return false;
	}
	else
		return true;	
}

bool CDShow::PlayMovie(char* filename, HWND hWnd, int xres, int yres)
{
	Release();
	Initialize();

	DWORD dwAttr = GetFileAttributes(filename);
	if (dwAttr == (DWORD) -1)
	{
		logthis("*** Couldn't find movie file.");
		return false;
	}

	pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);

	//Converting from multibyte to widechar
	char* mbfilename = new char[100];
	strcpy(mbfilename, filename);
	wchar_t* widebuffer = new wchar_t[100];
	mbstowcs(widebuffer, mbfilename, 100);

	pGraph->RenderFile(widebuffer, NULL);

	//Set the video window.
	pVidWin->put_Owner((OAHWND)hWnd);
	pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

	pVidWin->HideCursor(OATRUE);
	pVidWin->SetWindowPosition(0, 0, xres, yres);

	pMediaControl->Run();

	HANDLE event = CreateEvent(NULL, true, false, NULL);

	while(1)
	{
		WaitForSingleObject(event, 500);
		
		LONGLONG current, stop;
		current = 0;
		stop = 1;
		pSeeking->GetPositions(&current, &stop);

		if(current >= stop)
		{
			break;				
		}
		logthis(".");
	}
	
	logthis("Movie finished playing");
	return true;
}

void CDShow::Stop()
{
	if(pMediaControl == NULL)
		return;

	pMediaControl->Stop();
}