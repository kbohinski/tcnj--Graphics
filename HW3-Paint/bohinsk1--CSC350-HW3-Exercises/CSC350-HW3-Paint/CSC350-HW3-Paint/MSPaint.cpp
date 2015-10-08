/**
 * @author Kevin Bohinski <bohinsk1@tcnj.edu>
 * @version 1.0
 * @since 2015-9-30
 *
 * Course:        TCNJ - CSC 350 - Computer Graphics
 * Instructor:    Dr. Salgian
 * Project Name:  Homework 3
 * Description:   Homework 3, Exercise 1
 *
 * Filename:      MSPaint.cpp
 * Description:   OpenGL program to draw shapes on a canvas.
 * Last Modified: 2015-9-30
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

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define POLYLINE 4
#define CIRCLE 5
#define NUMBERPRIMITIVES 5
#define PI 3.14159265358979324

/* Use the STL extension of C++. */
using namespace std;

/* Setting Global Variables */

/* Color and Filled Variables */
static bool CHARTREUSE = false;
static float chartreuseR = (223.0 / 255.0);
static float chartreuseG = (255.0 / 255.0);
static float chartreuseB = (0.0 / 255.0);
static bool MUD = false;
static float mudR = (97.0 / 255.0);
static float mudG = (80.0 / 255.0);
static float mudB = (7.0 / 255.0);
static bool DEFAULT = true;
static bool BRIMMING = false;

/* Width and height of window */
static int width, height;

/* Size of a point */
static float pointSize = 3.0; 

/* What is currently drawing */
static int primitive = INACTIVE;

/* Number of points clicked so far */
static int pointCount = 0;

/* Coords of click */
static int tempX, tempY;

/* Does the user want a grid? */
static int isGrid = 1;

