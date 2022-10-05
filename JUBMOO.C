//====================================================================
//					Jubmoo for Window v.1.0a
//     start : 28/10/94
// 		v1.5			: 18/6/95
//			v1.0a			: 9/9/96
//====================================================================

// if defind DEBUG_ON , program will gen trace file with JM.deb
#define	DEBUG_OFF

#include <windows.h>
#include <ddeml.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <process.h>

#include "resource.h"
#include "jmalgr.h"
//======================= define structure ========================
struct PLAYER
{
	BYTE	PlayType;      // 0 as computer , 1 as player
	char	PlayName[16];  // player name
	BYTE	Card[14];     // card on hand
	short int	nScore;

} Player[4] =	{
				{ 0,"",{0,0,0,0,0,0,0,0,0,0,0,0,0,0},0 },

				{ 0,"",{0,0,0,0,0,0,0,0,0,0,0,0,0,0},0 },

				{ 0,"",{0,0,0,0,0,0,0,0,0,0,0,0,0,0},0 },

				{ 0,"",{0,0,0,0,0,0,0,0,0,0,0,0,0,0},0 }
				};

//=============== function declaration ======================
long FAR PASCAL WndProc (HWND, UINT, WORD, LONG);
int ChkCardPosition(int *,int *);
int CardSelected(int);
int NoCurrentSuit(int);
int UpdateOwnCard(void);
void ShowTxtMsg(char *);
void MoveToCenter(HWND);

int InitJubmoo(void);

HDDEDATA FAR PASCAL DdeCallbackServer (UINT,UINT,HCONV,HSZ,HSZ,HDDEDATA,DWORD,DWORD);
HDDEDATA FAR PASCAL DdeCallbackClient (UINT,UINT,HCONV,HSZ,HSZ,HDDEDATA,DWORD,DWORD);

int DistributeCard(void);
BYTE Swapoutcard(int,BYTE *,int);
int InitAnotherPlayer(void);
int InitGameStartup(int);

int StartWithServer(void);
int CleanupServer(void);
int StartWithClient(void);
int CleanupClient(void);

BOOL FAR PASCAL StartDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL ClientDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL AboutDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL OptionDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL StatisticDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL HistoryDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL PigPlayerDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL SetLogicDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL GameInfoDlg(HWND,UINT,WORD,LONG);
BOOL FAR PASCAL HelpContentDlg(HWND,UINT,WORD,LONG);

int StoreStructToMessage(void);
int StoreMessageToStruct(void);
int MessageToClientAction(void);
int MessageToServerAction(void);
int BeforePlayerOnActive(void);
int AfterPlayerOnActive(int);
int AfterPlayerOnActive1(void);
int AfterPlayerOnActive2(void);

int JubmooAI(void);

int MakeBmpCard(int);
int PutCardImage(HDC,int,int,int);
//int PutSliceCardImage(HDC,int,int,int);
int PutCardImagef(HDC,int,int);
//int PutSliceCardImagef(HDC,int,int,int axis);

int ShowActivePlayer(int,int mode);
int OutCardFromHand(int,BYTE);    // out card from playonact
int CalWhoGotCard(void);
int GotCardToHand(int,int);           // who got card and who last
int RecvCardUpdate(int,BYTE);   //   update recvcard
int CalScoreEachPlayer(int);
int WhoPigPlayer(void);

int CalCardSpeed(void);
int SetScoreColor(HDC,int);
int MoveCard(int,int,int,int,int,int flag); // flag for check image at start
															// and end point

int CollectAcknowledge(int,int,int);
int GetCardPosition(int);
int StoreNumCard(void);

int random(int val);
int random(int val)
{
	int	k;
	int m;   // for visual c
	float	l;  // for visual c

	k=val;

	//srand( (unsigned)time( NULL ) );
	l = (float)rand();
	l = l/32767*k;
	m = (int) l;  // in Borland use random(k) as m
	return m;
}
//====================== global variable =========================
char			szAppName[] = "JUBMOO";
char			szIniName[] = "jubmoo.ini";
HANDLE		ghInstance;
HINSTANCE	ghResInstance;
HWND			hWnd;
static HMENU			hMenu;
HDC			hdc;

HDC			hDCBmp,hdcStatus,hdcFrmUser,hdcTable;
HBITMAP		hBmpCard,hbmpStatus,hbmpFrmUser,hbmpTable;

DWORD			idInst;
BYTE			nPlayMode;
HSZ			hszService,hszTopic;
HSZ			hszShareService,hszShareTopic;
HSZ			hszPlayItem[4][8] ={{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
								{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
								{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
								{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}};
				// item of each player (server use only)
int			CurChannel[4] = {0,0,0,0};

HCONV			hConv;
char			CompConn[16];
char			PlayerName[16];
BYTE			StrMessage[150];

int			MyPlayer = UNKNOWN;

int			nHavePigPlayer = -1;
BOOL			bActivePlay=FALSE;	// start with inactive
BOOL			bStartedGame = FALSE; // start game state
BYTE			RecvCard[4][17];	// store Recv Card
BYTE			HistActCard[4] = {0,0,0,0};
int			PlaySequence[4]={0,1,2,3}; // sequence of player
int			CurSuit=FREE;			// current suit of card
int			PlayFirst = 0;			// who is first
int			PlayOnAct = 0;			// who is active

int			PosHistCard[4][2] = {{265,201},{190,152},{265,103},{340,152}};
int			PosPlayer[4][2] = {{2,302},{2,2},{526,2},{526,302}};
int			PosStatus[4][2] = {{10,308},{10,8},{440,8},{440,308}};

int			ScoreLimit = -1000;

char			tmpbuf[32];
int			CardSpeed = 8;
int			BackCard = 53;
int			GameNum = 0;
int			HistScore[20][4];
int			HistSum[4] = {0,0,0,0};
char			TxtMsg[64];
int			Logic[4];					// Logic of player
char			LogicName[8][16] = {"BullBoy",
											"SafeSelf",
											"SuperPig",
											"-none-",
											"-none-",
											"-none-",
											"-none-",
											"-none-"};	// Logic name

FARPROC		lpfnDdeCallback;
int CheatMode=FALSE;
//======= file debug =======
#ifdef DEBUG_ON
FILE			*fp = NULL;
#endif

//=================================================================
//====================== procedure ===============================
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpszCmdParam, int nCmdShow)
{
	MSG         msg ;
	WNDCLASS    wndclass ;
	HACCEL		hAccel;
	int			ScrWidth,ScrHeight;
//	HINSTANCE	hddelib;

	ghInstance = hInstance;
	ghResInstance = ghInstance;
	/*ghResInstance = LoadLibrary("JMRES.DLL");
	if ( ghResInstance == NULL )
	{
		MessageBox(GetActiveWindow(),"Can't load resource library",szAppName,MB_OK);
		return -1;
	}
	*/
	
	/*if ( (hddelib=LoadLibrary(winpath)) == NULL )
	{
		LPVOID lpMsgBuf;
 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,0,NULL);
		// Display the string.
		MessageBox( NULL, lpMsgBuf, "GetLastError", MB_OK|MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
		//MessageBox(GetActiveWindow(),"Can't load NetDDE, multiplayer fail",szAppName,MB_OK);
	}*/

	if (!hPrevInstance)
	{
		wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndclass.lpfnWndProc   = (WNDPROC)WndProc ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = 0 ;
		wndclass.hInstance     = hInstance ;
		wndclass.hIcon         = LoadIcon (ghResInstance,"jubmoo") ;
		wndclass.hCursor       = LoadCursor (ghResInstance,MAKEINTRESOURCE(BackCard));
											//NULL,IDC_ARROW) ;
		wndclass.hbrBackground = GetStockObject (LTGRAY_BRUSH) ;
		wndclass.lpszMenuName  = NULL;
		wndclass.lpszClassName = szAppName ;

		RegisterClass (&wndclass) ;
	}

	ScrWidth = 600 + GetSystemMetrics(SM_CXDLGFRAME)*2;
	ScrHeight = 420 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME)*2;
	hWnd = CreateWindowEx(
					WS_EX_DLGMODALFRAME,
					szAppName,         // window class name
					"Jubmoo for Windows",     // window caption
					DS_MODALFRAME| WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_BORDER ,	//WS_SYSMENU,//|WS_MINIMIZEBOX,//|WS_MAXIMIZEBOX,
					(GetSystemMetrics(SM_CXFULLSCREEN)-ScrWidth)/2,
					(GetSystemMetrics(SM_CYFULLSCREEN)-ScrHeight)/2,
					ScrWidth,           // initial x size
					ScrHeight,
					NULL,                    // parent window handle
					NULL,                    // window menu handle
					hInstance,               // program instance handle
					NULL) ;		     // creation parameters

	ShowWindow (hWnd, SW_SHOW) ;
	UpdateWindow (hWnd) ;

#ifdef DEBUG_ON
	fp = fopen("jm.deb","w");
#endif

	SendMessage(hWnd,WM_USER_INITIATE,0,0L);

	hAccel = LoadAccelerators(ghResInstance,szAppName);
	while (GetMessage (&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd,hAccel,&msg))
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
	}

//	FreeLibrary(ghResInstance);
//	FreeLibrary(hddelib);

	DdeUninitialize(idInst);
	// ===== close file debug =========
#ifdef DEBUG_ON
	if (fp != NULL) fclose(fp);
#endif

	return msg.wParam ;
}

