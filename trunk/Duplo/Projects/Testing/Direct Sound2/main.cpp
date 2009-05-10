#define FAST_SINE
//#define __WINDOWS_MM__
//#define __OS_WINDOWS__

#include <windows.h>

//#include <vector>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "Dup.h"

//using namespace Dup;

#include "../Modules/Dup_AllModules.h"




#include <dsound.h>
//#pragma comment(lib, "dsound.lib")



#include <mmsystem.h>
//#pragma comment(lib, "mmsystem.lib")
//#pragma comment(import, "mmsystem.dll")

//#import <mmsystem.dll>

 //SCS
#include "Base.h"
#include "SoundCollector.h"
#include "allinstruments.h"
#include "allmodules.h"
#include "StrFunc.h"





//#pragma comment(lib, "winmm.lib")


//#define __WINDOWS_DS__

//#pragma comment(lib, "multithreaded")
//#pragma comment(lib, "midiinfo.lib")


//
//#include "rtError.h"
//#include "rtMidi.h"
//#include "rtMidi.cpp"
//
//#include <signal.h>



#include "SCSCreateInstrument.h"
#include "drum_patches.h"
#include "DS2MidiReceiver.h"




#include "settings.h"






bool closed;
bool saveRecord;
SimpleDirectSound *dupDS;
StereoWaveBuffer *recordBuff;
MultiLowPass *multiLowPass;

void updateSound();


enum ENUM_PF {
  PF_ONCE = 0,
  PF_LOOPING = 1
} ;

// midi output feedback
MMRESULT midiOutOpenResult = MMSYSERR_NOERROR + 1;
HMIDIOUT hMidiOut = NULL;

// other

DS2MidiReceiver *midiReceiver;
Dup::MidiFileParser *midiFileParser;
bool errorInMidiFile;

__int64 startTime;
__int64 currentTime;
//__int64 musicPos;
__int64 newPos;
__int64 pos;
__int64 synthPos;


int sampleRate;
int nSamples;

dup_pos timeStep;


//extern HWND hMainWnd;  //handle to the window from the main windows program
HWND hwnd; // don't use external, will give error!
static LPDIRECTSOUND lpDS; //pointer to the directsound object
HDC hDC;
HPALETTE palette;




//void mycallback( double deltatime, std::vector< unsigned char > &message, void *userdata2 )
//{
//
//	int nBytes;
//	//double stamp;
//	
//	//stamp = midiin->getMessage( &message );
//							
//	nBytes = message.size();
//
//
//	if ( nBytes > 0 )
//	{
//		dup_uint8 data2[16];
//		for(int i=0;i<nBytes;i++)
//		{
//			data2[i] = message[i];
//		}		
//		midiReceiver->receiveEvent(data2);
//	}
//
//}

//typedef std::pair<int, dup_uint8*> SourceMidiMsg;

class SourceMidiMsg
{
public:
	dup_uint8 data[12]; // not sure of the size needed should be enough
	int source;
	
	SourceMidiMsg(int source) : source(source)	{}
};

Dup::Queue<SourceMidiMsg*, NULL> *midiQueue = NULL;
bool anythingOnMidiQueue;
static const unsigned int MIDI_MSG_QUEUE_SIZE = 256;
HANDLE midiQueueHMutex;
//HMUTEX midiQueueHMutex;


void initMidiQueue()
{
	midiQueueHMutex = CreateMutex(NULL, false, NULL);
	midiQueue = new Dup::Queue<SourceMidiMsg*, NULL>(MIDI_MSG_QUEUE_SIZE);
}

void midiInput(UINT inputStatus, DWORD midiMessage, int source)
{

	//MessageBox(hwnd,"a midi message 0","ok",MB_OK);
	
	//assert(mH->modulesNoneNull());

   //if (midiMsgQueue == NULL) midiMsgQueue = new midiMsgQueue(MIDI_MSG_QUEUE_SIZE); 
   
   //while (systemTaken);
   //systemTaken = true;
   
	
	switch (inputStatus) {

	  case MIM_DATA:
		{

			// Ignore Active Sensing messages
			if ((midiMessage & 0xff) == 0xfe || (midiMessage & 0xff) == 0xf8) {
			  break;
			}
			
			SourceMidiMsg *sourceMidiMsg = new SourceMidiMsg(source);
			//dup_uint8 *data = new dup_uint8[16]; // not sure of the size needed but this should be enough
			for(int i=0;i<sizeof(DWORD) / sizeof(dup_uint8); ++i)
			{
				sourceMidiMsg->data[i] = ((dup_uint8*)(&midiMessage))[i];
			}

			//midiReceiver->receiveEvent(data);		
			WaitForSingleObject(midiQueueHMutex, INFINITE);
			midiQueue->push(sourceMidiMsg);
			anythingOnMidiQueue = true;
			ReleaseMutex(midiQueueHMutex);
		}
		break;
	  default:
		break;
	  }

	//assert(mH->modulesNoneNull());
	//systemTaken = false;
}

static void CALLBACK midiInputCallback0( HMIDIOUT hmin, UINT inputStatus, 
			DWORD instancePtr, DWORD midiMessage, DWORD timestamp)
{	
	midiInput(inputStatus, midiMessage, 0);
}

static void CALLBACK midiInputCallback1( HMIDIOUT hmin, UINT inputStatus, 
			DWORD instancePtr, DWORD midiMessage, DWORD timestamp)
{
	//MessageBox(hwnd,"a mix midi message","ok",MB_OK);
	midiInput(inputStatus, midiMessage, 1);
}
  

void playMidiQueue()
{
	if (anythingOnMidiQueue)
	{
		bool haveMutex, go;
		haveMutex = (WaitForSingleObject(midiQueueHMutex, 0) == WAIT_OBJECT_0);
		if (haveMutex) go = !midiQueue->empty();
		else go = false;
		
		while(go)
		{
			SourceMidiMsg *sourceMidiMsg = midiQueue->pop();
			//dup_uint8 *data = midiQueue->pop();
			anythingOnMidiQueue = !midiQueue->empty();
			haveMutex = !ReleaseMutex(midiQueueHMutex);
			assert(!haveMutex);
			if (sourceMidiMsg != NULL)
			{
				//MessageBox(hwnd,"a midi message","ok",MB_OK);
				bool sendFeedback = false;
				midiReceiver->receiveEvent(((dup_uint8*)(&(sourceMidiMsg->data))), sourceMidiMsg->source, N_CHANNELS, sendFeedback);

				//if (midiOutOpenResult != MMSYSERR_NOERROR)
				//{
				//	MessageBox(hwnd,"we had an midi out (feedback) error before!","error",MB_OK);
				//}

				if (sendFeedback && (midiOutOpenResult == MMSYSERR_NOERROR) && REAL_TIME)
				{					
					DWORD midiMessage;
					int nIter = sizeof(DWORD) / sizeof(dup_uint8);
					for(int i=0;i<nIter; ++i)
					{
						((dup_uint8*)(&midiMessage))[i] = sourceMidiMsg->data[i];
					}
					//for(int i=0;i<nIter; ++i)
					//{
					//	((dup_uint8*)(&midiMessage))[nIter-1-i] = sourceMidiMsg->data[i];
					//}
					MMRESULT midiOutSendResult = midiOutShortMsg(hMidiOut, midiMessage);
					if (midiOutSendResult != MMSYSERR_NOERROR) MessageBox(hwnd,"midi out (feedback ) send error","error",MB_OK);
					//else MessageBox(hwnd,"main.cpp: sending feedback message","ok",MB_OK);
				}

				
				
				delete sourceMidiMsg;
				
			}
			//midiReceiver->receiveEvent(data, 0);
			
			go = false;
			if (anythingOnMidiQueue)
			{
				haveMutex = (WaitForSingleObject(midiQueueHMutex, 0) == WAIT_OBJECT_0);
				if (haveMutex) go = !midiQueue->empty();
			}
		}
		if (haveMutex)
		{
			anythingOnMidiQueue = false;
			haveMutex = !ReleaseMutex(midiQueueHMutex);			
		}
		assert(!haveMutex);

		//MessageBox(hwnd,"a mix midi message","ok",MB_OK);
	}
}


