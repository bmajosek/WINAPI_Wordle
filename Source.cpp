// wiekszosc rzeczy jest z tutoriala oraz ze strony https://learn.microsoft.com/pl-pl/windows/win32/
#include "frame.h"
#include "tut.h"
#include <string>
#include <time.h>
#include <windows.h>
#include <CommCtrl.h>
#include <fstream>
#include <vector>
#include <algorithm>
#define IDI_WORD                        115
#define MAX_LOADSTRING 100
std::string words[11][4];
int first, second, third, fourth;
std::string WordsToFind[4];
std::string GreenWords[4];
int xx = 326,yy = 411, k =1, CmdShow, p;
std::vector<std::string> Dict;
int row, col, which, liter, curr;
std::string keyLetters = "QWERTYUIOPASDFGHJKLZXCVBNM";
bool ready[4];
WCHAR let;
WCHAR buf[100];
HINSTANCE hInst; // Bieżące wystąpienie
HWND hWnd[4], hWndKey, hWndArr[50][4], Keys[26], Overlay[4]; // Uchwyt okna głównego
bool flaga = false;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
DWORD start = GetTickCount(); // Liczba milisekund od uruchomienia aplikacji

ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                MyRegisterClassKey(HINSTANCE hInstance);
ATOM                MyRegisterClassChild(HINSTANCE hInstance);
ATOM                MyRegisterClassKeys(HINSTANCE hInstance);
ATOM                MyRegisterClassOver(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcKey(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndKeys(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndOver(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool isInside(std::string word)
{
	return std::find(Dict.begin(), Dict.end(), word) != Dict.end();
}
// Funkcja timera
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	DWORD elapsed = GetTickCount() - start;
	WCHAR title[256];
	DWORD min = elapsed / 1000 / 60;
	DWORD sc = elapsed / 1000 - min * 60;
	swprintf_s(title, L"WORDLE - KEYBOARD: %02d:%02d seconds", min, sc);
	SetWindowText(hWndKey, title);
}

// Funkcja główna
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand((unsigned)time(NULL));

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TUTORIAL, szWindowClass, MAX_LOADSTRING);
	std::ifstream file("Wordle.txt");
	std::string line;
	if (file.is_open()) {
		while (getline(file, line)) {
			Dict.push_back(line);
		}
		file.close();
	}
	std::wstring title(L"WORDLE - PUZZLE\0");
	title.copy(szTitle, title.size());
	std::wstring WindowClass(L"nowaKlasa\0");
	WindowClass.copy(szWindowClass, WindowClass.size());

	MyRegisterClass(hInstance);
	MyRegisterClassKey(hInstance);
	MyRegisterClassChild(hInstance);
	MyRegisterClassKeys(hInstance);
	MyRegisterClassOver(hInstance);
	CmdShow = nCmdShow;
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TUTORIAL));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORD));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORD));

	return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassKey(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcKey;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TUTORIAL);
	wcex.lpszClassName = L"keyclass";
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORD));

	return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassChild(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ChildProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(251, 252, 255));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TUTORIAL);
	wcex.lpszClassName = L"ChildClass";
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassKeys(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndKeys;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(251, 252, 255));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"KeyBoard";
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassOver(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndOver;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"Overlay";
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int windowWidth = xx; // szerokość okna gry 326
	int windowHeight = yy; // wysokość okna gry 411
	int windowX = (screenWidth - windowWidth) / 2; // położenie X okna gry
	int windowY = (screenHeight - windowHeight) / 2; // położenie Y okna gry
	p = 0;
	if (hWndKey == nullptr)
	{
		hWndKey = CreateWindowW(
			L"keyclass",
			L"WORDLE - KEYBORD\0",
			WS_OVERLAPPEDWINDOW,
			windowX - 631 / 4,
			windowY + windowHeight,
			631,
			250,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);
		SetWindowLong(hWndKey, GWL_EXSTYLE, GetWindowLong(hWndKey, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWndKey, 0, (255 * 80) / 100, LWA_ALPHA);
		ShowWindow(hWndKey, nCmdShow);
		for (int i = 0; i < 10; i++)
		{
			WCHAR tit[10];
			swprintf_s(tit, L"%d", i); // Sformatuj tytuł okna
			Keys[i] = CreateWindowW(
				L"KeyBoard",
				tit,
				WS_CHILDWINDOW,
				6*(i+1) + 55 * i,
				6,
				55,
				55,
				hWndKey,
				nullptr,
				hInstance,
				nullptr
			);
			ShowWindow(Keys[i], nCmdShow);
			UpdateWindow(hWndKey);
			liter++;
		}
		for (int i = 0; i < 9; i++)
		{
			WCHAR tit[10];
			swprintf_s(tit, L"%d", i); // Sformatuj tytuł okna
			Keys[i+10] = CreateWindowW(
				L"KeyBoard",
				tit,
				WS_CHILDWINDOW,
				6 * (i + 1) + 55 * i + 28,
				61+6,
				55,
				55,
				hWndKey,
				nullptr,
				hInstance,
				nullptr
			);
			ShowWindow(Keys[i+10], nCmdShow);
			UpdateWindow(hWndKey);
			liter++;
		}
		for (int i = 0; i < 7; i++)
		{
			WCHAR tit[10];
			swprintf_s(tit, L"%d", i); // Sformatuj tytuł okna
			Keys[i+19] = CreateWindowW(
				L"KeyBoard",
				tit,
				WS_CHILDWINDOW,
				6 * (i + 1) + 55 * i + 55+6+28,
				61*2+6,
				55,
				55,
				hWndKey,
				nullptr,
				hInstance,
				nullptr
			);
			ShowWindow(Keys[i+19], nCmdShow);
			UpdateWindow(hWndKey);
			liter++;
		}
		flaga = true;
	}
	for (int i = 0; i < k; i++)
	{
		ready[i] = false;
		GreenWords[i].clear();
	}
	if (k == 1)
	{
		first = rand() % Dict.size();
		WordsToFind[0] = Dict[first];
		for (int i = 0; i < 4; i++) DestroyWindow(hWnd[i]);
		hWnd[0] = CreateWindowW(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			hWndKey,
			nullptr,
			hInstance,
			nullptr
		);
		LONG_PTR style = GetWindowLongPtr(hWnd[0], GWL_STYLE);
		style &= ~WS_SYSMENU;
		SetWindowLongPtr(hWnd[0], GWL_STYLE, style);

		ShowWindow(hWnd[0], nCmdShow);
		UpdateWindow(hWnd[0]);
	}
	else if (k == 2)
	{
		first = rand() % Dict.size();
		second = rand() % Dict.size();
		WordsToFind[0] = Dict[first];
		WordsToFind[1] = Dict[second];
		for (int i = 0; i < 4; i++) DestroyWindow(hWnd[i]);
		hWnd[0] = CreateWindowW(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			(screenWidth - windowWidth) / 4 - windowWidth/2,
			(screenHeight - windowHeight- 124) / 2,
			windowWidth,
			windowHeight + 124,
			hWndKey,
			nullptr,
			hInstance,
			nullptr
		);

		hWnd[1] = CreateWindowW(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			3* (screenWidth - windowWidth) / 4 + windowWidth / 2,
			(screenHeight - windowHeight - 124) / 2,
			windowWidth,
			windowHeight + 124,
			hWndKey,
			nullptr,
			hInstance,
			nullptr
		);
		LONG_PTR style = GetWindowLongPtr(hWnd[0], GWL_STYLE);
		style &= ~WS_SYSMENU;
		SetWindowLongPtr(hWnd[0], GWL_STYLE, style);
		style = GetWindowLongPtr(hWnd[1], GWL_STYLE);
		style &= ~WS_SYSMENU;
		SetWindowLongPtr(hWnd[1], GWL_STYLE, style);
		ShowWindow(hWnd[0], nCmdShow);
		UpdateWindow(hWnd[0]);
		ShowWindow(hWnd[1], nCmdShow);
		UpdateWindow(hWnd[1]);
	}
	else if (k == 4)
	{
		first = rand() % Dict.size();
		second = rand() % Dict.size();
		third = rand() % Dict.size();
		fourth = rand() % Dict.size();
		WordsToFind[0] = Dict[first];
		WordsToFind[1] = Dict[second];
		WordsToFind[2] = Dict[third];
		WordsToFind[3] = Dict[fourth];
		for (int i = 0; i < 4; i++) DestroyWindow(hWnd[i]);
		hWnd[0] = CreateWindowW(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			(screenWidth - windowWidth) / 4 - windowWidth / 2,
			(screenHeight - windowHeight - 248) / 4 - 124,
			windowWidth,
			windowHeight + 124*2,
			hWndKey,
			nullptr,
			hInstance,
			nullptr
		);
		hWnd[1] = CreateWindowW(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			(screenWidth - windowWidth) / 4 - windowWidth / 2,
			3*(screenHeight - windowHeight - 2*124) / 4 + 124,
			windowWidth,
			windowHeight + 124*2,
			hWndKey,
			nullptr,
			hInstance,
			nullptr
		);
		hWnd[2] = CreateWindowW(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			3 * (screenWidth - windowWidth) / 4 + windowWidth / 2,
			(screenHeight - windowHeight - 248) / 4 - 124,
			windowWidth,
			windowHeight + 124*2,
			hWndKey,
			nullptr,
			hInstance,
			nullptr
		);
		hWnd[3] = CreateWindowW(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			3 * (screenWidth - windowWidth) / 4 + windowWidth / 2,
			3 * (screenHeight - windowHeight - 2 * 124) / 4 + 124,
			windowWidth,
			windowHeight * 3,
			hWndKey,
			nullptr,
			hInstance,
			nullptr
		);
		LONG_PTR style = GetWindowLongPtr(hWnd[0], GWL_STYLE);
		style &= ~WS_SYSMENU;
		SetWindowLongPtr(hWnd[0], GWL_STYLE, style);
		style = GetWindowLongPtr(hWnd[1], GWL_STYLE);
		style &= ~WS_SYSMENU;
		SetWindowLongPtr(hWnd[1], GWL_STYLE, style);
		style = GetWindowLongPtr(hWnd[2], GWL_STYLE);
		style &= ~WS_SYSMENU;
		SetWindowLongPtr(hWnd[2], GWL_STYLE, style);
		style = GetWindowLongPtr(hWnd[3], GWL_STYLE);
		style &= ~WS_SYSMENU;
		SetWindowLongPtr(hWnd[3], GWL_STYLE, style);
		ShowWindow(hWnd[0], nCmdShow);
		ShowWindow(hWnd[1], nCmdShow);
		UpdateWindow(hWnd[0]);
		UpdateWindow(hWnd[1]);
		ShowWindow(hWnd[2], nCmdShow);
		ShowWindow(hWnd[3], nCmdShow);
		UpdateWindow(hWnd[2]);
		UpdateWindow(hWnd[3]);
	}

	int o = k == 1 ? 6 : k == 2 ? 8 : 10;
	for (int kk= 0; kk < k; kk++)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < o; j++)
			{
				WCHAR tit[10];
				swprintf_s(tit, L"%d", i * 5 + j); // Sformatuj tytuł okna
				hWndArr[i * o + j][kk] = CreateWindowW(
					L"ChildClass",
					tit,
					WS_CHILDWINDOW,
					6 * (i + 1) + 55 * i,
					6 * (j + 1) + 55 * j,
					55,
					55,
					hWnd[kk],
					nullptr,
					hInstance,
					nullptr
				);
				ShowWindow(hWndArr[i * o + j][kk], nCmdShow);
			}
		}
		UpdateWindow(hWnd[kk]);
	}

	return true;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	}
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = 326;
		minMaxInfo->ptMaxSize.x = minMaxInfo->ptMinTrackSize.x = 326;
		if (k == 1)
		{
			minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = 411;
			minMaxInfo->ptMaxSize.y = minMaxInfo->ptMinTrackSize.y = 411;
		}
		else if (k == 2)
		{

			minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = 411 + 124;
			minMaxInfo->ptMaxSize.y = minMaxInfo->ptMinTrackSize.y = 411 + 124;
		}
		
		else
		{

			minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = 411 + 248;
			minMaxInfo->ptMaxSize.y = minMaxInfo->ptMinTrackSize.y = 411 + 248;
		}
	}
	case WM_NCHITTEST: {
		// zrodlo: https://stackoverflow.com/questions/7773771/how-do-i-implement-dragging-a-window-using-its-client-area
		LRESULT hit = DefWindowProc(hwnd, message, wParam, lParam);
		if (hit == HTCLIENT) hit = HTCAPTION;
		return hit;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		if (wParam == 8)
		{
			RECT rc;
			GetWindowRect(hwnd, &rc);
			int width = rc.right - rc.left;
			int heigth = rc.bottom - rc.top;
			
			
			for (int i = 0; i < k; i++)
			{
				if (hWnd[i] == hwnd)
				{
					Overlay[i] = CreateWindowExW(
						WS_EX_LAYERED,
						L"Overlay",
						NULL,
						WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION,
						rc.left,
						rc.top,
						width,
						heigth,
						hwnd,
						nullptr,
						hInst,
						NULL
					);
					SetClassLongPtr(Overlay[i], GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(255, 0, 0)));
					SetLayeredWindowAttributes(Overlay[i], 0, (255 * 50) / 100, LWA_ALPHA);
					PAINTSTRUCT ps2;
					HDC hdc2 = BeginPaint(Overlay[i], &ps2);
					RECT clientrc;
					GetClientRect(Overlay[i], &clientrc);
					TCHAR wWordsToFind[10];
					for (int j = 0; j <= WordsToFind[i].length(); j++)
					{
						wWordsToFind[j] = WordsToFind[i][j];
					}
					HFONT hFont = CreateFont(
						-MulDiv(90, GetDeviceCaps(hdc, LOGPIXELSY), 110), // Height
						0, // Width
						0, // Escapement
						0, // Orientation
						FW_BOLD, // Weight
						false, // Italic
						FALSE, // Underline
						0, // StrikeOut
						EASTEUROPE_CHARSET, // CharSet
						OUT_DEFAULT_PRECIS, // OutPrecision
						CLIP_DEFAULT_PRECIS, // ClipPrecision
						DEFAULT_QUALITY, // Quality
						DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
						_T(" Verdana "));
					SelectObject(hdc2, hFont);
					SetTextColor(hdc2, RGB(0, 0, 0));
					SetBkMode(hdc2, TRANSPARENT);
					DrawText(hdc2, wWordsToFind, WordsToFind[i].length(), &clientrc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					ShowWindow(Overlay[i], CmdShow);
					UpdateWindow(Overlay[i]);
					DeleteObject(hFont);
					EndPaint(Overlay[i], &ps2);
					break;
				}
			}
			
		}
		else if (wParam == 7)
		{
			RECT rc;
			GetWindowRect(hwnd, &rc);
			int width = rc.right - rc.left;
			int heigth = rc.bottom - rc.top;

			for (int i = 0; i < k; i++)
			{
				if (hWnd[i] == hwnd)
				{
					Overlay[i] = CreateWindowExW(
						WS_EX_LAYERED,
						L"Overlay",
						NULL,
						WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION,
						rc.left,
						rc.top,
						width,
						heigth,
						hwnd,
						nullptr,
						hInst,
						NULL
					);
					SetClassLongPtr(Overlay[i], GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(0, 255, 0)));
					SetLayeredWindowAttributes(Overlay[i], 0, (255 * 30) / 100, LWA_ALPHA);
					RECT clientrc;
					GetClientRect(Overlay[i], &clientrc);
					ShowWindow(Overlay[i], CmdShow);
					UpdateWindow(Overlay[i]);

					break;
				}
			}
		}
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndProcKey(HWND hWndk, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	}
	case WM_CREATE:
	{
		SetTimer(hWndKey, 7, 250, TimerProc);
		WCHAR buffer[256];
		GetPrivateProfileString(L"Section1", L"FirstKey",
			NULL,buffer, 250,
			L".\\save.ini");
		if (buffer[0] == '1' || buffer[0] == '\0')
			k = 1;
		else if (buffer[0] == '2')
			k = 2;
		else
			k = 4;
		break;
	}

	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = 631;
		minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = 250;
		minMaxInfo->ptMaxSize.x = minMaxInfo->ptMinTrackSize.x = 631;
		minMaxInfo->ptMaxSize.y = minMaxInfo->ptMinTrackSize.y = 250;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EASY:
		{
			flaga = true;
			which = 0;
			col = 0;
			row = 0;
			curr = 0;
			k = 1;
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					words[i][j].clear();
				}
			}
			InitInstance(hInst, CmdShow);
			RECT rc;
			GetClientRect(hWndk, &rc);
			if (flaga) liter = 0;
			InvalidateRect(hWndk, &rc, TRUE);
			break;
		}
		case IDM_MED:
		{

			flaga = true;
			k = 2;
			which = 0;
			col = 0;
			curr = 0;
			row = 0;
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					words[i][j].clear();
				}
			}
			InitInstance(hInst, CmdShow);
			RECT rc;
			GetClientRect(hWndk, &rc);
			if (flaga) liter = 0;
			InvalidateRect(hWndk, &rc, TRUE);
			break;
		}
		case IDM_HARD:
		{
			flaga = true;

			k = 4;
			which = 0;
			col = 0;
			curr = 0;
			row = 0;
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					words[i][j].clear();
				}
			}
			InitInstance(hInst, CmdShow);
			RECT rc;
			GetClientRect(hWndk, &rc);
			if (flaga) liter = 0;
			InvalidateRect(hWndk, &rc, TRUE);
			break;
		}
		default:
			return DefWindowProc(hWndk, message, wParam, lParam);
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWndk, &ps);
		EndPaint(hWndk, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		int o = k == 1 ? 6 : k == 2 ? 8 : 10;
		if (wParam == VK_RETURN) {
			while (p < k && words[row][p] == "")
			{
				p++;
			}
			bool isInDict = isInside(words[row][p]);
			bool flagtmp = false;
			if (isInDict == false)
			{
				for (int kk = 0; kk < k; kk++)
				{
					for (int j = 0; j < 5; j++)
					{
						for (int i = 0; i < 5; i++)
						{
							if(ready[kk] == false)
								SendMessageW(hWndArr[j * o + row][kk], WM_PAINT, 50, 0);
						}
					}
					words[row][kk].clear();
				}
			}
			else
			{
				for (int kk = 0; kk < k; kk++)
				{
					curr = kk;
					if (ready[kk] == true) continue;
					for (int j = 0; j < 5; j++)
					{
						flagtmp = false;
						auto it = std::find(WordsToFind[kk].begin(), WordsToFind[kk].end(), words[row][kk][j]);
						int whereIs = keyLetters.find(words[row][kk][j] - 'a' + 'A');
						if (it == WordsToFind[kk].end())
						{
							SendMessageW(hWndArr[j * o + row][kk], WM_PAINT, j + 21, 0);
							SendMessageW(Keys[whereIs], WM_PAINT, j + 21, 0);
							continue;
						}
						int indx = std::distance(WordsToFind[kk].begin(), it);
						if (WordsToFind[kk][j] == words[row][kk][j])
						{
							SendMessageW(hWndArr[j * o + row][kk], WM_PAINT, j + 1, 0);
							SendMessageW(Keys[whereIs], WM_PAINT, j + 1, 0);
							if (std::find(GreenWords[kk].begin(), GreenWords[kk].end(), words[row][kk][j]) == GreenWords[kk].end())
								GreenWords[kk].push_back(words[row][kk][j]);
							if (words[row][kk] == WordsToFind[kk])
							{
								ready[kk] = true;
								SendMessageW(hWnd[kk], WM_PAINT, 7, 0);
							}
						}
						else if(std::find(GreenWords[kk].begin(),GreenWords[kk].end(),words[row][kk][j]) != GreenWords[kk].end())
						{
							SendMessageW(hWndArr[j * o + row][kk], WM_PAINT, j + 11, 0);
						}
						else
						{
							SendMessageW(hWndArr[j * o + row][kk], WM_PAINT, j + 11, 0);
							SendMessageW(Keys[whereIs], WM_PAINT, j + 11, 0);
						}
					}
				}

				int o = k == 1 ? 6 : k == 2 ? 8 : 10;
				row++;
				for (int i = 0; i < k; i++)
				{
					if (row >= o && ready[i] == false)
					{
						ready[i] = true;
						SendMessageW(hWnd[i], WM_PAINT, 8, 0);
					}
				}
			}
			
			col = 0;
			which = col * o + row;
			return 0;
		}
		break;
	}
	case WM_CHAR:
	{
		int o = k == 1 ? 6 : k == 2 ? 8 : 10;
		char kk = (char)wParam;
		for (int i = 0; i < k; i++)
		{
			if (ready[i] == true) continue;
			if (row >= o)
			{
				ready[i] = true;
				SendMessageW(hWnd[i], WM_PAINT, 8, 0);
			}
		}
		if (wParam == 8)
		{
			if (words[row][p].size() > 0)
			{
				for (int kk = 0; kk < k; kk++)
				{
					if (ready[kk] == true) continue;
					words[row][kk].pop_back();
				}
				col--;
				which = col * o + row;
				for (int i = 0; i < k; i++)
				{
					if (ready[i] == true) continue;
					SendMessageW(hWndArr[which][i], WM_PAINT, 50, 0);
				}
			}
			
			return 0;
		}
		else if (col < 5)
		{
			if (kk >= 'a' && kk <= 'z')
			{
				for (int i = 0; i < 4; i++)
				{
					if (ready[i] == true) continue;
					words[row][i].push_back(kk);
				}
				for (int i = 0; i < k; i++)
				{
					if (ready[i] == true) continue;
					SendMessageW(hWndArr[which][i], WM_PAINT, wParam + 'A' - 'a', 0);
				}
			}
			else
			{
				col--;
			}
		}
		col++;
		if (col >= 5)
		{
			col = 5;
		}
		which = col * o + row;
		return 0;
	}
	case WM_DESTROY:
	{
		TCHAR lvl[10];
		_stprintf_s(lvl, 10,
			_T("%d"), k);
		WritePrivateProfileStringW(NULL, NULL, NULL, L".\\save.ini"); 
		WritePrivateProfileStringW(L"Section1", L"FirstKey",
			lvl, L".\\save.ini");
		KillTimer(hWndKey, 7);
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWndk, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK ChildProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	}
	case WM_PAINT:
	{
		TCHAR ch = (TCHAR)wParam;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		HPEN hPen = CreatePen(PS_SOLID, 6, RGB(164, 174, 196));
		RECT rc;
		GetClientRect(hwnd, &rc);
		MoveToEx(hdc, rc.left, rc.top, NULL);
		SelectObject(hdc, hPen);
		LineTo(hdc, rc.right, rc.top);
		LineTo(hdc, rc.right, rc.bottom);
		LineTo(hdc, rc.left, rc.bottom);
		LineTo(hdc, rc.left, rc.top);
		DeleteObject(hPen);
		if (ch != NULL && GreenWords[curr].size() < 5)
		{
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			HDC hdc2 = GetDC(hwnd);
			HBRUSH hBrush = CreateSolidBrush(RGB(251,252,255));

			if (ch <= 7)
			{
				ch = words[row][p][ch - 1] + 'A' - 'a';
				hBrush = CreateSolidBrush(RGB(121, 184, 81));
				SetBkColor(hdc2, RGB(121, 184, 81));
				FillRect(hdc2, &clientRect, hBrush);
				
			}
			else if (ch <= 20)
			{
				ch = words[row][p][ch - 11] + 'A' - 'a';
				hBrush = CreateSolidBrush(RGB(243, 194, 55));
				SetBkColor(hdc2, RGB(243, 194, 55));
				FillRect(hdc2, &clientRect, hBrush);
			}
			else if (ch <= 30)
			{
				ch = words[row][p][ch - 21] + 'A' - 'a';
				hBrush = CreateSolidBrush(RGB(164, 174, 196));
				SetBkColor(hdc2, RGB(164, 174, 196));
				FillRect(hdc2, &clientRect, hBrush);
			}
			HFONT hFont = CreateFont(
				-MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 100), // Height
				0, // Width
				0, // Escapement
				0, // Orientation
				FW_BOLD, // Weight
				false, // Italic
				FALSE, // Underline
				0, // StrikeOut
				EASTEUROPE_CHARSET, // CharSet
				OUT_DEFAULT_PRECIS, // OutPrecision
				CLIP_DEFAULT_PRECIS, // ClipPrecision
				DEFAULT_QUALITY, // Quality
				DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
				_T(" Verdana "));
			SelectObject(hdc2, hFont);
			SetTextColor(hdc2, RGB(0, 0, 0));
			if (ch != 7)
			{
				if (ch == 50)
				{
					SetTextColor(hdc2, RGB(251, 252, 255));
				}
				ch = ch == 50 ? NULL : ch;
				DrawText(hdc2, &ch, 1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			if(ch == NULL)
			{
				FillRect(hdc2, &clientRect, hBrush);
				hPen = CreatePen(PS_SOLID, 6, RGB(164, 174, 196));
				MoveToEx(hdc2, rc.left, rc.top, NULL);
				SelectObject(hdc2, hPen);
				LineTo(hdc2, rc.right, rc.top);
				LineTo(hdc2, rc.right, rc.bottom);
				LineTo(hdc2, rc.left, rc.bottom);
				LineTo(hdc2, rc.left, rc.top);
				DeleteObject(hPen);
				
			}
			DeleteObject(hBrush);
			DeleteObject(hFont);
			ReleaseDC(hwnd, hdc2);
		}
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_NCHITTEST:
		return HTTRANSPARENT; // https://stackoverflow.com/questions/8969852/how-to-move-parent-window-without-border-from-child-using-wm-nchittest?fbclid=IwAR3lgS6l0vrn9pBl_qpU7AChm3O8wTTs9KL2mg37Z3aIZifPVPsWpe6qCM0
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndKeys(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps; 
		TCHAR ch;
		HDC hdc = BeginPaint(hwnd, &ps);
		if (which == 0)
		{
			ch = keyLetters[liter];
			if (flaga) liter++;
			HPEN hPen = CreatePen(PS_SOLID, 6, RGB(164, 174, 196));
			RECT rc;
			GetClientRect(hwnd, &rc);
			MoveToEx(hdc, rc.left, rc.top, NULL);
			SelectObject(hdc, hPen);
			LineTo(hdc, rc.right, rc.top);
			LineTo(hdc, rc.right, rc.bottom);
			LineTo(hdc, rc.left, rc.bottom);
			LineTo(hdc, rc.left, rc.top);
			DeleteObject(hPen);
		}
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		if (curr == 0)
		{
			HDC hdc2 = GetDC(hwnd);
			HFONT hFont = CreateFont(
				-MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 120), // Height
				0, // Width
				0, // Escapement
				0, // Orientation
				FW_BOLD, // Weight
				false, // Italic
				FALSE, // Underline
				0, // StrikeOut
				EASTEUROPE_CHARSET, // CharSet
				OUT_DEFAULT_PRECIS, // OutPrecision
				CLIP_DEFAULT_PRECIS, // ClipPrecision
				DEFAULT_QUALITY, // Quality
				DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
				_T(" Verdana "));
			SelectObject(hdc2, hFont);
			SetTextColor(hdc2, RGB(0, 0, 0));
			SetBkMode(hdc2, TRANSPARENT);
			DrawText(hdc2, &ch, 1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			DeleteObject(hFont);
		}
		
		ch = (TCHAR)wParam;
		if (ch != NULL && GreenWords[curr].size() < 5)
		{
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
			HDC hdc2 = GetDC(hwnd);
			HBRUSH hBrush = CreateSolidBrush(RGB(251, 252, 255));
			HPEN pen = CreatePen(PS_SOLID, 1, RGB(251, 252, 255));
			if (ch <= 7)
			{
				ch = words[row][p][ch - 1] + 'A' - 'a';
				hBrush = CreateSolidBrush(RGB(121, 184, 81));
				DeleteObject(pen);
				pen = CreatePen(PS_SOLID, 1, RGB(121, 184, 81));

			}

			else if (ch <= 20)
			{
				ch = words[row][p][ch - 11] + 'A' - 'a';
				hBrush = CreateSolidBrush(RGB(243, 194, 55));

				DeleteObject(pen);
				pen = CreatePen(PS_SOLID, 1, RGB(243, 194, 55));
			}

			else if (ch <= 30)
			{
				ch = words[row][p][ch - 21] + 'A' - 'a';
				hBrush = CreateSolidBrush(RGB(164, 174, 196));
				DeleteObject(pen);
				pen = CreatePen(PS_SOLID, 1, RGB(164, 174, 196));
			}
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc2, hBrush);
			HPEN oldPen = (HPEN)SelectObject(hdc2, pen);
			if (k == 1)
			{
				Rectangle(hdc2, 0, 0, 55, 55);
			}
			else if (k == 2)
			{
				if (curr == 0)
				{
					Rectangle(hdc2, 0, 0, 55/2, 55);
				}
				else
				{
					Rectangle(hdc2, 55/2,0, 55, 55);
				}
			}
			else
			{
				if (curr == 0)
				{
					Rectangle(hdc2, 0, 0, 55 / 2, 55/2);
				}
				else if(curr == 1)
				{
					Rectangle(hdc2, 0,55/2,55/2,55);
				}
				else if (curr == 2)
				{
					Rectangle(hdc2, 55/2,0,55,55/2);
				}
				else if (curr == 3)
				{
					Rectangle(hdc2,55/2,55/2,55,55);
				}
			}
			SelectObject(hdc, oldPen);
			DeleteObject(pen);
			DeleteObject(oldBrush);
			DeleteObject(hBrush);
			HFONT hFont = CreateFont(
				-MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 120), // Height
				0, // Width
				0, // Escapement
				0, // Orientation
				FW_BOLD, // Weight
				false, // Italic
				FALSE, // Underline
				0, // StrikeOut
				EASTEUROPE_CHARSET, // CharSet
				OUT_DEFAULT_PRECIS, // OutPrecision
				CLIP_DEFAULT_PRECIS, // ClipPrecision
				DEFAULT_QUALITY, // Quality
				DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
				_T(" Verdana "));
			SelectObject(hdc2, hFont);
			SetTextColor(hdc2, RGB(0, 0, 0));
			SetBkMode(hdc2, TRANSPARENT);
			DrawText(hdc2, &ch, 1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			DeleteObject(hFont);
			ReleaseDC(hwnd, hdc2);
		}
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndOver(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	}
	case WM_NCHITTEST: {
		// zrodlo: https://stackoverflow.com/questions/7773771/how-do-i-implement-dragging-a-window-using-its-client-area
		LRESULT hit = DefWindowProc(hwnd, message, wParam, lParam);
		if (hit == HTCLIENT) hit = HTCAPTION;
		return hit;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_WINDOWPOSCHANGING:
	{
		for (int i = 0; i < k; i++)
		{
			if (Overlay[i] == hwnd)
			{
				RECT rc;
				GetWindowRect(hwnd, &rc);
				SetWindowPos(hWnd[i], NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
				break;
			}
		}
		
		break;
	}
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}