//===================================================================
//					Main Process
//===================================================================
long FAR PASCAL WndProc (HWND hwnd,UINT message,WORD wParam,LONG lParam)
{
	PAINTSTRUCT ps;
	DLGPROC		lpfnDialog,lpfnStatDlg,lpfnPigDlg;
	static		LOGFONT		lf;
	static		iChkLButt = FALSE;
	static		iCrdPosX,iCrdPosY;
	HCURSOR		hCur;
	HGDIOBJ		htmp;
	int			i,j;
	MSG			msg;
	//HBRUSH		hBrush;
	POINT mpoint;

	COLORREF	textshrink[3] = {RGB(255,255,255),RGB(128,128,128),RGB(192,192,192)};
	int		PosFrmUser[4][2] = {{0,82},{0,0},{150,0},{150,82}};
/*
	static	HPALETTE hPalette;
	LPLOGPALETTE		lpLogPalette;
	static	HANDLE	hRes,hPal;
	static	LPBITMAPINFO	lpBitmapInfo;
	static	int		nColorData;
	LPSTR			lpBits;
*/
	switch (message)
	{
		case WM_CREATE:
			hMenu = LoadMenu(ghResInstance,"jubmoo");
			hMenu = GetSubMenu(hMenu,0);
			hdc = GetDC(hWnd);
			hDCBmp = CreateCompatibleDC(hdc);
			hBmpCard = LoadBitmap(ghResInstance,MAKEINTRESOURCE(1));
			SelectObject(hDCBmp,hBmpCard);
			hdcFrmUser = CreateCompatibleDC(hdc);
			hbmpFrmUser = LoadBitmap(ghResInstance,"frmuser");
			SelectObject(hdcFrmUser,hbmpFrmUser);
			hdcStatus = CreateCompatibleDC(hdc);
			hbmpStatus = LoadBitmap(ghResInstance,"anistat");
			SelectObject(hdcStatus,hbmpStatus);
			hdcTable = CreateCompatibleDC(hdc);
			hbmpTable = LoadBitmap(ghResInstance,"ctable");
			SelectObject(hdcTable,hbmpTable);
			// test load 256 color bitmap show palette
			/*
			hRes=LoadResource(ghResInstance,FindResource(ghResInstance,"CTABLE",RT_BITMAP));
			lpBitmapInfo = (LPBITMAPINFO) LockResource(hRes);
			if ( lpBitmapInfo->bmiHeader.biClrUsed != 0)
				nColorData = lpBitmapInfo->bmiHeader.biClrUsed;
			else
			{
				switch(lpBitmapInfo->bmiHeader.biBitCount)
				{
					case 1:
						nColorData = 2;
						break;
					case 4:
						nColorData = 16;
						break;
					case 8:
						nColorData = 256;
						break;
					case 24:
						nColorData = 0;
						break;
				}
			}
			hPal = GlobalAlloc(GMEM_MOVEABLE,sizeof(LOGPALETTE)+
										(nColorData*sizeof(PALETTEENTRY)));
			lpLogPalette = (LPLOGPALETTE) GlobalLock(hPal);
			lpLogPalette->palVersion = 0x300;
			lpLogPalette->palNumEntries = nColorData;
			for (i=0;i<nColorData;i++)
			{
				lpLogPalette->palPalEntry[i].peRed = lpBitmapInfo->bmiColors[i].rgbRed;
				lpLogPalette->palPalEntry[i].peGreen = lpBitmapInfo->bmiColors[i].rgbGreen;
				lpLogPalette->palPalEntry[i].peBlue = lpBitmapInfo->bmiColors[i].rgbBlue;
			}
			hPalette = CreatePalette(lpLogPalette);
			GlobalUnlock(hRes);
			GlobalUnlock(hPal);
			GlobalFree(hPal);
			*/
			// set font style
			lf.lfHeight = 24;
			lf.lfWidth = 0;
			lf.lfEscapement = 0;
			lf.lfOrientation = 0;
			lf.lfWeight = FW_BLACK;
			lf.lfItalic = 0;
			lf.lfUnderline = 0;
			lf.lfStrikeOut = 0;
			lf.lfCharSet = ANSI_CHARSET;
			lf.lfPitchAndFamily = DEFAULT_PITCH;
			strcpy(lf.lfFaceName,"Helv");
			ReleaseDC(hWnd,hdc);
			return 0;

		case WM_NCLBUTTONDBLCLK:
			if (wParam == HTCAPTION)
				if (IsIconic(hWnd))
					ShowWindow(hWnd,SW_RESTORE);
				else
					ShowWindow(hWnd,SW_MINIMIZE);
			return 0;
		case WM_USER_INITIATE:
			ShowTxtMsg("Program Startup!!!");
			CalCardSpeed();
			EnableMenuItem(hMenu,CM_NEWGAME,MF_GRAYED);
			EnableMenuItem(hMenu,CM_STARTGAME,MF_GRAYED);
			InitJubmoo();		// select config for start game
			SetTimer(hWnd,GAMEANIMATE_TIMER,500,NULL);
			return 0;
		case WM_TIMER:
			ShowActivePlayer(PlayOnAct,1);
			return 0;
		case WM_AFTERACTIVE:
			AfterPlayerOnActive(wParam);  // wParam = Card position
			return 0;
		case WM_AFTERACTIVE1:
			AfterPlayerOnActive1();
			return 0;
		case WM_AFTERACTIVE2:
			AfterPlayerOnActive2();
			return 0;
		case WM_BEFOREACTIVE:
			BeforePlayerOnActive();
			return 0;
		case WM_ROBOTANA:
			PostMessage(hWnd,WM_AFTERACTIVE,GetCardPosition(JubmooAI()),0);
			return 0;
		case WM_SHOWSTATISTIC:
			lpfnStatDlg = (DLGPROC)MakeProcInstance((FARPROC)StatisticDlg,
								ghResInstance);
			DialogBoxParam(ghResInstance,"statistic",hWnd,lpfnStatDlg,
							STAT_OFGAME);   // can't recv return value
			FreeProcInstance(lpfnStatDlg);
			if (nHavePigPlayer >= 0)
			{
				bActivePlay = FALSE;
				lpfnPigDlg = (DLGPROC)MakeProcInstance((FARPROC)PigPlayerDlg,
								ghResInstance);
				DialogBoxParam(ghResInstance,"pigplayer",hWnd,lpfnPigDlg,
								nHavePigPlayer);
				FreeProcInstance(lpfnPigDlg);
				if (MyPlayer == 0)
				{
					EnableMenuItem(hMenu,CM_NEWGAME,MF_ENABLED);
					ShowTxtMsg("Click right button and choose [New game] for play again");
				}
				// reset history for server
				GameNum = 0;
				for (i = 0 ; i <=3 ; i++)
					HistSum[i] = 0;
			}
			return 0;
		case WM_OUTCARDFROMHAND:
			CollectAcknowledge(OUTCARD,wParam,(int)lParam);
			OutCardFromHand(wParam,(BYTE)lParam);
			return 0;
		case WM_GOTCARDTOHAND:
			GotCardToHand(wParam,(int)lParam);
			CollectAcknowledge(GOTCARD,wParam,(int)lParam);
			return 0;
		case WM_SETACTIVEPLAYER:
			bActivePlay = TRUE;
			ShowTxtMsg("Choose one card in your hand");
			if (PeekMessage(&msg,hWnd,WM_LBUTTONDOWN,WM_LBUTTONDOWN,PM_NOREMOVE))
			{
				while (1)
				{
					if (!PeekMessage(&msg,hWnd,WM_LBUTTONDOWN,WM_LBUTTONDOWN,PM_REMOVE))
						break;
				}
			}
			hdc = GetDC(hWnd);
			BitBlt(hdc,PosStatus[0][0]+97,PosStatus[0][1]+33,40,40,
					hdcStatus,0,0,SRCCOPY);
			ReleaseDC(hWnd,hdc);
			return 0;
		case WM_PAINT:
			BeginPaint (hwnd, &ps) ;		
		
			htmp = (HBRUSH)SelectObject(ps.hdc,CreateSolidBrush(RGB(192,192,192)));		
			Rectangle(ps.hdc,0,0,800,600);		
			DeleteObject(SelectObject(ps.hdc,htmp));
			htmp = SelectObject(ps.hdc,CreatePen(PS_SOLID,1,RGB(128,128,128)));
			MoveToEx(ps.hdc,4,400,NULL);
			LineTo(ps.hdc,595,400);
			DeleteObject(SelectObject(ps.hdc,CreatePen(PS_SOLID,1,RGB(255,255,255))));
			MoveToEx(ps.hdc,5,401,NULL);
			LineTo(ps.hdc,596,401);

			SetTextColor(ps.hdc,RGB(255,255,255));
			TextOut(ps.hdc,20,403,TxtMsg,lstrlen(TxtMsg));
			SetTextColor(ps.hdc,RGB(0,0,0));
			TextOut(ps.hdc,19,402,TxtMsg,lstrlen(TxtMsg));
			SetBkMode(ps.hdc,TRANSPARENT);
			DeleteObject(SelectObject(ps.hdc,htmp));

			htmp = SelectObject(ps.hdc,CreateFontIndirect(&lf));
			/*
			SelectPalette(ps.hdc,hPalette,FALSE);
			SelectPalette(hdcTable,hPalette,FALSE);
			RealizePalette(ps.hdc);
			RealizePalette(hdcTable);

			hRes=LoadResource(ghResInstance,FindResource(ghResInstance,"CTABLE",RT_BITMAP));
			lpBitmapInfo = (LPBITMAPINFO) LockResource(hRes);
			lpBits = (LPSTR) lpBitmapInfo;
			lpBits+= (WORD) lpBitmapInfo->bmiHeader.biSize+
						(WORD) (nColorData*sizeof(RGBQUAD));
			SetDIBitsToDevice(ps.hdc,150,100,
					(WORD)lpBitmapInfo->bmiHeader.biWidth,
					(WORD)lpBitmapInfo->bmiHeader.biHeight,0,0,0,
					(WORD)lpBitmapInfo->bmiHeader.biHeight,
					lpBits,lpBitmapInfo,DIB_RGB_COLORS);
			GlobalUnlock(hRes);
			*/

			BitBlt(ps.hdc,150,100,300,200,hdcTable,0,0,SRCCOPY);

			for (i = 0 ; i<=3 ; i++)
			{
				BitBlt(ps.hdc,PosStatus[i][0],PosStatus[i][1],150,82,
						hdcFrmUser,PosFrmUser[PlaySequence[i]][0],PosFrmUser[PlaySequence[i]][1],SRCCOPY);
			}

			for (j = 0 ; j<=2 ; j++)
			{
				if (j==2) i = 1; else i = j*2;
				SetTextColor(ps.hdc,textshrink[j]);
				TextOut(ps.hdc,18+i,315+i,Player[PlaySequence[0]].PlayName,
							strlen(Player[PlaySequence[0]].PlayName));
				TextOut(ps.hdc,18+i,15+i,Player[PlaySequence[1]].PlayName,
							strlen(Player[PlaySequence[1]].PlayName));
				TextOut(ps.hdc,448+i,15+i,Player[PlaySequence[2]].PlayName,
							strlen(Player[PlaySequence[2]].PlayName));
				TextOut(ps.hdc,448+i,315+i,Player[PlaySequence[3]].PlayName,
							strlen(Player[PlaySequence[3]].PlayName));
			}
			for (i = 0 ; i<=3 ;i++)
			{
				SetTextColor(ps.hdc,RGB(0,0,0));
				TextOut(ps.hdc,PosStatus[i][0]+21,PosStatus[i][1]+43,tmpbuf,
					wsprintf(tmpbuf,"%d",Player[PlaySequence[i]].nScore));

				SetScoreColor(ps.hdc,Player[PlaySequence[i]].nScore);

				TextOut(ps.hdc,PosStatus[i][0]+20,PosStatus[i][1]+42,tmpbuf,
					wsprintf(tmpbuf,"%d",Player[PlaySequence[i]].nScore));
			}

			for (i = 1; i<= Player[PlaySequence[0]].Card[0] ; i++)
				//if (i != Player[PlaySequence[0]].Card[0])
				//	PutSliceCardImage(ps.hdc,Player[PlaySequence[0]].Card[i],
				//						175+(i-1)*15,302);
				//else
					PutCardImage(ps.hdc,Player[PlaySequence[0]].Card[i],
									175+(i-1)*15,302);
			MakeBmpCard(BackCard);
			for (i = 1; i<= Player[PlaySequence[1]].Card[0] ; i++)
				//if (i != Player[PlaySequence[1]].Card[0])
				//	PutSliceCardImagef(ps.hdc,40,98+(i-1)*9,VERT_AXIS);
					//PutSliceCardImage(ps.hdc,Player[PlaySequence[1]].Card[i],
					//				40,98+(i-1)*9);
				//else
				#ifdef DEBUG_ON
					PutCardImage(ps.hdc,Player[PlaySequence[1]].Card[i],
									20+(i%4)*15,98+(i/4)*35);
				#else
				if (CheatMode)
					PutCardImage(ps.hdc,Player[PlaySequence[1]].Card[i],
									20+(i%4)*15,98+(i/4)*35);
				else
					PutCardImagef(ps.hdc,40,98+(i-1)*9);
				#endif
			for (i = 1; i<= Player[PlaySequence[2]].Card[0] ; i++)
				//if (i != Player[PlaySequence[2]].Card[0])
				//	PutSliceCardImagef(ps.hdc,175+(i-1)*15,2,HORZ_AXIS);
					//PutSliceCardImage(ps.hdc,Player[PlaySequence[2]].Card[i],
					//				175+(i-1)*15,2);
				//else
				#ifdef DEBUG_ON
					PutCardImage(ps.hdc,Player[PlaySequence[2]].Card[i],
									175+(i-1)*15,2);
				#else
				if (CheatMode)
					PutCardImage(ps.hdc,Player[PlaySequence[2]].Card[i],
									175+(i-1)*15,2);
				else
					PutCardImagef(ps.hdc,175+(i-1)*15,2);
				#endif
			for (i = 1; i<= Player[PlaySequence[3]].Card[0] ; i++)
				//if (i != Player[PlaySequence[3]].Card[0])
				//	PutSliceCardImagef(ps.hdc,490,98+(i-1)*9,VERT_AXIS);
					//PutSliceCardImage(ps.hdc,Player[PlaySequence[3]].Card[i],
					//				490,98+(i-1)*9);
				//else
				#ifdef DEBUG_ON
					PutCardImage(ps.hdc,Player[PlaySequence[3]].Card[i],
									470+(i%4)*15,98+(i/4)*35);
				#else
				if (CheatMode)
					PutCardImage(ps.hdc,Player[PlaySequence[3]].Card[i],
									470+(i%4)*15,98+(i/4)*35);
				else
					PutCardImagef(ps.hdc,490,98+(i-1)*9);
				#endif
			//delete when test finish
			#ifdef DEBUG_ON
			MakeBmpCard(BackCard);
			#endif
			if (CheatMode)
				MakeBmpCard(BackCard);

			//======================

			for (i = 0; i<= 3 ; i++)
			{
				if (HistActCard[i] != 0)
				{
					PutCardImage(ps.hdc,HistActCard[i],
						PosHistCard[(i-MyPlayer>=0)?i-MyPlayer:i-MyPlayer+4][0],
						PosHistCard[(i-MyPlayer>=0)?i-MyPlayer:i-MyPlayer+4][1]);
					//MakeBmpCard(HistActCard[i]);
					//BitBlt(ps.hdc,PosHistCard[(i-MyPlayer>=0)?i-MyPlayer:i-MyPlayer+4][0],
					//		PosHistCard[(i-MyPlayer>=0)?i-MyPlayer:i-MyPlayer+4][1],
					//		CARDWIDTH,CARDHEIGHT,hDCBmp,0,0,SRCCOPY);
				}
			}
			ShowActivePlayer(PlayOnAct,2);
			DeleteObject(SelectObject(ps.hdc,htmp));
			EndPaint (hwnd, &ps) ;
			return 0 ;
		case WM_RBUTTONDOWN:
			mpoint.x = LOWORD(lParam);
			mpoint.y = HIWORD(lParam);
			ClientToScreen(hWnd,&mpoint);
			TrackPopupMenu(hMenu,0,mpoint.x,mpoint.y,0,hWnd,NULL);
			return 0;
		case WM_LBUTTONDOWN:
			if (bActivePlay == TRUE)
			{
				//if ( LButtonDown(LOWORD(lParam),HIWORD(lParam)) == 0 )
				iCrdPosX = LOWORD(lParam);
				iCrdPosY = HIWORD(lParam);
				if ( (iChkLButt = ChkCardPosition(&iCrdPosX,&iCrdPosY)) > 0 )
				{
					hCur = LoadCursor(ghResInstance,MAKEINTRESOURCE(BackCard+10));
					SetClassLong(hWnd, GCLP_HCURSOR,(LONG)hCur);
					SetCursor(hCur);
					hdc = GetDC(hWnd);
					iCrdPosX++;
					iCrdPosY--;
					PutCardImage(hdc,Player[MyPlayer].Card[iChkLButt],iCrdPosX,iCrdPosY);
					iCrdPosX++;
					iCrdPosY--;
					PutCardImage(hdc,Player[MyPlayer].Card[iChkLButt],iCrdPosX,iCrdPosY);
					ReleaseDC(hWnd,hdc);
				}
			}
			return 0;
		case WM_LBUTTONUP:
			if ( iChkLButt > 0 )
			{
				if ((LOWORD(lParam)>=iCrdPosX)&&(LOWORD(lParam)<=iCrdPosX+CARDWIDTH))
				{
					if ((HIWORD(lParam)>=iCrdPosY)&&(HIWORD(lParam)<=iCrdPosY+CARDHEIGHT))
					{
						CardSelected(iChkLButt);
					}
					else
					{
						UpdateOwnCard();
					}
				}
				else
				{
					UpdateOwnCard();
				}
				hCur = LoadCursor(ghResInstance,MAKEINTRESOURCE(BackCard));
				SetClassLong(hWnd, GCLP_HCURSOR,(LONG)hCur);
				SetCursor(hCur);
				iChkLButt = FALSE;
			}
		case WM_COMMAND:
			switch(wParam)
			{
				case CM_STARTGAME:
					bStartedGame = TRUE;
					EnableMenuItem(hMenu,CM_STARTGAME,MF_GRAYED);
					InitAnotherPlayer();
					lpfnDialog = (DLGPROC)MakeProcInstance((FARPROC)SetLogicDlg,
											ghResInstance);
					i=DialogBox(ghResInstance,"logic",hWnd,lpfnDialog);
					FreeProcInstance(lpfnDialog);
					InitGameStartup(i);
					break;
				case CM_NEWGAME:
					EnableMenuItem(hMenu,CM_NEWGAME,MF_GRAYED);
					for (i=0;i<=3; i++) Player[i].nScore = 0;
					lpfnDialog = (DLGPROC)MakeProcInstance((FARPROC)SetLogicDlg,
											ghResInstance);
					i=DialogBox(ghResInstance,"logic",hWnd,lpfnDialog);
					FreeProcInstance(lpfnDialog);
					InitGameStartup(i);
					break;
				case CM_OPTION:
					lpfnDialog = (DLGPROC)MakeProcInstance(
										(FARPROC)OptionDlg,
										ghResInstance);
					if (DialogBox(ghResInstance,"option",hWnd,lpfnDialog)==1)
					{
						hCur = LoadCursor(ghResInstance,MAKEINTRESOURCE(BackCard));
						SetClassLong(hWnd, GCLP_HCURSOR,(LONG)hCur);
						SetCursor(hCur);
						sprintf(tmpbuf,"%d",BackCard-52);
						WritePrivateProfileString("Init","BackCard",tmpbuf,szIniName);
						InvalidateRect(hWnd,NULL,FALSE);
						UpdateWindow(hWnd);
					}
					FreeProcInstance(lpfnDialog);
					break;
				case CM_STATISTIC:
					lpfnDialog = (DLGPROC)MakeProcInstance(
										(FARPROC)StatisticDlg,
										ghResInstance);
					DialogBoxParam(ghResInstance,"statistic",hWnd,lpfnDialog,
									STAT_ONHAND);
					FreeProcInstance(lpfnDialog);
					break;
				case CM_HISTORY:
					lpfnDialog = (DLGPROC)MakeProcInstance(
										(FARPROC)HistoryDlg,
										ghResInstance);
					DialogBox(ghResInstance,"history",hWnd,lpfnDialog);
					FreeProcInstance(lpfnDialog);
					break;
				case CM_EXIT:
					DestroyWindow(hWnd);
					break;
				case CM_CONTENT:
					lpfnDialog = (DLGPROC)MakeProcInstance((FARPROC)HelpContentDlg,
											ghResInstance);
					DialogBox(ghResInstance,"content",hWnd,lpfnDialog);
					FreeProcInstance(lpfnDialog);
					break;
				case CM_ABOUT:
					lpfnDialog = (DLGPROC)MakeProcInstance((FARPROC)AboutDlg,
											ghResInstance);
					DialogBox(ghResInstance,"about",hWnd,lpfnDialog);
					FreeProcInstance(lpfnDialog);
					break;
				case CM_GAMEINFO:
					lpfnDialog = (DLGPROC)MakeProcInstance((FARPROC)GameInfoDlg,
											ghResInstance);
					DialogBox(ghResInstance,"gameinfo",hWnd,lpfnDialog);
					FreeProcInstance(lpfnDialog);
					break;
			}
			return 0;

		case WM_DESTROY:
			if (nPlayMode == SERVERMODE) CleanupServer();
			if (nPlayMode == CLIENTMODE) CleanupClient();
			KillTimer(hWnd,GAMEANIMATE_TIMER);
			DeleteDC(hDCBmp);
			DeleteObject(hBmpCard);
			DeleteDC(hdcFrmUser);
			DeleteObject(hbmpFrmUser);
			DeleteDC(hdcStatus);
			DeleteObject(hbmpStatus);
			DeleteDC(hdcTable);
			DeleteObject(hbmpTable);
/*			DeleteObject(hPalette);
			FreeResource(hRes);
*/			PostQuitMessage (0) ;
			return 0 ;
	}

	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

//int LButtonDown(int xPos,int yPos)
int ChkCardPosition(int *x_Pos,int *yPos)
{
	int suit;
	int xPos;


	if ((*yPos>=302)&&(*yPos<=398))
	{
		xPos = *x_Pos-175;
		if ((xPos >= 15*(Player[MyPlayer].Card[0]-1))&&
			(xPos <= 15*(Player[MyPlayer].Card[0]-1)+CARDWIDTH))
		{	// check for last card in screen
			xPos = Player[MyPlayer].Card[0];
		}
		else
		{
			if (xPos <0 ) return 0;
			xPos = xPos/15;
			xPos++;
		}

		if ( (xPos>=1)&&(xPos<=Player[MyPlayer].Card[0]) )
		{
			// check format of card with first card
			suit = (Player[MyPlayer].Card[xPos]-1) / 13;

			if ((suit == CurSuit)||(CurSuit == FREE)||NoCurrentSuit(MyPlayer))
			{
				*yPos = 302;
				*x_Pos = 175+((xPos-1)*15);
				return xPos;
			}
		}
	}
	return 0;
}

int CardSelected(int xPos)
{
	HSZ		hszTmpItem;
	HDDEDATA	hData;

	hdc = GetDC(hWnd);
	if (nPlayMode == SERVERMODE)
	{
		//OutCardFromHand(PlayOnAct,(BYTE)xPos);
		PostMessage(hWnd,WM_AFTERACTIVE,xPos,0);
		bActivePlay = FALSE;
		BitBlt(hdc,PosStatus[0][0]+97,PosStatus[0][1]+33,40,40,
				hdcFrmUser,97,33,SRCCOPY);
		ReleaseDC(hWnd,hdc);
	}
	if (nPlayMode == CLIENTMODE)
	{
		PostMessage(hWnd,WM_OUTCARDFROMHAND,MyPlayer,(BYTE)xPos);
		bActivePlay = FALSE;
		BitBlt(hdc,PosStatus[0][0]+97,PosStatus[0][1]+33,40,40,
				hdcFrmUser,97,33,SRCCOPY);
		ReleaseDC(hWnd,hdc);
		StrMessage[0] = CLISER_MSG_OUTCARD;
		StrMessage[1] = 4; // size of message
		StrMessage[2] = MyPlayer;
		StrMessage[3] = (BYTE)xPos;
		hszTmpItem = DdeCreateStringHandle(idInst,PlayerName,0);
		hData = DdeCreateDataHandle ( idInst, StrMessage,
							StrMessage[1], 0L, hszTmpItem,CF_TEXT, 0 );
		if ( hData != NULL )
			hData = DdeClientTransaction ( (LPBYTE)hData, 0xFFFFFFFF, hConv,
					hszTmpItem, CF_TEXT, XTYP_POKE, 5000, NULL);
		DdeFreeStringHandle(idInst,hszTmpItem);
	}
	return 0;
}


int NoCurrentSuit(int who)
{
	int i;
	for (i = 1; i<= Player[who].Card[0];i++)
	{
		if ( CurSuit == (Player[who].Card[i]-1)/13 )
		{
			return FALSE;
		}
	}

	return TRUE;
}

int UpdateOwnCard()
{
	// card Y start at 302 , end at 398
	// card X start at 175 , end at 426
	RECT	rectp = {175,300,428,398};
	InvalidateRect(hWnd,&rectp,TRUE);
	UpdateWindow(hWnd);
	return 0;
}

void ShowTxtMsg(char *ShwMsg)
{
	RECT	rc;
	sprintf(TxtMsg,ShwMsg);
	rc.top = 402;
	rc.left = 5;
	rc.bottom = 428;
	rc.right = 500;
	InvalidateRect(hWnd,&rc,TRUE);
	UpdateWindow(hWnd);
}
void MoveToCenter(HWND hWnd)
{
	RECT swp;
	int width;
	int height;

	width = GetSystemMetrics(SM_CXFULLSCREEN);
	height = GetSystemMetrics(SM_CYFULLSCREEN);

	GetWindowRect(hWnd,&swp);

	MoveWindow(hWnd,(width-(swp.right-swp.left))/2,
					(height-(swp.bottom-swp.top))/2,
					swp.right-swp.left,
					swp.bottom-swp.top,FALSE);
}

int CalCardSpeed()
{
	DWORD dftm[5];
	int i,sp=0;

	for (i = 0 ;i<=4;i++)
	{
		CardSpeed = 12;//21-i*4;
		dftm[i] = GetCurrentTime();
		MoveCard(BackCard,PosPlayer[i%4][0],PosPlayer[i%4][1],265,152,0);
		dftm[i] = GetCurrentTime()-dftm[i];
		dftm[i] = dftm[i]/CardSpeed;
		sp += (int)dftm[i];
	}
	sp = sp/5;
	if ( sp == 0 ) sp = 5;
	CardSpeed = 220/sp;
	if (CardSpeed<5) CardSpeed=5;
	if (CardSpeed>50) CardSpeed=50;

	return 0;
}

int SetScoreColor(HDC hdcf,int score)
{

	if (score >= ScoreLimit*2/7)
		SetTextColor(hdcf,RGB(0,255,0));
	else if (score >= ScoreLimit*3/7)
			SetTextColor(hdcf,RGB(0,0,255));
	else if (score >= ScoreLimit*4/7)
			SetTextColor(hdcf,RGB(0,255,255));
	else if (score >= ScoreLimit*5/7)
			SetTextColor(hdcf,RGB(255,255,0));
	else if (score >= ScoreLimit*6/7)
			SetTextColor(hdcf,RGB(255,0,255));
	else SetTextColor(hdcf,RGB(255,0,0));
	return 0;
}

int ShowActivePlayer(int whoact,int mode)
{
	static prevplayer = 0;
	int idx,idx2;
	static anii=0;

	if (whoact>3) whoact = 0;

	//hdc = GetDC(hWnd);
	idx=(whoact-MyPlayer>=0)?whoact-MyPlayer:whoact-MyPlayer+4;
	if (mode == 1)
	{
		anii++;
		if (anii==4) anii =0;
		BitBlt(hdc,PosStatus[idx][0]+97,PosStatus[idx][1]+33,40,40,
//		BitBlt(hdc,PosStatus[PlaySequence[whoact]][0]+97,PosStatus[PlaySequence[whoact]][1]+33,40,40,
				hdcStatus,anii*40,0,SRCCOPY);
		return 0;
	}
	if (mode == 0)
	{
		idx2=(prevplayer-MyPlayer>=0)?prevplayer-MyPlayer:prevplayer-MyPlayer+4;
		BitBlt(hdc,PosStatus[idx2][0]+97,PosStatus[idx2][1]+33,40,40,
			hdcFrmUser,97,33,SRCCOPY);
		BitBlt(hdc,PosStatus[idx][0]+97,PosStatus[idx][1]+33,40,40,
			hdcStatus,0,0,SRCCOPY);
		prevplayer = whoact;
		return 0;
	}
	if (mode == 2)
	{
		if (MyPlayer == UNKNOWN) return 0;
		BitBlt(hdc,PosStatus[idx][0]+97,PosStatus[idx][1]+33,40,40,
				hdcStatus,0,0,SRCCOPY);
		return 0;
	}
	//ReleaseDC(hWnd,hdc);
	return 0;
}

int MakeBmpCard(int num)
{
	HDC htmp;

	hBmpCard = LoadBitmap(ghResInstance,MAKEINTRESOURCE(num));
	htmp = SelectObject(hDCBmp,hBmpCard);
	DeleteObject(htmp);
	return 0;
}
// ============ Put Image of card ================================
int PutCardImage(HDC hdcsc,int  cardnum,int xpos, int ypos)
{
	BYTE PixelBk[12][2] = { {0,0},{1,0},{0,1},
							{70,0},{70,1},{69,0},
							{0,94},{0,95},{1,95},
							{70,95},{70,94},{69,95} };
	COLORREF	ctmp[12];
	int i;

	MakeBmpCard(cardnum);
	for (i = 0 ; i <= 11 ;i++)
		ctmp[i] = GetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1]);
	BitBlt(hdcsc,xpos,ypos,CARDWIDTH,CARDHEIGHT,hDCBmp,0,0,SRCCOPY);
	for (i = 0 ; i <= 11 ;i++)
		SetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1],ctmp[i]);


	return 0;
}
/*int PutSliceCardImage(HDC hdcsc,int  cardnum,int xpos, int ypos)
{
	BYTE PixelBk[12][2] = { {0,0},{1,0},{0,1},
							{70,0},{70,1},{69,0},
							{0,94},{0,95},{1,95},
							{70,95},{70,94},{69,95} };
	COLORREF	ctmp[12];
	int i,wt,hi;

	wt = 17;
	hi = CARDHEIGHT;
	MakeBmpCard(cardnum);
	for (i = 0 ; i <= 11 ;i++)
		ctmp[i] = GetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1]);
	BitBlt(hdcsc,xpos,ypos,wt,hi,hDCBmp,0,0,SRCCOPY);
	for (i = 0 ; i <= 11 ;i++)
		SetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1],ctmp[i]);


	return 0;
}*/
int PutCardImagef(HDC hdcsc,int xpos, int ypos)
{
	BYTE PixelBk[12][2] = { {0,0},{1,0},{0,1},
							{70,0},{70,1},{69,0},
							{0,94},{0,95},{1,95},
							{70,95},{70,94},{69,95} };
	COLORREF	ctmp[12];
	int i;

	for (i = 0 ; i <= 11 ;i++)
		ctmp[i] = GetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1]);
	BitBlt(hdcsc,xpos,ypos,CARDWIDTH,CARDHEIGHT,hDCBmp,0,0,SRCCOPY);
	for (i = 0 ; i <= 11 ;i++)
		SetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1],ctmp[i]);


	return 0;
}
/*int PutSliceCardImagef(HDC hdcsc,int xpos, int ypos, int axis)
{
	BYTE PixelBk[12][2] = { {0,0},{1,0},{0,1},
							{70,0},{70,1},{69,0},
							{0,94},{0,95},{1,95},
							{70,95},{70,94},{69,95} };
	COLORREF	ctmp[12];
	int i,wt,hi;

	if (axis == HORZ_AXIS)
	{
		wt = 17;
		hi = CARDHEIGHT;
	}
	else
	{
		wt = CARDWIDTH;
		hi = 11;
	}
	for (i = 0 ; i <= 11 ;i++)
		ctmp[i] = GetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1]);
	BitBlt(hdcsc,xpos,ypos,wt,hi,hDCBmp,0,0,SRCCOPY);
	for (i = 0 ; i <= 11 ;i++)
		SetPixel(hdcsc,xpos+PixelBk[i][0],ypos+PixelBk[i][1],ctmp[i]);


	return 0;
}*/




