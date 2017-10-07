#include<windows.h>
#include<gl/GL.h>
#include<GL/glu.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HBRUSH hBrushForBColor;
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;


GLclampf red = 0.0f;
GLclampf green = 0.0f;
GLclampf blue = 0.0f;

int paintingSequenceCounter = 3;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbFullscreen = false;
bool gbEscaseKeyPressed = false;
bool gbActiveWindow = false;

int intX = 0, intY = 0;

void resize(int, int);
void initialize(void);
void uninitialize(void);
void display(void);
void ToggleFullscreen(void);
void drawTriangle(void);
GLclampf getRandomNumer(void);
GLfloat angle = 0.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpszcmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");
	HWND hwnd;
	bool bDone = false;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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
	TCHAR szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		szExeFileName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
		0,
		0,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);


	ghwnd = hwnd;

	initialize();

	ShowWindow(hwnd, nCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);


	while (bDone == false)
	{
		if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				if (gbEscaseKeyPressed == true)
				{
					bDone = true;
				}
				//Sleep(300);

				display();
			}
		}
	}
	uninitialize();
	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;

	GetClientRect(hwnd, &rect);
	int width = rect.right;
	int height= rect.bottom;

	intX = LOWORD(lParam);
	intY = HIWORD(lParam);
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CHAR:
		
		switch (wParam)
		{
		case '1': //R
			intX = 0;
			intY = height / 2;
			resize(width/2, height/2);
			break;
		case '2':
			intX = width / 2;
			intY = height / 2;
			resize(width / 2, height / 2);
			break;
		case '3':
			intX = 0;
			intY = 0;
			resize(width / 2, height / 2);
			break;
		case '4': //R
			intX = width /2;
			intY = 0;
			resize(width / 2, height / 2);
			break;
		case '5': //R
			intX = 0;
			intY = 0;
			resize(width/2, height);
			break;
		case '6': //R
			intX = width / 2;
			intY = 0;
			resize(width / 2, height);
			break;

		case '7': //R
			intX = 0;
			intY = height/2;
			resize(width, height/2);
			break;

		case '8': //R
			intX = 0;
			intY = 0;
			resize(width, height / 2);
			break;

		case '9': //R
			intX = width / 4;// set x as half less than middle point (1/4)
			intY = height / 4;// set y as half less than middle point (1/4)
			resize(width/2, height/2);
			break;
		case 0X47: //G
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f;
			break;
		case 0X42: //B
			red = 0.0f;
			green = 0.0f;
			blue = 1.0f;
			break;
		case 0X59: //Y
			red = 1.0f;
			green = 1.0f;
			blue = 0.0f;
			break;
		case 0X4b: //K
			red = 0.0f;
			green = 0.0f;
			blue = 0.0f;
			break;
		case 0X4d: //M
			red = 1.0f;
			green = 0.0f;
			blue = 1.0f;
			break;
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			gbEscaseKeyPressed = true;
			break;
		case 'f'://for 'f' or 'F'
			if (gbFullscreen == false)
			{
				ToggleFullscreen();
				gbFullscreen = true;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = false;
			}
			break;
		default:
			MessageBox(hwnd, TEXT("Unhandled key pressed"), TEXT("Button Pressed Event"), 0);
			break;
		}
		glClearColor(red, green, blue, 0.0f);
		break;
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
		{
			gbActiveWindow = true;
		}
		else
		{
			gbActiveWindow = false;
		}
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_CLOSE:
		gbEscaseKeyPressed = true;
		uninitialize();
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW_MATRIX);

	glLoadIdentity();
	drawTriangle();
	SwapBuffers(ghdc);
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	//glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glViewport(intX, intY, (GLsizei)width, (GLsizei)height);
	//gluPerspective(0.0f, (GLfloat)width / (GLfloat)height, 100.0f, 100.0f);

	
}

void uninitialize(void)
{
	if (gbFullscreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPED);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(ghrc);
	ghrc = NULL;
	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;
	DestroyWindow(ghwnd);
}

void initialize(void)
{

	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cBlueBits = 8;
	pfd.cGreenBits = 8;
	pfd.cAlphaBits = 8;

	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == false)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	if (wglMakeCurrent(ghdc, ghrc) == false)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	resize(WIN_WIDTH, WIN_HEIGHT);
	glClearColor(red, green, blue, 0.0f);

}

void ToggleFullscreen(void)
{
	//variable declarations
	MONITORINFO mi;

	//code
	if (gbFullscreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}

	else
	{
		//code
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
}

void drawTriangle() {
	glLineWidth(2);

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.50f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.50f, -0.50f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.50f, -0.50f, 0.0f);

	glEnd();
}
