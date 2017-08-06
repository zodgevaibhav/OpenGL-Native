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
void drawCircleUsingPoints(GLfloat , int );
bool gbEscapeKeyIsPressed = false;
bool gbIsActiveWindow = false;
bool gbFullscreen = false;
void drawTriangle(void);
void drawCube(void);
GLfloat angle_Pyramid = 0.0f;

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
	
	void DrawMultiColoredTriangle(void);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTranslatef(0.0f, 0.0f, -3.0f);
	drawTriangle();
	drawCube();
	//drawCircleUsingPoints(3.14 / (2.0f / 3), 2);
	//drawCube();

	
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
	angle_Pyramid = angle_Pyramid + 0.1f;
	if (angle_Pyramid >= 360.0f)
		angle_Pyramid = 0.0f;
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

void drawCircleUsingPoints(GLfloat reduaceRadiosBy, int colorCode) {
//	glLoadIdentity();
	GLfloat num_vertex = 1000.0f, cx = 0.01f, cy = 0.01f, radius = 0.5f;;
	GLfloat vertex[4];
	GLfloat texcoord[2];
	GLfloat PI = 3.1415;
	const GLfloat delta_angle = 2.0*PI / num_vertex;


	glTranslatef(0.0f, 0.0f, -3.0f);
	glRotatef(angle_Pyramid, 1.0f, 1.0f, 1.0f);

	glBegin(GL_TRIANGLE_FAN);

	//draw the vertex at the center of the circle
	texcoord[0] = 0.5;
	texcoord[1] = 0.5;
	glTexCoord2fv(texcoord);

	vertex[0] = vertex[1] = vertex[2] = 0.0;
	vertex[3] = 1.0;
	glVertex4fv(vertex);

	for (int i = 0; i < num_vertex; i++)
	{
		texcoord[0] = (cos(delta_angle*i) + 1.0)*0.5;
		texcoord[1] = (sin(delta_angle*i) + 1.0)*0.5;
		glTexCoord2fv(texcoord);

		vertex[0] = cos(delta_angle*i) * radius;
		vertex[1] = sin(delta_angle*i) * radius;
		vertex[2] = 0.0;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	texcoord[0] = (1.0 + 1.0)*0.5;
	texcoord[1] = (0.0 + 1.0)*0.5;
	glTexCoord2fv(texcoord);

	vertex[0] = 1.0 * radius;
	vertex[1] = 0.0 * radius;
	vertex[2] = 0.0;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();


}


void drawTriangle()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
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
}

void drawCube()
{
	//glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
	glRotatef(angle_Pyramid, 1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	//TOP FACE
	glColor3f(1.0f, 0.0f, 0.0f); //RED
	glVertex3f(0.5f, 0.5f, -0.5f);  //right-top corner of top face
	glVertex3f(-0.5f, 0.5f, -0.5f); //left-top corner of top face
	glVertex3f(-0.5f, 0.5f, 0.5f); //left-bottom corner of top face
	glVertex3f(0.5f, 0.5f, 0.5f); //right-bottom corner of top face

								  //BOTTOM FACE
	glColor3f(0.0f, 1.0f, 0.0f); //GREEN
	glVertex3f(0.5f, -0.5f, -0.5f); //right-top corner of bottom face
	glVertex3f(-0.5f, -0.5f, -0.5f); //left-top corner of bottom face
	glVertex3f(-0.5f, -0.5f, 0.5f); //left-bottom corner of bottom face
	glVertex3f(0.5f, -0.5f, 0.5f); //right-bottom corner of bottom face

								   //FRONT FACE
	glColor3f(0.0f, 0.0f, 1.0f); //BLUE
	glVertex3f(0.5f, 0.5f, 0.5f); //right-top corner of front face
	glVertex3f(-0.5f, 0.5f, 0.5f); //left-top corner of front face
	glVertex3f(-0.5f, -0.5f, 0.5f); //left-bottom corner of front face
	glVertex3f(0.5f, -0.5f, 0.5f); //right-bottom corner of front face

								   //BACK FACE
	glColor3f(0.0f, 1.0f, 1.0f); //CYAN
	glVertex3f(0.5f, 0.5f, -0.5f); //right-top of back face
	glVertex3f(-0.5f, 0.5f, -0.5f); //left-top of back face
	glVertex3f(-0.5f, -0.5f, -0.5f); //left-bottom of back face
	glVertex3f(0.5f, -0.5f, -0.5f); //right-bottom of back face

									//RIGHT FACE
	glColor3f(1.0f, 0.0f, 1.0f); //MAGENTA
	glVertex3f(0.5f, 0.5f, -0.5f); //right-top of right face
	glVertex3f(0.5f, 0.5f, 0.5f); //left-top of right face
	glVertex3f(0.5f, -0.5f, 0.5f); //left-bottom of right face
	glVertex3f(0.5f, -0.5f, -0.5f); //right-bottom of right face

									//LEFT FACE
	glColor3f(1.0f, 1.0f, 0.0f); //YELLOW
	glVertex3f(-0.5f, 0.5f, 0.5f); //right-top of left face
	glVertex3f(-0.5f, 0.5f, -0.5f); //left-top of left face
	glVertex3f(-0.5f, -0.5f, -0.5f); //left-bottom of left face
	glVertex3f(-0.5f, -0.5f, 0.5f); //right-bottom of left face
	glEnd();
}