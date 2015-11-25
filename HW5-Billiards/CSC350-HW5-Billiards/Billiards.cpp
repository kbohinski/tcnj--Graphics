/**
 * @author Kevin Bohinski <bohinsk1@tcnj.edu>
 * @version 1.0
 * @since 2015-10-27
 *
 * Course:        TCNJ - CSC 350 - Computer Graphics
 * Instructor:    Dr. Salgian
 * Project Name:  Homework 5
 * Description:   Homework 5, Exercise 1
 *
 * Filename:      Billiards.cpp
 * Description:   Write your first animation in OpenGL.
 * Last Modified: 2015-10-27
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

/* Use the STL extension of C++. */
using namespace std;

/* Global Vars */
	/* Coefficient of kinetic friction */
	float kinetic_mu = 0.0006;
	/*
	 * As we are multiplying by a small val,
	 * the ball's mag will never truly = 0.
	 * This checks and stops when it gets close.
	 */
	float stopperVal = 0.0075;
	/* Did the user click or has friction stopped it yet */
	bool animationActive = false;
	/* Number of loops since animation calcuations started */
	int time = 0;
	/* Delay value used for glutTimerFunc(fps, animationFunction, 1); */
	int fps = 20;
	/* Value the mouse calcuations are divided by to make the power more reasonable. */
	int clickPower = 12;

	/* Width & Height of window */
	int width = 800;
	int height = 400;
	/* Width & Height of frustum */
	int frustumW = 5;
	int frustumH = 5;
	/* Width & Height ratios used for mouse calcuations */
	int ratioW = width / (frustumW * 2);
	int ratioH = height / (frustumH * 2);
	/* Value used to push everything back into the frustum */
	float pushBack = -15.0;
	/* Value used to correct mouse clicks for the near and far planes */
	float planeRatio = -pushBack / 5.0;

	/* Green table size */
	float tblSize = 12;
	/* Variables for walls */
	float lrWallOffset = tblSize + 0.5;
	float tbWallOffset = tblSize + 0.5;
	float lrWallThickness = lrWallOffset - tblSize + 0.12;
	float tbWallThickness = tbWallOffset - tblSize + 0.12;

	/* Variables for ball position */
	float bx = 0.0;
	float by = 0.0;
	/* Variables for ball velocity */
	float bvx = 0.0;
	float bvy = 0.0;
	float bvm = 0.0;
	/* Variables for ball rotation */
	float brx = 0.0;
	float bry = 0.0;
	float btheta = 2.0;
	/* Variable for ball size */
	float bSize = 0.8;

	/* Variables for color */
	float tableR = 0.2;
	float tableG = 0.8;
	float tableB = 0.0;
	float wallR = 0.75;
	float wallG = 0.25;
	float wallB = 0.0;
	float ballR = 1.0;
	float ballG = 1.0;
	float ballB = 1.0;

/*
 * Prints instructions to window.
 */
void printInteraction(void) {
	cout << "Kevin Bohinski" << endl;
	cout << "2015-10-27" << endl;
	cout << "TCNJ - CSC 350 - Graphics" << endl;
	cout << "Dr. Salgian" << endl;
	cout << "" << endl;
	cout << "Homework 5 - Billiards.cpp" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "Interaction:" << endl;
	cout << "Click: Pushes ball" << endl;
	cout << "Esc  : Exit" << endl;
}

