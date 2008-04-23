// CalAmp plugin
// Copyright (C) 2002, Cal Henderson <cal@iamal.com>

#include <windows.h>
#include <wininet.h>
#include <stdio.h>

#include "gen.h"
#include "frontend.h"

/////////////////////////////////////////////////////////
//
// forward declare some routines
//

void _main(void);

void config();
void quit();
int init();
int timer();
void fetch_url(char query[255]);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID CALLBACK InternetStatusCallback(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);

/////////////////////////////////////////////////////////
//
// and some variables
//

void *lpWndProcOld;
char szAppName[] = "Cal Amp";

UINT		g_timer;
UINT		g_interval = 500;
char		g_TrackName[255];
int		g_CurrentState = 0;
HINTERNET	g_InetHandle;
boolean		g_InetOk;
char		g_RootUrl[255] = "http://amp.iamcal.com/update_database.php";

winampGeneralPurposePlugin plugin = {
	GPPHDR_VER,
	"",
	init,
	config,
	quit,
};

/////////////////////////////////////////////////////////
//
// and hereth start the code...
//

void config() {
	// config code would go here...
}


void quit() {
	KillTimer(plugin.hwndParent, g_timer);
	fetch_url(""); //stop the current track
	InternetCloseHandle(g_InetHandle);
}


int init() {

	g_InetHandle = InternetOpen(szAppName, PRE_CONFIG_INTERNET_ACCESS, NULL, 0, INTERNET_FLAG_ASYNC );
	InternetSetStatusCallback(g_InetHandle, &InternetStatusCallback);
	g_InetOk = (g_InetHandle != NULL);

	{
		static char c[512];
		char filename[512],*p;
		GetModuleFileName(plugin.hDllInstance, filename,sizeof(filename));
		p = filename+lstrlen(filename);
		while (p >= filename && *p != '\\') p--;
		wsprintf((plugin.description=c),"%s Plug-In v1.0 (%s)",szAppName,p+1);
	}

	lpWndProcOld = (void *) GetWindowLong(plugin.hwndParent, GWL_WNDPROC);
	SetWindowLong(plugin.hwndParent, GWL_WNDPROC, (long) WndProc);

	g_timer = 11;
	SetTimer(plugin.hwndParent, g_timer, g_interval, NULL);

	return 0;
}


int timer() {

	char *		l_TrackName;
	int		l_CurrentTrack;
	int		l_CurrentState;
	int		l_PreviousState;
	boolean		l_NewTrack;
	boolean		l_NewState;
	char 		l_url[255];

	// get track name
	l_CurrentTrack = SendMessage(plugin.hwndParent, WM_USER, 0, IPC_GETLISTPOS);
	l_TrackName = (char *) SendMessage(plugin.hwndParent, WM_USER, l_CurrentTrack, IPC_GETPLAYLISTTITLE);
	l_NewTrack = (strcmp(l_TrackName, g_TrackName));
	if (l_NewTrack) strcpy(g_TrackName, l_TrackName);

	// get playing state
	l_CurrentState = SendMessage(plugin.hwndParent, WM_USER, 0, IPC_ISPLAYING);
	l_PreviousState = g_CurrentState;
	g_CurrentState = l_CurrentState;
	l_NewState = ((l_CurrentState == 1) && (l_PreviousState != 1));

	// are we playing a new track, or moving into a playing state?
	if ((l_NewTrack && (l_CurrentState == 1)) || l_NewState){
		sprintf(l_url, "?%s", l_TrackName);
		fetch_url(l_url);
	}

	// have we stopped?
	if ((l_PreviousState == 1) && (l_CurrentState != 1)){
		fetch_url("");
	}

	return 0;
}


void fetch_url(char query[255]){
	int		flags;
	char		url[255];
	HINTERNET	hRequest;

	if (g_InetOk){
		flags = INTERNET_FLAG_RELOAD || INTERNET_FLAG_DONT_CACHE || INTERNET_FLAG_EXISTING_CONNECT;
		strcpy(url, g_RootUrl);
		strcat(url, query);
		hRequest = InternetOpenUrl( g_InetHandle, url, NULL, -1, flags , 1 );
		InternetCloseHandle(hRequest);
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	if ((message == WM_TIMER) && (wParam == g_timer)) return timer();

	return CallWindowProc(lpWndProcOld, hwnd, message, wParam, lParam);
}


VOID CALLBACK InternetStatusCallback(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength){
	char message[255];

	switch(dwInternetStatus){
		case INTERNET_STATUS_RESOLVING_NAME		: strcpy(message, "INTERNET_STATUS_RESOLVING_NAME"); break;
		case INTERNET_STATUS_NAME_RESOLVED		: strcpy(message, "INTERNET_STATUS_NAME_RESOLVED"); break;
		case INTERNET_STATUS_CONNECTING_TO_SERVER	: strcpy(message, "INTERNET_STATUS_CONNECTING_TO_SERVER"); break;
		case INTERNET_STATUS_CONNECTED_TO_SERVER	: strcpy(message, "INTERNET_STATUS_CONNECTED_TO_SERVER"); break;
		case INTERNET_STATUS_SENDING_REQUEST		: strcpy(message, "INTERNET_STATUS_SENDING_REQUEST"); break;
		case INTERNET_STATUS_REQUEST_SENT		: strcpy(message, "INTERNET_STATUS_ REQUEST_SENT"); break;
		case INTERNET_STATUS_RECEIVING_RESPONSE		: strcpy(message, "INTERNET_STATUS_RECEIVING_RESPONSE"); break;
		case INTERNET_STATUS_RESPONSE_RECEIVED		: strcpy(message, "INTERNET_STATUS_RESPONSE_RECEIVED"); break;
		case INTERNET_STATUS_REDIRECT			: strcpy(message, "INTERNET_STATUS_REDIRECT"); break;
		case INTERNET_STATUS_CLOSING_CONNECTION		: strcpy(message, "INTERNET_STATUS_CLOSING_CONNECTION"); break;
		case INTERNET_STATUS_CONNECTION_CLOSED		: strcpy(message, "INTERNET_STATUS_CONNECTION_CLOSED"); break;
		case INTERNET_STATUS_HANDLE_CREATED		: strcpy(message, "INTERNET_STATUS_HANDLE_CREATED"); break;
		case INTERNET_STATUS_HANDLE_CLOSING		: strcpy(message, "INTERNET_STATUS_HANDLE_CLOSING"); break;
		case INTERNET_STATUS_REQUEST_COMPLETE		: strcpy(message, "INTERNET_STATUS_REQUEST_COMPLETE"); break;
	}

	// uncomment this line to see connection statuses
	//MessageBox(plugin.hwndParent, message, "status", MB_OK | MB_SYSTEMMODAL);

	return;
}


BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved){
	return TRUE;
}

__declspec( dllexport ) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin() {
	return &plugin;
}