//===============================================================
int InitJubmoo(void)
{
	DLGPROC		lpfnDlg;
	int i;
	HCURSOR		hCur;

	HKEY hKey;
	DWORD dw;
	
	char		winpath[100];
	GetWindowsDirectory(winpath,sizeof(winpath));
	strcat(winpath,"\\NETDDE.EXE");
	// up process netdde ( default is off )
	ShowTxtMsg("Start NetDDE deamon.... please wait....!");
	if ( _spawnl(_P_NOWAIT,winpath,winpath,NULL) == -1 )
		MessageBox(hWnd,"Error start netdde. Can't play multiplayer",szAppName,MB_OK);

	Sleep(100);
	ShowTxtMsg("");
	// check for NetDDE (win3.1)
	/*
	GetPrivateProfileString("DDEShares","JUBMOO$","",
							tmpbuf,sizeof(tmpbuf),
							"SYSTEM.INI");

	if (lstrcmp(tmpbuf,"jubmoo,card,,15,,0,,0,0,0")!=0)
	{
		WritePrivateProfileString("DDEShares","JUBMOO$",
								"jubmoo,card,,15,,0,,0,0,0","SYSTEM.INI");
	}
	*/
	// check for NetDDE (win95)
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,
					"SOFTWARE\\Microsoft\\NetDDE\\DDE Shares\\JUBMOO$",
					0,NULL,REG_OPTION_NON_VOLATILE, // win95 ignore
					KEY_ALL_ACCESS,NULL, // win95 ignore 
					&hKey,&dw);
	if ( dw == REG_CREATED_NEW_KEY )
	{
		// add ddeshare registry
		DWORD vd = 0;
		DWORD vd1 = 15;
		char vd2[16];

		RegSetValueEx(hKey,
						"Additional item count",
						0,REG_DWORD,(CONST BYTE*)&vd,sizeof(DWORD));
		RegSetValueEx(hKey,
						"Permissions1",
						0,REG_DWORD,(CONST BYTE*)&vd1,sizeof(DWORD));
		RegSetValueEx(hKey,
						"Permissions2",
						0,REG_DWORD,(CONST BYTE*)&vd,sizeof(DWORD));
		sprintf(vd2,"jubmoo");
		RegSetValueEx(hKey,
						"Application",
						0,REG_SZ,(CONST BYTE*)&vd2,strlen(vd2)+1);
		sprintf(vd2,"");
		RegSetValueEx(hKey,
						"Item",
						0,REG_SZ,(CONST BYTE*)&vd2,strlen(vd2)+1);
		RegSetValueEx(hKey,
						"Password1",
						0,REG_SZ,(CONST BYTE*)&vd2,strlen(vd2)+1);
		RegSetValueEx(hKey,
						"Password2",
						0,REG_SZ,(CONST BYTE*)&vd2,strlen(vd2)+1);
		sprintf(vd2,"card");
		RegSetValueEx(hKey,
						"Topic",
						0,REG_SZ,(CONST BYTE*)&vd2,strlen(vd2)+1);
	}
	if ( hKey ) RegCloseKey(hKey);
	//////////////////////////////////////////////////////////////

	// start up
	lpfnDlg = (DLGPROC)MakeProcInstance((FARPROC)StartDlg,ghResInstance);
	if ( DialogBox(ghResInstance,"startup",hWnd,lpfnDlg) == -1 )
		DestroyWindow(hWnd);
	FreeProcInstance(lpfnDlg);

	if ( GetPrivateProfileInt("Init","Kasamsun",0,szIniName) == 220673 )
		CheatMode = TRUE;

	if ( (i = GetPrivateProfileInt("Init","BackCard",4,szIniName)) == 0 )
	{
		WritePrivateProfileString("Init","BackCard","4",szIniName);
	}
	else
	{
		if ( i>=1 && i<=4 )
			BackCard = i+52;
		else
		{
			BackCard = 56;
			WritePrivateProfileString("Init","BackCard","4",szIniName);
		}
		hCur = LoadCursor(ghResInstance,MAKEINTRESOURCE(BackCard));
		SetClassLong(hWnd, GCLP_HCURSOR,(LONG)hCur);
		SetCursor(hCur);
	}

	switch(nPlayMode)
	{
		case SERVERMODE:

			if (StartWithServer()==-1)
			{
				DestroyWindow(hWnd);
				return -1;
			}
			sprintf(TxtMsg,"Click right button and choose [Start game] when ready");
			Player[0].PlayType = HUMAN;
			strcpy(Player[0].PlayName,PlayerName);
			MyPlayer = 0;
			InvalidateRect(hWnd,NULL,TRUE);
			EnableMenuItem(hMenu,CM_STARTGAME,MF_ENABLED);

			break;

		case CLIENTMODE:

			lpfnDlg = (DLGPROC)MakeProcInstance((FARPROC)ClientDlg,ghResInstance);
			if (DialogBox(ghResInstance,"clientmode",hWnd,lpfnDlg)==-1)
			{
				// if exit
				FreeProcInstance(lpfnDlg);
				DestroyWindow(hWnd);
				return -1;
			}
			FreeProcInstance(lpfnDlg);
			break;
	}
	return 0L;
}


