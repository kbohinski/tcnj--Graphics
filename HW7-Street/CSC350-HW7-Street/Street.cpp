/**
 * @author Kevin Bohinski <bohinsk1@tcnj.edu>
 * @version 1.0
 * @since 2015-12-1
 *
 * Course:        TCNJ - CSC 350 - Computer Graphics
 * Instructor:    Dr. Salgian
 * Project Name:  Homework 7
 * Description:   Homework 7, Exercise 1
 *
 * Filename:      Street.cpp
 * Description:   Practice lighting in OpenGL.
 * Last Modified: 2015-12-1
 *
 * Modified from Sumanta Guha.
 */

#include <cmath>
#include <cstdlib>
#include <iostream>

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
#define ONE_BY_ROOT_THREE 0.57735

/* Use the STL extension of C++. */
using namespace std;

/* Global Vars */
	/* Streetlight position */
	float streetlightPos[] = { -2.2, 0.8, 3.5, 1.0 };

	/* Car color vars */
	float carR = 0.875;
	float carG = 1.0;
	float carB = 0.0;

	/* Car pos vars */
	float carX = 10.0;
	float carY = -3.0;
	float carZ = -1.0;
	float carXV = -0.0001;

	/* Headlight */
	float headPos[] = { 0.0, 0.0, 0.0, 1.0 };
	float headAng = 50.0;
	float headDir[] = { 1.0, 0.0, 0.0 };
	float headAttenuation = 0.0;
	float headDifSpec[] = { 1.0, 1.0, 1.0, 1.0 };

	/* Light property vector. */
	float ambient[] = { 0.1, 0.1, 0.1, 1.0 };

	/* Material property vectors. */
	float buildProp1[] = { 0.9, 0.0, 0.0, 1.0 };
	float buildProp2[] = { 0.0, 0.9, 0.0, 1.0 };
	float buildSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float buildShine[] = { 25.0 };

	float streetlightProp[] = { 0.9, 0.9, 0.9, 1.0 };
	float streetlightSpec[] = { 0.0, 0.0, 0.0, 1.0 };
	float streetlightShine[] = { 0.1 };

	float roadProp[] = { 0.05, 0.05, 0.05, 1.0 };
	float roadSpec[] = { 0.05, 0.05, 0.05, 1.0 };
	float roadShine[] = { 0.0 };

	float carProp[] = { carR, carG, carB, 1.0 };
	float carSpec[] = { 25.0, 25.0, 25.0, 1.0 };
	float carShine[] = { 25.0 };

	/* Delay value used for glutTimerFunc(fps, animationFunction, 1); */
	int fps = 1000 / 60;

	/* Box vertex co-ordinate vectors. */
	static float vertices[] = {
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, -1.0,
		1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,
		-1.0, 1.0, -1.0,
		-1.0, -1.0, -1.0
	};

	/* Vertex indices of each box side, 6 groups of 4. */
	static unsigned char quadIndices[] = {
		3, 2, 1, 0,
		7, 6, 2, 3,
		4, 5, 6, 7,
		0, 1, 5, 4,
		4, 7, 3, 0,
		6, 5, 1, 2
	};

	/* Box vertex normal vectors = normalized unit vector pointing from origin to vertex. */
	static float normals[] = {
		ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
		ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
		ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
		ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
		-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
		-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
		-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
		-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE
	};

/*
 * Prints instructions to window.
 */
void printInteraction(void) {
	cout << "Kevin Bohinski" << endl;
	cout << "2015-12-1" << endl;
	cout << "TCNJ - CSC 350 - Graphics" << endl;
	cout << "Dr. Salgian" << endl;
	cout << "" << endl;
	cout << "Homework 7 - Street.cpp" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "Interaction:" << endl;
	cout << "Esc  : Exit" << endl;
}

/*
 * Init routine
 */
void setup(void) {
	glClearColor(0.0, 0.0, 0.25, 0.0);
	glEnable(GL_DEPTH_TEST);

	/* Turn on OpenGL lighting. */
	glEnable(GL_LIGHTING);

	/* Moon position */
	float moonPos[] = { 0.0, 200.0, 0.0, 0.0 };

	/* Attenuation factor */
	float streetlightAttenuation = 0.006;

	/* Light property vectors. */
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
	float moonDifSpec[] = { 0.2, 0.2, 0.2, 1.0 };
	float streetlightDifSpec[] = { 0.7, 0.7, 0.5, 1.0 };

	/* Moon properties including position. */
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, moonDifSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, moonDifSpec);
	glLightfv(GL_LIGHT0, GL_POSITION, moonPos);

	/* Streetlight properties including position. */
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, streetlightDifSpec);
	glLightfv(GL_LIGHT1, GL_SPECULAR, streetlightDifSpec);
	glLightfv(GL_LIGHT1, GL_POSITION, streetlightPos);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, streetlightAttenuation);

	/* Headlight */
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, headDifSpec);
	glLightfv(GL_LIGHT2, GL_SPECULAR, headDifSpec);
	glLightfv(GL_LIGHT2, GL_POSITION, headPos);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, headAng);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, headDir);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, headAttenuation);

	/* Turn on the lights! */
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	/* Global ambient light. */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);

	/* Enable two-sided lighting. */
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	/* Enable local viewpoint. */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	/* Enable two vertex arrays: position and normal. */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/* Specify locations for the position and normal arrays. */
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);
}

/*
 * Animation function
 */
void animationFunction(int unused) {
	carX -= carXV;
	if (carX > 10) {
		carX = -10.0;
	}
	glutPostRedisplay();
	glutTimerFunc(fps, animationFunction, 1);
}

/*
 * Draw routine
 */
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/* Position the box for viewing. */
	gluLookAt(0.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	/* Draw short building */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, buildProp1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buildSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, buildShine);
	glPushMatrix();
		glTranslatef(-2.5, -2.0, -3);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, quadIndices);
	glPopMatrix();

	/* Draw cone building */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, buildProp2);
	glPushMatrix();
		glTranslatef(2.2, -1.0, -3);
		glutSolidCone(2.0, 1.0, 10.0, 10.0);
	glPopMatrix();

	/* Draw streetlight */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, streetlightProp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, streetlightSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, streetlightShine);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
		glTranslatef(-streetlightPos[0], streetlightPos[1], streetlightPos[2]);
		glutWireSphere(0.2, 10.0, 10.0);
	glPopMatrix();

	/* Draw road */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, roadProp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roadSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, roadShine);
	glColor3f(0.3, 0.3, 0.3);
	glPushMatrix();
		glTranslatef(0.0, (carY - 1), carZ);
		glScalef(6.0, 0.1, 1.25);
		glutSolidCube(3.0);
	glPopMatrix();

	/* Draw car */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, carProp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, carSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, carShine);
	glColor3f(carR, carG, carB);
	glPushMatrix();
		glTranslatef(carX, carY, carZ);
		glutSolidTeapot(1.0);
	glPopMatrix();

	/* Headlight */
	glPushMatrix();
		glTranslatef((carX - 0.5), carY, carZ);
		glLightfv(GL_LIGHT2, GL_POSITION, headPos);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, headAng);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, headDir);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, headAttenuation);
	glPopMatrix();

	glutSwapBuffers();
	animationFunction(1);
}

/************************************ BEGIN STANDARD FUNCTIONS ************************************/

/*
 * OpenGL resize routine
 */
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
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
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Street.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}