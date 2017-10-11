//Header files.

#include <windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Header.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

int currentWidth;
int currentHeight;
float angleCube;
GLuint	textureFootBall; 
GLuint	textureFootBallGround;
GLuint	textureGoalNet;
GLuint	textureIndianAudience;

GLUquadric* quadric = NULL;

GLfloat x = -5.0;
bool canStartRotate = false;
bool canPause = false;
//Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Initialize();
void FirstResize();
void UnInitialize();
void Display();
void DrawMultiColoredCube();
void DrawSphere();
void resize(int, int);
void update();
int LoadGLTextures(GLuint *, TCHAR[]);

void drawMan(void);
void drawGround(void);
void drawNet(void);
void drawLeftAudience(void);


HWND gHwnd;
HDC gHdc;
HGLRC gHglrc;

bool gbFullscreen;
bool gbEscapeKeyIsPressed;
bool gbActiveWindow;
bool bDone;

WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
DWORD dwStyle;

//Code
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdArgs, int cmdShow)
{

    TCHAR className[] = TEXT("OpenGL_Window");
    WNDCLASSEX wndClassEx;
    HWND hWnd;
    MSG msg;

    wndClassEx.cbSize = sizeof(WNDCLASSEX);
    wndClassEx.cbClsExtra = 0;
    wndClassEx.cbWndExtra = 0;
    wndClassEx.hbrBackground = CreateSolidBrush(RGB(0.0f, 0.0f, 0.0f));
    wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndClassEx.hInstance = hInstance;
    wndClassEx.lpfnWndProc = WndProc;
    wndClassEx.lpszMenuName = NULL;
    wndClassEx.lpszClassName = className;
    wndClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    RegisterClassEx(&wndClassEx);

    int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    int monitorHeight = GetSystemMetrics(SM_CYSCREEN);

    int x = (monitorWidth / 2) - (WIN_WIDTH / 2);
    int y = (monitorHeight / 2) - (WIN_HEIGHT / 2);


    hWnd = CreateWindowEx(WS_EX_APPWINDOW, className, TEXT("Point"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
        x, y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

    gHwnd = hWnd;
    Initialize();

    ShowWindow(hWnd, cmdShow);
    //Game Loop
    while (bDone == false)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                bDone = TRUE;
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
                if (gbEscapeKeyIsPressed == true)
                    bDone = true;
            }
            Display();
            update();
        }
    }
    UnInitialize();
    return (int)msg.wParam;
}

void Initialize()
{
    PIXELFORMATDESCRIPTOR pfd;

    int iPixelFormatIndex;
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nVersion = 1;
    pfd.cColorBits = 32;
    pfd.cGreenBits = 8;
    pfd.cRedBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 32;
    pfd.cDepthBits = 32;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);

    gHdc = GetDC(gHwnd);

    iPixelFormatIndex = ChoosePixelFormat(gHdc, &pfd);
    if (iPixelFormatIndex == 0)
    {
        ReleaseDC(gHwnd, gHdc);
        gHdc = NULL;
    }

    if (SetPixelFormat(gHdc, iPixelFormatIndex, &pfd) == false)
    {
        ReleaseDC(gHwnd, gHdc);
        gHdc = NULL;
    }

    gHglrc = wglCreateContext(gHdc);
    if (gHglrc == NULL)
    {
        ReleaseDC(gHwnd, gHdc);
        gHdc = NULL;
    }

    if (wglMakeCurrent(gHdc, gHglrc) == NULL)
    {
        wglDeleteContext(gHglrc);
        gHglrc = NULL;
        ReleaseDC(gHwnd, gHdc);
        gHdc = NULL;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    LoadGLTextures(&textureFootBall, MAKEINTRESOURCE(IDBITMAP_FOOTBALL));
	LoadGLTextures(&textureFootBallGround, MAKEINTRESOURCE(IDBITMAP_FOOTBALL_GROUND));
	LoadGLTextures(&textureGoalNet, MAKEINTRESOURCE(IDBITMAP_FOOTBALL_GOAL_NET));
	LoadGLTextures(&textureIndianAudience, MAKEINTRESOURCE(IDBITMAP_FOOTBALL_INDIAN_AUDIENCE));

    glEnable(GL_TEXTURE_2D); 

    FirstResize();
}