int StartWithServer(void)
{
	//char abc[1024];
	//NDdeIsValidAppTopicList((LPTSTR) abc);
	
	lpfnDdeCallback = MakeProcInstance((FARPROC)DdeCallbackServer,ghInstance);
	if (DdeInitialize(&idInst,(PFNCALLBACK) lpfnDdeCallback,
						APPCLASS_STANDARD,0L))
		return -1;

	hszService = DdeCreateStringHandle(idInst,"jubmoo",0);
	hszTopic = DdeCreateStringHandle(idInst,"card",0);
	DdeNameService(idInst,hszService,NULL,DNS_REGISTER);
	return 0;

}

int CleanupServer(void)
{
	int i,j;
   	
	DdeNameService(idInst,hszService,NULL,DNS_UNREGISTER);
	DdeFreeStringHandle(idInst,hszService);
	DdeFreeStringHandle(idInst,hszTopic);

	for (i = 1 ; i<=3 ; i++)
	{

		if (Player[i].PlayType == HUMAN)
		{
			for (j = 0 ; j<= 7 ; j++)
				DdeFreeStringHandle(idInst,hszPlayItem[i][j]);
		}
	}
	return 0;
}

int StartWithClient(void)
{
	HSZ			hszTmpItem;
	HDDEDATA	hData;
	int			i;
	UINT		errcod;

	lpfnDdeCallback = MakeProcInstance((FARPROC)DdeCallbackClient,ghInstance);
	if (DdeInitialize(&idInst,(PFNCALLBACK) lpfnDdeCallback,
						APPCLASS_STANDARD,0L))
		return -1;
	// ==========for stand alone test
	if (strcmp(CompConn,"")==0)
	{
		hszShareService = DdeCreateStringHandle(idInst,"jubmoo",0);
		hszShareTopic = DdeCreateStringHandle(idInst,"card",0);
	}
	else
	{
		sprintf(tmpbuf,"\\\\%s\\NDDE$",CompConn);
		hszShareService = DdeCreateStringHandle(idInst,tmpbuf,0);
		hszShareTopic = DdeCreateStringHandle(idInst,"JUBMOO$",0);
	}

	hConv = DdeConnect(idInst,hszShareService,hszShareTopic,NULL);

	errcod = DdeGetLastError(idInst);

	DdeFreeStringHandle(idInst,hszShareService);
	DdeFreeStringHandle(idInst,hszShareTopic);

	if (hConv == NULL) return -1;

	// create hot link to server for each connection, item = player name
	for (i = 0 ; i<=7 ; i++)
	{
		sprintf(tmpbuf,"%s%c%c",PlayerName,65+i,0);
		hszTmpItem=  DdeCreateStringHandle(idInst,tmpbuf,0);
		hData = DdeClientTransaction(NULL,0,hConv,hszTmpItem,CF_TEXT,
						XTYP_ADVSTART|XTYPF_ACKREQ,5000,NULL);
		DdeFreeStringHandle(idInst,hszTmpItem);
	}

	// send player name to server
	hszTmpItem = DdeCreateStringHandle(idInst,"playername",0);
	hData = DdeCreateDataHandle ( idInst, PlayerName,
					sizeof (PlayerName), 0L, hszTmpItem,CF_TEXT, 0 );
	if ( hData != NULL )
		hData = DdeClientTransaction ( (LPBYTE)hData, 0xFFFFFFFF, hConv,
			 hszTmpItem, CF_TEXT, XTYP_POKE, 5000, NULL);
	DdeFreeStringHandle(idInst,hszTmpItem);

	return 0;
}
int CleanupClient(void)
{
	HSZ			hszTmpItem;
	int			i;

	// kill hot link
	for (i = 0 ; i<=7 ; i++)
	{
		sprintf(tmpbuf,"%s%c%c",PlayerName,65+i,0);
		hszTmpItem = DdeCreateStringHandle(idInst,tmpbuf,0);
		DdeClientTransaction(NULL,0,hConv,hszTmpItem,CF_TEXT,
							XTYP_ADVSTOP,5000,NULL);
		DdeFreeStringHandle(idInst,hszTmpItem);
	}
	return 0;
}

//===============================================================
//                 call back function of DDE Server mode
//===============================================================
HDDEDATA FAR PASCAL DdeCallbackServer (UINT iType,UINT iFmt,HCONV hconv,
										HSZ hsz1,HSZ hsz2,HDDEDATA hData,
										DWORD dwData1,DWORD dwData2)
{
	static		CurNoPlayer=0;
	int			i;
	//LPBYTE		lpszAdviseData;
	//DWORD		cbDataLen;

	switch(iType)
	{
		case XTYP_CONNECT: // hsz1=topic,hsz2=service
			if (( CurNoPlayer < 3 )&&(bStartedGame == FALSE))
			{
				DdeQueryString(idInst,hsz1,tmpbuf,sizeof(tmpbuf),0);
				if ( strcmp(tmpbuf,"card") != 0 )
					return (HDDEDATA)FALSE;
				DdeQueryString(idInst,hsz2,tmpbuf,sizeof(tmpbuf),0);
				if ( strcmp(tmpbuf,"jubmoo") != 0 )
					return (HDDEDATA)FALSE;
				CurNoPlayer++;
				return (HDDEDATA)TRUE;
			}
			return (HDDEDATA)FALSE;

		case XTYP_POKE:   // hsz1=topic,hsz2=item,hData
			if ( hsz1 == hszTopic )
			{
				DdeQueryString(idInst,hsz2,tmpbuf,sizeof(tmpbuf),0);
				if (strcmp(tmpbuf,"playername") == 0)
				{
					Player[CurNoPlayer].PlayType = HUMAN;
					DdeGetData(hData,Player[CurNoPlayer].PlayName,
								sizeof(Player[CurNoPlayer].PlayName),0);
					for (i = 0 ; i<=7 ; i++)
					{
						sprintf(tmpbuf,"%s%c%c",Player[CurNoPlayer].PlayName,
								65+i,0);
						hszPlayItem[CurNoPlayer][i] = DdeCreateStringHandle(
										idInst,tmpbuf,0); // store item
					}
					StrMessage[0] = SERCLI_MSG_PLAYERNUMBER;
					StrMessage[1] = 4;
					StrMessage[2] = (BYTE) CurNoPlayer;
					StrMessage[3] = (BYTE) abs(ScoreLimit/100);
					DdePostAdvise(idInst,hszTopic,hszPlayItem[CurNoPlayer][0]);
					InvalidateRect(hWnd,NULL,TRUE);
				}
				else
				{// change get data to recv pointer
					//lpszAdviseData = DdeAccessData(hData,&cbDataLen);
					//for (i = 0;i < cbDataLen; i++);
					//	StrMessage[i] = *lpszAdviseData++;
					//DdeUnaccessData(hData);
					DdeGetData(hData,StrMessage,sizeof(StrMessage),0);
					if (MessageToServerAction() == 0 )
						return (HDDEDATA)DDE_FACK;
					else
						return (HDDEDATA)DDE_FNOTPROCESSED;
				}
			}
			return NULL;

		case XTYP_ADVSTART:
			return (HDDEDATA)TRUE;
		case XTYP_ADVSTOP:
			return (HDDEDATA)TRUE;
		case XTYP_REQUEST:	// send data to client by req of client
			return NULL;

		case XTYP_ADVREQ:  // send data to client ,hsz2=item, hsz1=topic
			if (hsz1 == hszTopic)
			{
				return DdeCreateDataHandle(idInst,StrMessage,
						StrMessage[1],0,hsz2,CF_TEXT,0);
			}
			return NULL;

		case XTYP_DISCONNECT:
			hConv = NULL;
			MessageBox(hWnd,"CLIENT has disconnected.",
							szAppName,MB_OK|MB_ICONSTOP);
			DestroyWindow(hWnd);
			return NULL;

	}

	return NULL;
}

//===============================================================
//                 call back function of DDE client mode
//===============================================================
HDDEDATA FAR PASCAL DdeCallbackClient (UINT iType,UINT iFmt,HCONV hconv,
										HSZ hsz1,HSZ hsz2,HDDEDATA hData,
										DWORD dwData1,DWORD dwData2)
{
	//LPBYTE	lpszAdviseData;
	//DWORD	cbDataLen;

	switch(iType)
	{
		case XTYP_ADVDATA:
			DdeQueryString(idInst,hsz1,tmpbuf,sizeof(tmpbuf),0);
			if ((strcmp(tmpbuf,"card")==0)||(strcmp(tmpbuf,"JUBMOO$")==0))
			{	// change get data to recv pointer
				//lpszAdviseData = DdeAccessData(hData,&cbDataLen);
				//for (i = 0;i < cbDataLen; i++);
				//	StrMessage[i] = *lpszAdviseData++;
				//DdeUnaccessData(hData);
				DdeGetData(hData,StrMessage,sizeof(StrMessage),0);
				if (MessageToClientAction() == 0 )
					return (HDDEDATA)DDE_FACK;
				else
					return (HDDEDATA)DDE_FNOTPROCESSED;
			}
			return (HDDEDATA)DDE_FNOTPROCESSED;

		case XTYP_DISCONNECT:
			hConv = NULL;
			MessageBox(hWnd,"SERVER has disconnected.",
							szAppName,MB_OK|MB_ICONSTOP);
			DestroyWindow(hWnd);
			return NULL;
	}

	return NULL;
}

int MessageToServerAction(void)
{
	BYTE		mesgno;

	mesgno = (BYTE) StrMessage[0];

	switch (mesgno)
	{
		case CLISER_MSG_OUTCARD:
			//OutCardFromHand(PlayOnAct,StrMessage[2]);
			PostMessage(hWnd,WM_AFTERACTIVE,StrMessage[3],0);
			return 0;
	}
	return -1;
}

int MessageToClientAction(void)
{
	BYTE		mesgno;
	int			i,j,pos;

	mesgno = (BYTE) StrMessage[0];

	switch (mesgno)
	{
		case SERCLI_MSG_PLAYERNUMBER:
			MyPlayer = StrMessage[2];
			ScoreLimit = -(StrMessage[3]*100);
			sprintf(TxtMsg,"You're player #%d. Limit score at %d, Wait for dealer start",MyPlayer,ScoreLimit);
			PlaySequence[0] = MyPlayer;
			PlaySequence[1] = MyPlayer+1;if (PlaySequence[1]>3) PlaySequence[1]-=4;
			PlaySequence[2] = MyPlayer+2;if (PlaySequence[2]>3) PlaySequence[2]-=4;
			PlaySequence[3] = MyPlayer+3;if (PlaySequence[3]>3) PlaySequence[3]-=4;
			InvalidateRect(hWnd,NULL,TRUE);
			return 0;
		case SERCLI_MSG_PLAYSTRUCT:
			StoreMessageToStruct();
			// clear recv card
			for (i = 0 ;i<=3 ;i++)
				for (j = 0 ; j<=17 ; j++)
					RecvCard[i][j] = 0;
			PlayOnAct = StrMessage[149];
			sprintf(TxtMsg,"Wait for %s play",Player[PlayOnAct].PlayName);
			InvalidateRect(hWnd,NULL,TRUE);
			// reset acknownledge of client
			CollectAcknowledge(RESET_ACK,PlayOnAct,0);
			return 0;
		case SERCLI_MSG_ACTIVE:
			//bActivePlay = TRUE;
			PostMessage(hWnd,WM_SETACTIVEPLAYER,0,0);
			return 0;
		case SERCLI_MSG_OUTCARD:
			for (i = 1 ; i<=Player[StrMessage[2]].Card[0] ; i++)
			{
				if (StrMessage[3] == Player[StrMessage[2]].Card[i])
					pos = i;
			}
			CurSuit = StrMessage[4];
//			HistActCard[StrMessage[2]] = (BYTE)StrMessage[3];
			PostMessage(hWnd,WM_OUTCARDFROMHAND,StrMessage[2],(BYTE)pos);
			return 0;
		case SERCLI_MSG_GOTCARD:
			PlayOnAct = StrMessage[2];
			PostMessage(hWnd,WM_GOTCARDTOHAND,StrMessage[2],StrMessage[3]);
			return 0;
		case SERCLI_MSG_SHOWSCORE:
			PostMessage(hWnd,WM_SHOWSTATISTIC,0,0);
			return 0;
	}
	return -1;
}

//=========================================================
//       Distribute Card to each Player
//=========================================================
int DistributeCard(void)
{
	BYTE	suitcard[53];         // make comment for visual c random
	int		i,j,k;
	int m;   // for visual c
	float	l;  // for visual c
	int		tmp;

	for ( i = 0 ; i<=51 ; i++)
		suitcard[i] = i+1;
	suitcard[52] = 0;

	//srand(2326);
	// ==============
	k=52;
//	randomize();
	for (i = 0 ; i<=3 ; i++)
	{
		srand( (unsigned)time( NULL ) );
		for ( j = 1 ; j <=13 ; j++ )
		{
			l = (float)rand();
			l = l/32767*k;
			m = (int) l;  // in Borland use random(k) as m
			Player[i].Card[j] =	Swapoutcard(m,&suitcard[0],k-1);
			k--;
		}
		Player[i].Card[0] = 13;   // have 13 cards
		for ( j = 0 ; j<=17 ; j++ )
			RecvCard[i][j] = 0;   // clear recv cards from hand
	}
	// sort card
	for ( i = 0 ; i<=3 ; i++)
	{
		for (k = 13 ; k > 1 ; k--)
		for (j = 1 ; j < k ; j++)
			if ( Player[i].Card[j] > Player[i].Card[j+1] )
			{
				tmp = Player[i].Card[j+1];
				Player[i].Card[j+1] = Player[i].Card[j];
				Player[i].Card[j] = tmp;
			}
	}
	return 0;
}
BYTE Swapoutcard(int pos,BYTE *ptrc,int end)
{
	int i,k;

	k = *(ptrc+pos);
	for (i=pos ; i<= end ; i++ )
		*(ptrc+i) = *(ptrc+i+1);

	return k;
}

int StoreStructToMessage(void)
{
	char *p;
	int i;

	p = (char*)(&Player);
	for (i = 0; i < sizeof(struct PLAYER)*4/*142*/ ; i++)
		StrMessage[i+2] = *(p+i);

	return 0;
}
int StoreMessageToStruct(void)
{
	char *p;
	int i;

	p = (char*)(&Player);

	for (i = 0; i < sizeof(struct PLAYER)*4/*142*/ ; i++)
		*(p+i) = StrMessage[i+2];

	return 0;
}

int InitAnotherPlayer(void)
{
	char cpname[3][16] = { "_Sukorn","_PIG","_TATA" };
	int i,j;

	i = 0;
	while ( Player[i].PlayType == HUMAN)
		i++; // count player
	j = 1;
	while (i<=3)
	{
		Player[i].PlayType = COMPUTER;
		sprintf(tmpbuf,cpname[j-1],j);
		strcpy(Player[i].PlayName,tmpbuf);
		i++;j++;
	}

	return 0;
}


//===========================================================
//            Start game with player #Whofirst
//===========================================================
int InitGameStartup(int WhoFirst)
{
	int i;

	PlayFirst = WhoFirst;
	PlayOnAct = WhoFirst;
	DistributeCard();

	for (i = 1 ; i<=3 ; i++)
	{
		if (Player[i].PlayType == HUMAN)
		{	// send Player structure
			StrMessage[0] = SERCLI_MSG_PLAYSTRUCT;
			StrMessage[1] = (BYTE) 150;
			StoreStructToMessage();
			StrMessage[149] = (BYTE) PlayOnAct;
			DdePostAdvise(idInst,hszTopic,hszPlayItem[i][CurChannel[i]]);
			if (CurChannel[i] >= 7) CurChannel[i] = 0; else CurChannel[i]++;
		}
	}
	PostMessage(hWnd,WM_BEFOREACTIVE,0,0);
	InvalidateRect(hWnd,NULL,TRUE);
	// reset acknownledge of server process
	CollectAcknowledge(RESET_ACK,PlayOnAct,0);
	return 0;
}