/* This function will initialize Direct Sound */
/* it returns TRUE if it was successful or FALSE otherwise */

BOOL Sound_Init(void)
{
    if (DS_OK == DirectSoundCreate(NULL,&lpDS,NULL)) //create direct sound object
    {
        //ok, DirectSound Object created, let's take control now...
        if (DS_OK == IDirectSound_SetCooperativeLevel(lpDS, hwnd, DSSCL_PRIORITY))
        {
            //MessageBox(hwnd,"Cooperative Level Succefully Set!","Good!!",MB_OK);
            return TRUE;
        }
        else //SetCoop was unsuccessful, let's give up control
        {
            IDirectSound_Release(lpDS);
            MessageBox(hwnd,"Set Cooperative Level didn't Work!","BAD!!",MB_OK);
            return FALSE;
        }
    }
    else  //DSObj creation was unsuccessful
    {
        MessageBox(hwnd,"Could Not Create Direct Sound Object","Error!",MB_OK);
        return (FALSE);
    }
	
	return TRUE;
}

/*here's a simple function to shut down direct sound when you're done with it */
void Sound_Exit(void)
{
 /*   if (lpDS)
    {
        IDirectSound_Release(lpDS);
    }
    else
    {
        MessageBox(hwnd,"Sound was not initialized and cannot Exit!",
                "Error!",MB_OK);
    }*/
}


/*
 * create a sound buffer (primary or secondary)
 * Volume can always be controlled
 */
bool createSoundBuffer(LPDIRECTSOUNDBUFFER *m_pDSbuffer, bool primaryBuff, bool cntrlFreq, int nSamples,
					   int nChannels, int sampleRate, int resolution)
{

	
	// setting the WAVEFORMATEX structure

	LPWAVEFORMATEX lpwfx = new WAVEFORMATEX;

	lpwfx->wFormatTag = WAVE_FORMAT_PCM;
	lpwfx->nChannels = nChannels;
	lpwfx->nSamplesPerSec = sampleRate;
	lpwfx->wBitsPerSample = resolution;
	lpwfx->nBlockAlign = nChannels * resolution / 8;
	lpwfx->nAvgBytesPerSec = sampleRate * lpwfx->nBlockAlign;
	lpwfx->cbSize = 0;
	



	// setting the DSBUFFERDESC structure

	DSBUFFERDESC *dsBuffDesc = new DSBUFFERDESC;

	int freqCntrl = 0;	
	if (cntrlFreq) {
		freqCntrl = DSBCAPS_CTRLFREQUENCY;
	}
	
	int isPrimaryBuff = 0;
	if (cntrlFreq) {
		isPrimaryBuff = DSBCAPS_PRIMARYBUFFER;
	}
		
	dsBuffDesc->dwSize = sizeof(DSBUFFERDESC);
	dsBuffDesc->dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
	if (cntrlFreq) dsBuffDesc->dwFlags = dsBuffDesc->dwFlags | DSBCAPS_CTRLFREQUENCY;
	if (isPrimaryBuff) dsBuffDesc->dwFlags = dsBuffDesc->dwFlags | DSBCAPS_PRIMARYBUFFER;
	
	dsBuffDesc->dwBufferBytes = lpwfx->nBlockAlign * nSamples;
	dsBuffDesc->dwReserved = 0;
	dsBuffDesc->lpwfxFormat = lpwfx;


	//try to create the sound buffer
	

	if (FAILED(lpDS->CreateSoundBuffer(dsBuffDesc, m_pDSbuffer, NULL))) {
		MessageBox(hwnd,"Could Not Create Sound Buffer","Error!",MB_OK);
		return false;
	}
	return true;	

}



void* CreatePixels(int Width, int Height, int Depth);

void InitDC(HWND hwnd, HDC* hDC, bool openGL = false);
void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

const char g_szClassName[] = "myDirectSoundTest2";
dup_uint8 keyTable[256];
bool keyPlaying[256];
bool keyNumber[256];

void setupKeyTable()
{
	
	for(int i=0;i<256;i++)
	{
		keyNumber[i] = false;
		keyPlaying[i] = false;
		keyTable[i] = 0;
	}
		
	keyNumber[49] = true; keyTable[49] = 59; //1
	keyNumber[50] = true; keyTable[50] = 61; //2
	keyNumber[51] = true; keyTable[51] = 63; //3
	keyNumber[52] = true; keyTable[52] = 61; //4
	keyNumber[53] = true; keyTable[53] = 66; //5
	keyNumber[54] = true; keyTable[54] = 68; //6
	keyNumber[55] = true; keyTable[55] = 70; //7
	keyNumber[56] = true; keyTable[56] = 72; //8
	keyNumber[57] = true; keyTable[57] = 73; //9
	keyNumber[48] = true; keyTable[48] = 75; //0
	keyNumber[58] = true; keyTable[58] = 61; //+
	
	keyTable[97] = 51; //a
	keyTable[98] = 60; //b
	keyTable[99] = 60; //c
	keyTable[100] = 53; //d
	keyTable[101] = 64; //e
	keyTable[102] = 54; //f
	keyTable[103] = 55; //g
	keyTable[104] = 56; //h
	keyTable[105] = 72; //i
	keyTable[106] = 57; //j
	keyTable[107] = 58; //k
	keyTable[108] = 59; //l
	keyTable[109] = 60; //m
	keyTable[110] = 60; //n
	keyTable[111] = 74; //o
	keyTable[112] = 76; //p
	keyTable[113] = 60; //q
	keyTable[114] = 65; //r
	keyTable[115] = 52; //s
	keyTable[116] = 67; //t
	keyTable[117] = 71; //u
	keyTable[118] = 60; //v
	keyTable[119] = 62; //w
	keyTable[120] = 60; //x
	keyTable[121] = 69; //y
	keyTable[122] = 60; //z

	keyTable[168] = 79; //¨

	keyTable[128] = 77; //å
	
}