/*
 * Draw routine
 */
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	/* Draw Table */
		glPushMatrix();
		glTranslatef(0.0, 0.0, pushBack);
		glColor3f(tableR, tableG, tableB);
		glRectf(-tblSize, -tblSize, tblSize, tblSize);
		glPopMatrix();

		/* Draw Walls */
			glColor3f(wallR, wallG, wallB);

			/* Left Wall */
				glPushMatrix();
				glTranslatef(0.0, 0.0, pushBack);
				glTranslatef(-lrWallOffset, 0.0, 0.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glRectf(-lrWallThickness, -lrWallOffset, lrWallThickness, lrWallOffset);
				glPopMatrix();

			/* Right Wall */
				glPushMatrix();
				glTranslatef(0.0, 0.0, pushBack);
				glTranslatef(lrWallOffset, 0.0, 0.0);
				glRotatef(-90.0, 0.0, 1.0, 0.0);
				glRectf(-lrWallThickness, -lrWallOffset, lrWallThickness, lrWallOffset);
				glPopMatrix();

			/* Top Wall */
				glPushMatrix();
				glTranslatef(0.0, 0.0, pushBack);
				glTranslatef(0.0, tbWallOffset, 0.0);
				glRotatef(-90.0, 1.0, 0.0, 0.0);
				glRectf(-tbWallOffset, -tbWallThickness, tbWallOffset, tbWallThickness);
				glPopMatrix();

			/* Bot Wall */
				glPushMatrix();
				glTranslatef(0.0, 0.0, pushBack);
				glTranslatef(0.0, -tbWallOffset, 0.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				glRectf(-tbWallOffset, -tbWallThickness, tbWallOffset, tbWallThickness);
				glPopMatrix();
		/* End Draw Walls */
	/* End Draw Table */

	/* Draw Ball */
		glTranslatef(bx, by, 0.0);
		glTranslatef(0.0, 0.0, pushBack);
		glRotatef(btheta, bvy, -bvx, 0.0);
		glColor3f(ballR, ballG, ballB);
		glutWireSphere(bSize, 15, 10);
	/* End Draw Ball */

	glutSwapBuffers();
}

/*
 * Animation function
 */
void animationFunction(int unused) {
	/*
	 * As we are multiplying by a small val,
	 * the ball's mag will never truly = 0.
	 * This checks and stops when it gets close.
	 */
	if ((bvm - stopperVal) <= 0) {
		time = 0;
		animationActive = false;
	}
	if (animationActive) {
		/* If ball has done a complete roll, start a new roll */
		btheta += (time * bvm);
		if (btheta > 360.0) {
			btheta -= 360.0;
		}
		/* Friction and velocity calcuations */
		bvx += -(kinetic_mu * time * bvx);
		bvy += -(kinetic_mu * time * bvy);
		/* Calculate ball velocity magnitude for stopping and rotation purposes */
		bvm = sqrt(pow(bvy, 2.0) + pow(bvx, 2.0));
		/* Check for collisions */
		if ((bx + bvx + bSize) >= (tblSize)) {
			bvx *= -1;
		}
		if ((bx + bvx - bSize) <= -(tblSize)) {
			bvx *= -1;
		}
		if ((by + bvy + bSize) >= (tblSize)) {
			bvy *= -1;
		}
		if ((by + bvy - bSize) <= -(tblSize)) {
			bvy *= -1;
		}
		/* After collision correction, make ball move */
		bx += bvx;
		by += bvy;
		/* Add to time as another loop has been run */
		time++;
		/* Draw/animate */
		glutPostRedisplay();
		glutTimerFunc(fps, animationFunction, 1);
	}
}

/*
 * Mouse Callback function
 */
void mouseCallback(int button, int state, int x, int y) {
	/* If click and not animating, start to animate */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !animationActive) {
		animationActive = true;
	}
	/* If click and animating, stop current animations */
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !animationActive) {
		animationActive = false;
		time = 0;
	}
	/* If not click and animating, continue animating */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && animationActive) {
		/* Fix click input to match world */
		y = (height - y);
		float fx = (x - (width / 2)) / ratioW;
		float fy = (y - (height / 2)) / ratioH;
		fx *= planeRatio;
		fy *= planeRatio;
		/* Calcuate velocity and make the velocity reasonable */
		bvx = (bx - fx) / clickPower;
		bvy = (by - fy) / clickPower;
		/* Calculate ball velocity magnitude for stopping and rotation purposes */
		bvm = sqrt(pow(bvy, 2.0) + pow(bvx, 2.0));
		/* Calcuate rotation */
		brx = sin(PI / 2) * fy;
		bry = -sin(PI / 2) * fx;
		/* Animate */
		animationFunction(0);
	}
}

/************************************ BEGIN STANDARD FUNCTIONS ************************************/

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
	glFrustum(-frustumW, frustumH, -frustumW, frustumH, 5.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	width = w;
	height = h;
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

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Billiards.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseCallback);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}