//============================================================
//                     Player Management
//============================================================
int BeforePlayerOnActive(void)
{

	if (Player[PlayOnAct].PlayType == HUMAN)
	{
		if (PlayOnAct == 0)
		{	// server on active
			PostMessage(hWnd,WM_SETACTIVEPLAYER,0,0);
			//bActivePlay = TRUE;
		}
		else
		{   // send mesg active to player
			StrMessage[0] = SERCLI_MSG_ACTIVE;
			StrMessage[1] = 2;
			DdePostAdvise(idInst,hszTopic,hszPlayItem[PlayOnAct][CurChannel[PlayOnAct]]);
			if (CurChannel[PlayOnAct] >= 7)
				CurChannel[PlayOnAct] = 0;
			else
				CurChannel[PlayOnAct]++;
		}
	}
	else
	{
		PostMessage(hWnd,WM_ROBOTANA,0,0);
	}
	//sprintf(tmpbuf,"Wait for %s play",Player[PlayOnAct].PlayName);
	//ShowTxtMsg(tmpbuf);
	return 0;
}
int AfterPlayerOnActive(int cardout)
{
	int			i;

	// server can change CurSuit only
	if (PlayOnAct == PlayFirst)
		CurSuit = (Player[PlayOnAct].Card[cardout]-1)/13;

	for (i = 1 ; i<=3; i++) // send for Playonact first
	{
		if ((Player[i].PlayType == HUMAN)&&(i != PlayOnAct))
		{	// send screen update
			//j = (PlayOnAct-PlayFirst>=0)?PlayOnAct-PlayFirst:PlayOnAct-PlayFirst+4;
			StrMessage[0] = SERCLI_MSG_OUTCARD;
			StrMessage[1] = 5;
			StrMessage[2] = (BYTE)PlayOnAct;                     // player actv
			StrMessage[3] = Player[PlayOnAct].Card[cardout];     // card number
			StrMessage[4] = CurSuit;//(BYTE)(HistActCard[PlayFirst]-1)/13;
			DdePostAdvise(idInst,hszTopic,hszPlayItem[i][CurChannel[i]]);
			if (CurChannel[i] >= 7) CurChannel[i] = 0; else CurChannel[i]++;
		}
	}

	//SendMessage(hWnd,WM_OUTCARDFROMHAND,PlayOnAct,cardout);
	PostMessage(hWnd,WM_OUTCARDFROMHAND,PlayOnAct,cardout);

	PlayOnAct++;
	if (PlayOnAct>3) PlayOnAct-=4;

	if (PlayOnAct == PlayFirst)
		PostMessage(hWnd,WM_AFTERACTIVE1,0,0);
	else
		PostMessage(hWnd,WM_BEFOREACTIVE,0,0);

	return 0;
}
// check step 2
int AfterPlayerOnActive1(void)
{
	int i;
	int playlast;

	// set PlayFirst , send mesg got card to all client
	playlast = PlayOnAct;
	PlayOnAct = CalWhoGotCard();
	PlayFirst = PlayOnAct;

	for (i = 1 ; i<=3 ; i++)
	{
		if (Player[i].PlayType == HUMAN)
		{
			StrMessage[0] = SERCLI_MSG_GOTCARD;
			StrMessage[1] = 4;
			StrMessage[2] = (BYTE)PlayOnAct;
			StrMessage[3] = (BYTE)playlast;
			DdePostAdvise(idInst,hszTopic,hszPlayItem[i][CurChannel[i]]);
			if (CurChannel[i] >= 7) CurChannel[i] = 0; else CurChannel[i]++;
		}
	}
	//SendMessage(hWnd,WM_GOTCARDTOHAND,PlayOnAct,0);
	PostMessage(hWnd,WM_GOTCARDTOHAND,PlayOnAct,playlast);

	if (Player[PlayOnAct].Card[0] == 0)
		PostMessage(hWnd,WM_AFTERACTIVE2,0,0);
	else
		PostMessage(hWnd,WM_BEFOREACTIVE,0,0);

	return 0;
}
int AfterPlayerOnActive2(void)
{
	int i;
	// send mesg cal score to client
	for (i = 1; i<=3 ; i++)
	{
		if (Player[i].PlayType == HUMAN)
		{
			StrMessage[0] = SERCLI_MSG_SHOWSCORE;
			StrMessage[1] = 2;
			DdePostAdvise(idInst,hszTopic,hszPlayItem[i][CurChannel[i]]);
			if (CurChannel[i] >= 7) CurChannel[i] = 0; else CurChannel[i]++;
		}
	}
	SendMessage(hWnd,WM_SHOWSTATISTIC,0,0);
	//PostMessage(hWnd,WM_SHOWSTATISTIC,0,0);

	if (nHavePigPlayer < 0)
	{
		InitGameStartup(PlayFirst);
		nHavePigPlayer = -1;
	}

	return 0;
}

int MoveCard(int Cnum,int x1,int y1,int x2,int y2,int flag)
{
	int		i;//,j;
	double	m,c,step;
	HDC		hdcBk,hdcBkc;
	HBITMAP	hbmpBk,hbmpBkc;
	int		pvx,pvy,crx,cry,lx,ly,dx,dy;
	int		crdspd;
	//HDC		hdcBk,;
	//HBITMAP	hbmpBk;
	//int		pvx,pvy,crx,cry;
	if (x2-x1 == 0)
		m = (double)1;
	else
		m = (double)(y2-y1)/(x2-x1);
	c = (double)(y1-(m*x1));

	if (flag == 0) // if got card speed up card
	{
		step = (double)((x2-x1)/CardSpeed);
		crdspd = CardSpeed;
	}
	else
	{
		step = (double)((x2-x1)/(CardSpeed/3));
		crdspd = CardSpeed/3;
	}

/*	hdc = GetDC(hWnd);
	hdcBk = CreateCompatibleDC(hdc);
	hbmpBk = CreateCompatibleBitmap(hdc,CARDWIDTH,CARDHEIGHT);
	SelectObject(hdcBk,hbmpBk);
	MakeBmpCard(Cnum);

	pvx = x1;
	pvy = y1;
	BitBlt(hdcBk,0,0,CARDWIDTH,CARDHEIGHT,hdc,pvx,pvy,SRCCOPY);
	for ( i = 1 ;i<=15 ; i++)
	{
		crx = x1+step*i;
		cry = m*(x1+step*i)+c;
		BitBlt(hdc,pvx,pvy,CARDWIDTH,CARDHEIGHT,
				hdcBk,0,0,SRCCOPY);
		BitBlt(hdcBk,0,0,CARDWIDTH,CARDHEIGHT,
				hdc,crx,cry,SRCCOPY);
		BitBlt(hdc,crx,cry,CARDWIDTH,CARDHEIGHT,
				hDCBmp,0,0,SRCCOPY);
		for (j = 0 ; j<= 10000 ; j++);
		pvx = crx;
		pvy = cry;
	}
	BitBlt(hdc,crx,cry,CARDWIDTH,CARDHEIGHT,hdcBk,0,0,SRCCOPY);
	ReleaseDC(hWnd,hdc);
	DeleteDC(hdcBk);
	DeleteObject(hbmpBk);
*/
	hdc = GetDC(hWnd);
	hdcBk = CreateCompatibleDC(hdc);
	hbmpBk = CreateCompatibleBitmap(hdc,CARDWIDTH,CARDHEIGHT);
	SelectObject(hdcBk,hbmpBk);
	hdcBkc = CreateCompatibleDC(hdc);
	hbmpBkc = CreateCompatibleBitmap(hdc,600,420);
	SelectObject(hdcBkc,hbmpBkc);
	MakeBmpCard(Cnum);

	pvx = x1;
	pvy = y1;
	if (flag != 0) // if move from empty will clear image at start point
					// if you have image at center screen, edit this code
// **** rem PosHistCard[4][2] = {{265,201},{190,152},{265,103},{340,152}};
	{
		BitBlt(hdcBk,0,0,CARDWIDTH,CARDHEIGHT,
					hdcTable,
					PosHistCard[flag-1][0]-150,
					PosHistCard[flag-1][1]-100,
					SRCCOPY);
	}
	else
	{
		BitBlt(hdcBk,0,0,CARDWIDTH,CARDHEIGHT,hdc,pvx,pvy,SRCCOPY);
	}

	for ( i = 0 ;i<=crdspd ; i++)
	{
		if (i == crdspd)
		{
			crx = x2; cry = y2;
		}
		else
		{
			crx = x1+(int)step*i;
			cry = (int)(m*(x1+step*i)+c);
		}
		if (crx > pvx)
			lx = pvx;
		else
			lx = crx;
		dx = abs(pvx-crx);
		if (cry > pvy)
			ly = pvy;
		else
			ly = cry;
		dy = abs(pvy-cry);
		BitBlt(hdcBkc,lx,ly,dx+CARDWIDTH,dy+CARDHEIGHT,
				hdc,lx,ly,SRCCOPY);
		BitBlt(hdcBkc,pvx,pvy,CARDWIDTH,CARDHEIGHT,
				hdcBk,0,0,SRCCOPY);
		BitBlt(hdcBk,0,0,CARDWIDTH,CARDHEIGHT,
				hdcBkc,crx,cry,SRCCOPY);
		if (i == crdspd)  // draw card with correct corner at dest point
			PutCardImage(hdcBkc,Cnum,crx,cry);
		else
			BitBlt(hdcBkc,crx,cry,CARDWIDTH,CARDHEIGHT,
					hDCBmp,0,0,SRCCOPY);
		BitBlt(hdc,lx,ly,dx+CARDWIDTH,dy+CARDHEIGHT,
				hdcBkc,lx,ly,SRCCOPY);
		pvx = crx;
		pvy = cry;
	}
	if (flag != 0) // clear card image at dest. point
		BitBlt(hdc,crx,cry,CARDWIDTH,CARDHEIGHT,hdcBk,0,0,SRCCOPY);
	ReleaseDC(hWnd,hdc);
	DeleteDC(hdcBk);
	DeleteObject(hbmpBk);
	DeleteDC(hdcBkc);
	DeleteObject(hbmpBkc);

	return 0;
}

//=====================================================
//			update card when card is out from who
//=====================================================
int OutCardFromHand(int who,BYTE whcard) // play on act , position card
{
	int i;
	int idx;
	int lastplay;

	RECT	rect;
	#ifdef DEBUG_ON
	RECT	rectp[4] = {{175,300,428,398},
						{20,98,146,302},
						{175,2,426,98},
						{470,98,596,302}};
	#else
	RECT	rectp[4] = {{175,300,428,398},
						{40,98,111,302},
						{175,2,426,98},
						{490,98,561,302}};
	if (CheatMode)
	{
		rectp[0].left = 175;rectp[0].top = 300;
		rectp[0].right = 428;rectp[0].bottom = 398;
		rectp[1].left = 20;rectp[1].top = 98;
		rectp[1].right = 146;rectp[1].bottom = 302;
		rectp[2].left = 175;rectp[2].top = 2;
		rectp[2].right = 426;rectp[2].bottom = 98;
		rectp[3].left = 470;rectp[3].top = 98;
		rectp[3].right = 596;rectp[3].bottom = 302;
	}
	#endif

//	if (MyPlayer == 0)
//	{
		HistActCard[who] = Player[who].Card[whcard];
/*	}
	else
	{
		if (who == MyPlayer)
		{
			HistActCard[who] = Player[who].Card[whcard];
		}
		else
		{	// client calc position again for correction
			for (i = 1 ; i<=Player[who].Card[0] ; i++)
			{
				if (HistActCard[who] == Player[who].Card[i])
				{
					whcard = i;
				}
			}
		}
	}
*/
	for (i = whcard;i<Player[who].Card[0];i++)
		Player[who].Card[i] = Player[who].Card[i+1];
	Player[who].Card[i] = 0;
	Player[who].Card[0]--;

	// update only histactcard
	idx = (who-MyPlayer>=0)?who-MyPlayer:who-MyPlayer+4;
	// update card in hand of my player
	rect = rectp[idx];
	InvalidateRect(hWnd,&rect,TRUE);
	UpdateWindow(hWnd);
	// clear last card
	MoveCard(HistActCard[who],
				PosPlayer[idx][0],
				PosPlayer[idx][1],
				PosHistCard[idx][0],
				PosHistCard[idx][1],0);

/*	// update HistActCard
	rect.left = PosHistCard[idx][0];
	rect.top = PosHistCard[idx][1];
	rect.right = rect.left+CARDWIDTH;
	rect.bottom = rect.top+CARDHEIGHT;
	InvalidateRect(hWnd,&rect,TRUE);
	UpdateWindow(hWnd);
*/
	if (MyPlayer != 0) // client set next player , server not ess.
	{
		if (who+1 > 3) PlayOnAct = 0; else PlayOnAct = who+1;
	}
	lastplay = 0;
	for ( i = 0; i<=3 ; i++)
		if ( HistActCard[i] == 0 ) lastplay++;
	if ( lastplay == 0 ) // last player
	{   // not update next player
	}
	else
	{	// update next player
		sprintf(tmpbuf,"Wait for %s play",Player[PlayOnAct].PlayName);
		ShowTxtMsg(tmpbuf);
		ShowActivePlayer(PlayOnAct,0);
	}

	return 0;
}
//==========================================
//			Calculate Who got card
//==========================================
int CalWhoGotCard(void)
{
	int i,WhoGot;
	int tcnum,tsuit,cnum,suit;

	WhoGot = PlayFirst;
	tcnum = HistActCard[PlayFirst];
	tsuit = (tcnum-1) / 13;
	for (i = 0 ; i<=3 ; i++)
	{
		cnum = HistActCard[i];
		suit = (cnum-1) / 13;
		if ( (tsuit == suit)&&(cnum > tcnum) )
		{
			tcnum = HistActCard[i];
			WhoGot = i;
		}
	}

	return(WhoGot);
}

//=======================================================
//		update recv card of player which received
//=======================================================
int GotCardToHand(int who,int wholast)
{
	int		i,j,tmp;
	int		idx,idx3;
	//RECT	rect;
	MSG		msg;

	idx = (who-MyPlayer>=0)?who-MyPlayer:who-MyPlayer+4;

	if (MyPlayer != 0 ) // client set next player , server not ess.
		PlayOnAct = who;
	ShowActivePlayer(who,0);

	sprintf(tmpbuf,"%s got card",Player[PlayOnAct].PlayName);
	ShowTxtMsg(tmpbuf);

	// fix bug of lost msg
	// wait for 1 - 1.5 sec.
	for (i = 0 ; i<=2 ; i++)
	{
		while (!PeekMessage(&msg,hWnd,WM_TIMER,WM_TIMER,PM_REMOVE))
		{
			WaitMessage();
		}
	}

	for (j = wholast ; j<=wholast+3 ; j++)
	{
		i = (j<=3)?j:j-4;
		switch (HistActCard[i])
		{
			case 9: //10 club
				RecvCardUpdate(who,9);
				break;
			case 23: // J diamond
				RecvCardUpdate(who,23);
				break;
			case 37:  // Q poedum
				RecvCardUpdate(who,37);
				break;
			default:
				if ( (HistActCard[i]>=40) && (HistActCard[i]<=52) )
				{
					RecvCardUpdate(who,HistActCard[i]);
				}
				break;
		}
		tmp = HistActCard[i];
		HistActCard[i] = 0;
		// update screen

		idx3 = (i-MyPlayer>=0)?i-MyPlayer:i-MyPlayer+4;
		//rect.left = PosHistCard[idx3][0];
		//rect.top = PosHistCard[idx3][1];
		//rect.right = rect.left+CARDWIDTH;
		//rect.bottom = rect.top+CARDHEIGHT;
		//InvalidateRect(hWnd,&rect,TRUE);
		//UpdateWindow(hWnd);
		MoveCard(tmp,
					PosHistCard[idx3][0],
					PosHistCard[idx3][1],
					PosPlayer[idx][0],
					PosPlayer[idx][1],idx3+1); // clear image at start point
	}

	sprintf(tmpbuf,"Wait for %s play",Player[PlayOnAct].PlayName);
	ShowTxtMsg(tmpbuf);

	CurSuit = FREE;
	return 0;
}
int RecvCardUpdate(int who,BYTE card)
{
	int i,j;
	for (i = 1 ;i <= RecvCard[who][0]+1 ; i++)
	{
		if (RecvCard[who][i]==0)
		{
			RecvCard[who][i] = card;
			RecvCard[who][0]++;
			return 0;
		}
		else
		{
			if ( card < RecvCard[who][i])
			{
				for ( j=RecvCard[who][0] ; j>=i; j--)
				{
					RecvCard[who][j+1] = RecvCard[who][j];
				}
				RecvCard[who][i] = card;
				RecvCard[who][0]++;
				return 0;
			}
		}
	}
	return 0;
}


