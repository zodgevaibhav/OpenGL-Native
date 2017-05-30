#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpszcmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)30;//GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName,
		TEXT("My Application"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR strMessage[255];
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		wsprintf(strMessage, TEXT(" Clicked on cordinates X-%d, Y-%d, points-%d", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),MAKEPOINTS(lParam)));
		MessageBox(hwnd,strMessage,TEXT("Coordinates of button"),0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			MessageBox(hwnd, TEXT("Escape button pressed"), TEXT("Button Pressed Event"), 0);
			break;
		case 0X41:
			MessageBox(hwnd, TEXT("A button pressed"), TEXT("Button Pressed Event"), 0);
			break;
		case 0X42:
			MessageBox(hwnd, TEXT("B button pressed"), TEXT("Button Pressed Event"), 0);
			break;
		case 0X46:
			MessageBox(hwnd, TEXT("F button pressed"), TEXT("Button Pressed Event"), 0);
			break;
		case 0X4C:
			MessageBox(hwnd, TEXT("L button pressed"), TEXT("Button Pressed Event"), 0);
			break;
		case 0X54:
			MessageBox(hwnd, TEXT("T button pressed"), TEXT("Button Pressed Event"), 0);
			break;
		case 0X51:
			MessageBox(hwnd, TEXT("Q button pressed"), TEXT("Button Pressed Event"), 0);
			break;
		}
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}