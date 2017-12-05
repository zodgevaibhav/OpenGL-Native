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
GLfloat angle = 0.0f;

GLfloat angle_x = 0.0f;
GLfloat angle_y = 0.0f;
GLfloat angle_z = 0.0f;



GLfloat isXRotation =0.0f;
GLfloat isYRotation = 0.0f;
GLfloat isZRotation= 0.0f;


GLfloat light_model_ambient[] = { 0.2f,0.2f,0.2f,0.0f };
GLfloat light_model_local_viewer[] = {0.0f};

GLfloat light_white_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light_white_defuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light_white_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light_white_position[] = { 0.0f,0.0f,0.0f,0.0f };


GLfloat  s1_ambient_material[] = { 0.0215f, 0.1745f, 0.0215f,1.0f };
GLfloat  s1_difuse_material[] = { 0.07568f, 0.61424f, 0.07568f,1.0f };
GLfloat  s1_specular_material[] = { 0.633f, 0.727811f, 0.633f,1.0f };
GLfloat  s1_shininess[] = { 0.6f *128.0f };

GLfloat  s2_ambient_material[] = { 0.135f, 0.2225f, 0.1575f,1.0f };
GLfloat  s2_difuse_material[] = { 0.54f, 0.89f, 0.63f,1.0f };
GLfloat  s2_specular_material[] = { 0.316228f, 0.316228f, 0.316228f,1.0f };
GLfloat  s2_shininess[] = { 0.1f *128.0f };

GLfloat  s3_ambient_material[] = { 0.05375f, 0.05f, 0.06625f,1.0f };
GLfloat  s3_difuse_material[] = { 0.18275f, 0.17f, 0.22525f,1.0f };
GLfloat  s3_specular_material[] = { 0.332741f, 0.328634f, 0.346435f,1.0f };
GLfloat  s3_shininess[] = { 0.3f*128.0f };

GLfloat  s4_ambient_material[] = { 0.25f, 0.20725f, 0.20725f,1.0f };
GLfloat  s4_difuse_material[] = { 1.0f, 0.829f, 0.829f,1.0f };
GLfloat  s4_specular_material[] = { 0.296648f, 0.296648f, 0.296648f,1.0f };
GLfloat  s4_shininess[] = { 0.088f*128.0f };

GLfloat  s5_ambient_material[] = { 0.1745f, 0.01175f, 0.01175f,1.0f };
GLfloat  s5_difuse_material[] = { 0.61424f, 0.04136f, 0.04136f,1.0f };
GLfloat  s5_specular_material[] = { 0.727811f, 0.626959f, 0.626959f,1.0f };
GLfloat  s5_shininess[] = { 0.6f *128.0f };

GLfloat  s6_ambient_material[] = { 0.1f, 0.18725f, 0.1745f,1.0f };
GLfloat  s6_difuse_material[] = { 0.396f, 0.74151f, 0.69102f,1.0f };
GLfloat  s6_specular_material[] = { 0.297254f, 0.30829f, 0.306678f,1.0f };
GLfloat  s6_shininess[] = { 0.1f *128.0f };

GLfloat  s7_ambient_material[] = { 0.329412f, 0.223529f, 0.027451f,1.0f };
GLfloat  s7_difuse_material[] = { 0.780392f, 0.568627f, 0.113725f,1.0f };
GLfloat  s7_specular_material[] = { 0.992157f, 0.941176f, 0.807843f,1.0f };
GLfloat  s7_shininess[] = { 0.21794872f *128.0f };

GLfloat  s8_ambient_material[] = { 0.2125f, 0.1275f, 0.054f,1.0f };
GLfloat  s8_difuse_material[] = { 0.714f, 0.4284f, 0.18144f,1.0f };
GLfloat  s8_specular_material[] = { 0.393548f, 0.271906f, 0.166721f,1.0f };
GLfloat  s8_shininess[] = { 0.2f *128.0f };

GLfloat  s9_ambient_material[] = { 0.25f, 0.25f, 0.25f,1.0f };
GLfloat  s9_difuse_material[] = { 0.4f, 0.4f, 0.4f,1.0f };
GLfloat  s9_specular_material[] = { 0.774597f, 0.774597f, 0.774597f,1.0f };
GLfloat  s9_shininess[] = { 0.6f *128.0f };

GLfloat  s10_ambient_material[] = { 0.19125f, 0.0735f, 0.0225f,1.0f };
GLfloat  s10_difuse_material[] = { 0.7038f, 0.27048f, 0.0828f,1.0f };
GLfloat  s10_specular_material[] = { 0.256777f, 0.137622f, 0.086014f,1.0f };
GLfloat  s10_shininess[] = { 0.1f * 128.0f };

GLfloat  s11_ambient_material[] = { 0.24725f, 0.1995f, 0.0745f,1.0f };
GLfloat  s11_difuse_material[] = { 0.75164f, 0.60648f, 0.22648f,1.0f };
GLfloat  s11_specular_material[] = { 0.628281f, 0.555802f, 0.366065f,1.0f };
GLfloat  s11_shininess[] = { 0.4f * 128.0f };

