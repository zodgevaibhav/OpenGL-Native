#include<GL/freeglut.h>

bool gbFullScreen = false;
static int shoulder = 0;
static int elbow = 0;

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
	glutCreateWindow("Assignment40A-RoboticArmFreeGLUT");
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
	case 'E': // 'E'
		elbow = (elbow + 3) % 360;
		break;
	case 'e': // 'e'
		elbow = (elbow - 3) % 360;
		break;
	case 'S': // 'S'
		shoulder = (shoulder + 3) % 360;
		break;
	case 's': // 's'
		shoulder = (shoulder - 3) % 360;
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
	
	////glTranslatef(0.0f, 0.0f, -6.0f);
	//glLineWidth(3);
	//glBegin(GL_LINES);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(-1.0f, 0.0f, 0.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);
	//glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// view transformation
	glTranslatef(0.0f, 0.0f, -12.0f);

	// model transformations
	glPushMatrix();

	glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(2.0f, 0.5f, 1.0f);
	quadric = gluNewQuadric();
	glColor3f(0.5f, 0.35f, 0.05f);
	gluSphere(quadric, 0.5, 10, 10);
	glPopMatrix();

	glTranslatef(1.0f, 0.0f, 0.0f);
	glRotatef((GLfloat)elbow, 0.0f, 0.0f, 1.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(2.0f, 0.5f, 1.0f);
	quadric = gluNewQuadric();
	glColor3f(0.5f, 0.35f, 0.05f);
	gluSphere(quadric, 0.5, 10, 10);
	glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();
}