/* ============================================= BEGIN CLASSES ============================================= */

	/* ============================== BEGIN POINT CLASS & METHODS ============================== */
	class Point {
	public:
		/* Constructor */
		Point(int xVal, int yVal, float rVal, float gVal, float bVal) {
			x = xVal;
			y = yVal;
			r = rVal;
			g = gVal;
			b = bVal;
		}
		/* Methods */
		void drawPoint(void);
		int getX();
		int getY();
		void setX(int xVal);
		void setY(int yVal);
	private:
		/* Private Variables */
		int x, y;
		float r, g, b;
		static float size;
	};

	/* Draws a point */
	void Point::drawPoint() {
		glPointSize(size);
		glColor3f(r, g, b);
		glBegin(GL_POINTS);
			glVertex3f(x, y, 0.0);
		glEnd();
	}

	float Point::size = pointSize;

	/**
	 * @return : The X coordinate of the point.
	 */
	int Point::getX() {
		return x;
	}

	/**
	 * @return : The Y coordinate of the point.
	 */
	int Point::getY() {
		return y;
	}

	/**
	 * @param : The X coordinate of the point.
	 */
	void Point::setX(int xVal) {
		x = xVal;
	}

	/**
	 * @param : The Y coordinate of the point.
	 */
	void Point::setY(int yVal) {
		y = yVal;
	}
	/* ============================== END POINT CLASS & METHODS ============================== */

	/* ============================== BEGIN LINE CLASS & METHODS ============================== */
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
	/* ==============================  END LINE CLASS & METHODS  ============================== */

	/* ============================== BEGIN POLYLINE CLASS & METHODS ============================== */
	class PolyLine {
	public:
		/* Constructor */
		PolyLine(vector<Point> pointsVal, float rVal, float gVal, float bVal) {
			points = pointsVal;
			r = rVal;
			g = gVal;
			b = bVal;
		}
		/* Methods */
		void drawPline();
	private:
		/* Private Variables */
		vector<Point> points;
		float r;
		float g;
		float b;
	};

	/* Draws a PolyLine */
	void PolyLine::drawPline() {
		/* Fails when using an iterator here???? Defaulting to for loop. */
		/* Please explain if you see this! */
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			for (int i = 0; i < points.size(); i++) {
				glVertex2f(points[i].getX(), points[i].getY());
			}
		glEnd();
	}
	/* ==============================  END POLYLINE CLASS & METHODS  ============================== */

	/* ============================== BEGIN RECTANGLE CLASS & METHODS ============================== */
	class Rect {
	public:
		/* Constructor */
		Rect(int x1Val, int y1Val, int x2Val, int y2Val, float rVal, float gVal, float bVal, bool brimmedVal) {
			x1 = x1Val;
			y1 = y1Val;
			x2 = x2Val;
			y2 = y2Val;
			r = rVal;
			g = gVal;
			b = bVal;
			brimmed = brimmedVal;
		}
		/* Methods */
		void drawRectangle();
	private:
		/* Private Variables */
		int x1;
		int y1;
		int x2;
		int y2;
		float r;
		float g;
		float b;
		bool brimmed;
	};

	/* Draws a Rectangle */
	void Rect::drawRectangle() {
		glColor3f(r, g, b);
		if (!brimmed) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glRectf(x1, y1, x2, y2);
	}
	/* ==============================  END RECTANGLE CLASS & METHODS  ============================== */

	/* ============================== BEGIN CIRCLE CLASS & METHODS ============================== */
	class Circle {
	public:
		/* Constructor */
		Circle(int cxVal, int cyVal, int x, int y, float rcolorVal, float gVal, float bVal, bool brimmedVal) {
			cx = cxVal;
			cy = cyVal;
			r = sqrt(pow(((x - cx)), 2) + pow(((y - cy)), 2));
			numVertices = r;
			rcolor = rcolorVal;
			g = gVal;
			b = bVal;
			brimmed = brimmedVal;
		}
		/* Methods */
		void drawCircle();
	private:
		/* Private Variables */
		int cx;
		int cy;
		float r;
		float rcolor;
		float g;
		float b;
		bool brimmed;
		int numVertices;
	};

	/* Draws a Circle */
	void Circle::drawCircle() {
		glColor3f(rcolor, g, b);

		if (!brimmed) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_LINE_LOOP);
			float t = 0;
			for (int i = 0; i < numVertices; i++) {
				glVertex3f(cx + r * cos(t), cy + r * sin(t), 0.0);
				t += 2 * PI / numVertices;
			}
			glEnd();
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_TRIANGLE_FAN);
			float tmpX;
			float tmpY;
			glVertex3f(cx, cy, 0.0);
			float t = 0;
			for (int i = 0; i < numVertices; i++) {
				if (i == 0) {
					tmpX = cx + r * cos(t);
					tmpY = cy + r * sin(t);
				}
				glVertex3f(cx + r * cos(t), cy + r * sin(t), 0.0);
				t += 2 * PI / numVertices;
			}
			glVertex3f(tmpX, tmpY, 0.0);
			glEnd();
		}
	}
	/* ==============================  END CIRCLE CLASS & METHODS  ============================== */

/* =============================================  END CLASSES  ============================================= */
/* ============================================= BEGIN DRAWLERS ============================================= */

	/* Vectors to hold all things already drawn */
	vector<Point> points;
	vector<Point> tmpPoints;
	vector<Line> lines;
	vector<PolyLine> plines;
	vector<Rect> rectangles;
	vector<Circle> circles;

	/* Temp variables for the center of a circle */
	Point circleTmp(0, 0, 0.0, 0.0, 0.0);

	/* Iterators for vectors */
	vector<Point>::iterator pointsIterator;
	vector<Line>::iterator linesIterator;
	vector<PolyLine>::iterator plinesIterator;
	vector<Rect>::iterator rectanglesIterator;
	vector<Circle>::iterator circleIterator;

	/* Method to draw all points already drawn */
	void drawPoints(void) {
		pointsIterator = points.begin();
		while (pointsIterator != points.end()) {
			pointsIterator->drawPoint();
			pointsIterator++;
		}
	}

	/* Method to draw all lines already drawn */
	void drawLines(void) {
		linesIterator = lines.begin();
		while (linesIterator != lines.end()) {
			linesIterator->drawLine();
			linesIterator++;
		}
	}

	/* Method to draw all PolyLines already drawn */
	void drawPlines(void) {
		plinesIterator = plines.begin();
		while (plinesIterator != plines.end()) {
			plinesIterator->drawPline();
			plinesIterator++;
		}
	}

	/* Method to draw all rectangles already drawn */
	void drawRectangles(void) {
		rectanglesIterator = rectangles.begin();
		while (rectanglesIterator != rectangles.end()) {
			rectanglesIterator->drawRectangle();
			rectanglesIterator++;
		}
	}

	/* Method to draw all circles already drawn */
	void drawCircles(void) {
		circleIterator = circles.begin();
		while (circleIterator != circles.end()) {
			circleIterator->drawCircle();
			circleIterator++;
		}
	}