GLfloat  s12_ambient_material[] = { 0.19225f, 0.19225f, 0.19225f,1.0f };
GLfloat  s12_difuse_material[] = { 0.50754f, 0.50754f, 0.50754f,1.0f };
GLfloat  s12_specular_material[] = { 0.508273f, 0.508273f, 0.508273f,1.0f };
GLfloat  s12_shininess[] = { 0.4f * 128.0f };

GLfloat  s13_ambient_material[] = { 0.0f, 0.0f, 0.0f,1.0f };
GLfloat  s13_difuse_material[] = { 0.01f, 0.01f, 0.01f,1.0f };
GLfloat  s13_specular_material[] = { 0.50f, 0.50f, 0.50f,1.0f };
GLfloat  s13_shininess[] = { 0.25f * 128.0f };

GLfloat  s14_ambient_material[] = { 0.0f, 0.1f, 0.06f,1.0f };
GLfloat  s14_difuse_material[] = { 0.0f, 0.50980392f, 0.50980392f,1.0f };
GLfloat  s14_specular_material[] = { 0.50196078f, 0.50196078f, 0.50196078f,1.0f };
GLfloat  s14_shininess[] = { 0.25f * 128.0f };

GLfloat  s15_ambient_material[] = { 0.0f, 0.0f, 0.0f,1.0f };
GLfloat  s15_difuse_material[] = { 0.1f, 0.35f, 0.1f,1.0f };
GLfloat  s15_specular_material[] = { 0.45f, 0.55f, 0.45f,1.0f };
GLfloat  s15_shininess[] = { 0.25f * 128.0f };

GLfloat  s16_ambient_material[] = { 0.0f, 0.0f, 0.0f,1.0f };
GLfloat  s16_difuse_material[] = { 0.5f, 0.0f, 0.0f,1.0f };
GLfloat  s16_specular_material[] = { 0.7f, 0.6f, 0.6f,1.0f };
GLfloat  s16_shininess[] = { 0.25f * 128.0f };

GLfloat  s17_ambient_material[] = { 0.0f, 0.0f, 0.0f,1.0f };
GLfloat  s17_difuse_material[] = { 0.55f, 0.55f, 0.55f,1.0f };
GLfloat  s17_specular_material[] = { 0.70f, 0.70f, 0.70f,1.0f };
GLfloat  s17_shininess[] = { 0.25f * 128.0f };

GLfloat  s18_ambient_material[] = { 0.0f, 0.0f, 0.0f,1.0f };
GLfloat  s18_difuse_material[] = { 0.5f, 0.5f, 0.0f,1.0f };
GLfloat  s18_specular_material[] = { 0.60f, 0.60f, 0.50f,1.0f };
GLfloat  s18_shininess[] = { 0.25f * 128.0f };

GLfloat  s19_ambient_material[] = { 0.02f, 0.02f, 0.02f,1.0f };
GLfloat  s19_difuse_material[] = { 0.01f, 0.01f, 0.01f,1.0f };
GLfloat  s19_specular_material[] = { 0.4f, 0.4f, 0.4f,1.0f };
GLfloat  s19_shininess[] = { 0.078125f * 128.0f };

GLfloat  s20_ambient_material[] = { 0.0f, 0.05f, 0.05f,1.0f };
GLfloat  s20_difuse_material[] = { 0.4f, 0.5f, 0.5f,1.0f };
GLfloat  s20_specular_material[] = { 0.04f, 0.7f, 0.7f,1.0f };
GLfloat  s20_shininess[] = { 0.078125f * 128.0f };

GLfloat  s21_ambient_material[] = { 0.0f, 0.05f, 0.0f,1.0f };
GLfloat  s21_difuse_material[] = { 0.4f, 0.5f, 0.4f,1.0f };
GLfloat  s21_specular_material[] = { 0.04f, 0.7f, 0.04f,1.0f };
GLfloat  s21_shininess[] = { 0.078125f  * 128.0f };

GLfloat  s22_ambient_material[] = { 0.05f, 0.0f, 0.0f,1.0f };
GLfloat  s22_difuse_material[] = { 0.5f, 0.4f, 0.4f,1.0f };
GLfloat  s22_specular_material[] = { 0.7f, 0.04f, 0.04f,1.0f };
GLfloat  s22_shininess[] = { 0.078125f * 128.0f };

GLfloat  s23_ambient_material[] = { 0.05f, 0.05f, 0.05f,1.0f };
GLfloat  s23_difuse_material[] = { 0.5f, 0.5f, 0.5f,1.0f };
GLfloat  s23_specular_material[] = { 0.7f, 0.7f, 0.7f,1.0f };
GLfloat  s23_shininess[] = { 0.078125f * 128.0f };