int keyTrans = 0;
char *inputBuffer;
int currentInput;
static bool input;
//the window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool noteDown = false;
	//static bool chooseInstr = false; 
	//static bool chooseBank = false;	
	static dup_uint8 *data = NULL;
	//static int patchBank = 0;
	static int intrChooseState = 0;
	static int patchAccumulator;
	
	initMidiQueue();
	
	

	//unsigned int timerOK;
	switch(msg)
	{
		
		case WM_DESTROY:				// Window is being destroyed, cleanup
			PostQuitMessage(0);			// Tell the application to terminate after the window is gone.
			closed = true;
			
		break;
		case 258:
			
			//if (chooseInstr)
			//{
			//	int patch = wParam - 97;
			//	patch += patchBank*26;

			//	*(data+1) = patch;
			//	midiReceiver->receiveEvent(data, 0, N_CHANNELS);
			//	chooseInstr = false;
			//}
			//else if (chooseBank)
			//{
			//	patchBank = wParam - 97;
			//	chooseBank = false;
			//}
			if (intrChooseState > 0) 
			{
				if (wParam >= 48 && wParam <= 57)
				{
					int number = wParam - 48;
					
					patchAccumulator += number*intrChooseState;
					intrChooseState /= 10;

					if (intrChooseState == 0)
					{
						if (patchAccumulator < 0) patchAccumulator = -patchAccumulator;
						*(data+1) = patchAccumulator % 128;
						midiReceiver->receiveEvent(data, 0, N_CHANNELS);
						delete[] data;
						data = NULL;
					}
				}
				else
				{
					intrChooseState = 0;
				}
			}			
			else if (wParam == 39) // (*) patch select
			{
				if(data != NULL) delete[] data;
				//chooseInstr = true;
				data = new dup_uint8[2];
				*data = 0xC0;				
				intrChooseState = 100;
				patchAccumulator = 0;
			}
			else if (wParam == 108) // (l) save instrument info
			{
				// channel=0
				//voise=1
				SCSPlayer *SCSP = (SCSPlayer *)(mH->getModule(0*100+10*1+5)); // get first channel player

				SoundCollector *sc = NULL;
				if (SCSP) sc = SCSP->getSoundCollector();

				if (sc)
				{
					char numb[2];
					numb[1] = '\0';

					for(int i=0; i<sc->InstrumentSize(); ++i)
					{
						Instrument *instr = sc->getInstrument(0);

						if (instr)
						{
							numb[0] = i + 48;
							std::string nStr(numb);
							std::string str1("instr_descr_");
							std::string fileName(str1 + nStr + ".txt");
							instr->printToFile(fileName);
						}
					}
				}
			}			
			else if (wParam == 45) // octave +
			{
				keyTrans = keyTrans+12;				
			}
			else if (wParam == 46) // octave -
			{
				keyTrans = keyTrans-12;				
			}
			else if (wParam == 99) // (c) debug print last midi msg
			{
				midiReceiver->debugPrintLastMsg(hwnd);
			}			
			else if (wParam == 109) // (m) save recordbuffer to disk
			{
				saveRecord = true;				
			}
			else if (wParam == 118) // output basefrequency
			{				
				MessageBox(hwnd,inttostring(baseFre),"BaseFrequency:",MB_OK);				
			}
			else if (wParam == 98) // (b) change basefrequency -
			{				
				baseFre = baseFre - BASE_FRE_STEP;
				FoundTone = baseFre;				
			}
			else if (wParam == 110) // (n) change basefrequency +
			{				
				baseFre = baseFre + BASE_FRE_STEP;
				FoundTone = baseFre;
			}
			//else if (wParam == 120) // bank select
			//{
			//	chooseBank = true;
			//}
			else if (wParam == 122) // (z) all notes off
			{
				for(int i=0; i<16; ++i)
				{
					for(int j=0; j<127; ++j)
					{
						data = new dup_uint8[3];
						*data = 0x80 | i;
						*(data+1) = j;
						*(data+2) = 100;
						midiReceiver->receiveEvent(data, 0, N_CHANNELS);
					}
				}
			}
			else if (!keyPlaying[wParam])
			{
				data = new dup_uint8[3];
				*data = 0x90 | (KEYBOARD_CHANNEL-1);
				
				keyPlaying[wParam] = true;				
				if (wParam == 228) *(data+1) = 127; // clear direct sound buffer
				else *(data+1) = keyTable[wParam]+keyTrans;
				*(data+2) = 100;
				midiReceiver->receiveEvent(data, 0, N_CHANNELS);

				if (DOUBLE_KEYBOARD_CHANNELS)
				{
					*data = 0x90 | KEYBOARD_CHANNEL;
					*(data+1) = *(data+1) + KEYBOARD_2_TRANS;
					midiReceiver->receiveEvent(data, 0, N_CHANNELS);
				}



				noteDown = true;
			}
			
		break;
		case 257:
		if (intrChooseState == 0)
		//if (!chooseInstr && !chooseBank)
		{
			
			int offset;		
			data = new dup_uint8[3];
			*data = 0x80 | (KEYBOARD_CHANNEL-1);
			if (keyNumber[wParam] == true) offset = 0;
			else  offset = 32;			
			if (wParam == 222)
			{
				*(data+1) = 127;
				keyPlaying[228] = false;
			}
			else
			{
				*(data+1) = keyTable[wParam+offset]+keyTrans;
				keyPlaying[wParam+offset] = false;
			}
			*(data+2) = 100;
			midiReceiver->receiveEvent(data, 0, N_CHANNELS);			

			if (DOUBLE_KEYBOARD_CHANNELS)
			{
				*data = 0x80 | KEYBOARD_CHANNEL;
				*(data+1) = *(data+1) + KEYBOARD_2_TRANS;
				midiReceiver->receiveEvent(data, 0, N_CHANNELS);
			}
		}
		break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			closed = true;
		break;
		case WM_QUIT:
			DestroyWindow(hwnd);
			closed = true;
		break;
		default:
		{
			if (noteDown == true)
			{
				noteDown = false;
			}

			
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		break;
	}
	return 0;
}

class TestClass
{
	int h;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// if (REAL_TIME) SetPriorityClass(GetCurrentProcess(), 0x00008000); // ABOVE_NORMAL_PRIORITY_CLASS
	// else SetPriorityClass(GetCurrentProcess(), 0x00004000); // BELOW_NORMAL_PRIORITY_CLASS
	//if (REAL_TIME) ; //SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
	//else 
	if (!REAL_TIME) SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	if ( ! CHANNEL_SPECIFIC_N_VOICES)
	{
		for (int i=0; i<16; ++i)
		{
			CHANNEL_N_VOICES[i] = N_VOICES;
		}
	}

	
	//CHANNEL_SPECIFIC_N_VOICES

	
	double panLeftChannel = -1.0;
	double panRightChannel = 1.0;
	
	
	closed = false;
	saveRecord  = false;
	
	// keyboardPlay init
	setupKeyTable();
	inputBuffer = new char[1024];
	currentInput = 0;
	input = true;
	keyTrans = 0;
	transpose = TRANSPOSE;

	midiReceiver = new DS2MidiReceiver(DRUM_CHANNEL, MIDI_RECEIVER, SYNTH_FRE, CHANNEL_N_VOICES, N_DRUMS, REAL_TIME, false);
	errorInMidiFile = false;
	
	
	// ---------------------  win32 window ----------------------------------
		


		WNDCLASSEX wc;

		MSG Msg;

