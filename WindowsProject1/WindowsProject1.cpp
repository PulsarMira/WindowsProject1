#include "framework.h"
#include "WindowsProject1.h"

#define ID_FIRSTCHILD   100

using namespace std;
HWND child[2]{};
HWND MDIClient{};
LPCTSTR childClass = TEXT("ChildClass");
LPCTSTR mainClass = TEXT("MainClass");
RECT rt{};

const int width = 1024;
const int height = 768;

VOID OnPaint1(HDC hdc) {
	SetTextColor(hdc, RGB(255, 100, 0));
	//TextOutA(hdc, 200, 200, L"Hello!", wstrlen("Hello!"));
}

/*VOID OnPaint2(HDC hdc) {

	SetTextColor(hdc, RGB(255, 100, 0));
	TextOutA(hdc, 50, 300, "Good Bye", strlen("Good Bye"));
}*/


//прототипы только функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

/*LRESULT CALLBACK ChildProc2(HWND, UINT, WPARAM, LPARAM);*/

HINSTANCE hinst;


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	HWND hwnd; //локальная переменная для hWnd
	MSG msg; //локальная переменная для сообщений
	WNDCLASSEX w{};  //class window, windowClass - name (shirina, style etc)
	hinst = hInstance;
	//w.style = CS_HREDRAW | CS_VREDRAW;
	w.cbSize = sizeof(w);
	w.lpfnWndProc = WndProc; //чтобы была возможность обработки сообщений формы (отдельная функция)
	w.hInstance = hInstance; //запускаем приложение, это первый параметр нашей функции
	w.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
	w.lpszClassName = mainClass; //имя класса нашего окна
	w.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	w.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

	//теперь регистрация нашего окна
	if (!RegisterClassEx(&w))
		MessageBox(NULL, TEXT("SOSIZHOPAu1"), NULL, MB_OK);

	w.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	w.lpfnWndProc = ChildProc;
	w.lpszClassName = childClass;

	if (!RegisterClassEx(&w))
		MessageBox(NULL, TEXT("ZHOPAW2"), NULL, MB_OK);

	//теперь создадим само окно
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, mainClass, TEXT("My task"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, HWND_DESKTOP, NULL, hInstance, NULL);

	if (!hwnd)
		MessageBox(NULL, TEXT("ZHOPA0"), NULL, MB_OK);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));
	//теперь показать наше окно
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//зацикливаем программу, чтобы она не закрывалась сама по себе (сообщения)
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateMDISysAccel(MDIClient, &msg) &&
			!TranslateAccelerator(hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);//ссылка на сообщения
			DispatchMessage(&msg);
		}
	}
	return msg.wParam; //можно и другое
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //сама функция, для сообщений
{

	switch (uMsg) {
	case WM_CREATE: {
		GetClientRect(hwnd, &rt);

		CLIENTCREATESTRUCT ccs;

		// Извлекаем дескриптор меню окна назначаем
		// идентификатор первому дочернему окну.
		/*if (i == 0) { ccs.lpfnWndProc = ChildProc1; }//  ChildProc2
		else { w.lpfnWndProc = ChildProc2; }*/
		ccs.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		ccs.idFirstChild = ID_FIRSTCHILD;

		// Создаем дочернее окно MDI
		MDIClient = CreateWindowEx(0, TEXT("MDIClient"), NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			hwnd, GetMenu(hwnd), hinst, (LPVOID)& ccs);

		if (!MDIClient)
			MessageBox(NULL, TEXT("ZHOPA"), NULL, MB_OK);

		for (int i = 0; i < 2; i++) // Создание двух дочерних окна
		{
			child[i] = CreateWindowEx(WS_EX_MDICHILD, childClass, NULL,
				WS_CHILD | WS_VISIBLE,
				(rt.right / 2) * i, 0, rt.right / 2, rt.bottom,
				//0, 0, 0, 0,
				MDIClient, GetMenu(hwnd), hinst, 0);


			if (!child[i])
				MessageBox(NULL, TEXT("ZHOPA1"), NULL, MB_OK);
		}

		break;
	}
	case WM_SIZE: {
		GetClientRect(hwnd, &rt);
		SetWindowPos(MDIClient, NULL, 0, 0, rt.right, rt.bottom, SWP_NOZORDER);
		for (int i = 0; i < 2; i++)
			SetWindowPos(child[i], NULL, (rt.right / 2) * i, 0, (rt.right / 2), rt.bottom, SWP_NOZORDER);
		//SendMessage(MDIClient, WM_MDITILE, 0, 0);
		// тут изменяем размер дочернего окна при помощи SetWindowPos()
		break;
	}

	case WM_DESTROY: {

		PostQuitMessage(0);
		break;
	}

	default:
		return DefFrameProc(hwnd, MDIClient, uMsg, wParam, lParam);
	}
	return 0;
}
#include <string>
std::wstring s;
LRESULT CALLBACK ChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //сама функция, для сообщений
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg) {
	case WM_CHAR:
	{
		switch (wParam)
		{
		case VK_BACK: {
			if (!s.empty())
			{
				s.pop_back();
			}
			break;
		}
		default:
			s += (TCHAR)wParam;
			break;
		}

		InvalidateRect(child[0], NULL, TRUE);
		InvalidateRect(child[1], NULL, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rt);
		DrawText(hdc, s.data(), s.size(), &rt, DT_LEFT
			| DT_END_ELLIPSIS | DT_WORD_ELLIPSIS | DT_MODIFYSTRING);

		EndPaint(hwnd, &ps);
		break;
	}

	case WM_DESTROY: {

		break;
	}
	default:
		return DefMDIChildProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
