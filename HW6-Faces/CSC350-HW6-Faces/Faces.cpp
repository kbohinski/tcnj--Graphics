/**
 * @author Kevin Bohinski <bohinsk1@tcnj.edu>
 * @version 1.0
 * @since 2015-11-20
 *
 * Course:        TCNJ - CSC 350 - Computer Graphics
 * Instructor:    Dr. Salgian
 * Project Name:  Homework 6
 * Description:   Homework 6, Exercise 1
 *
 * Filename:      Faces.cpp
 * Description:   Draw a figure from the textbook.
 * Last Modified: 2015-11-20
 *
 * Modified from Sumanta Guha.
 */

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
//#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#define PI 3.14159265

/* Use the STL extension of C++. */
using namespace std;

/* Global Vars */
/* Font */
static long font = (long)GLUT_BITMAP_8_BY_13;
/* Angles to rotate the object. */
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
/* Wireframe or filled? */
bool isWire = true;
/* Number of divisions for the disk*/
int N = 36.0;
/* Color Vars */
float darkR = 131.0 / 255.0;
float darkG = 205.0 / 255.0;
float darkB = 208.0 / 255.0;
float lightR = 219.0 / 255.0;
float lightG = 240.0 / 255.0;
float lightB = 241.0 / 255.0;
/* Vectors for outer 6 points */
vector<float> pointsX;
vector<float> pointsY;
/* Vectors for all inner points */
vector<float> innersX;
vector<float> innersY;

/*
 * Prints instructions to window.
 */
void printInteraction(void) {
	cout << "Kevin Bohinski" << endl;
	cout << "2015-11-20" << endl;
	cout << "TCNJ - CSC 350 - Graphics" << endl;
	cout << "Dr. Salgian" << endl;
	cout << "" << endl;
	cout << "Homework 6 - Faces.cpp" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "Interaction:" << endl;
	cout << "z/Z: Rotates about the z axis" << endl;
	cout << "x/X: Rotates about the x axis" << endl;
	cout << "y/Y: Rotates about the y axis" << endl;
	cout << "f  : Toggles wireframe/filled" << endl;
	cout << "Esc: Exit" << endl;
}

/*
 * Routine to draw a bitmap character string.
 */
void writeBitmapString(void *font, char *string) {
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

/*
 * Draw Routine.
 */
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	writeBitmapString((void*)font, "Press 'f' to toggle wireframe.");
	
	/* Pushes into frustum */
	glTranslatef(0.0, 0.0, -10.0);

	/* Commands to turn the disk */
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	/* Vars for the angle calculations */
	float angle;
	int i;

	/* Draws base disk & saves relevant points */
	if (isWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glColor3f(darkR, darkG, darkB);
	bool everyOther = false;
	glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= N; ++i) {
			angle = 2 * PI * i / N;
			if (i % 3 == 0 && pointsX.size() < 6 && everyOther) {
				pointsX.push_back(cos(angle) * 4.0);
				pointsY.push_back(sin(angle) * 4.0);
			}
			everyOther = !everyOther;
			if (i < 37) {
				innersX.push_back(cos(angle) * 2.0);
				innersY.push_back(sin(angle) * 2.0);
			}
			glVertex3f(cos(angle) * 2.0, sin(angle) * 2.0, 0.0);
			glVertex3f(cos(angle) * 4.0, sin(angle) * 4.0, 0.0);
		}
	glEnd();

	/* Draws triangles */
	glColor3f(lightR, lightG, lightB);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(pointsX.at(0), pointsY.at(0), 0.0);
		glVertex3f(innersX.at(0), innersY.at(0), 0.0);
		glVertex3f(innersX.at(1), innersY.at(1), 0.0);
		glVertex3f(innersX.at(2), innersY.at(2), 0.0);
		glVertex3f(innersX.at(3), innersY.at(3), 0.0);
		glVertex3f(innersX.at(4), innersY.at(4), 0.0);
		glVertex3f(innersX.at(5), innersY.at(5), 0.0);
		glVertex3f(innersX.at(6), innersY.at(6), 0.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(pointsX.at(1), pointsY.at(1), 0.0);
		glVertex3f(innersX.at(6), innersY.at(6), 0.0);
		glVertex3f(innersX.at(7), innersY.at(7), 0.0);
		glVertex3f(innersX.at(8), innersY.at(8), 0.0);
		glVertex3f(innersX.at(9), innersY.at(9), 0.0);
		glVertex3f(innersX.at(10), innersY.at(10), 0.0);
		glVertex3f(innersX.at(11), innersY.at(11), 0.0);
		glVertex3f(innersX.at(12), innersY.at(12), 0.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(pointsX.at(2), pointsY.at(2), 0.0);
		glVertex3f(innersX.at(12), innersY.at(12), 0.0);
		glVertex3f(innersX.at(13), innersY.at(13), 0.0);
		glVertex3f(innersX.at(14), innersY.at(14), 0.0);
		glVertex3f(innersX.at(15), innersY.at(15), 0.0);
		glVertex3f(innersX.at(16), innersY.at(16), 0.0);
		glVertex3f(innersX.at(17), innersY.at(17), 0.0);
		glVertex3f(innersX.at(18), innersY.at(18), 0.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(pointsX.at(3), pointsY.at(3), 0.0);
		glVertex3f(innersX.at(18), innersY.at(18), 0.0);
		glVertex3f(innersX.at(19), innersY.at(19), 0.0);
		glVertex3f(innersX.at(20), innersY.at(20), 0.0);
		glVertex3f(innersX.at(21), innersY.at(21), 0.0);
		glVertex3f(innersX.at(22), innersY.at(22), 0.0);
		glVertex3f(innersX.at(23), innersY.at(23), 0.0);
		glVertex3f(innersX.at(24), innersY.at(24), 0.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(pointsX.at(4), pointsY.at(4), 0.0);
		glVertex3f(innersX.at(24), innersY.at(24), 0.0);
		glVertex3f(innersX.at(25), innersY.at(25), 0.0);
		glVertex3f(innersX.at(26), innersY.at(26), 0.0);
		glVertex3f(innersX.at(27), innersY.at(27), 0.0);
		glVertex3f(innersX.at(28), innersY.at(28), 0.0);
		glVertex3f(innersX.at(29), innersY.at(29), 0.0);
		glVertex3f(innersX.at(30), innersY.at(30), 0.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(pointsX.at(5), pointsY.at(5), 0.0);
		glVertex3f(innersX.at(30), innersY.at(30), 0.0);
		glVertex3f(innersX.at(31), innersY.at(31), 0.0);
		glVertex3f(innersX.at(32), innersY.at(32), 0.0);
		glVertex3f(innersX.at(33), innersY.at(33), 0.0);
		glVertex3f(innersX.at(34), innersY.at(34), 0.0);
		glVertex3f(innersX.at(35), innersY.at(35), 0.0);
		glVertex3f(innersX.at(36), innersY.at(36), 0.0);
	glEnd();

	glFlush();
}

/*
 * Init routine
 */
void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

/*
 * OpenGL resize routine
 */
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
}

/*
 * Keyboard input processing routine
 */
void keyInput(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case 'f':
		isWire = !isWire;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

/*
 * Main routine
 */
int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Faces.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	printInteraction();

	setup();

	glutMainLoop();
}