/**
 * @author Kevin Bohinski <bohinsk1@tcnj.edu>
 * @version 1.0
 * @since 2015-12-9
 *
 * Course:        TCNJ - CSC 350 - Computer Graphics
 * Instructor:    Dr. Salgian
 * Project Name:  Extra Credit
 * Description:   For an extra 10 points on the final exam,
 *                write an OpenGL program that animates the
 *                Cohen-Sutherland algorithm. Draw a fixed
 *                clipping rectangle R, but allow the user
 *                to specify the endpoints of an arbitrary segment.
 *                Subsequently, highlight its subsegments as
 *                they are recursively processed.
 *
 * Filename:      Clipping.cpp
 * Description:   OpenGL program to draw shapes on a canvas.
 * Last Modified: 2015-12-9
 *
 * Modified from Sumanta Guha.
 */

#include <cstdlib>
#include <cmath>
#include <vector>
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

/* Width and height of window */
static int width, height;
int time = 0;

/* Coords of rect */
float smaller = (250 - 100);
float larger = (250 + 100);

/* Outcodes */
const int in = 0;
const int l = 1;
const int r = 2;
const int b = 4;
const int t = 8;

/* Coords of click */
static int tempX, tempY;
int pointCount = 0;

/* Delay value used for glutTimerFunc(fps, animationFunction, 1); */
int fps = 20;

/* Use the STL extension of C++. */
using namespace std;

/*
 * Prints instructions to window.
 */
void printInteraction(void) {
	cout << "Kevin Bohinski" << endl;
	cout << "2015-12-9" << endl;
	cout << "TCNJ - CSC 350 - Graphics" << endl;
	cout << "Dr. Salgian" << endl;
	cout << "" << endl;
	cout << "Extra Credit - Clipping.cpp" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "Interaction:" << endl;
	cout << "Click Twice: Draws line" << endl;
	cout << "Esc        : Exit" << endl;
}

/*
 * Line Class
 */
class Line {
public:
	/* Constructor */
	Line(int x1Val, int y1Val, int x2Val, int y2Val, float rVal, float gVal, float bVal) {
		x1 = x1Val;
		y1 = y1Val;
		x2 = x2Val;
		y2 = y2Val;
		r = rVal;
		g = gVal;
		b = bVal;
	}
	/* Methods */
	void drawLine();
private:
	/* Private Variables */
	int x1;
	int y1;
	int x2;
	int y2;
	float r;
	float g;
	float b;
};

/* Draws a line */
void Line::drawLine() {
	glColor3f(r, g, b);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

vector<Line> lines;
vector<Line>::iterator linesIterator;
vector<Line> lines1;
vector<Line>::iterator linesIterator1;

/* Method to draw all lines */
void drawLines(void) {
	glLineWidth(1.0);
	linesIterator1 = lines1.begin();
	while (linesIterator1 != lines1.end()) {
		linesIterator1->drawLine();
		linesIterator1++;
	}
	glLineWidth(1.5);
	linesIterator = lines.begin();
	while (linesIterator != lines.end()) {
		linesIterator->drawLine();
		linesIterator++;
	}
}

int getOutCode(double x, double y) {
	int toReturn = in;

	if (x < smaller) {
		toReturn |= l;
	}
	else if (x > larger) {
		toReturn |= r;
	}
	if (y < smaller) {
		toReturn |= b;
	}
	else if (y > larger) {
		toReturn |= t;
	}

	return toReturn;
}

void csClip(double x0, double y0, double x1, double y1) {
	int oc0 = getOutCode(x0, y0);
	int oc1 = getOutCode(x1, y1);
	bool clipped = false;

	while (true) {
		/* Bitwise or */
		if (!(oc0 | oc1)) {
			clipped = true;
			break;
		}
		/* Bitwise and */
		else if (oc0 & oc1) {
			break;
		}
		/* Needs Clipping */
		else {
			double x;
			double y;
			int out = 0;

			if (oc0) {
				out = oc0;
			}
			else {
				out = oc1;
			}

			/* Clip via slope calcuations */
			if (out & t) {
				/* Top Clipper */
				x = x0 + (x1 - x0) * (larger - y0) / (y1 - y0);
				y = larger;
			}
			else if (out & b) {
				/* Bottom Clipper */
				x = x0 + (x1 - x0) * (smaller - y0) / (y1 - y0);
				y = smaller;
			}
			else if (out & r) {
				/* Right Clipper */
				y = y0 + (y1 - y0) * (larger - x0) / (x1 - x0);
				x = larger;
			}
			else if (out & l) {
				/* Left Clipper */
				y = y0 + (y1 - y0) * (smaller - x0) / (x1 - x0);
				x = smaller;
			}

			/* Save clipped values as real values */
			if (out == oc0) {
				x0 = x;
				y0 = y;
				oc0 = getOutCode(x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				oc1 = getOutCode(x1, y1);
			}
		}
	}
	if (clipped) {
		lines.push_back(Line(x0, y0, x1, y1, 0, 0, 0));
	}
}

/*
 * Animation function
 */
void animationFunction(int unused) {
	if (time >= 80000) {
		time = 0;
		lines1.clear();
	}
	time++;
	glutPostRedisplay();
	glutTimerFunc(fps, animationFunction, 1);
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glRectf(smaller, smaller, larger, larger);
	drawLines();

	glutSwapBuffers();
	animationFunction(1);
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Set viewing box dimensions equal to window dimensions. */
	glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

	/* Pass the size of the OpenGL window to globals. */
	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseControl(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		/* Correct from mouse to OpenGL co-ordinates. */
		y = height - y;

		/* Click outside canvas - do nothing. */
		if (x < 0 || x > width || y < 0 || y > height);

		/* Click in canvas. */
		if (pointCount == 0) {
			tempX = x; tempY = y;
			pointCount++;
		}
		else {
			lines1.push_back(Line(tempX, tempY, x, y, 1, 0, 0));
			csClip(tempX, tempY, x, y);
			pointCount = 0;
		}
	}
	glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Clipping.cpp");
	printInteraction();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}