int LoadGLTextures(GLuint *texture, TCHAR imageResourceId[])
{
    //variable declarations
    HBITMAP hBitmap;
    BITMAP bmp;
    int iStatus = FALSE;

    //code
    glGenTextures(1, texture); //1 image
    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hBitmap) //if bitmap exists ( means hBitmap is not null )
    {
        iStatus = TRUE;
        GetObject(hBitmap, sizeof(bmp), &bmp);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //pixel storage mode (word alignment/4 bytes)
        glBindTexture(GL_TEXTURE_2D, *texture); //bind texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

        DeleteObject(hBitmap); //delete unwanted bitmap handle
    }
    return(iStatus);
}
void FirstResize()
{
    //TBD.. revisit
    currentWidth = WIN_WIDTH - GetSystemMetrics(SM_CXSIZEFRAME) * 4;
    currentHeight = WIN_HEIGHT - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYSIZEFRAME) * 4;
    resize(currentWidth, currentHeight);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Functions Declarations
    void ToggleFullscreen();
    void resize(int, int);

    switch (message)
    {
    case WM_CREATE:
        break;

    case WM_ACTIVATE:
        if (HIWORD(wParam) == 0)
            gbActiveWindow = true;
        else
            gbActiveWindow = false;
        break;

    case WM_LBUTTONDOWN:
        break;

    case WM_RBUTTONDOWN:
        break;

    case WM_SIZE:
        resize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_PAINT:
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
		case 'R':
			x = -5;
			canStartRotate = false;
			canPause = false;
			break;
		case 'S':
			canStartRotate = true;
			break;
        case VK_ESCAPE:
            if (gbEscapeKeyIsPressed == false)
                gbEscapeKeyIsPressed = true;
            break;

        case 0x46:  //for 'f'
            if (gbFullscreen == true)
            {
                ToggleFullscreen();
                gbFullscreen = false;
            }
            else
            {
                ToggleFullscreen();
                gbFullscreen = true;
            }
            break;
        }
        break;

    case WM_ERASEBKGND:
        return(0);
        break;

    case WM_CLOSE:
        UnInitialize();
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void ToggleFullscreen()
{
    MONITORINFO mi = { sizeof(MONITORINFO) };

    if (!gbFullscreen)
    {
        // Make fullscreen
        dwStyle = GetWindowLong(gHwnd, GWL_STYLE);
        if (dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if (GetWindowPlacement(gHwnd, &wpPrev) && GetMonitorInfo(
                MonitorFromWindow(gHwnd, MONITORINFOF_PRIMARY), &mi))
            {
                SetWindowLong(gHwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(gHwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                    mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
                    SWP_NOZORDER | SWP_FRAMECHANGED);
            }
        }
        ShowCursor(FALSE);
    }
    else
    {
        SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(gHwnd, &wpPrev);
        SetWindowPos(gHwnd, HWND_TOP, 0, 0, 0, 0,
            SWP_NOMOVE |
            SWP_NOSIZE |
            SWP_NOOWNERZORDER |
            SWP_NOZORDER |
            SWP_FRAMECHANGED);
        ShowCursor(TRUE);
    }
}

void resize(int width, int height)
{
    if (height == 0)
        height = 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void UnInitialize()
{
    if (gbFullscreen == true)
    {
        dwStyle = GetWindowLong(gHwnd, GWL_STYLE);
        SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(gHwnd, &wpPrev);
        SetWindowPos(gHwnd, HWND_TOP, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
    }

    wglMakeCurrent(NULL, NULL);

    wglDeleteContext(gHglrc);
    gHglrc = NULL;

    ReleaseDC(gHwnd, gHdc);
    gHdc = NULL;

    DestroyWindow(gHwnd);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0f, 5.0f, -45.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	drawGround();
    DrawSphere();
	drawNet();
	//drawLeftAudience();
	//drawMan();
	
    SwapBuffers(gHdc);
}

void update()
{
    if (angleCube > 360)
        angleCube = 0;

    angleCube += 0.3;
}

void DrawSphere()
{
	glBindTexture(GL_TEXTURE_2D, textureFootBall);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	if (!canPause)
	{
		glLoadIdentity();
		if (!canStartRotate)
		{
			glLoadIdentity();
			glTranslatef(-2.0, -5.4, -15.0f);
			//glTranslatef(-5.0f, -0.5f, -20.0f);
		}
		else
		{
			glLoadIdentity();
			glTranslatef(-2.0, -0.4 + x, -20.0f - (x));
			x = x + 0.013f;
			if (x > 10.0f)
			{
			//	x = -5.0f;
				canPause = true;
			}
		}

		if (canStartRotate)
		{
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(-angleCube, 0.0f, 0.0f, 1.0f);
		}

		gluSphere(quadric, 1.0f, 100, 100);
	}
	else
	{
		glLoadIdentity();
		glTranslatef(-2.0, -0.4 + x, -20.0f - (x));
		//x = x + 0.013f;
		gluSphere(quadric, 1.0f, 100, 100);
	}
}

void drawMan()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
	glLoadIdentity();
	glTranslated(-6.0f, 0.0f, -20.0f);
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-6.0f,3.0f,0.0f);
		glVertex3f(-6.0f, -2.5f, 0.0f);
	glEnd();
}

void drawGround()
{

	glBindTexture(GL_TEXTURE_2D, textureFootBallGround);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();
	glTranslated(0.0f, 0.0f, -30.0f);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-25.0f,-20.0f,0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(25.0f, -20.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(25.0f, 20.0f, -60.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-25.0f, 20.0f, -60.0f);
	glEnd();	
}

void drawNet()
{
	glBindTexture(GL_TEXTURE_2D, textureGoalNet);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();

	glTranslated(0.0f, 0.0f, -60.0f);
	glLineWidth(8);
	glBegin(GL_POLYGON);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-15.0f, 10.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-15.0f, 23.0f, 0.0f);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(15.0f, 23.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(15.0f, 10.0f, 0.0f);

	glEnd();
}

void drawLeftAudience()
{
	glBindTexture(GL_TEXTURE_2D, textureIndianAudience);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLoadIdentity();
	glTranslated(0.0f, 0.0f, -30.0f);
	glPointSize(10);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-20.0f, -13.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-40.0f, 20.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-40.0f, 50.0f, -100.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-20.0f, 50.0f, -100.0f);

	glEnd();
}