/* =============================================  END DRAWLERS  ============================================= */
/* ============================================= BEGIN DEPENDANT DRAWLERS ============================================= */

	/* Method to draw point selection box */
	void drawPointSelectionBox(void) {
		/* If selected */
		if (primitive == POINT) glColor3f(1.0, 1.0, 1.0);
		else glColor3f(0.8, 0.8, 0.8);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glRectf(0.0, 0.9*height, 0.1*width, height);

		/* Draw Bounds */
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(0.0, 0.9*height, 0.1*width, height);

		/* Draw Icon */
		glPointSize(pointSize);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex3f(0.05*width, 0.95*height, 0.0);
		glEnd();
	}

	/* Method to draw line selection box */
	void drawLineSelectionBox(void) {
		/* If selected */
		if (primitive == LINE) glColor3f(1.0, 1.0, 1.0);
		else glColor3f(0.8, 0.8, 0.8);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

		/* Draw Bounds */
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

		/* Draw Icon */
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f(0.025*width, 0.825*height, 0.0);
			glVertex3f(0.075*width, 0.875*height, 0.0);
		glEnd();
	}

	/* Method to draw rectangle selection box */
	void drawRectangleSelectionBox(void) {
		/* If selected */
		if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0);
		else glColor3f(0.8, 0.8, 0.8);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

		/* Draw Bounds */
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

		/* Draw Icon */
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
		glEnd();
	}

	/* Method to draw PolyLine selection box */
	void drawPolyLineSelectionBox(void) {
		/* If selected */
		if (primitive == POLYLINE) glColor3f(1.0, 1.0, 1.0);
		else glColor3f(0.8, 0.8, 0.8);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

		/* Draw Bounds */
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

		/* Draw Icon */
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.025*width, 0.625*height, 0.0);
			glVertex3f(0.0375*width, 0.675*height, 0.0);
			glVertex3f(0.0625*width, 0.625*height, 0.0);
			glVertex3f(0.075*width, 0.675*height, 0.0);
		glEnd();
	}

	/* Method to draw circle selection box */
	void drawCircleSelectionBox(void) {
		/* If selected */
		if (primitive == CIRCLE) glColor3f(1.0, 1.0, 1.0);
		else glColor3f(0.8, 0.8, 0.8);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

		/* Draw Bounds */
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

		/* Draw Icon */
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		float t = 0;
		int numVertices = 8;
		float r = .025;
		glBegin(GL_LINE_LOOP);
			for (int i = 0; i < numVertices; i++) {
				glVertex3f((.05 * width + r * width * cos(t)), (.55 * height + r * height * sin(t)), 0.0);
				t += 2 * PI / numVertices;
			}
		glEnd();
	}

	/* Method to draw unused area of selection bar */
	void drawInactiveArea(void) {
		glColor3f(0.6, 0.6, 0.6);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
	}

	/* Method to draw a temp point */
	void drawTempPoint(void) {
		glColor3f(1.0, 0.0, 0.0);
		glPointSize(pointSize);
		glBegin(GL_POINTS);
			glVertex3f(tempX, tempY, 0.0);
		glEnd();
	}

	/* Method to draw the grid */
	void drawGrid(void) {
		int i;

		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x5555);
		glColor3f(0.75, 0.75, 0.75);

		glBegin(GL_LINES);
		for (i = 2; i <= 9; i++) {
			glVertex3f(i*0.1*width, 0.0, 0.0);
			glVertex3f(i*0.1*width, height, 0.0);
		}
		for (i = 1; i <= 9; i++) {
			glVertex3f(0.1*width, i*0.1*height, 0.0);
			glVertex3f(width, i*0.1*height, 0.0);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

/* =============================================  END DEPENDANT DRAWLERS  ============================================= */
/* ============================================= BEGIN TRADITIONAL ROUTINES ============================================= */

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawPoints();
	drawLines();
	drawRectangles();
	drawCircles();
	drawPlines();
	if (((primitive == LINE) || (primitive == RECTANGLE)) &&
		(pointCount == 1)) drawTempPoint();
	if (primitive == POLYLINE) {
		vector<Point>::iterator tmpPointsIterator;

		tmpPointsIterator = tmpPoints.begin();
		while (tmpPointsIterator != tmpPoints.end()) {
			tmpPointsIterator->drawPoint();
			tmpPointsIterator++;
		}
	}
	if (primitive == CIRCLE && pointCount != 0) {
		circleTmp.drawPoint();
	}
	if (isGrid) drawGrid();

	drawPointSelectionBox();
	drawLineSelectionBox();
	drawRectangleSelectionBox();
	drawPolyLineSelectionBox();
	drawCircleSelectionBox();
	drawInactiveArea();

	glutSwapBuffers();
}