//=================================================================
// 					Start up Dialog
//=================================================================
BOOL FAR PASCAL StartDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	HWND	hTemp;
	char tbuf[8];

	switch(message)
	{
		case WM_INITDIALOG:
			SendDlgItemMessage(hDlg,IDC_SERVER,BM_SETCHECK,1,0L);    // set defalut
			SendDlgItemMessage(hDlg,ScoreLimit=GetPrivateProfileInt("Init","Score",
										1000,szIniName),
										BM_SETCHECK,2,0L);
			ScoreLimit=-ScoreLimit;
			if ( GetPrivateProfileString("Init","Name","",
					tbuf,sizeof(tbuf),szIniName) > 1 )
			{
				SetDlgItemText(hDlg,IDC_NAME,tbuf);
			}
			nPlayMode = SERVERMODE;
			MoveToCenter(hDlg);
			return 	TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case 500:
				case 1000:
				case 1500:
				case 2000:
				case 2500:
				case 3000:
					ScoreLimit = -(int)(wParam);
					break;
				case IDC_SERVER:
					nPlayMode = SERVERMODE;
					break;
				case IDC_CLIENT:
					nPlayMode = CLIENTMODE;
					break;
				case IDC_CONTINUE:
					hTemp = GetDlgItem(hDlg,IDC_NAME);
					GetWindowText(hTemp,tmpbuf,sizeof(tmpbuf));
					if (strlen(tmpbuf)<=1)
					{
						MessageBox(hDlg,"Your name should longer 1",
										szAppName,MB_OK|MB_ICONSTOP);
						SetFocus(hTemp);
						break;
					}
					if (strlen(tmpbuf)>8)
					{
						MessageBox(hDlg,"Your name shouldn't longer 8",
										szAppName,MB_OK|MB_ICONSTOP);
						SetFocus(hTemp);
						break;
					}
					strcpy(PlayerName,tmpbuf); // store user name
					WritePrivateProfileString("Init","Name",tmpbuf,szIniName);
					sprintf(tmpbuf,"%d",-ScoreLimit);
					WritePrivateProfileString("Init","Score",tmpbuf,szIniName);
					EndDialog(hDlg,0);
					break;
				case IDC_EXIT:
					nPlayMode = (BYTE)UNKNOWN;
					EndDialog(hDlg,-1);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
//=================================================================
// 					Client mode Dialog
//=================================================================
BOOL FAR PASCAL ClientDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	HWND		hTemp;

	switch(message)
	{
		case WM_INITDIALOG:
			MoveToCenter(hDlg);
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_CONTINUE:
					hTemp = GetDlgItem(hDlg,IDC_DEST);
					GetWindowText(hTemp,tmpbuf,sizeof(tmpbuf));
					strcpy(CompConn,tmpbuf); // store computer which connect

					// connect to server
					/*if (StartWithClient()==-1)
					{
						DestroyWindow(hWnd);
					}
					EndDialog(hDlg,-1);
					*/
					if (StartWithClient()==-1)
					{
						MessageBox(hWnd,"Can't connect to server.",
										szAppName,MB_OK|MB_ICONSTOP);
						SetFocus(hDlg);
					}
					else
					{
						EndDialog(hDlg,0);
					}
					break;
				case IDC_EXIT:
					EndDialog(hDlg,-1);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
//=================================================================
// 					About Dialog
//=================================================================
BOOL FAR PASCAL AboutDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	static	HWND	hIcon[4];
	static	curtm=0,curps=0;
	RECT		pr = {20,20,98,98};
	int i;

	switch(message)
	{
		case WM_INITDIALOG:
			MoveToCenter(hDlg);
			SetTimer(hDlg,ABOUTICON_TIMER,150,NULL);
			for ( i = 0 ; i<=3 ; i++ )
				hIcon[i] = GetDlgItem(hDlg,1291+i);
			return TRUE;
		case WM_TIMER:
			MoveWindow(hIcon[curps%4]    ,20,20+curtm,32,32,0);
			MoveWindow(hIcon[(curps+1)%4],64-curtm,20,32,32,0);
			MoveWindow(hIcon[(curps+2)%4],64,64-curtm,32,32,0);
			MoveWindow(hIcon[(curps+3)%4],20+curtm,64,32,32,0);
			curtm+=2;if (curtm == 44) curtm = 0;
			curps++; if (curps ==  4) curps = 0;
			InvalidateRect(hDlg,&pr,TRUE);
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					KillTimer(hDlg,ABOUTICON_TIMER);
					EndDialog(hDlg,0);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
//=================================================================
// 					Option Dialog
//=================================================================
BOOL FAR PASCAL OptionDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	static	HDC	hdcbc;
	static	HBITMAP hbmpbc;
	static	int bcard;
	HANDLE	htmp;
	int		ret;
	LPDRAWITEMSTRUCT DIS;
	HDC		hdctmp;

	switch(message)
	{
		case WM_INITDIALOG:
			hdctmp = GetDC(hDlg);
			hdcbc = CreateCompatibleDC(hdctmp);
			hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(BackCard));
			SelectObject(hdcbc,hbmpbc);

			bcard = BackCard;

			ReleaseDC(hDlg,hdctmp);
			MoveToCenter(hDlg);
			return TRUE;
		case WM_DRAWITEM:
			DIS = (LPDRAWITEMSTRUCT) lParam;
			if ((DIS->CtlID >= IDC_BCARD1) && (DIS->CtlID <= IDC_BCARDS))
			{
				if (DIS->CtlID == IDC_BCARDS)
					hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(bcard));
				else
					if (DIS->CtlID == IDC_BCARDC)
						hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(BackCard));
					else
						hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(DIS->CtlID-48));
				DeleteObject(SelectObject(hdcbc,hbmpbc));
				BitBlt(DIS->hDC,0,0,CARDWIDTH,CARDHEIGHT,hdcbc,0,0,SRCCOPY);
				BitBlt(DIS->hDC,CARDWIDTH,0,1,CARDHEIGHT,hdcbc,CARDWIDTH-1,0,SRCCOPY);
			}
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_BCARD1:
				case IDC_BCARD2:
				case IDC_BCARD3:
				case IDC_BCARD4:
					bcard = (int)wParam-48;
					htmp = GetDlgItem(hDlg,IDC_BCARDS);
					InvalidateRect(htmp,NULL,FALSE);
					UpdateWindow(htmp);
					break;
				case IDOK:
					DeleteDC(hdcbc);
					DeleteObject(hbmpbc);
					ret = 0;
					if (BackCard != bcard)
					{
						BackCard = bcard;
						ret = 1;
					}
					EndDialog(hDlg,ret);
					break;
				case IDCANCEL:
					DeleteDC(hdcbc);
					DeleteObject(hbmpbc);
					EndDialog(hDlg,0);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
