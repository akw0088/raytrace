#include "include.h"


int	cxClient = 1, cyClient = 1;
int *pixel = NULL;

#define WM_TICK 1

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);


bool draw(scene &myScene, int *pixel);

void draw_pixels(HDC hdc, HDC hdcMem, int width, int height)
{
	HBITMAP hBitmap, hOldBitmap;

	hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SetBitmapBits(hBitmap, sizeof(int) * width * height, pixel);
	hdcMem = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
	// This scaling is a little strange because Stretch maintains aspect ratios
	StretchBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, width, height, SRCCOPY);
	SelectObject(hdcMem, hOldBitmap);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Raytrace");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;
     
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(NULL_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;
          
	if (!RegisterClass (&wndclass))
	{
		MessageBox(NULL, TEXT ("Program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	} 


	hwnd = CreateWindow (szAppName, TEXT ("Raytrace"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
     
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT	rect = {0, 0, 1, 1};
	static HDC	hdcMem;
	static int	old_style = 0;
	static int	new_style = WS_CHILD | WS_VISIBLE;
	static int	xres, yres;
	static scene	myScene;
	HDC		hdc;
	PAINTSTRUCT	ps;
	int		i;


     
	switch (message)
	{
	case WM_CREATE:
	{
		HMONITOR hmon;
		MONITORINFO mi = {sizeof(MONITORINFO)};

		if (!init("scene.txt", myScene))
		{
			printf("Failure when reading the Scene file");
			return -1;
		}


		hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		GetMonitorInfo(hmon, &mi);

		xres = abs(mi.rcMonitor.right - mi.rcMonitor.left);
		yres = abs(mi.rcMonitor.bottom - mi.rcMonitor.top);

		pixel = (int *)malloc(sizeof(int) * xres * yres);
		if (pixel == NULL)
		{
			perror("malloc failed");
			return 0;
		}

		SetTimer(hwnd, WM_TICK, 16, NULL);

		return 0;
	}
	case WM_TIMER:
	{ 

		if (!draw(myScene, pixel))
		{
			printf("Failure when creating the image file.\n");
			return -1;
		}

		myScene.sphereContainer[0].pos.x += 2;
		myScene.sphereContainer[1].pos.x -= 2;
		myScene.sphereContainer[1].pos.y -= 2;
		myScene.sphereContainer[2].pos.y += 2;


		InvalidateRect(hwnd, &rect, FALSE);
         
		return 0;
	}
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		rect.right = cxClient;
		rect.bottom = cyClient;

		myScene.sizex = cxClient;
		myScene.sizey = cyClient;


		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			old_style = SetWindowLong(hwnd, GWL_STYLE, new_style);
			new_style = old_style;
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, xres, yres, 0);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps);
		draw_pixels(hdc, hdcMem, cxClient, cyClient);
		EndPaint(hwnd, &ps);
		return 0;
	     
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}




