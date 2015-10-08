/*
	Kevin Bohinski
	8/30/15

	CSC 350 - Assignment 1
	Glew/Glut Name Test
	Driver.cpp

	OpenGL program to draw a square.
	Modified from Sumanta Guha.
*/

/*
	Resources for setting up with Windows 8.1 (64-bit) & Visual Studio 2015:
	http://www.sumantaguha.com/files/materials/installGuideWindows.pdf
	https://www.youtube.com/watch?v=8p76pJsUP44
	http://www.cs.uregina.ca/Links/class-info/315/WWW/Lab1/GLUT/windows.html

	Path for GL/* in Windows 8.1 (64-bit):
	C:\Program Files (x86)\Windows Kits\8.1\Include\um\gl
*/

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

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string) {
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Drawing routine.
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(25.0, 50.0, 0.0);

	writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (char *) "        Kevin Bohinski");

	// Draw a polygon with specified vertices.
	glBegin(GL_LINE_LOOP);
	glVertex3f(20.0, 20.0, 0.0);
	glVertex3f(80.0, 20.0, 0.0);
	glVertex3f(80.0, 80.0, 0.0);
	glVertex3f(20.0, 80.0, 0.0);
	glEnd();

	glFlush();
}

// Initialization routine.
void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("square.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}