		//registrering the windows class
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = g_szClassName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, "window register failed", "Error", MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}

	//MessageBox(hwnd,"forward 02","ok",MB_OK);	

		// creating window
		hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName,"Duplo Synth - SCS",WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
			CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

		if(hwnd == NULL)
		{
			MessageBox(NULL, "window creation failed", "Error", MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}		
		
		//ShowWindow(hwnd, nCmdShow);
		ShowWindow(hwnd, SW_SHOW);

		
		
		InitDC(hwnd, &hDC);
		//HGLRC hRC;
		//EnableOpenGL(hwnd, &hDC, &hRC);


		//glClearColor(0.0, 0.0, 0.0, 0.0);	
		//glClear(GL_COLOR_BUFFER_BIT);


	//void* pixels = CreatePixels(20,20,4);


	

	// ---------------------  Direct sound + midi ----------------------------------


	
	m_pDSbuffer = NULL;
	
	bool soundInitOK = Sound_Init();
	
	bool BufferOK = false;

	sampleRate = SAMPLE_RATE;
	int nChannels = 2;
	int buffLength = BUFFLENGTH;
	nSamples = (sampleRate*buffLength)/1000;	
	int resolution = 16;
	int buffSize = (resolution*nSamples*nChannels)/8;
	//char *filename = "1869.wav";

	//loadLength = sampleRate*nChannels*resolution*30;
	
	

	//MessageBox(hwnd,"hello test1","ok",MB_OK);
	
	
	//MessageBox(hwnd,"forward 03","ok",MB_OK);
	
	if (soundInitOK && REAL_TIME)
	{
		BufferOK = createSoundBuffer(&m_pDSbuffer, false, false, nSamples, nChannels, sampleRate, resolution);
		//MessageBox(NULL, unitestrings("LPDIRECTSOUNDBUFFER: ", inttostring((int)m_pDSbuffer)), "Adress1", MB_ICONEXCLAMATION | MB_OK);		

		////tmp test
		//int size = sizeof(TestClass);
		//char tmpChars[2];
		//tmpChars[0] = '0' + size;
		//tmpChars[1] = '\0';
		//MessageBox(NULL, tmpChars, "mmm size", MB_ICONEXCLAMATION | MB_OK);
	}	

	LPVOID lpvAudio1; 
    DWORD  dwBytes1;

	MMRESULT midiInOpenResult = 0;	
	HMIDIIN hMidiIn;
	HMIDIIN hMidiInMix;

	

	//MessageBox(hwnd,"hello test2","ok",MB_OK);

	if (BufferOK)
	{			
		if ( FAILED( m_pDSbuffer->Lock( 
            0,              // Offset of lock start. 
            0,              // Size of lock; ignored in this case. 
            &lpvAudio1,     // Address of lock start. 
            &dwBytes1,      // Number of bytes locked. 
            NULL,           // Address of wraparound start. 
            NULL,           // Number of wraparound bytes. 
            DSBLOCK_ENTIREBUFFER ) ) )  // Flag. 
		{ 
			// Error handling. 
			MessageBox(NULL, "locking Soundbuffer failed, will not play any sound", "Error", MB_ICONEXCLAMATION | MB_OK);
			goto error;
		}
	}
	
	{
		{

			//MessageBox(hwnd,"hello test3","ok",MB_OK);

			// duplo
			Dup::timePos = 0.0;
			Dup::beatPos = 0.0;

			int waveFre = SYNTH_FRE;
			timeStep = 1.0/waveFre;
			//dup_uint32 buffSize = waveFre*8;


			//ModuleHandler moduleH();
			mH = new Dup::ModuleHandler(1);
			unsigned char *midiData;
			int midiDataLength = 0;
			
			if (USE_MIDI_FILE)
			{
				char midiFileName[256];
				//char *midiFileName = "noff_remix.mid";
				
				
				strncpy(midiFileName, MIDI_FILE_NAME, 255);
				
				//MessageBox(NULL, inttostring(strlen(MIDI_FILE_NAME)), "midifilename", MB_ICONEXCLAMATION | MB_OK);		
				midiDataLength = filelength(&(midiFileName[0]));
				
				
				midiData = (unsigned char *)loadstring(midiFileName, midiDataLength);

				
			}
			else midiData = (unsigned char *)"";
			//(Dup::MidiEventReceiver *)
			midiFileParser = new Dup::MidiFileParser(waveFre, N_CHANNELS, (Dup::MidiEventReceiver *)midiReceiver, midiData, midiDataLength, false, mH, 1);


			// ------------  SCS INIT and intrument --------------


			//FoundTone = Frequency(Frequency(220.0, 0, 3),0,0);
			SetSineTable(SINE_TABLE_SIZE, ExactSin);
			FoundTone = baseFre;
			Tempo = 110.0;
			
			GlobalVol = 20.0;

			PlayStep = CovFunc(1.0/(double)waveFre, Tempo);
			TimeStep = 1.0/waveFre;

			SoundCollector *soundCollector;
			
			Harp *SCSi1;

			// ---------------------------------------------------






			
			// constructordata2 for simpleDirectSound
			int interval = 2;	
			Dup::MessageConstructor *msgC = new Dup::MessageConstructor(sizeof(nSamples)+sizeof(interval));
			msgC->addData(&nSamples, sizeof(nSamples));
			msgC->addData(&interval, sizeof(interval));
			char *data2 = (char *)lpvAudio1;
			dupDS = new SimpleDirectSound(mH, 2, msgC->getParser(), data2, data2+2);

			dupDS->setVol(DS_VOLUME);
			if (!mH->addModule(dupDS)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
			
			delete msgC;
			msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
			msgC->addData32(((dup_uint32)(sampleRate/(waveFre/HIGH_CUT_DIVISION))));
			msgC->addData32(HIGH_CUT_ORDER);
			//msgC->addData32(1);
			multiLowPass = new MultiLowPass(mH, 10, msgC->getParser());
			if (!mH->addModule(multiLowPass)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
			mH->connectModules(multiLowPass->getID(), multiLowPass->Y_OUTPUT_L, dupDS->getID(), dupDS->WAVE_INPUT_L);
			mH->connectModules(multiLowPass->getID(), multiLowPass->Y_OUTPUT_R, dupDS->getID(), dupDS->WAVE_INPUT_R);

			delete msgC;
			msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
			msgC->addData32(waveFre/LOW_CUT_FRE);
			HighPass *highPass = new HighPass(mH, 3, msgC->getParser());
			if (!mH->addModule(highPass)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);			
			mH->connectModules(highPass->getID(), highPass->Y_OUTPUT_L, multiLowPass->getID(), multiLowPass->Y_INPUT_L);
			mH->connectModules(highPass->getID(), highPass->Y_OUTPUT_R, multiLowPass->getID(), multiLowPass->Y_INPUT_R);

			

			if (RECORD_SONG)
			{
				dup_uint32 waveBuffSize = ((__int64)WAVE_FILE_LENGTH * (__int64)sampleRate)/1000;
				
				//Dup::MessageConstructor *msgC;
				delete msgC;
				msgC = new Dup::MessageConstructor(sizeof(waveBuffSize));				
				msgC->addData(&waveBuffSize, sizeof(waveBuffSize));			
				recordBuff = new StereoWaveBuffer(mH, 4, msgC->getParser());
				recordBuff->setVol(WAVE_FILE_VOL);
				if (!mH->addModule(recordBuff)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);								
				mH->connectModules(multiLowPass->getID(), multiLowPass->Y_OUTPUT_L, recordBuff->getID(), recordBuff->WAVE_INPUT_L);
				mH->connectModules(multiLowPass->getID(), multiLowPass->Y_OUTPUT_R, recordBuff->getID(), recordBuff->WAVE_INPUT_R);
				recordBuff->record();
			}

			


			Dup::MessageParser dummyParser(NULL, 0, 0x80);


			
			





			// ---- voices ----

			/*
			NotePlayer *notePlayer;
			TriSawOscillator *osc;
			TimeEnv1 *env;
			Decay *dec;
			*/

			SCSPlayer *SCSP;

			dup_moduleid tmpID;
			ChannelPlayer *channelPlayer;


			for(int channel = 0; channel<N_CHANNELS; channel++)
			{



				
				if (channel != DRUM_CHANNEL/100) // if not drum channel
				{
					
					
					// ---- reverb ----
					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					msgC->addData32((waveFre*(95+channel))/1000);
					Delay *delL1 = new Delay(mH, 2000+channel*100+6, msgC->getParser());
					delL1->setInputBaseVal(delL1->AMP_INPUT, 0.25);

					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					msgC->addData32((waveFre*213+60*((channel%3)-1))/1000);
					Delay *delR1 = new Delay(mH, 2000+channel*100+7, msgC->getParser());
					delR1->setInputBaseVal(delR1->AMP_INPUT, 0.3);

					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					msgC->addData32((waveFre*270)/1000);
					Delay *delL2 = new Delay(mH, 2000+channel*100+8, msgC->getParser());
					delL2->setInputBaseVal(delL2->AMP_INPUT, 0.15 * 0.04*((double)((channel%3)-1)));

					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					msgC->addData32((waveFre*755)/1000);
					Delay *delR2 = new Delay(mH, 2000+channel*100+9, msgC->getParser());
					delR2->setInputBaseVal(delR2->AMP_INPUT, 0.09);

					// controls amount of reverb
					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					//msgC->addData32(8);
					//Delay *delL3 = new Delay(mH, 2000+channel*100+16, msgC->getParser());
					Passer *delL3 = new Passer(mH, 2000+channel*100+16, msgC->getParser());
					delL3->setInputBaseVal(delL3->AMP_INPUT, 1.0);

					// controls amount of reverb
					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					//msgC->addData32(8);
					//Delay *delR3 = new Delay(mH, 2000+channel*100+17, msgC->getParser());
					Passer *delR3 = new Passer(mH, 2000+channel*100+17, msgC->getParser());
					delR3->setInputBaseVal(delR3->AMP_INPUT, 1.0);

					// controls amount of directly feeded sound
					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					//msgC->addData32(8);
					//Delay *delL4 = new Delay(mH, 2000+channel*100+18, msgC->getParser());
					Passer *delL4 = new Passer(mH, 2000+channel*100+18, msgC->getParser());
					delL4->setInputBaseVal(delL4->AMP_INPUT, 1.0);

					// controls amount of directly feeded sound
					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					//msgC->addData32(8);
					//Delay *delR4 = new Delay(mH, 2000+channel*100+19, msgC->getParser());
					Passer *delR4 = new Passer(mH, 2000+channel*100+19, msgC->getParser());
					delR4->setInputBaseVal(delR4->AMP_INPUT, 1.0);

					
					// final compressor
					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					msgC->addData32(SYNTH_FRE);
					Compressor *compressor = new Compressor(mH, 2000+channel*100+21, msgC->getParser());
					//compressor->setInputBaseVal(compressor->Y_INPUT_L, 1.0);
					//compressor->setInputBaseVal(compressor->Y_INPUT_R, 1.0);




					if (!mH->addModule(delL1)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					if (!mH->addModule(delL2)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					if (!mH->addModule(delL3)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					if (!mH->addModule(delL4)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					if (!mH->addModule(delR1)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					if (!mH->addModule(delR2)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					if (!mH->addModule(delR3)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					if (!mH->addModule(delR4)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);					
					if (!mH->addModule(compressor)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					
					
					//std::cout << "place 1" << std::endl;
					

					// Left delay connections			
					delL1->getOutput(delL1->Y_OUTPUT)->connect(delL1->getInput(delL1->Y_INPUT));
					delL1->getOutput(delL1->Y_OUTPUT)->connect(delL3->getInput(delL3->Y_INPUT));
					delL2->getOutput(delL2->Y_OUTPUT)->connect(delL2->getInput(delL2->Y_INPUT));
					delL2->getOutput(delL2->Y_OUTPUT)->connect(delL3->getInput(delL3->Y_INPUT));
					
					// Right delay connections			
					delR1->getOutput(delR1->Y_OUTPUT)->connect(delR1->getInput(delR1->Y_INPUT));
					delL1->getOutput(delR1->Y_OUTPUT)->connect(delR3->getInput(delR3->Y_INPUT));
					delR2->getOutput(delR2->Y_OUTPUT)->connect(delR2->getInput(delR2->Y_INPUT));
					delL2->getOutput(delR2->Y_OUTPUT)->connect(delR3->getInput(delR3->Y_INPUT));
					
					// back and forth			
					delR1->getOutput(delR1->Y_OUTPUT)->connect(delL1->getInput(delL1->Y_INPUT));
					delR1->getOutput(delR1->Y_OUTPUT)->connect(delL2->getInput(delL2->Y_INPUT));
					delR1->getOutput(delR1->Y_OUTPUT)->connect(delR2->getInput(delR2->Y_INPUT));
					delL1->getOutput(delL1->Y_OUTPUT)->connect(delR1->getInput(delR1->Y_INPUT));
					delL1->getOutput(delL1->Y_OUTPUT)->connect(delR2->getInput(delR2->Y_INPUT));
					delL1->getOutput(delL1->Y_OUTPUT)->connect(delL2->getInput(delL2->Y_INPUT));

					delR2->getOutput(delR2->Y_OUTPUT)->connect(delL2->getInput(delL2->Y_INPUT));
					delR2->getOutput(delR2->Y_OUTPUT)->connect(delL1->getInput(delL1->Y_INPUT));
					delR2->getOutput(delR2->Y_OUTPUT)->connect(delR1->getInput(delR1->Y_INPUT));
					delL2->getOutput(delL2->Y_OUTPUT)->connect(delR2->getInput(delR2->Y_INPUT));
					delL2->getOutput(delL2->Y_OUTPUT)->connect(delR1->getInput(delR1->Y_INPUT));
					delL2->getOutput(delL2->Y_OUTPUT)->connect(delL1->getInput(delL1->Y_INPUT));
					
					// Output of reverb output
					delL3->getOutput(delL3->Y_OUTPUT)->connect(compressor->getInput(compressor->Y_INPUT_L));
					delR3->getOutput(delR3->Y_OUTPUT)->connect(compressor->getInput(compressor->Y_INPUT_R));

					// Output of directly feeded output
					delL4->getOutput(delL4->Y_OUTPUT)->connect(compressor->getInput(compressor->Y_INPUT_L));
					delR4->getOutput(delR4->Y_OUTPUT)->connect(compressor->getInput(compressor->Y_INPUT_R));

					// Output of compressor
					compressor->getOutput(compressor->Y_OUTPUT_L)->connect(highPass->getInput(highPass->Y_INPUT_L));
					compressor->getOutput(compressor->Y_OUTPUT_R)->connect(highPass->getInput(highPass->Y_INPUT_R));


					//// Channel Lowpass Filter
					//delete msgC;
					//msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
					////msgC->addData32(waveFre / 22050);
					//msgC->addData32((dup_uint32)(waveFre / 44100));
					//msgC->addData32(HIGH_CUT_ORDER);
					//MultiLowPass *chanFilter = new MultiLowPass(mH, 2000+channel*100+20, msgC->getParser());
					//if (!mH->addModule(chanFilter)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
					//mH->connectModules(chanFilter->getID(), chanFilter->Y_OUTPUT_L, delL1->getID(), delL1->Y_INPUT);
					//mH->connectModules(chanFilter->getID(), chanFilter->Y_OUTPUT_R, delR1->getID(), delR1->Y_INPUT);
					//mH->connectModules(chanFilter->getID(), chanFilter->Y_OUTPUT_L, delL4->getID(), delL4->Y_INPUT);
					//mH->connectModules(chanFilter->getID(), chanFilter->Y_OUTPUT_R, delR4->getID(), delR4->Y_INPUT);
					
					
					// msg constructor for channel player
					delete msgC;
					msgC = new Dup::MessageConstructor(sizeof(dup_uint16)+sizeof(dup_moduleid)*(*(CHANNEL_N_VOICES+channel)));			
					msgC->addData16(*(CHANNEL_N_VOICES+channel));
								

					//for(int voice = 1;(voice<=CHANNEL_N_VOICES[channel] && CHANNEL_SPECIFIC_N_VOICES) || (voice<=N_VOICES && !CHANNEL_SPECIFIC_N_VOICES);voice++)
					for(int voice = 1;voice<=CHANNEL_N_VOICES[channel] ;voice++)
					{

						SCSP = new SCSPlayer(mH, channel*100+10*voice+5, dummyParser);
						//SCSP->setSoundCollector(new SoundCollector());
						SCSP->setVolMod(NORMAL_SCSP_VOL);
						tmpID = SCSP->getID();
						msgC->addData(&tmpID, sizeof(tmpID));
						if (!mH->addModule(SCSP)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
						//mH->connectModules(SCSP->getID(), SCSP->L_OUTPUT, highPass->getID(), highPass->Y_INPUT_L);
						//mH->connectModules(SCSP->getID(), SCSP->R_OUTPUT, highPass->getID(), highPass->Y_INPUT_R);
						mH->connectModules(SCSP->getID(), SCSP->L_OUTPUT, delL1->getID(), delL1->Y_INPUT);
						mH->connectModules(SCSP->getID(), SCSP->R_OUTPUT, delR1->getID(), delR1->Y_INPUT);						
						mH->connectModules(SCSP->getID(), SCSP->L_OUTPUT, delL4->getID(), delL4->Y_INPUT);
						mH->connectModules(SCSP->getID(), SCSP->R_OUTPUT, delR4->getID(), delR4->Y_INPUT);
						//mH->connectModules(SCSP->getID(), SCSP->L_OUTPUT, chanFilter->getID(), chanFilter->Y_INPUT_L);
						//mH->connectModules(SCSP->getID(), SCSP->R_OUTPUT, chanFilter->getID(), chanFilter->Y_INPUT_R);
						

					}
					// channelplayer
					channelPlayer = new ChannelPlayer(mH, channel*100+5, msgC->getParser());			
					if (!mH->addModule(channelPlayer)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);
				}

			}

			//MessageBox(NULL, "place2", "ok", MB_ICONEXCLAMATION | MB_OK);

			// --- drums ---
			int nDrums;
			std::vector<std::string> drumSampleLines;
			if (USE_DRUM_SAMPLER)
			{				
				std::ifstream drumSamplesDescriptionFile(DRUM_SAMPLES_DESCRITION_FILENAME);
				if (drumSamplesDescriptionFile.good())
				{
					std::string line;
					while (std::getline(drumSamplesDescriptionFile, line)) 
					{
						if (line.length() > 0)
						{
							if (line[0] != '#') drumSampleLines.push_back(line);
						}
					}
				}
				else
				{
					MessageBox(NULL, "Cannot find drum samples description file!", "Error", MB_ICONEXCLAMATION | MB_OK);
				}

				nDrums = drumSampleLines.size();
			}
			else
			{
				nDrums = N_DRUMS;
			}

			
			for(int drum=1;drum <= nDrums;drum++)
			{
				SCSP = new SCSPlayer(mH, DRUM_CHANNEL+5+drum, dummyParser);				
				soundCollector = new SoundCollector();
				SCSP->setSoundCollector(soundCollector);
				SCSP->setVolMod(0.4);
				mH->addModule(SCSP);
				mH->connectModules(SCSP->getID(), SCSP->L_OUTPUT, highPass->getID(), highPass->Y_INPUT_L);
				mH->connectModules(SCSP->getID(), SCSP->R_OUTPUT, highPass->getID(), highPass->Y_INPUT_R);
				
			}

			//MessageBox(hwnd,"forward 04","ok",MB_OK);

			//MessageBox(NULL, "place2.5", "ok", MB_ICONEXCLAMATION | MB_OK);
			
			NoteDrum *noteDrum = new NoteDrum(mH, DRUM_CHANNEL+5, dummyParser);
			if (!mH->addModule(noteDrum)) MessageBox(NULL, "module id taken!", "Error", MB_ICONEXCLAMATION | MB_OK);

			dup_moduleid id = 5;

			if (USE_DRUM_SAMPLER)
			{
				//MessageBox(NULL, "drum sampler used!", "ok", MB_ICONEXCLAMATION | MB_OK);
				while(drumSampleLines.size() > 0)
				{
					id++;

					std::string line(drumSampleLines.back());					
					drumSampleLines.pop_back();
					std::istringstream lineStream(line);
					
					int note;
					std::string drumSampleFilename;
					double vol;

					lineStream >> note;
					lineStream >> drumSampleFilename;
					lineStream >> vol;

					SCSP = (SCSPlayer *)(mH->getModule(DRUM_CHANNEL+id));
					soundCollector = SCSP->getSoundCollector();
					SCSP->setVolMod(vol*DRUM_SAMPLER_SCSP_VOL);
					SoundPlayer *SCSi1 = new SoundPlayer(drumSampleFilename.c_str());
					soundCollector->setInstrument(SCSi1);
					soundCollector->addAP(&(SCSi1->AmpL), &panLeftChannel, soundCollector->getInstrumentUse());
					soundCollector->addAP(&(SCSi1->AmpR), &panRightChannel, soundCollector->getInstrumentUse());
					noteDrum->setData(note, DRUM_CHANNEL+id, 440, 0.0);					
				}				
			}
			else
			{

				if ( ! setupGMDrums(mH, DRUM_CHANNEL+id+1, noteDrum))
				{
					MessageBox(NULL, "Error: too many gm-drums created!", "ok", MB_ICONEXCLAMATION | MB_OK);
				}


			}

			
			//MessageBox(hwnd,"forward 05","ok",MB_OK);

			
			//MessageBox(NULL, "place3", "ok", MB_ICONEXCLAMATION | MB_OK);

				

				
				


			//RtMidiIn *midiin = new RtMidiIn();
			//std::vector<unsigned char> message;


			// Check available ports.
			//unsigned int nPorts = midiin->getPortCount();			
			//if ( nPorts == 0 ) {
			//	MessageBox(NULL, "no ports availible", "Error", MB_ICONEXCLAMATION | MB_OK);					
			//	//goto cleanup;
			//}
			//else
			//{
			//	if (nPorts <= 9)
			//	{
			//		nPorts = nPorts + 48;					
			//		if (SHOW_PORTS) MessageBox(NULL, (LPCSTR)&nPorts, "ports", MB_ICONEXCLAMATION | MB_OK);
			//	}
			//	else
			//	{
			//		if (SHOW_PORTS) MessageBox(NULL, ">=10", "ports", MB_ICONEXCLAMATION | MB_OK);
			//	}
			//	//std::cout << "n ports = " << nPorts << " \n";
			//}
			
			////_DMUS_PORTCAPS portInfo;
			////HRESULT returnVal;			
			////DWORD midiPortIdx;
			////int inputMidiPort = 0;
			////for(midiPortIdx=0; inputMidiPort <= MIDI_PORT; ++midiPortIdx)
			////{				
			////	returnVal = EnumPort(midiPortIdx, &portInfo);
			////	if (returnVal != S_OK) break;
			////	if (portInfo.dwClass == DMUS_PC_INPUTCLASS) ++inputMidiPort;
			////}
			////assert(inputMidiPort <= MIDI_PORT+1);
			////if (inputMidiPort == MIDI_PORT+1)
			////{
			////	// successfull
			////	_DMUS_PORTPARAMS8 portParams;
			////	portParams.dwSize = sizeof(portParams);
			////	portParams.dwValidParams = 0;

			////	LPDIRECTMUSICPORT port;
			////	if (CreatePort(&(portInfo.guidPort), &portParams, &port, NULL) == S_OK)
			////	{

			////	}
			////	else
			////	{
			////		MessageBox(NULL, "could not create midi port", "Error", MB_ICONEXCLAMATION | MB_OK);
			////	}

			////}
			////else
			////{
			////	MessageBox(NULL, "midi port not found", "Error", MB_ICONEXCLAMATION | MB_OK);
			////}

			if (!(MIDI_IN_PORT < midiInGetNumDevs())) MessageBox(NULL, "midi in port not found", "Error", MB_ICONEXCLAMATION | MB_OK);
			if (!(MIDI_IN_MIX_PORT < midiInGetNumDevs())) MessageBox(NULL, "midi in \"mix\" port not found", "Error", MB_ICONEXCLAMATION | MB_OK);
			if (!(MIDI_OUT_PORT < midiOutGetNumDevs())) MessageBox(NULL, "midi out port not found", "Error", MB_ICONEXCLAMATION | MB_OK);

			midiInOpenResult = midiInOpen(&hMidiIn, MIDI_IN_PORT,
											(DWORD)&midiInputCallback0, (DWORD)NULL,
											CALLBACK_FUNCTION);
			if (midiInOpenResult == MMSYSERR_NOERROR)
			{
				midiInStart( hMidiIn );
			}
			else MessageBox(NULL, "Error opening midi in port", "Error", MB_ICONEXCLAMATION | MB_OK);


			midiInOpenResult = midiInOpen(&hMidiInMix, MIDI_IN_MIX_PORT,
											(DWORD)&midiInputCallback1, (DWORD)NULL,
											CALLBACK_FUNCTION);
			if (midiInOpenResult == MMSYSERR_NOERROR)
			{
				midiInStart( hMidiInMix );
			}
			else MessageBox(NULL, "Error opening midi in mix port", "Error", MB_ICONEXCLAMATION | MB_OK);


			midiOutOpenResult = midiOutOpen(&hMidiOut, MIDI_OUT_PORT,
											(DWORD)NULL, (DWORD)NULL,
											CALLBACK_NULL);
			/*			
			if (midiOutOpenResult == MMSYSERR_NOERROR)
			{
				midiOutStart( hMidiOut );
			}
			else
			*/
			if (midiOutOpenResult != MMSYSERR_NOERROR) MessageBox(NULL, "Error opening midi out port", "Error", MB_ICONEXCLAMATION | MB_OK);



			//MessageBox(hwnd,"forward 06","ok",MB_OK);

			
			//MessageBox(NULL, "place4", "ok", MB_ICONEXCLAMATION | MB_OK);
			
			
			//midiin->openPort( MIDI_PORT );

			// Don't ignore sysex, timing, or active sensing messages.
			//midiin->ignoreTypes(false, false, false);
			

			double stamp;



			//m_pDSbuffer->Unlock(lpvAudio1, dwBytes1, NULL, 0); 
			

			// play soundbuffer

			//MessageBox(hwnd,"hello test1","ok",MB_OK);
			
			int hr = DS_OK;
			if (REAL_TIME)
			{
				int hr = playDSBuffer(m_pDSbuffer);
				if (hr == DSERR_BUFFERLOST)
				{
					MessageBox(NULL, "sound buffer was lost", "Error", MB_ICONEXCLAMATION | MB_OK);
				}
				else if (hr != DS_OK)
				{
					MessageBox(NULL, "unable to play", "Error", MB_ICONEXCLAMATION | MB_OK);
				}
			}
			if (hr == DS_OK)
			{
				startTime = GetTickCount();								
				currentTime = 0;
				//musicPos = 0;
				//newPos;
				if (REAL_TIME) pos = nSamples/2;
				else pos = 0;
				
				
				synthPos = pos*SYNTH_FRE/SAMPLE_RATE;



				//MessageBox(hwnd,"hello test2","ok",MB_OK);
		

				//MessageBox(NULL, "place5", "ok", MB_ICONEXCLAMATION | MB_OK);
				
				/*
				while(GetMessage(&Msg, NULL, 0, 0))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);

					glFlush();
					SwapBuffers(hDC);

					updateSound();
				}
				*/

				
				
				//MessageBox(hwnd, "okey", "running", MB_ICONEXCLAMATION | MB_OK);

				bool midiQueueDone = false;
				if (midiFileParser->errorStringExists())
				{
					errorInMidiFile = true;	
					MessageBox(hwnd,"error in midi file","error",MB_OK);
				}
				//MessageBox(NULL, "place6", "ok", MB_ICONEXCLAMATION | MB_OK);
				if (USE_MIDI_FILE && !errorInMidiFile)
				{
					//MessageBox(NULL, "place70", "ok", MB_ICONEXCLAMATION | MB_OK);
					if (!midiFileParser->parseFile())
					{
						errorInMidiFile = true;						
					}
					//MessageBox(NULL, "place71", "ok", MB_ICONEXCLAMATION | MB_OK);
					
					
					//if (REAL_TIME)
					//{
					//	if (!midiFileParser->parseFile())
					//	{
					//		errorInMidiFile = true;						
					//	}
					//}
					//else
					//{
					//	for (int i=0; i<16; ++i)
					//	{
					//		if (!midiFileParser->parseFile())
					//		{
					//			errorInMidiFile = true;
					//			break;
					//		}
					//	}
					//}
				}
				//MessageBox(NULL, "place72", "ok", MB_ICONEXCLAMATION | MB_OK);
				if (errorInMidiFile) MessageBox(hwnd, midiFileParser->getErrorString(),"error in midifile parsing",MB_OK);
#ifdef DEBUG_PRINT_RUNNING
				MessageBox(hwnd,"hello test3","ok",MB_OK);
#endif

				while(!closed)
				{
					
					
					if(PeekMessage(&Msg, 0,0,0,PM_REMOVE))
					{
						if(Msg.message == WM_QUIT) break;
						TranslateMessage(&Msg);
						DispatchMessage(&Msg);
					}
#ifdef DEBUG_PRINT_RUNNING
					MessageBox(hwnd,"hello test4","ok",MB_OK);
#endif
					updateSound();					
					//glFlush();
					//SwapBuffers(hDC);

					
					//midiQueueDone = false;
					//while(!midiQueueDone)
					//{
					//	double stamp = midiin->getMessage( &message );
					//	if (message.size() < 1)
					//	{
					//		midiQueueDone = true;
					//	}
					//	else
					//	{
					//		mycallback(0.0, &message, NULL);
					//	}
					//}
#ifdef DEBUG_PRINT_RUNNING				
					MessageBox(hwnd,"hello test5","ok",MB_OK);
#endif
					
					Sleep(4);
					//MessageBox(NULL, "place80", "ok", MB_ICONEXCLAMATION | MB_OK);

					if (currentTime >= STOP_TIME)
					{
						MessageBox(hwnd,"stop time!","ok",MB_OK);
						saveRecord = true;
						DestroyWindow(hwnd);						
					}
					if (saveRecord == true)
					{
						MessageBox(hwnd,"save record","ok",MB_OK);
						if (RECORD_SONG) recordBuff->saveToDisk(waveFileName, sampleRate);
						saveRecord = false;
					}
					

				}
 
 

				MessageBox(hwnd,"okey stopping","ok",MB_OK);
				

				if (REAL_TIME) m_pDSbuffer->Stop();

				

				

				mH->deleteAll();
				cleanupSineTable();
				if (RECORD_SONG == true)
				{
					MessageBox(hwnd, "Recordning song done","done!",MB_OK);
				}
				
				//delete[] loadData;
			}
			
		}		
	}

error:

	/*
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		glFlush();
		SwapBuffers(hDC);
	}
	*/

	if (midiInOpenResult == MMSYSERR_NOERROR)
	{
		midiInReset( hMidiIn );
		midiInStop( hMidiIn );
		midiInClose( hMidiIn );

		midiInReset( hMidiInMix );
		midiInStop( hMidiInMix );
		midiInClose( hMidiInMix );
	}

	if (midiOutOpenResult == MMSYSERR_NOERROR)
	{
		midiOutReset( hMidiOut );
		//midiOutStop( hMidiOut );
		midiOutClose( hMidiOut );
	}

	if (soundInitOK) Sound_Exit();
	//DisableOpenGL(hwnd, hDC, hRC);
	ReleaseDC(hwnd, hDC);


	return Msg.wParam;

}


void InitDC(HWND hwnd, HDC* hDC, bool openGL)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	// get device context
	*hDC = GetDC(hwnd);

	// set the pixel format for DC
	ZeroMemory(&pfd, sizeof(pfd));	
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = openGL ?
						PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER :
						(PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL);

	pfd.iPixelType = openGL ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
	pfd.cColorBits = openGL ? 24 : 1;
	
	
	pfd.cDepthBits = openGL ? 16 : 0; //set but only used if using opengl
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(*hDC, &pfd);
	bool result = SetPixelFormat(*hDC, iFormat, &pfd);
	if (! result) MessageBox(hwnd, "Settingpixel format failed!","error", MB_OK);

	
	//DWORD dwVersion;
	//WORD dwMajorVersion, dwMinorVersion, dwBuild;
	//dwVersion = GetVersion();

	//// Get the Windows version.
 //   dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
 //   dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	////dwMajorVersion = LOBYTE(LOWORD(dwVersion));
 //  // dwMinorVersion = HIBYTE(LOWORD(dwVersion));

 //   // Get the build number.
 //   if (dwVersion < 0x80000000)              
 //       dwBuild = (DWORD)(HIWORD(dwVersion));
	//   //dwBuild = HIWORD(dwVersion);
 //   else                                      // Windows Me/98/95
 //       dwBuild =  0;

	//
	//LOGPALETTE lPalette;
	//lPalette.palVersion = dwMajorVersion;
	//lPalette.palNumEntries = 2;
	////PALETTEENTRY entrys[2];
	////entrys[0].peRed = 0;
	////entrys[0].peBlue = 0;
	////entrys[0].peGreen = 0;
	////entrys[0].peFlags = NULL;
	////entrys[1].peRed = 0;
	////entrys[1].peBlue = 100;
	////entrys[1].peGreen = 255;
	////entrys[1].peFlags = NULL;
	//lPalette.palPalEntry[0].peRed = 0;
	//lPalette.palPalEntry[0].peBlue = 0;
	//lPalette.palPalEntry[0].peGreen = 0;
	//lPalette.palPalEntry[0].peFlags = 0;
	//palette = CreatePalette(&lPalette   /* logical palette */ );
	//if (palette == NULL) MessageBox(hwnd, "could not create a palette.","error", MB_OK);

}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{


	////Create enable reder context (RC)
	//*hRC = wglCreateContext(*hDC);
	//wglMakeCurrent( *hDC, *hRC);

	//InitDC(hwnd, hDC, true)
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	//wglMakeCurrent( NULL, NULL);
	//wglDeleteContext(hRC);
	//ReleaseDC(hwnd, hDC);
}

void* CreatePixels(int Width, int Height, int Depth)
{	
	int Size = Width * Height;
	void* Pixels = new char[Size*Depth];
	for(int i=0;i<Size;i++)
	{
		
		*((char*)Pixels+i*Depth) = 255;
		*((char*)Pixels+i*Depth+3) = 100;
	}
	return Pixels;
	return 0;
}

void updateSound()
{
	
	
	int newSynthPos;	

	if((GetTickCount() > currentTime+startTime) || (!REAL_TIME))
	{
		
		if (REAL_TIME)
		{
			currentTime = GetTickCount()-startTime;
			//newpos = ((currentTime*((__int64)sampleRate))/(1000)+(nSamples)/(2))%(nSamples);
			newPos = (currentTime*((__int64)sampleRate))/(1000)+(nSamples)/(2);
		}
		else
		{
			currentTime = currentTime+30;
			//newpos = ((currentTime*sampleRate)/1000)%nSamples;
			newPos = ((currentTime*((__int64)sampleRate))/1000)+1;
			
		}
		
		while (pos != newPos)
		{
			

			/*
			if ((nChannels*resolution)/8 == 1)
			{
				//MessageBox(hwnd, unitestrings("freq: ", inttostring(musicPos/sampleRate)),"message",MB_OK);
				*(Data+pos) = *(loadData+musicPos);
			}
			else if ((nChannels*resolution)/8 == 2)
			{
				*(Data+pos*2) = *(loadData+musicPos*2);
				*(Data+pos*2+1) = *(loadData+musicPos*2+1);
			}
			else if ((nChannels*resolution)/8 == 4)
			{
				*(Data+pos*4) = *(loadData+musicPos*4);
				*(Data+pos*4+1) = *(loadData+musicPos*4+1);
				*(Data+pos*4+2) = *(loadData+musicPos*4+2);
				*(Data+pos*4+3) = *(loadData+musicPos*4+3);
			}
			*/
			
			/*
			musicPos++;

			
			if (musicPos%100000 == 0)
			{
				
				//MessageBox(hwnd, "check msg","message",MB_OK);
				
			
				stamp = midiin->getMessage( &message );
				

				int nBytes = message.size();

				if (nBytes > 0)
				{
					MessageBox(hwnd, "ok midi","message",MB_OK);
				}
				
				//message.get
			
			}
			
			*/

			
			//while(systemTaken);
			//systemTaken = true;
			
			//MessageBox(hwnd,"running synth0","ok",MB_OK);

			newSynthPos = pos*SYNTH_FRE/sampleRate;
			if (newSynthPos > synthPos)
			{
				synthPos = newSynthPos;
#ifdef DEBUG_PRINT_RUNNING
				MessageBox(hwnd,"running synth1","ok",MB_OK);
#endif
				assert(mH->modulesNoneNull());
#ifdef DEBUG_PRINT_RUNNING
				MessageBox(hwnd,"running synth2","ok",MB_OK);
#endif
				mH->transferAll();
				assert(mH->modulesNoneNull());
				mH->dispatchMessages();
				assert(mH->modulesNoneNull());
				mH->runAllModules();
				assert(mH->modulesNoneNull());
				Dup::timePos = Dup::timePos+timeStep;
				mH->updateBeat();
				assert(mH->modulesNoneNull());
			}
			else
			{
				//MessageBox(hwnd,"running synth3","ok",MB_OK);
				dupDS->transferValues();
				if(RECORD_SONG) recordBuff->transferValues();
				multiLowPass->transferValues();

				dupDS->run();

				/*
				POINT points[9];
				
				(points+0)->x = 20;
				(points+0)->y = 10;
				
				(points+1)->x = 40;
				(points+1)->y = 100;
				
				(points+2)->x = 60;
				(points+2)->y = 20;
				
				(points+3)->x = 80;
				(points+3)->y = 100;
				
				(points+4)->x = 100;
				(points+4)->y = 30;
				
				(points+5)->x = 120;
				(points+5)->y = 100;

				(points+6)->x = 140;
				(points+6)->y = 40;

				(points+7)->x = 160;
				(points+7)->y = 100;

				(points+8)->x = 180;
				(points+8)->y = 50;
				
				//BOOL Polyline(
				//  HDC hdc,            // handle to device context
				//  CONST POINT *lppt,  // array of endpoints
				//  int cPoints         // number of points in array
				//);
				bool result = Polyline( 
					hDC,				// handle to device context
					points,				// array of endpoints
					9					// number of points in array
				);

				if (!result) MessageBox(hwnd,"could not draw lines","ok",MB_OK);

				*/

				//MessageBox(hwnd,"running synth4","ok",MB_OK);
				if(RECORD_SONG) recordBuff->run();
				multiLowPass->run();	
			}
#ifdef DEBUG_PRINT_RUNNING
			MessageBox(hwnd,"running synth5","ok",MB_OK);
#endif

			if (USE_MIDI_FILE && !errorInMidiFile)
			{
#ifdef DEBUG_PRINT_RUNNING
				MessageBox(hwnd,"running synth5-1","ok",MB_OK);
#endif
				bool stop = !(midiFileParser->parseContinue());
#ifdef DEBUG_PRINT_RUNNING
				MessageBox(hwnd,"running synth5-2","ok",MB_OK);
#endif
				if (stop) MessageBox(hwnd, "error in midifile","midifile",MB_OK);
				//if (midiFileParser->fileEnd()) MessageBox(hwnd, "midifile end","midifile",MB_OK);
				stop = (stop || midiFileParser->fileEnd()) && (!REAL_TIME);
#ifdef DEBUG_PRINT_RUNNING
				MessageBox(hwnd,"running synth5-3","ok",MB_OK);
#endif
				if (stop)
				{
#ifdef DEBUG_PRINT_RUNNING
					MessageBox(hwnd,"running synth5-4","ok",MB_OK);
#endif
					DestroyWindow(hwnd);
				}
#ifdef DEBUG_PRINT_RUNNING
				MessageBox(hwnd,"running synth5-5","ok",MB_OK);
#endif
			}

#ifdef DEBUG_PRINT_RUNNING
			MessageBox(hwnd,"running synth6","ok",MB_OK);
#endif
			//pos = (pos+1)%nSamples;
			pos++;
			
			//systemTaken = false;

			
		}

#ifdef DEBUG_PRINT_RUNNING
		MessageBox(hwnd,"finished running synth","ok",MB_OK);
#endif
	}

	playMidiQueue();

}

int playDSBuffer(LPDIRECTSOUNDBUFFER m_pDSbuffer)
{
	int hr = m_pDSbuffer->Play(0,0,PF_LOOPING);
	if(!FAILED(hr))
	{
		startTime = GetTickCount();								
		currentTime = 0;
		synthPos = 0;
		//musicPos = 0;
		newPos;
		if (REAL_TIME) pos = nSamples/2;
		else pos = 0;
	}
	return hr;
	return 0;
}
