#include<windows.h>
#include<gl/GL.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib,"opengl32.lib")

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
GLfloat angle = 0.0f;


void resize(int, int);
void initialize(void);
void uninitialize(void);
void display(void);
void ToggleFullscreen(void);
void drawTriangle(void);
void drawRectangle(void);
void updateAngle(void);
GLclampf getRandomNumer(void);

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
				display();
			}
		}
	}
	uninitialize();
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
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f;
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
		case 0x46: //for 'f' or 'F'
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
	//glOrtho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
	drawTriangle();
	glLoadIdentity();
	//glOrtho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
	drawRectangle();
	//glOrtho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
	SwapBuffers(ghdc);
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	/*
	GLfloat b = -50.0f, t = 50.0f;
	if (width < height)
	{
	b = -50.0f * (GLfloat)(height / width);
	t = 50.0f * (GLfloat)(height / width);
	}
	else {
	b = -50.0f * (width / height);
	t = 50.0f * (width / height);
	}
	*/
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
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

	glTranslatef(-0.50f, 0.0f, -0.6f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.45f, 0.0f);
	glVertex3f(-0.45f, -0.45f, 0.0f);

	glColor3f(0.0f, 0.45f, 0.0f);
	glVertex3f(-0.45f, -0.45f, 0.0f);
	glVertex3f(0.45f, -0.45f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.45f, -0.45f, 0.0f);
	glVertex3f(0.0f, 0.45f, 0.0f);

	glEnd();
}

void drawRectangle() {
	glTranslatef(0.50f, 0.0f, -0.6f);
	glRotatef(angle, 1.0f, 0.0f, 0.0f);
	updateAngle();
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.45f, 0.45f, 0.0f);
	//glVertex3f(-0.45f, 0.45f, 0.0f);

	glVertex3f(-0.45f, 0.45f, 0.0f);
//	glVertex3f(-0.45f, -0.45f, 0.0f);

	glVertex3f(-0.45f, -0.45f, 0.0f);
//	glVertex3f(0.45f, -0.45f, 0.0f);

	glVertex3f(0.45f, -0.45f, 0.0f);
	glVertex3f(0.45f, 0.45f, 0.0f);
	glEnd();
}

void updateAngle()
{
	angle = angle + 0.1f;
	if (angle > 360.0f)
		angle = 0.0f;
}