//=================================================================
// 					Statistic Dialog
//=================================================================
BOOL FAR PASCAL StatisticDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	int		i,j;
	PAINTSTRUCT		ps;
	static		LOGFONT		lf;
	static		dmode;
	HFONT		htmpFont;

	HDC		hdcDlg;
	static	int PlayScore[4] = {0,0,0,0};

	switch(message)
	{
		case WM_INITDIALOG:
			// set font style
			lf.lfHeight = 24;
			lf.lfWidth = 0;
			lf.lfEscapement = 0;
			lf.lfOrientation = 0;
			lf.lfWeight = FW_BLACK;
			lf.lfItalic = 0;
			lf.lfUnderline = 0;
			lf.lfStrikeOut = 0;
			lf.lfCharSet = ANSI_CHARSET;
			lf.lfPitchAndFamily = DEFAULT_PITCH;
			strcpy(lf.lfFaceName,"Helv");
			switch(lParam)
			{
				case STAT_ONHAND:
					SetWindowText(hDlg,"Card on hand , Current score");
					dmode = STAT_ONHAND;
					break;
				case STAT_OFGAME:
					dmode = STAT_OFGAME;
					for (i = 0 ; i<=3 ; i++)
					{
						PlayScore[i] = CalScoreEachPlayer(i);
						Player[i].nScore += PlayScore[i];
					}
					if (GameNum >= 20)
					{
						for (i=0;i<=3;i++)
							HistSum[i] = HistSum[i]+HistScore[GameNum%20][i];
					}
					//if (GameNum < 20)
					//{
						for (i = 0; i<= 3; i++)
						{
							HistScore[GameNum%20][i] = PlayScore[PlaySequence[i]];
						}
						GameNum++;
					//}
					SetWindowText(hDlg,"Score of a game");
					InvalidateRect(hWnd,NULL,FALSE);
					UpdateWindow(hWnd);
					break;
			}
			MoveToCenter(hDlg);
			return TRUE;
		case WM_PAINT:
			hdcDlg = BeginPaint(hDlg,&ps);
			SetBkMode(hdcDlg,TRANSPARENT);
			htmpFont = SelectObject(hdcDlg,CreateFontIndirect(&lf));
			if (dmode == STAT_OFGAME)
			{
				for (i = 0;i<=3 ;i++)
				{
					SetTextColor(hdcDlg,RGB(0,0,0));
					TextOut(hdcDlg,461,16+i*54,tmpbuf,
						sprintf(tmpbuf,"%5d",PlayScore[PlaySequence[i]]));
					SetTextColor(hdcDlg,RGB(128,128,128));
					TextOut(hdcDlg,460,15+i*54,tmpbuf,
						sprintf(tmpbuf,"%5d",PlayScore[PlaySequence[i]]));
				}
			}
			for (i = 0; i<=3 ;i++)
			{
				SetTextColor(hdcDlg,RGB(0,0,0));
				TextOut(hdcDlg,461,39+i*54,tmpbuf,
						sprintf(tmpbuf,"%5d",Player[PlaySequence[i]].nScore));
				TextOut(hdcDlg,12,22+i*54,tmpbuf,
						sprintf(tmpbuf,"%s",Player[PlaySequence[i]].PlayName));
				SetTextColor(hdcDlg,RGB(128,128,128));
				TextOut(hdcDlg,11,21+i*54,tmpbuf,
						sprintf(tmpbuf,"%s",Player[PlaySequence[i]].PlayName));
				SetScoreColor(hdcDlg,Player[PlaySequence[i]].nScore);
				TextOut(hdcDlg,460,38+i*54,tmpbuf,
						sprintf(tmpbuf,"%5d",Player[PlaySequence[i]].nScore));
			}
			for (j = 0; j<=3 ;j++)
			{
				for (i = 1; i<=RecvCard[PlaySequence[j]][0] ; i++)
					//if (i != RecvCard[PlaySequence[j]][0])
					//	PutSliceCardImage(hdcDlg,RecvCard[PlaySequence[j]][i],i*15+132,20+j*52);
					//else
						PutCardImage(hdcDlg,RecvCard[PlaySequence[j]][i],i*15+132,20+j*52);
			}
			DeleteObject(SelectObject(hdcDlg,htmpFont));
			EndPaint(hDlg,&ps);
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
					// check pig player
					nHavePigPlayer = WhoPigPlayer();
					EndDialog(hDlg,0);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
int CalScoreEachPlayer(int who)
{
	int CardValue[13] = { 2,3,4,5,6,7,8,9,10,20,30,40,50 };
	int	ScoreHeart,ScoreTotal;
	int numheart=0;
	int i;
	int	have10club = FALSE;

	if (RecvCard[who][0] == 16)
	{
		return 1000; // big chuan
	}
	if ((RecvCard[who][1] == 9)&&(RecvCard[who][0]==1))
	{
		return 50; // 10 club only
	}
	if (RecvCard[who][0] == 0)
	{
		return -50; // havn't any card
	}
	ScoreTotal = 0;
	ScoreHeart = 0;
	numheart = 0;
	have10club = FALSE;
	for (i = 1 ; i<=RecvCard[who][0] ; i++)
	{
		switch(RecvCard[who][i])
		{
			case 9:
				have10club = TRUE;
				break;
			case 23:
				ScoreTotal += 100;
				break;
			case 37:
				ScoreTotal -= 100;
				break;
			default:
				ScoreHeart -= CardValue[RecvCard[who][i]-40];
				numheart++;
				break;
		}
	}
	if (numheart == 13) ScoreHeart = -ScoreHeart;
	if (have10club == TRUE)
	{
		return 2*(ScoreHeart+ScoreTotal);
	}
	else
	{
		return (ScoreHeart+ScoreTotal);
	}

}

int WhoPigPlayer()
{
	int i,pgp,maxsc;

	maxsc = Player[0].nScore;
	pgp = -1;
	for (i = 0 ; i<=3 ; i++)
	{
		if ((Player[i].nScore <= ScoreLimit)&&
				Player[i].nScore <= maxsc)
		{
			pgp = i;
			maxsc = Player[i].nScore;
		}
	}
	if (pgp == -1) return -1;
	return pgp;
}
//=================================================================
// 					History Dialog
//=================================================================
BOOL FAR PASCAL HistoryDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	PAINTSTRUCT		ps;
	int		i,j,k,l,sum[4];
	HDC		hdcDlg;
	static	int listmode;

	switch(message)
	{
		case WM_INITDIALOG:
			i = GetPrivateProfileInt("Init","History",1,szIniName);
			if ( i == 1 )
				listmode = 0;  // list each
			else
				listmode = 1;
			MoveToCenter(hDlg);
			return TRUE;
		case WM_PAINT:
			hdcDlg = BeginPaint(hDlg,&ps);
			SetBkMode(hdcDlg,TRANSPARENT);
			for (j = 0 ;j<=3 ; j++)
			{
				if (GameNum > 20)
				{
					sum[j] = HistSum[j];
					k = 20;
					l = GameNum%20;
				}
				else // <= 20
				{
					sum[j] = 0;
					k = GameNum;
					l = 0;
				}
				for (i = 0; i< k ; i++)
				{
					if (listmode == 0) // is list each
					{
						SetTextColor(hdcDlg,RGB(0,0,0));
						TextOut(hdcDlg,25+j*73,26+i*12,tmpbuf,
								sprintf(tmpbuf,"%5d",HistScore[l][j]));
						if (HistScore[l][j] >= 0)
							SetTextColor(hdcDlg,RGB(0,255,0));
						else if (HistScore[l][j] > -100)
							SetTextColor(hdcDlg,RGB(0,0,255));
						else SetTextColor(hdcDlg,RGB(255,0,0));
						TextOut(hdcDlg,24+j*73,25+i*12,tmpbuf,
								sprintf(tmpbuf,"%5d",HistScore[l][j]));
					}
					else // list sum
					{
						SetTextColor(hdcDlg,RGB(0,0,0));
						TextOut(hdcDlg,25+j*73,26+i*12,tmpbuf,
								sprintf(tmpbuf,"%5d",sum[j]+=HistScore[l][j]));
						SetScoreColor(hdcDlg,sum[j]);
						TextOut(hdcDlg,24+j*73,25+i*12,tmpbuf,
								sprintf(tmpbuf,"%5d",sum[j]));
					}
					l++;
					if (l>=20) l = 0;
				}
				SetTextColor(hdcDlg,RGB(0,0,0));
				TextOut(hdcDlg,25+j*73,273,tmpbuf,
						sprintf(tmpbuf,"%5d",Player[PlaySequence[j]].nScore));
				TextOut(hdcDlg,10+j*73,6,tmpbuf,
						sprintf(tmpbuf,"%s",Player[PlaySequence[j]].PlayName));
				SetTextColor(hdcDlg,RGB(128,128,128));
				TextOut(hdcDlg,9+j*73,5,tmpbuf,
						sprintf(tmpbuf,"%s",Player[PlaySequence[j]].PlayName));
				SetScoreColor(hdcDlg,Player[PlaySequence[j]].nScore);
				TextOut(hdcDlg,24+j*73,272,tmpbuf,
						sprintf(tmpbuf,"%5d",Player[PlaySequence[j]].nScore));
			}
			EndPaint(hDlg,&ps);
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_LISTTYPE:
					if (listmode == 0)
						listmode = 1;
					else
						listmode = 0;
					InvalidateRect(hDlg,NULL,TRUE);
					UpdateWindow(hDlg);
					break;
				case IDOK:
					if ( listmode == 0 )
						WritePrivateProfileString("Init","History","1",szIniName);
					else
						WritePrivateProfileString("Init","History","0",szIniName);
					EndDialog(hDlg,0);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
//=================================================================
// 					PigPlayer Dialog
//=================================================================
BOOL FAR PASCAL PigPlayerDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	HDC					hdcdlg;
	static	HDC			hdcmem,hdcmem2,hdcpig,hdcmpig;
	static	HBITMAP		hbmpmem,hbmpmem2,hbmppig,hbmpmpig;
	PAINTSTRUCT			ps;
	static	direct = 1;
	static	float s,v,t,g;
	int dx, dy ;
	int startx, starty ;
	DLGPROC	lpfnDlg;
	static	struct PIGRUN { int PosX,PosY,PrevPosX,PrevPosY; } pigrun;

	switch(message)
	{
		case WM_INITDIALOG:
			sprintf(tmpbuf,"%s is a PIG !!! ha ha ha",Player[(int)lParam].PlayName);
			ShowTxtMsg(tmpbuf);
			pigrun.PosX = pigrun.PrevPosX = 20;
			pigrun.PosY = pigrun.PrevPosY = 50;
			v = (float)0.0;t = (float)1.0;g = (float)1.0;
			hdcdlg = GetDC(hDlg);
			hdcmem = CreateCompatibleDC(hdcdlg);
			hbmpmem = LoadBitmap(ghResInstance,MAKEINTRESOURCE(1101));
			SelectObject(hdcmem,hbmpmem);
			hdcmem2 = CreateCompatibleDC(hdcdlg);
			hbmpmem2 = LoadBitmap(ghResInstance,MAKEINTRESOURCE(1101));
			SelectObject(hdcmem2,hbmpmem2);

			hdcpig = CreateCompatibleDC(hdcdlg);
			hbmppig = LoadBitmap(ghResInstance,"rgdw");
			SelectObject(hdcpig,hbmppig);
			hdcmpig = CreateCompatibleDC(hdcdlg);
			hbmpmpig = LoadBitmap(ghResInstance,"mrgdw");
			SelectObject(hdcmpig,hbmpmpig);

			SetBkMode(hdcmem,TRANSPARENT);
			TextOut(hdcmem,30,68,Player[(int)lParam].PlayName,
						lstrlen(Player[(int)lParam].PlayName));
			TextOut(hdcmem2,30,68,Player[(int)lParam].PlayName,
						lstrlen(Player[(int)lParam].PlayName));
			ReleaseDC(hDlg,hdcdlg);
			SetTimer(hDlg,PIGJUMP_TIMER,100,NULL);
			MoveToCenter(hDlg);
			return TRUE;
		case WM_PAINT:
			hdcdlg = BeginPaint(hDlg,&ps);
			BitBlt(hdcdlg,20,20,400,120,hdcmem,0,0,SRCCOPY);
			EndPaint(hDlg,&ps);
			return TRUE;
		case WM_TIMER:
			if (direct == 1) pigrun.PosX += 2;
			else pigrun.PosX -= 2;
			if (pigrun.PosX >= 332)
				direct = 0;
			if (pigrun.PosX <= 0)
				direct = 1;
			if (v == 0)
			{
				if (direct == 1)
				{
					hbmppig = LoadBitmap(ghResInstance,"rgdw");
					DeleteObject(SelectObject(hdcpig,hbmppig));
					hbmpmpig = LoadBitmap(ghResInstance,"mrgdw");
					DeleteObject(SelectObject(hdcmpig,hbmpmpig));
				}
				else
				{
					hbmppig = LoadBitmap(ghResInstance,"lfdw");
					DeleteObject(SelectObject(hdcpig,hbmppig));
					hbmpmpig = LoadBitmap(ghResInstance,"mlfdw");
					DeleteObject(SelectObject(hdcmpig,hbmpmpig));
				}
			}
			if (pigrun.PosY >= 70)
			{
				v = -v;
				if (direct == 1)
				{
					hbmppig = LoadBitmap(ghResInstance,"rgup");
					DeleteObject(SelectObject(hdcpig,hbmppig));
					hbmpmpig = LoadBitmap(ghResInstance,"mrgup");
					DeleteObject(SelectObject(hdcmpig,hbmpmpig));
				}
				else
				{
					hbmppig = LoadBitmap(ghResInstance,"lfup");
					DeleteObject(SelectObject(hdcpig,hbmppig));
					hbmpmpig = LoadBitmap(ghResInstance,"mlfup");
					DeleteObject(SelectObject(hdcmpig,hbmpmpig));
				}
			}
			else v = v+1*t;
			s = v*t;
			pigrun.PosY = pigrun.PosY +(int)s;
			// put pig image to screen
			BitBlt(hdcmem2,pigrun.PrevPosX,pigrun.PrevPosY,68,48,
					hdcmem,pigrun.PrevPosX,pigrun.PrevPosY,SRCCOPY);
			BitBlt(hdcmem2,pigrun.PosX,pigrun.PosY,68,48,
					hdcmpig,0,0,SRCAND);
			BitBlt(hdcmem2,pigrun.PosX,pigrun.PosY,68,48,
					hdcpig,0,0,SRCINVERT);
			dx = pigrun.PosX - pigrun.PrevPosX;
			dy = pigrun.PosY - pigrun.PrevPosY;

			if( dx >= 0 )
				startx = pigrun.PrevPosX;
			else
			{
				dx = -dx;
				startx = pigrun.PosX;
			}

			if( dy >= 0 )
				starty = pigrun.PrevPosY;
			else
			{
				dy = -dy;
				starty = pigrun.PosY;
			}
			hdcdlg = GetDC(hDlg);
			BitBlt (hdcdlg, startx+20 , starty+20 , 68+dx, 48+dy,
					hdcmem2, startx, starty ,SRCCOPY) ;

			pigrun.PrevPosX = pigrun.PosX;
			pigrun.PrevPosY = pigrun.PosY;
			ReleaseDC(hDlg,hdcdlg);
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_CONTINUE:
					DeleteDC(hdcmem);
					DeleteObject(hbmpmem);
					DeleteDC(hdcmem2);
					DeleteObject(hbmpmem2);
					DeleteDC(hdcpig);
					DeleteObject(hbmppig);
					DeleteDC(hdcmpig);
					DeleteObject(hbmpmpig);
					KillTimer(hDlg,PIGJUMP_TIMER);
					EndDialog(hDlg,0);
					break;
				case IDC_HISTORY:
					lpfnDlg = (DLGPROC)MakeProcInstance(
										(FARPROC)HistoryDlg,
										ghResInstance);
					DialogBox(ghResInstance,"history",hDlg,lpfnDlg);
					FreeProcInstance(lpfnDlg);
					break;
			}
			return TRUE;
	}
	return FALSE;
}

//=================================================================
// 					Set Logic Dialog
//=================================================================
BOOL FAR PASCAL SetLogicDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	int i,j;
	HANDLE	htmp;
	char		tmpstr[16];
	static	nStartPlayer;

	switch(message)
	{
		case WM_INITDIALOG:
			MoveToCenter(hDlg);
			nStartPlayer = PlayOnAct;
			for (i=0;i<=3;i++)
			{
				Logic[i] = 0;
				SetDlgItemText(hDlg,IDC_NAME1+i,Player[i].PlayName);
				if ( Player[i].PlayType==COMPUTER )
				{
					htmp=GetDlgItem(hDlg,IDC_LOGIC1+i);
					EnableWindow(htmp,TRUE);
					for (j=0;j<NO_OF_LOGIC;j++)
						SendDlgItemMessage(hDlg,IDC_LOGIC1+i,CB_ADDSTRING,
											0,(LPARAM)((LPSTR)LogicName[j]));
					sprintf(tmpbuf,"Logic%d",i);
					j = GetPrivateProfileInt("Init",tmpbuf,0,szIniName);
					if ( j==0 )
						WritePrivateProfileString("Init",tmpbuf,"0",szIniName);
					SendDlgItemMessage(hDlg,IDC_LOGIC1+i,CB_SETCURSEL,j,0);
				}
			}
			SendDlgItemMessage(hDlg,IDC_NAME1+nStartPlayer,BM_SETCHECK,1,0);
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_NAME1:case IDC_NAME2:case IDC_NAME3:case IDC_NAME4:
					nStartPlayer=wParam-IDC_NAME1;
					break;
				case IDOK:
					for (i=0;i<=3;i++)
						if ( Player[i].PlayType==COMPUTER )
						{
							Logic[i] = (int)SendDlgItemMessage(hDlg,IDC_LOGIC1+i,
										CB_GETCURSEL,0,0);
							sprintf(tmpbuf,"Logic%d",i);
							sprintf(tmpstr,"%d",Logic[i]);
							WritePrivateProfileString("Init",tmpbuf,tmpstr,szIniName);
						}
					EndDialog(hDlg,nStartPlayer);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
//=================================================================
// 					Game Information Dialog
//=================================================================
BOOL FAR PASCAL GameInfoDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	int i;

	switch(message)
	{
		case WM_INITDIALOG:
			MoveToCenter(hDlg);
			for (i=0;i<=3;i++)
			{
				SetDlgItemText(hDlg,IDC_NAME1+i,Player[i].PlayName);
				if ( Player[i].PlayType==COMPUTER )
					SetDlgItemText(hDlg,IDC_LOGIC1+i,LogicName[Logic[i]]);
			}
			sprintf(tmpbuf,"%d",ScoreLimit);
			SetDlgItemText(hDlg,IDC_SCORELIMIT,tmpbuf);
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					EndDialog(hDlg,0);
					break;
			}
			return TRUE;
	}
	return FALSE;
}
//=================================================================
// 					HelpContent Dialog
//=================================================================
BOOL FAR PASCAL HelpContentDlg(HWND hDlg,UINT message,WORD wParam,LONG lParam)
{
	static	HDC	hdcbc;
	static	HBITMAP hbmpbc;
	LPDRAWITEMSTRUCT DIS;
	HDC		hdctmp;

	switch(message)
	{
		case WM_INITDIALOG:
			hdctmp = GetDC(hDlg);
			hdcbc = CreateCompatibleDC(hdctmp);
			hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(1));
			SelectObject(hdcbc,hbmpbc);
			ReleaseDC(hDlg,hdctmp);
			MoveToCenter(hDlg);
			return TRUE;
		case WM_DRAWITEM:
			DIS = (LPDRAWITEMSTRUCT) lParam;
			if ((DIS->CtlID >= 101) && (DIS->CtlID <= 116))
			{
				switch(DIS->CtlID)
				{
					case 114:
						hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(9));
						break;
					case 115:
						hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(23));
						break;
					case 116:
						hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(37));
						break;
					case 101:case 102:case 103:case 104:case 105:case 106:
					case 107:case 108:case 109:case 110:case 111:case 112:
					case 113:
						hbmpbc = LoadBitmap(ghResInstance,MAKEINTRESOURCE(DIS->CtlID-61));
						break;
				}
				DeleteObject(SelectObject(hdcbc,hbmpbc));
				BitBlt(DIS->hDC,0,0,CARDWIDTH,CARDHEIGHT,hdcbc,0,0,SRCCOPY);
				BitBlt(DIS->hDC,CARDWIDTH,0,1,CARDHEIGHT,hdcbc,CARDWIDTH-1,0,SRCCOPY);
			}
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					DeleteDC(hdcbc);
					DeleteObject(hbmpbc);
					EndDialog(hDlg,0);
					break;
			}
			return TRUE;
	}
	return FALSE;
}

/*=======================================================*/
/*                 AI function 									*/
/*=======================================================*/

//====================== AI variable =========================
int CurOrder = 1;					// Current order of this round -> 1-4
int CurFace = FREE;				// Current face of this round  -> FREE,1-4
int HistLeaded[4];				// no. of each face leaded
int CurScore[4];              // Current Score of current game
int ChkVoid[4][4];  				// Check void flag of each player, each face
int NumCard[4][4];				// number of card on each player and each face
BYTE StatCard[53];					// status of card
int ChkChuan;
int ChkBigChuan;
int ChkScore;
int MyPlayerID = 0;
int MyVar[4][10];		// variable , constant flag to store some knownledge
//================ AI function declaration ==================
int CardPlayer(int);
int CountAllLeaded(void);
int CountGotCard(int,int);
int CountLeaded(int);
int CurrentOrder(void);
int FaceLeaded(void);
int GameCardLeft(int);
int GameMaxCard(void);
int GameMaxPlayer(void);
int GameMinCard(void);
int GameMinPlayer(void);
int GameScore(int);
int HandFaceLeft(int);
int HandLessCard(int);
int HandLessEqCard(int);
int HandMaxCard(int);
int HandMinCard(int);
int HandMoreCard(int);
int HandMoreEqCard(int);
int HandNextCard(int);
int HandPrevCard(int);
int IsBigChuanBroken(void);
int IsCardOnGame(int);
int IsCardOnHand(int);
int IsCardOutGame(int);
int IsChuanBroken(void);
int IsFaceOnGame(int);
int IsGotCard(int,int);
int IsNoScoreCard(void);
int IsNoMoreScoreCard(void);
int IsVoid(int,int);
int LastScore(int);
int NextPlayer(int);
int PrevPlayer(int);
int RandomAnyCard(void);
int RandomCard(int);
int RandomFace(void);
int TotalScore(int);
//========== if OK send it to upper ================
int LeadedScore(void);
//=====================================================

