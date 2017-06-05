#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HBRUSH hBrushForBColor;
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
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 0X52: //R
			hBrushForBColor = CreateSolidBrush(RGB(255, 0, 0));
			InvalidateRect(hwnd, NULL, true);
			break;
		case 0X47: //G
			hBrushForBColor = CreateSolidBrush(RGB(0, 255, 0));
			InvalidateRect(hwnd, NULL, true);
			break;
		case 0X42: //B
			hBrushForBColor = CreateSolidBrush(RGB(0, 0, 255));
			InvalidateRect(hwnd, NULL, true);
			break;
		case 0X59: //Y
			hBrushForBColor = CreateSolidBrush(RGB(255, 255, 0));
			InvalidateRect(hwnd, NULL, true);
			break;
		case 0X4b: //K
			hBrushForBColor = CreateSolidBrush(RGB(0, 0, 0));
			InvalidateRect(hwnd, NULL, true);
			break;
		case 0X4d: //M
			hBrushForBColor = CreateSolidBrush(RGB(255, 0, 255));
			InvalidateRect(hwnd, NULL, true);
			break;
		default:
			MessageBox(hwnd, TEXT("Unhandled key pressed"), TEXT("Button Pressed Event"), 0);
			break;
		}
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(hBrushForBColor));
		EndPaint(hwnd, &ps);
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}