GLfloat  s24_ambient_material[] = { 0.05f, 0.05f, 0.0f,1.0f };
GLfloat  s24_difuse_material[] = { 0.5f, 0.5f, 0.4f,1.0f };
GLfloat  s24_specular_material[] = { 0.7f, 0.7f, 0.04f,1.0f };
GLfloat  s24_shininess[] = { 0.078125f  * 128.0f };

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
				//update();
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
		case 0X58://X
			angle = 0.0f;
			isXRotation = 1.0f;
			isYRotation = 0.0f;
			isZRotation = 0.0f;
			break;
		case 0X59://Y
			angle = 0.0f;
			isXRotation = 0.0f;
			isYRotation = 1.0f;
			isZRotation = 0.0f;
			break;
		case 0X5A://Z	
			angle = 0.0f;
			isXRotation = 0.0f;
			isYRotation = 0.0f;
			isZRotation = 1.0f;
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

	
	
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, light_model_local_viewer);

	glEnable(GL_LIGHT0);

	quadric = gluNewQuadric();

	glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_white_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_white_defuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_white_specular);

	
	glPushMatrix();
	gluLookAt(0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
			glRotatef(angle, isXRotation, isYRotation, isZRotation);
			
			if (isXRotation == 1.0f)
			{
				light_white_position[0] = angle;
				
			}
			else if (isYRotation==1.0f)
			{
				light_white_position[1] = angle;
				
			}
			else if (isZRotation=1.0f)
			{
				
				light_white_position[2] = angle;
			}
			glLightfv(GL_LIGHT0, GL_POSITION, light_white_position);
		glPopMatrix();
	glPopMatrix();
	//*********************************************** Line 1 ****************************************
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_SPECULAR, s1_specular_material);
		glMaterialfv(GL_FRONT, GL_SHININESS, s1_shininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT, s1_ambient_material);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, s1_difuse_material);

		glTranslatef(-10.0f, 5.0f, -18.0f);
		////glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();
	
	glPushMatrix();

		glMaterialfv(GL_FRONT, GL_SPECULAR, s2_specular_material);
		glMaterialfv(GL_FRONT, GL_SHININESS, s2_shininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT, s2_ambient_material);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, s2_difuse_material);

		glTranslatef(-5.0f, 5.0f, -18.0f);
		//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

		glMaterialfv(GL_FRONT, GL_SPECULAR, s3_specular_material);
		glMaterialfv(GL_FRONT, GL_SHININESS, s3_shininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT, s3_ambient_material);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, s3_difuse_material);

		glTranslatef(0.0f, 5.0f, -18.0f);
		//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

		glMaterialfv(GL_FRONT, GL_SPECULAR, s4_specular_material);
		glMaterialfv(GL_FRONT, GL_SHININESS, s4_shininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT, s4_ambient_material);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, s4_difuse_material);

		glTranslatef(5.0f, 5.0f, -18.0f);
		//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	//*********************************************** Line 2 ****************************************
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, s5_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s5_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s5_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s5_difuse_material);

	glTranslatef(-10.0f, 3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s6_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s6_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s6_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s6_difuse_material);

	glTranslatef(-5.0f, 3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s7_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s7_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s7_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s7_difuse_material);

	glTranslatef(0.0f, 3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s8_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s8_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s8_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s8_difuse_material);

	glTranslatef(5.0f, 3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();



	//*********************************************** Line 3 ****************************************
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, s9_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s9_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s9_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s9_difuse_material);

	glTranslatef(-10.0f, 1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s10_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s10_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s10_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s10_difuse_material);

	glTranslatef(-5.0f, 1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s11_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s11_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s11_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s11_difuse_material);

	glTranslatef(0.0f, 1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s12_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s12_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s12_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s12_difuse_material);

	glTranslatef(5.0f, 1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	//*********************************************** Line 4 ****************************************
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, s13_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s13_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s13_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s13_difuse_material);

	glTranslatef(-10.0f, -1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s14_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s14_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s14_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s14_difuse_material);

	glTranslatef(-5.0f, -1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s15_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s15_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s15_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s15_difuse_material);

	glTranslatef(0.0f, -1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s16_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s16_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s16_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s16_difuse_material);

	glTranslatef(5.0f, -1.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	//*********************************************** Line 5 ****************************************
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, s17_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s17_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s17_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s17_difuse_material);

	glTranslatef(-10.0f, -3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s18_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s18_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s18_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s18_difuse_material);

	glTranslatef(-5.0f, -3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s19_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s19_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s19_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s19_difuse_material);

	glTranslatef(0.0f, -3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s20_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s20_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s20_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s20_difuse_material);

	glTranslatef(5.0f, -3.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	//*********************************************** Line 6 ****************************************
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, s21_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s21_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s21_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s21_difuse_material);

	glTranslatef(-10.0f, -5.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s22_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s22_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s22_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s22_difuse_material);

	glTranslatef(-5.0f, -5.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s23_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s23_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s23_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s23_difuse_material);

	glTranslatef(0.0f, -5.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, s24_specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, s24_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, s24_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, s24_difuse_material);

	glTranslatef(5.0f, -5.0f, -18.0f);
	//glRotatef(angle_Pyramid, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.75, 30, 30);
	glPopMatrix();


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
	angle = angle + 0.5f;
	if (angle >= 360.0f)
		angle = 0.0f;
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

