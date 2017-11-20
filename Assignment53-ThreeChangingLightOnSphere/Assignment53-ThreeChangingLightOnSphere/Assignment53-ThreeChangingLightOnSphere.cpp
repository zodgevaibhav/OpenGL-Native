#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//Global declaration of the callback of the window
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void drawPyramid(void);
void drawCube(void);

void updateBlueAngle(void);
void updateRedAngle(void);
void updateGreenAngle(void);

//gloabal variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbEscapeKeyIsPressed = false;
bool gbIsActiveWindow = false;
bool gbFullscreen = false;

GLboolean gbPyramid = GL_TRUE;
GLboolean gbCube = GL_FALSE;
GLboolean gbSphere = GL_FALSE;
GLboolean gbAll = GL_FALSE;

bool isLightingEnabled = false;

GLUquadric *quadric = NULL;
GLfloat angle_Pyramid = 0.0f;

GLfloat angle_red = 0.0f;
GLfloat angle_green = 0.0f;
GLfloat angle_blue = 0.0f;

GLfloat light_red_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light_red_defuse[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light_red_specular[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light_red_position[] = { 0.0f,0.0f,0.0f,0.0f };

GLfloat light_blue_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light_blue_defuse[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat light_blue_specular[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light_blue_position[] = { 0.0f,0.0f,0.0f,0.0f };

GLfloat light_green_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light_green_defuse[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat light_green_specular[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light_green_position[] = { 0.0f,0.0f,0.0f,0.0f };


GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat material_defuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shining[] = { 50.0f };


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
				updateBlueAngle();
				updateRedAngle();
				updateGreenAngle();
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
		case 0X4c: //L
			if (isLightingEnabled == false)
			{
				glEnable(GL_LIGHTING);
				isLightingEnabled = true;
			}
			else {
				glDisable(GL_LIGHTING);
				isLightingEnabled = false;
			}
			break;
		case 0X43://C
			gbCube = GL_TRUE;
			gbPyramid = GL_FALSE;
			gbSphere = GL_FALSE;
			gbAll = GL_FALSE;
			break;
		case 0X50://P
			gbCube = GL_FALSE;
			gbPyramid = GL_TRUE;
			gbSphere = GL_FALSE;
			gbAll = GL_FALSE;
			break;
		case 0X53://S
			gbCube = GL_FALSE;
			gbPyramid = GL_FALSE;
			gbSphere = GL_TRUE;
			gbAll = GL_FALSE;
			break;
		case 0X41://A
			gbCube = GL_FALSE;
			gbPyramid = GL_FALSE;
			gbSphere = GL_FALSE;
			gbAll = GL_TRUE;
			break;
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

	glClearDepth(1.0f); // set depth buffer
	glEnable(GL_DEPTH_TEST); // enable depth testing
	glDepthFunc(GL_LEQUAL); // type of depth testing

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_red_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_red_defuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_red_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_red_position);

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_blue_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_blue_defuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_blue_specular);
	//glLightfv(GL_LIGHT2, GL_POSITION, light_blue_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_green_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_green_defuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_green_specular);
	//glLightfv(GL_LIGHT1, GL_POSITION, light_blue_position);

	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shining);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shining);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	quadric = gluNewQuadric();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
		gluLookAt(0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
			glRotatef(angle_red, 1, 0, 0);
			light_red_position[1] = angle_red;
			glLightfv(GL_LIGHT0, GL_POSITION, light_red_position);
		glPopMatrix();

		glPushMatrix();
			glRotatef(angle_blue, 0, 0, 1);
			light_blue_position[1] = angle_blue;
			glLightfv(GL_LIGHT2, GL_POSITION, light_blue_position);
		glPopMatrix();
		
		glPushMatrix();
			glRotatef(angle_green, 0, 1, 0);
			light_green_position[1] = angle_green;
			glLightfv(GL_LIGHT1, GL_POSITION, light_green_position);
		glPopMatrix();
	glPopMatrix();
		glTranslatef(0.0f, 0.0f, -2.0f);
		glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glColor3f(1.0f, 1.0f, 0.0f);
		gluSphere(quadric, 0.75, 30, 30);
	

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

void updateRedAngle(void)
{
	//code
	angle_red = angle_red + 0.1f;
	if (angle_red >= 360.0f)
		angle_red = 0.0f;
}

void updateGreenAngle(void)
{
	//code
	angle_green = angle_green + 0.1f;
	if (angle_green >= 360.0f)
		angle_green = 0.0f;
}

void updateBlueAngle(void)
{
	//code
	angle_blue = angle_blue + 0.1f;
	if (angle_blue >= 360.0f)
		angle_blue = 0.0f;
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

void drawPyramid(void)
{

	glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);
	//FRONT FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glNormal3f(0.0f, 0.447214f, 0.894427f);
	glVertex3f(0.0f, 1.0f, 0.0f); //apex
	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(-1.0f, -1.0f, 1.0f); //left-corner of front face
	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(1.0f, -1.0f, 1.0f); //right-corner of front face

								   //RIGHT FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glNormal3f(0.894427f, 0.447214f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); //apex
	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(1.0f, -1.0f, 1.0f); //left-corner of right face
	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(1.0f, -1.0f, -1.0f); //right-corner of right face

									//BACK FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glNormal3f(0.0f, 0.447214f, -0.894427f);
	glVertex3f(0.0f, 1.0f, 0.0f); //apex
	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(1.0f, -1.0f, -1.0f); //left-corner of back face
	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(-1.0f, -1.0f, -1.0f); //right-corner of back face

									 //LEFT FACE
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glNormal3f(-0.894427f, 0.447214f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); //apex
	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(-1.0f, -1.0f, -1.0f); //left-corner of left face
	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(-1.0f, -1.0f, 1.0f); //right-corner of left face
	glEnd();
}

void drawCube(void)
{

	glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	//TOP FACE
	glColor3f(1.0f, 0.0f, 0.0f); //RED
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);  //right-top corner of top face
	glVertex3f(-1.0f, 1.0f, -1.0f); //left-top corner of top face
	glVertex3f(-1.0f, 1.0f, 1.0f); //left-bottom corner of top face
	glVertex3f(1.0f, 1.0f, 1.0f); //right-bottom corner of top face

								  //BOTTOM FACE
	glColor3f(0.0f, 1.0f, 0.0f); //GREEN
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f); //right-top corner of bottom face
	glVertex3f(-1.0f, -1.0f, -1.0f); //left-top corner of bottom face
	glVertex3f(-1.0f, -1.0f, 1.0f); //left-bottom corner of bottom face
	glVertex3f(1.0f, -1.0f, 1.0f); //right-bottom corner of bottom face

								   //FRONT FACE
	glColor3f(0.0f, 0.0f, 1.0f); //BLUE
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f); //right-top corner of front face
	glVertex3f(-1.0f, 1.0f, 1.0f); //left-top corner of front face
	glVertex3f(-1.0f, -1.0f, 1.0f); //left-bottom corner of front face
	glVertex3f(1.0f, -1.0f, 1.0f); //right-bottom corner of front face

								   //BACK FACE
	glColor3f(0.0f, 1.0f, 1.0f); //CYAN
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f); //right-top of back face
	glVertex3f(-1.0f, 1.0f, -1.0f); //left-top of back face
	glVertex3f(-1.0f, -1.0f, -1.0f); //left-bottom of back face
	glVertex3f(1.0f, -1.0f, -1.0f); //right-bottom of back face

									//RIGHT FACE
	glColor3f(1.0f, 0.0f, 1.0f); //MAGENTA
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f); //right-top of right face
	glVertex3f(1.0f, 1.0f, 1.0f); //left-top of right face
	glVertex3f(1.0f, -1.0f, 1.0f); //left-bottom of right face
	glVertex3f(1.0f, -1.0f, -1.0f); //right-bottom of right face

									//LEFT FACE
	glColor3f(1.0f, 1.0f, 0.0f); //YELLOW
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f); //right-top of left face
	glVertex3f(-1.0f, 1.0f, -1.0f); //left-top of left face
	glVertex3f(-1.0f, -1.0f, -1.0f); //left-bottom of left face
	glVertex3f(-1.0f, -1.0f, 1.0f); //right-bottom of left face
	glEnd();
}