int CollectAcknowledge(int act,int par1,int par2)
{
	int i,j,k;

	switch(act)
	{
		case OUTCARD:
			// CASE of Outcard from hand
			// player 'par1' take out card no. 'par2'
			if (CurOrder == 1)
			{
				CurFace = (Player[par1].Card[par2]-1)/13;
				HistLeaded[CurFace]++;
			}
			else
			{
				// update chkvoid
				if ( ChkVoid[par1][CurFace] == FALSE )
					if ( (Player[par1].Card[par2]-1)/13 != CurFace )
						ChkVoid[par1][CurFace] = TRUE;
			}
			CurOrder++;
			if ( CurOrder > 4 ) CurOrder = 1;
			NumCard[par1][(Player[par1].Card[par2]-1)/13]--;
			StatCard[Player[par1].Card[par2]] = 1;
			MyPlayerID = PlayOnAct;
			//sprintf(tmpbuf,"%d%d%d%d,%d%d%d%d,%d%d%d%d",
			//			ChkVoid[1][0],ChkVoid[1][1],ChkVoid[1][2],ChkVoid[1][3],
			//			ChkVoid[2][0],ChkVoid[2][1],ChkVoid[2][2],ChkVoid[2][3],
			//			ChkVoid[3][0],ChkVoid[3][1],ChkVoid[3][2],ChkVoid[3][3]);
			//SetWindowText(hWnd,tmpbuf);
			break;
		case GOTCARD:
			// player 'par1' got card in this round
			CurFace = FREE;
			MyPlayerID = par1;
			for (i=0;i<=3;i++)
				CurScore[i] = CalScoreEachPlayer(i);
			if ( ChkBigChuan == FALSE )
			{
				j = 0;
				for (i=0;i<=3;i++)
					if ( RecvCard[i][0] > 0 )
						j++;
				if ( j>1 )
					ChkBigChuan = TRUE;
			}
			if ( ChkChuan == FALSE )
			{
				k = 0;
				for (i=0;i<=3;i++)
					for (j=1;j<=RecvCard[i][0];j++)
						if ( (RecvCard[i][j]-1)/13 == HEART )
						{
							k++;
							break;
						}
				if ( k>1 )
					ChkChuan = TRUE;
			}
			if ( IsNoMoreScoreCard() )
				ChkScore = TRUE;
			break;
		case RESET_ACK:
			// start with 'par1'
			MyPlayerID = par1;
			CurFace = FREE;
			for (i = 0;i<=3 ;i++ )
			{
				HistLeaded[i] = 0;
				CurScore[i] = -50;
				for (j = 0; j<=3 ;j++)
					ChkVoid[i][j] = FALSE;
			}
			StoreNumCard();
			for (i=0;i<=52;i++)
				StatCard[i] = 0;
			ChkChuan = FALSE;
			ChkBigChuan = FALSE;
			ChkScore = FALSE;
			for (i=0;i<=3;i++)
				for (j=0;j<=9;j++)
					MyVar[i][j] = 0;
			break;
	}

	return 0;
}

int GetCardPosition(int card)
{
	int i,j;

	for (i=1;i<=Player[PlayOnAct].Card[0];i++)
		if (Player[PlayOnAct].Card[i] == card)
			return i;
	// if can't find required card return first card
	sprintf(tmpbuf,"Card %d not found",card);
	MessageBox(hWnd,tmpbuf,"Jubmoo",MB_OK);

	i = HistActCard[PlayFirst];
	i = (i-1)/13; // i is suit , 0,1,2,3

	// scan for club
	for (j = 1 ; j<= Player[PlayOnAct].Card[0] ; j++)
		if ( (Player[PlayOnAct].Card[j] >= (i*13+1)) &&
			(Player[PlayOnAct].Card[j] <= (i*13+13)) )
		{
			return j;
		}
	i = random(Player[PlayOnAct].Card[0])+1;
	return i;
}
int StoreNumCard()
{
	int i,j;

	for (i=0;i<=3;i++)
	{
		for (j=1;j<=Player[i].Card[0];j++)
			NumCard[i][(Player[i].Card[j]-1)/13]++;
	}
	return 0;
}
/*=================================================================*/
/*=================================================================*/
/*						            	 AI library	  							 */
/*=================================================================*/
/*=================================================================*/
int CardPlayer(int player)
{
	if (HistActCard[player] > 0)
		return HistActCard[player];
	else
		return -1;
}
/*=================================================================*/
int CountAllLeaded()
{
	return 13-Player[MyPlayerID].Card[0];
}
/*=================================================================*/
int CountGotCard(int player,int face)
{
	int i,sum;
	sum = 0;
	for (i=1;i<=RecvCard[player][0];i++)
		if ( (RecvCard[player][i]-1)/13 == face )
			sum++;
	return sum;
}
/*=================================================================*/
int CountLeaded(int face)
{
	return HistLeaded[face];
}
/*=================================================================*/
int CurrentOrder()
{
	return CurOrder;
}
/*=================================================================*/
int FaceLeaded()
{
	return CurFace;
}
/*=================================================================*/
int GameCardLeft(int face)
{
	int i,sum;

	sum = 0;
	for (i=0;i<=3;i++)
		sum+=NumCard[i][face];
	return sum;
}
/*=================================================================*/
int GameMaxCard()
{
	int i,maxcard,np,fc;

	switch(CurOrder)
	{
		case 1:
			return -1;
		case 2:
			return HistActCard[PlayFirst];
		case 3:
			np = (PlayFirst+1>3)?PlayFirst-3:PlayFirst+1;
			fc = (HistActCard[np]-1)/13;
			if ( fc == CurFace )
			{
				if ( HistActCard[PlayFirst] > HistActCard[np] )
					return HistActCard[PlayFirst];
				else
					return HistActCard[np];
			}
			return HistActCard[PlayFirst];
		case 4:
			maxcard = HistActCard[PlayFirst];
			for (i=1;i<=2;i++)
			{
				np = (PlayFirst+i>3)?PlayFirst+i-4:PlayFirst+i;
				fc = (HistActCard[np]-1)/13;
				if ( fc == CurFace )
				{
					if ( HistActCard[np] > maxcard )
						maxcard = HistActCard[np];
				}
			}
			return maxcard;
	}
	return -1;
}
/*=================================================================*/
int GameMaxPlayer()
{
	int i,maxcard,maxplayer,np,fc;

	switch(CurOrder)
	{
		case 1:
			return -1;
		case 2:
			return PlayFirst;
		case 3:
			np = (PlayFirst+1>3)?PlayFirst-3:PlayFirst+1;
			fc = (HistActCard[np]-1)/13;
			if ( fc == CurFace )
			{
				if ( HistActCard[PlayFirst] > HistActCard[np] )
					return PlayFirst;
				else
					return np;
			}
			return PlayFirst;
		case 4:
			maxplayer = PlayFirst;
			maxcard = HistActCard[PlayFirst];
			for (i=1;i<=2;i++)
			{
				np = (PlayFirst+i>3)?PlayFirst+i-4:PlayFirst+i;
				fc = (HistActCard[np]-1)/13;
				if ( fc == CurFace )
				{
					if ( HistActCard[np] > maxcard )
					{
						maxplayer = np;
						maxcard = HistActCard[np];
					}
				}
			}
			return maxplayer;
	}
	return -1;
}
/*=================================================================*/
int GameMinCard()
{
	int i,mincard,np,fc;

	switch(CurOrder)
	{
		case 1:
			return -1;
		case 2:
			return HistActCard[PlayFirst];
		case 3:
			np = (PlayFirst+1>3)?PlayFirst-3:PlayFirst+1;
			fc = (HistActCard[np]-1)/13;
			if ( fc == CurFace )
			{
				if ( HistActCard[PlayFirst] < HistActCard[np] )
					return HistActCard[PlayFirst];
				else
					return HistActCard[np];
			}
			return HistActCard[PlayFirst];
		case 4:
			mincard = HistActCard[PlayFirst];
			for (i=1;i<=2;i++)
			{
				np = (PlayFirst+i>3)?PlayFirst+i-4:PlayFirst+i;
				fc = (HistActCard[np]-1)/13;
				if ( fc == CurFace )
				{
					if ( HistActCard[np] < mincard )
						mincard = HistActCard[np];
				}
			}
			return mincard;
	}
	return -1;
}
/*=================================================================*/
int GameMinPlayer()
{
	int i,mincard,minplayer,np,fc;

	switch(CurOrder)
	{
		case 1:
			return -1;
		case 2:
			return PlayFirst;
		case 3:
			np = (PlayFirst+1>3)?PlayFirst-3:PlayFirst+1;
			fc = (HistActCard[np]-1)/13;
			if ( fc == CurFace )
			{
				if ( HistActCard[PlayFirst] < HistActCard[np] )
					return PlayFirst;
				else
					return np;
			}
			return PlayFirst;
		case 4:
			minplayer = PlayFirst;
			mincard = HistActCard[PlayFirst];
			for (i=1;i<=2;i++)
			{
				np = (PlayFirst+i>3)?PlayFirst+i-4:PlayFirst+i;
				fc = (HistActCard[np]-1)/13;
				if ( fc == CurFace )
				{
					if ( HistActCard[np] < mincard )
					{
						minplayer = np;
						mincard = HistActCard[np];
					}
				}
			}
			return minplayer;
	}
	return -1;
}
/*=================================================================*/
int GameScore(int player)
{
	return CurScore[player];
}
/*=================================================================*/
int HandFaceLeft(int face)
{
	return NumCard[MyPlayerID][face];
}
/*=================================================================*/
int HandLessCard(int card)
{
	int i,sum;
	sum = 0;
	for (i=Player[MyPlayerID].Card[0];i>=1;i--)
		if ( Player[MyPlayerID].Card[i] < card )
		{
			if ( (Player[MyPlayerID].Card[i]-1)/13 == (card-1)/13 )
				sum++;
			else
				return sum;
		}
	return sum;
}
/*=================================================================*/
int HandLessEqCard(int card)
{
	int i,sum;
	sum = 0;
	for (i=Player[MyPlayerID].Card[0];i>=1;i--)
		if ( Player[MyPlayerID].Card[i] <= card )
		{
			if ( (Player[MyPlayerID].Card[i]-1)/13 == (card-1)/13 )
				sum++;
			else
				return sum;
		}
	return sum;
}
/*=================================================================*/
int HandMaxCard(int face)
{
	int i;
	for (i=Player[MyPlayerID].Card[0];i>=1;i--)
		if ( (Player[MyPlayerID].Card[i]-1)/13 == face )
			return Player[MyPlayerID].Card[i];
	return -1;
}
/*=================================================================*/
int HandMinCard(int face)
{
	int i;
	for (i=1;i<=Player[MyPlayerID].Card[0];i++)
		if ( (Player[MyPlayerID].Card[i]-1)/13 == face )
			return Player[MyPlayerID].Card[i];
	return -1;
}
/*=================================================================*/
int HandMoreCard(int card)
{
	int i,sum;
	sum = 0;
	for (i=1;i<=Player[MyPlayerID].Card[0];i++)
		if ( Player[MyPlayerID].Card[i] > card )
		{
			if ( (Player[MyPlayerID].Card[i]-1)/13 == (card-1)/13 )
				sum++;
			else
				return sum;
		}
	return sum;
}
/*=================================================================*/
int HandMoreEqCard(int card)
{
	int i,sum;
	sum = 0;
	for (i=1;i<=Player[MyPlayerID].Card[0];i++)
		if ( Player[MyPlayerID].Card[i] >= card )
		{
			if ( (Player[MyPlayerID].Card[i]-1)/13 == (card-1)/13 )
				sum++;
			else
				return sum;
		}
	return sum;
}
/*=================================================================*/
int HandNextCard(int card)
{
	int i;
	for (i=1;i<=Player[MyPlayerID].Card[0];i++)
		if ( Player[MyPlayerID].Card[i] > card )
		{
			if ( (Player[MyPlayerID].Card[i]-1)/13 == (card-1)/13 )
				return Player[MyPlayerID].Card[i];
			else
				return -1;
		}
	return -1;
}
/*=================================================================*/
int HandPrevCard(int card)
{
	int i;
	for (i=Player[MyPlayerID].Card[0];i>=1;i--)
		if ( Player[MyPlayerID].Card[i] < card )
		{
			if ( (Player[MyPlayerID].Card[i]-1)/13 == (card-1)/13 )
				return Player[MyPlayerID].Card[i];
			else
				return -1;
		}
	return -1;
}
/*=================================================================*/
int IsBigChuanBroken()
{
	return ChkBigChuan;
}
/*=================================================================*/
int IsCardOnGame(int card)
{
	int i;
	for (i=0;i<=3;i++)
	{
		if (HistActCard[i] == card)
			return TRUE;
	}
	return FALSE;
}
/*=================================================================*/
int IsCardOnHand(int card)
{
	int i;
	for (i=1;i<=Player[MyPlayerID].Card[0];i++)
	{
		if (Player[MyPlayerID].Card[i] == card)
			return TRUE;
	}
	return FALSE;
}
/*=================================================================*/
int IsCardOutGame(int card)
{
	if ( StatCard[card] == 1 )
		return TRUE;
	else
		return FALSE;
}
/*=================================================================*/
int IsChuanBroken()
{
	return ChkChuan;
}
/*=================================================================*/
int IsFaceOnGame(int face)
{
	int i;
	for (i=0;i<=3;i++)
	{
		if (HistActCard[i]>0)
			if ( ((HistActCard[i]-1)/13) == face)
				return TRUE;
	}
	return FALSE;
}
/*=================================================================*/
int IsGotCard(int player,int card)
{
	int i;

	for (i=1;i<=RecvCard[player][0];i++)
	{
		if ( RecvCard[player][i] == card )
			return TRUE;
	}
	return FALSE;
}
/*=================================================================*/
int IsNoScoreCard()
{
	return ChkScore;
}
/*=================================================================*/
int IsNoMoreScoreCard()
{
	if (  (StatCard[CLUB_10]==1) && (StatCard[DIAMOND_J]==1)&&
			(StatCard[SPADE_Q]==1) && (StatCard[HEART_2] ==1)&&
			(StatCard[HEART_3]==1) && (StatCard[HEART_4] ==1)&&
			(StatCard[HEART_5]==1) && (StatCard[HEART_6] ==1)&&
			(StatCard[HEART_7]==1) && (StatCard[HEART_8] ==1)&&
			(StatCard[HEART_9]==1) && (StatCard[HEART_10]==1)&&
			(StatCard[HEART_J]==1) && (StatCard[HEART_Q] ==1)&&
			(StatCard[HEART_K]==1) && (StatCard[HEART_A] ==1)	)
		return TRUE;
	return FALSE;
}
/*=================================================================*/
int IsVoid(int player,int face)
{
	return ChkVoid[player][face];
}
/*=================================================================*/
int LastScore(int player)
{
	return Player[player].nScore;
}
/*=================================================================*/
int NextPlayer(int seq)
{
	int i;
	if (CurOrder==4) return UNKNOWN;
	i = MyPlayerID+seq;
	return (i>3)?i%4:i;
}
/*=================================================================*/
int PrevPlayer(int seq)
{
	int i;
	if (CurOrder==1) return UNKNOWN;
	i = MyPlayerID-seq;
	return (i<0)?-(i%4):i;
}
/*=================================================================*/
int RandomAnyCard()
{
	return Player[MyPlayerID].Card[1+random(Player[MyPlayerID].Card[0])];
}
/*=================================================================*/
int RandomCard(face)
{
	int i,fcrd = 0;

	if (NumCard[MyPlayerID][face]<=0) return -1;

	for (i=0;i<face;i++)
		fcrd+=NumCard[MyPlayerID][i];

	return Player[MyPlayerID].Card[fcrd+1+random(NumCard[MyPlayerID][face])];
}
/*=================================================================*/
int RandomFace()
{
	int i,j;
	int nface[4] = { 0,0,0,0 };

	j = 0;
	for ( i=0;i<=3;i++ )
	{
		if (NumCard[MyPlayerID][i] > 0)
		{
			nface[j]=i;
			j++;
		}
	}
	if (j==0) return -1;
	return nface[random(j)];
}
/*=================================================================*/
int TotalScore(int player)
{
	return Player[player].nScore+CurScore[player];
}
/*=================================================================*/

//============================================================
//           Algorithm for Jubmoo
//============================================================

int JubmooAI(void)
{
	switch(Logic[MyPlayerID])
	{
		case 0:
			return BullBoy();
		case 1:
			return SafeSelf();
		case 2:
			return SuperPig();
		default:
			return SafeSelf();
	}
}

