#include<GL/freeglut.h>

bool gbFullScreen = false;

static int year = 0;
static int day = 0;

GLUquadric *quadric = NULL;
int main(int argc, char** argv) {
	int iScreenWidth = 0, iScreenHeight = 0;
	//function prototypes
	void display(void);
	void resize(int, int);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void initialize(void);
	void uninitialize(void);
	iScreenWidth = GetSystemMetrics(0);
	iScreenHeight = GetSystemMetrics(1);
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(((iScreenWidth / 2) - 400), ((iScreenHeight / 2) - 300));
	glutCreateWindow("Assignment41A-SolarSystemPushMatrix");
	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return (0);
}

void keyboard(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'D':
		day = (day + 6) % 360;
		break;
	case 'd':
		day = (day - 6) % 360;
		break;
	case 'Y':
		year = (year + 3) % 360;
		break;
	case 'y':
		year = (year - 3) % 360;
		break;
	case 27: //Esc key
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (gbFullScreen == false) {
			glutFullScreen();
			gbFullScreen = true;
		}
		else {
			glutLeaveFullScreen();
			gbFullScreen = false;
		}
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;
	default:
		break;
	}
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION); // select projection matrix
	glLoadIdentity(); // reset projection matrix

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); // calculate the aspect ratio of the view
}
void initialize() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void uninitialize() {
	//code here
}


void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); 

	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();

	glRotatef(90.0f, 1.0f, 0.0f, .0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	quadric = gluNewQuadric();
	glColor3f(1.0f, 1.0f, 0.0f);
	
	gluSphere(quadric, 0.75, 30, 30);

	glPopMatrix();

	glPushMatrix();
		glRotatef((GLfloat)year, 0.0f, 1.0f, 0.0f);
		glTranslatef(1.5f, 0.0f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, .0f);
		glRotatef((GLfloat)day, 0.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		quadric = gluNewQuadric();
		glColor3f(0.4f, 0.9f, 1.0f);
		gluSphere(quadric, 0.2, 20, 20);
	glPopMatrix();

	glutSwapBuffers();
}



