#include<windows.h>
#include<gl/GL.h>
#include<math.h>
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

void resize(int, int);
void initialize(void);
void uninitialize(void);
void display(void);
void ToggleFullscreen(void);
void drawGraphOnWindow(void);
void getRgbColor(int, GLfloat*, GLfloat*, GLfloat*);
void drawCircleUsingPoints(GLfloat, int);
void drawTriangle();
void drawLine(void);
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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Assignment25-DeathlyHolow"),
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
				Sleep(100);
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
	GLfloat red, green, blue;
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(3);

		drawTriangle();
		drawCircleUsingPoints(3.14 / (2.0f / 3), 2);
		drawLine();
		

	SwapBuffers(ghdc);
}

void resize(int windth, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)windth, (GLsizei)height);
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

GLclampf getRandomNumer() {
	if (rand() % 2 == 0)
	{
		return 1.0f;
	}
	else {
		return 0.0f;
	}
}


void drawGraphOnWindow() {
	glLineWidth(1);
	for (GLfloat i = 0.0;i <= 1.0;i = i + 0.02)
	{
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);

		glVertex3f(i, 1.0f, 0.0f);
		glVertex3f(i, -1.0f, 0.0f);

		glEnd();
	}

	for (GLfloat i = 0.0;i <= 1.0;i = i + 0.02)
	{
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 1.0f);

		glVertex3f(-i, 1, 0.0f);
		glVertex3f(-i, -1, 0.0f);

		glEnd();
	}

	for (GLfloat i = 0.0;i <= 1.0;i = i + 0.02)
	{
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 1.0f);

		glVertex3f(-1.0f, i, 0.0f);
		glVertex3f(1.0f, i, 0.0f);

		glEnd();
	}

	for (GLfloat i = 0.0;i <= 1.0;i = i + 0.02)
	{
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);

		glVertex3f(-1.0f, -(i), 0.0f);
		glVertex3f(1.0f, -(i), 0.0f);

		glEnd();
	}
}

void getRgbColor(int colorCode, GLfloat *red, GLfloat *green, GLfloat *blue)
{

	switch (colorCode) {
	case 1: //Red
		*red = 1.0;
		*green = 0.0f;
		*blue = 0.0f;
		break;

	case 2: //Green
		*red = 0.0;
		*green = 1.0f;
		*blue = 0.0f;
		break;

	case 3: //Blue
		*red = 0.0;
		*green = 0.0f;
		*blue = 1.0f;
		break;

	case 4: //Cyan
		*red = 0.0;
		*green = 1.0f;
		*blue = 1.0f;
		break;

	case 5: //Magneta
		*red = 1.0;
		*green = 0.0f;
		*blue = 1.0f;
		break;

	case 6: //Yellow
		*red = 1.0f;
		*green = 1.0f;
		*blue = 0.0f;
		break;

	case 7: //White
		*red = 1.0f;
		*green = 1.0f;
		*blue = 1.0f;
		break;

	case 8: //Dark Gray
		*red = 0.25f;
		*green = 0.25f;
		*blue = 0.25f;
		break;

	case 9: //Middle Gray
		*red = 0.5f;
		*green = 0.5f;
		*blue = 0.5f;
		break;

	case 10: //White Gray
		*red = 0.75f;
		*green = 0.75f;
		*blue = 0.75f;
		break;
	}
}

void drawCircleUsingPoints(GLfloat reduaceRadiosBy, int colorCode) {

	GLfloat Ox, Oy, perimeter;
	GLfloat size = 1.0f, s,r;
	Ox = 1.0f*(-size / 2) + 1.0f*(size / 2);
	Oy = 1.0f*(-size / 2) + 1.0f*(-size / 2) + 1.0f*(size / 2);
	perimeter = 1.0f + 1.0f + 1.0f;
	s = (1.0f + 1.0f + 1.0f) / 2;
	r = sqrt(s*(s - 1.0f)*(s - 1.0f)*(s - 1.0f)) / s+0.013;

	float angle;
	glBegin(GL_POINTS);
	const float PI = 3.14;

	glColor3f(1.0f, 0.0f, 0.0f);
	for (angle = 0.0f;angle < 2.0f *PI;angle = angle + 0.0001f)
		glVertex3f(r*cos(angle), r*sin(angle)-0.19, 0.0f);
		//glVertex3f(cos(angle) / reduaceRadiosBy, sin(angle) / reduaceRadiosBy, 0.0f);
	
	glEnd();
	
}

void drawTriangle()
{
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	GLfloat size = 1.0f;//cos(angle) / reduaceRadiosBy;
	glVertex3f(0.0f, size / 2, 0.0f);
	glVertex3f(-size / 2, (-size / 2), 0.0f);

	glVertex3f(-size / 2, (-size / 2), 0.0f);
	glVertex3f(size / 2, (-size / 2), 0.0f);

	glVertex3f(size / 2, (-size / 2), 0.0f);
	glVertex3f(0.0f, size / 2, 0.0f);
	glEnd();

}

void drawLine()
{
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	GLfloat size = 1.0f;//cos(angle) / reduaceRadiosBy;
	glVertex3f(0.0f, size / 2, 0.0f);
	glVertex3f(0, -size / 2, 0.0f);

	
	glEnd();

}