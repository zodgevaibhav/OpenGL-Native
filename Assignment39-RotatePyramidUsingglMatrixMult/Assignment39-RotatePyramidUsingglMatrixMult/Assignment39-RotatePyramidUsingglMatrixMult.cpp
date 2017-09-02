#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include<math.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//Global declaration of the callback of the window
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//gloabal variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbEscapeKeyIsPressed = false;
bool gbIsActiveWindow = false;
bool gbFullscreen = false;

GLfloat angle_Pyramid = 0.0f;

GLfloat identityMatrix[16];
GLfloat translationMatrix[16];
GLfloat scaleMatrix[16];
GLfloat xRotationMatrix[16];
GLfloat yRotationMatrix[16];
GLfloat zRotationMatrix[16];

void glTranslationMatrixCustom(void);
void glLoadIdentityMatrixCustom(void);
void glXRotateCustom(void);
void glYRotateCustom(void);
void glZRotateCustom(void);
void glScaleCustom(void);

//WinMain() - Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//function prototype
	void initialize(void);
	void update(void);
	void display(void);
	void uninitialize(void);

	//variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	TCHAR szClassName[] = TEXT("OGLRTR");
	MSG msg;
	bool bDone = false;

	//code
	//Initializing all (12) members of struct WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;

	//Registering the class of window
	RegisterClassEx(&wndclass);

	//Creation of the actual window
	TCHAR szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		szExeFileName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
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

	//To enable the window to appear on screen
	ShowWindow(hwnd, nCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//Message Loop
	while (bDone == false)
	{
		if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbIsActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true)
					bDone = true;

				//Call your rendering function here
				update();
				display();
			}
		}
	}

	uninitialize();
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototype
	void resize(int, int);
	void ToggleFullscreen(void);

	//code
	switch (iMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbIsActiveWindow = true;
		else
			gbIsActiveWindow = false;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
			break;
		case 0x46: //'F' Key
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
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void initialize(void)
{
	//function prototype
	void resize(int, int);

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	//code
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
	pfd.cDepthBits = 32;

	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
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

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	//function prototype
	void DrawMultiColoredTriangle(void);
	glLoadIdentityMatrixCustom(); //fill matrix
	glTranslationMatrixCustom();//fill matrix

	glXRotateCustom();//fill matrix
	glYRotateCustom();//fill matrix
	glZRotateCustom();//fill matrix


	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TRIANGLE
	glMatrixMode(GL_MODELVIEW);

	//glLoadIdentity();
	glLoadMatrixf(identityMatrix);

	//glTranslatef(0.0f, 0.0f, -5.0f);
	glMultMatrixf(translationMatrix);

	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glMultMatrixf(xRotationMatrix);
	glMultMatrixf(yRotationMatrix);
	glMultMatrixf(zRotationMatrix);

	glBegin(GL_TRIANGLES);
	//FRONT FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(0.0f, 1.0f, 0.0f); //apex

	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(-1.0f, -1.0f, 1.0f); //left-corner of front face

	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(1.0f, -1.0f, 1.0f); //right-corner of front face

								   //RIGHT FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(0.0f, 1.0f, 0.0f); //apex

	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(1.0f, -1.0f, 1.0f); //left-corner of right face

	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(1.0f, -1.0f, -1.0f); //right-corner of right face

									//BACK FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(0.0f, 1.0f, 0.0f); //apex

	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(1.0f, -1.0f, -1.0f); //left-corner of back face

	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(-1.0f, -1.0f, -1.0f); //right-corner of back face

									 //LEFT FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(0.0f, 1.0f, 0.0f); //apex

	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(-1.0f, -1.0f, -1.0f); //left-corner of left face

	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(-1.0f, -1.0f, 1.0f); //right-corner of left face
	glEnd();

	SwapBuffers(ghdc);
}

void resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void update(void)
{
	//code
	//angle_Pyramid = angle_Pyramid *(3.1415 / 180)+0.1f;// 0.1f;
	angle_Pyramid = angle_Pyramid + 0.1f;
	if (angle_Pyramid >= 360.0f)
		angle_Pyramid = 0.0f;

	angle_Pyramid = angle_Pyramid*(3.1415f / 180.0f);
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
				SetWindowPos(ghwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					(mi.rcMonitor.right - mi.rcMonitor.left),
					(mi.rcMonitor.bottom - mi.rcMonitor.top),
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
}

void uninitialize(void)
{
	//code
	if (gbFullscreen == true)
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}

	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(ghwnd);
	ghwnd = NULL;
}


void glLoadIdentityMatrixCustom()
{
	identityMatrix[0] = 1.0f;
	identityMatrix[1] = 0.0f;
	identityMatrix[2] = 0.0f;
	identityMatrix[3] = 0.0f;
	identityMatrix[4] = 0.0f;
	identityMatrix[5] = 1.0f;
	identityMatrix[6] = 0.0f;
	identityMatrix[7] = 0.0f;
	identityMatrix[8] = 0.0f;
	identityMatrix[9] = 0.0f;
	identityMatrix[10] = 1.0f;
	identityMatrix[11] = 0.0f;
	identityMatrix[12] = 0.0f;
	identityMatrix[13] = 0.0f;
	identityMatrix[14] = 0.0f;
	identityMatrix[15] = 1.0f;
}

void glTranslationMatrixCustom()
{
	translationMatrix[0] = 1.0f;
	translationMatrix[1] = 0.0f;
	translationMatrix[2] = 0.0f;
	translationMatrix[3] = 0.0f;
	translationMatrix[4] = 0.0f;
	translationMatrix[5] = 1.0f;
	translationMatrix[6] = 0.0f;
	translationMatrix[7] = 0.0f;
	translationMatrix[8] = 0.0f;
	translationMatrix[9] = 0.0f;
	translationMatrix[10] = 1.0f;
	translationMatrix[11] = 0.0f;
	translationMatrix[12] = 0.0f;
	translationMatrix[13] = 0.0f;
	translationMatrix[14] = -5.0f;
	translationMatrix[15] = 1.0f;
}

void glScaleMatrixCustom()
{
	scaleMatrix[0] = 0.75f;
	scaleMatrix[1] = 0.0f;
	scaleMatrix[2] = 0.0f;
	scaleMatrix[3] = 0.0f;
	scaleMatrix[4] = 0.0f;
	scaleMatrix[5] = 0.75f;
	scaleMatrix[6] = 0.0f;
	scaleMatrix[7] = 0.0f;
	scaleMatrix[8] = 0.0f;
	scaleMatrix[9] = 0.0f;
	scaleMatrix[10] = 0.75f;
	scaleMatrix[11] = 0.0f;
	scaleMatrix[12] = 0.0f;
	scaleMatrix[13] = 0.0f;
	scaleMatrix[14] = 0.0f;
	scaleMatrix[15] = 1.0f;
}

void glXRotateCustom()
{
	xRotationMatrix[0] = 1.0f;
	xRotationMatrix[1] = 0.0f;
	xRotationMatrix[2] = 0.0f;
	xRotationMatrix[3] = 0.0f;
	xRotationMatrix[4] = 0.0f;
	xRotationMatrix[5] = (GLfloat)cos(angle_Pyramid);
	xRotationMatrix[6] = (GLfloat)sin(angle_Pyramid);
	xRotationMatrix[7] = 0.0f;
	xRotationMatrix[8] = 0.0f;
	xRotationMatrix[9] = -sin(angle_Pyramid);
	xRotationMatrix[10] = cos(angle_Pyramid);
	xRotationMatrix[11] = 0.0f;
	xRotationMatrix[12] = 0.0f;
	xRotationMatrix[13] = 0.0f;
	xRotationMatrix[14] = 0.0f;
	xRotationMatrix[15] = 1.0f;
}

void glYRotateCustom()
{
	yRotationMatrix[0] = (GLfloat)cos(angle_Pyramid);
	yRotationMatrix[1] = 0.0f;
	yRotationMatrix[2] = (GLfloat)-sin(angle_Pyramid);
	yRotationMatrix[3] = 0.0f;
	yRotationMatrix[4] = 0.0f;
	yRotationMatrix[5] = 1.0f;
	yRotationMatrix[6] = 0.0f;
	yRotationMatrix[7] = 0.0f;
	yRotationMatrix[8] = (GLfloat)sin(angle_Pyramid);
	yRotationMatrix[9] = 0.0f;
	yRotationMatrix[10] = (GLfloat)cos(angle_Pyramid);
	yRotationMatrix[11] = 0.0f;
	yRotationMatrix[12] = 0.0f;
	yRotationMatrix[13] = 0.0f;
	yRotationMatrix[14] = 0.0f;
	yRotationMatrix[15] = 1.0f;
}

void glZRotateCustom()
{
	zRotationMatrix[0] = (GLfloat)cos(angle_Pyramid);
	zRotationMatrix[1] = (GLfloat)sin(angle_Pyramid);
	zRotationMatrix[2] = 0.0f;
	zRotationMatrix[3] = 0.0f;
	zRotationMatrix[4] = (GLfloat)-sin(angle_Pyramid);
	zRotationMatrix[5] = (GLfloat)cos(angle_Pyramid);
	zRotationMatrix[6] = 0.0f;
	zRotationMatrix[7] = 0.0f;
	zRotationMatrix[8] = 0.0f;
	zRotationMatrix[9] = 0.0f;
	zRotationMatrix[10] = 1.0f;
	zRotationMatrix[11] = 0.0f;
	zRotationMatrix[12] = 0.0f;
	zRotationMatrix[13] = 0.0f;
	zRotationMatrix[14] = 0.0f;
	zRotationMatrix[15] = 1.0f;
}