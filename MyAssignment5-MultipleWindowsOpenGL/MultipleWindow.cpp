#include <windows.h>

LRESULT CALLBACK windowprocessforwindow1(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow2, UINT message, WPARAM wParam, LPARAM lParam);

bool window1closed = false;
bool window2closed = false;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	bool endprogram = false;

	//create window 1

	WNDCLASSEX windowclassforwindow1;
	ZeroMemory(&windowclassforwindow1, sizeof(WNDCLASSEX));
	windowclassforwindow1.cbClsExtra = NULL;
	windowclassforwindow1.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow1.cbWndExtra = NULL;
	windowclassforwindow1.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclassforwindow1.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow1.hIcon = NULL;
	windowclassforwindow1.hIconSm = NULL;
	windowclassforwindow1.hInstance = hInst;
	windowclassforwindow1.lpfnWndProc = (WNDPROC)windowprocessforwindow1;
	windowclassforwindow1.lpszClassName = L"windowclass 1";
	windowclassforwindow1.lpszMenuName = NULL;
	windowclassforwindow1.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowclassforwindow1))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	HWND handleforwindow1 = CreateWindowEx(NULL,
		windowclassforwindow1.lpszClassName,
		L"Parent Window",
		WS_OVERLAPPEDWINDOW,
		200,
		150,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL                /* No Window Creation data */
		);

	if (!handleforwindow1)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow1, nShowCmd);

	// create window 2

	WNDCLASSEX windowclassforwindow2;
	ZeroMemory(&windowclassforwindow2, sizeof(WNDCLASSEX));
	windowclassforwindow2.cbClsExtra = NULL;
	windowclassforwindow2.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow2.cbWndExtra = NULL;
	windowclassforwindow2.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclassforwindow2.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow2.hIcon = NULL;
	windowclassforwindow2.hIconSm = NULL;
	windowclassforwindow2.hInstance = hInst;
	windowclassforwindow2.lpfnWndProc = (WNDPROC)windowprocessforwindow2;
	windowclassforwindow2.lpszClassName = L"window class2";
	windowclassforwindow2.lpszMenuName = NULL;
	windowclassforwindow2.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowclassforwindow2))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window class creation failed for window 2",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	HWND handleforwindow2 = CreateWindowEx(NULL,
		windowclassforwindow2.lpszClassName,
		L"Child Window",
		WS_OVERLAPPEDWINDOW,
		200,
		150,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);

	if (!handleforwindow2)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow2, nShowCmd);
	SetParent(handleforwindow2, handleforwindow1);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (endprogram == false) {
		if (GetMessage(&msg, NULL, 0, 0));
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (window1closed == true && window2closed == true) {
			endprogram = true;
		}
	}
	MessageBox(NULL,
		L"Both Windows are closed.  Program will now close.",
		L"",
		MB_ICONINFORMATION);
	return 0;
}

LRESULT CALLBACK windowprocessforwindow1(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: {
		MessageBox(NULL,
			L"Window 1 closed",
			L"Message",
			MB_ICONINFORMATION);

		window1closed = true;
		return 0;
	}
					 break;
	}

	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}

LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: {
		MessageBox(NULL,
			L"Window 2 closed",
			L"Message",
			MB_ICONINFORMATION);

		window2closed = true;
		return 0;
	}
					 break;
	}

	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}