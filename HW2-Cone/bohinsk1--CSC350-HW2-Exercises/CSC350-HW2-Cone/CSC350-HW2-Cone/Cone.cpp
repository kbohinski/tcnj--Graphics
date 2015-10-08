/*
 * @author Kevin Bohinski <bohinsk1@tcnj.edu>
 * @version 1.0
 * @since 2015-9-19
 *
 * Course:        TCNJ - CSC 350 - Computer Graphics
 * Instructor:    Dr. Salgian
 * Project Name:  Homework 2
 * Description:   Homework 2, Exercise 2
 *
 * Filename:      Cone.cpp
 * Description:   OpenGL program to draw a cone without using OpenGL cone primitives.
 * Last Modified: 2015-9-19
 *
 * Modified from Sumanta Guha.
 */

#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#define PI 3.14159265

using namespace std; 

/*
 * Draw routine
 */
void drawScene(void) {
	float radius = 10.0;
	float angle;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0);

	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0, 0.0, -50.0);
		for (angle = -10 * PI; angle <= 10 * PI; angle += PI / 20.0)
			glVertex3f(radius * cos(angle), radius * sin(angle), 0.0);
	glEnd();

	glFlush();
}

/*
 * Init routine
 */
void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);

	/* Show wireframes */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/*
 * OpenGL resize routine
 */
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*                                  vvvv can't be 0.0 */
	glFrustum(-10.0, 10.0, -10.0, 10.0, 10.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, -50.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

/*
 * Keyboard input processing routine
 */
void keyInput(unsigned char key, int x, int y) {
	switch (key) {

	/* Close on esc key */
	case 27:
		exit(0);
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

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cone.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}