/* Function to pick the tool to draw with */
void pickPrimitive(int y) {
	if (y < (1 - NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
	else if (y < (1 - 4 * 0.1)*height) primitive = CIRCLE;
	else if (y < (1 - 3 * 0.1)*height) primitive = POLYLINE;
	else if (y < (1 - 2 * 0.1)*height) primitive = RECTANGLE;
	else if (y < (1 - 1 * 0.1)*height) primitive = LINE;
	else primitive = POINT;
}

void mouseControl(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		/* Correct from mouse to OpenGL co-ordinates. */
		y = height - y;

		/* Click outside canvas - do nothing. */
		if (x < 0 || x > width || y < 0 || y > height);

		/* Click in left selection area. */
		else if (x < 0.1*width) {
			pickPrimitive(y);
			pointCount = 0;
		}

		/* Click in canvas. */
		else {
			if (primitive == POINT) {
				if (DEFAULT) {
					points.push_back(Point(x, y, 0.0, 0.0, 0.0));
				}
				else if (CHARTREUSE) {
					points.push_back(Point(x, y, chartreuseR, chartreuseG, chartreuseB));
				}
				else if (MUD) {
					points.push_back(Point(x, y, mudR, mudG, mudB));
				}
			}
			else if (primitive == LINE) {
				if (pointCount == 0) {
					tempX = x; tempY = y;
					pointCount++;
				}
				else {
					if (DEFAULT) {
						lines.push_back(Line(tempX, tempY, x, y, 0.0, 0.0, 0.0));
					}
					else if (CHARTREUSE) {
						lines.push_back(Line(tempX, tempY, x, y, chartreuseR, chartreuseG, chartreuseB));
					}
					else if (MUD) {
						lines.push_back(Line(tempX, tempY, x, y, mudR, mudG, mudB));
					}
					pointCount = 0;
				}
			}
			else if (primitive == RECTANGLE) {
				if (pointCount == 0) {
					tempX = x; tempY = y;
					pointCount++;
				}
				else {
					if (DEFAULT) {
						rectangles.push_back(Rect(tempX, tempY, x, y, 0.0, 0.0, 0.0, BRIMMING));
					}
					else if (CHARTREUSE) {
						rectangles.push_back(Rect(tempX, tempY, x, y, chartreuseR, chartreuseG, chartreuseB, BRIMMING));
					}
					else if (MUD) {
						rectangles.push_back(Rect(tempX, tempY, x, y, mudR, mudG, mudB, BRIMMING));
					}
					pointCount = 0;
				}
			}
			else if (primitive == POLYLINE) {
				tmpPoints.push_back(Point(x, y, 0.0, 0.0, 0.0));
			}
			else if (primitive == CIRCLE) {
				if (pointCount == 0) {
					circleTmp.setX(x);
					circleTmp.setY(y);
					pointCount++;
				}
				else if (pointCount == 1) {
					if (DEFAULT) {
						circles.push_back(Circle(circleTmp.getX(), circleTmp.getY(), x, y, 0.0, 0.0, 0.0, BRIMMING));
					}
					else if (CHARTREUSE) {
						circles.push_back(Circle(circleTmp.getX(), circleTmp.getY(), x, y, chartreuseR, chartreuseG, chartreuseB, BRIMMING));
					}
					else if (MUD) {
						circles.push_back(Circle(circleTmp.getX(), circleTmp.getY(), x, y, mudR, mudG, mudB, BRIMMING));
					}
					pointCount = 0;
				}
			}
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && primitive == POLYLINE) {
		if (DEFAULT) {
			plines.push_back(PolyLine(tmpPoints, 0.0, 0.0, 0.0));
		}
		else if (CHARTREUSE) {
			plines.push_back(PolyLine(tmpPoints, chartreuseR, chartreuseG, chartreuseB));
		}
		else if (MUD) {
			plines.push_back(PolyLine(tmpPoints, mudR, mudG, mudB));
		}
		tmpPoints.clear();
	}
	glutPostRedisplay();
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

void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void clearAll(void) {
	points.clear();
	lines.clear();
	rectangles.clear();
	plines.clear();
	circles.clear();
	primitive = INACTIVE;
	pointCount = 0;
}

void rightMenu(int id) {
	if (id == 1) {
		clearAll();
		glutPostRedisplay();
	}
	if (id == 2) exit(0);
}

void grid_menu(int id) {
	if (id == 3) isGrid = 1;
	if (id == 4) isGrid = 0;
	glutPostRedisplay();
}

void color_menu(int id) {
	if (id == 5) {
		CHARTREUSE = false;
		MUD = false;
		DEFAULT = true;
	}
	if (id == 6) {
		CHARTREUSE = true;
		MUD = false;
		DEFAULT = false;
	}
	if (id == 7) {
		MUD = true;
		CHARTREUSE = false;
		DEFAULT = false;
	}
	glutPostRedisplay();
}

void fill_menu(int id) {
	if (id == 8) {
		BRIMMING = true;
	}
	else if (id == 9) {
		BRIMMING = false;
	}
	glutPostRedisplay();
}

void makeMenu(void) {
	int sub_menu;
	sub_menu = glutCreateMenu(grid_menu);
	glutAddMenuEntry("On", 3);
	glutAddMenuEntry("Off", 4);

	int colorChoice;
	colorChoice = glutCreateMenu(color_menu);
	glutAddMenuEntry("Default", 5);
	glutAddMenuEntry("Chartreuse", 6);
	glutAddMenuEntry("Mud", 7);

	int fillChoice;
	fillChoice = glutCreateMenu(fill_menu);
	glutAddMenuEntry("Filled", 8);
	glutAddMenuEntry("Empty", 9);

	glutCreateMenu(rightMenu);
	glutAddSubMenu("Grid", sub_menu);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 2);
	glutAddSubMenu("Colors", colorChoice);
	glutAddSubMenu("Fill", fillChoice);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	makeMenu();
}

void printInteraction(void) {
	cout << "Interaction:" << endl;
	cout << "Left click on a box on the left to select a primitive." << endl
		<< "Then left click on the drawing area: once for point, twice for line or rectangle." << endl
		<< "Right click for menu options." << endl;
}

int main(int argc, char **argv) {
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("MSPaint.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}