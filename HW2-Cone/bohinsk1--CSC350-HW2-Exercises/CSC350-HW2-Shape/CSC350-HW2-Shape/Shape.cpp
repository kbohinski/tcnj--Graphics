/*
 * @author Kevin Bohinski <bohinsk1@tcnj.edu>
 * @version 1.0
 * @since 2015-9-19
 *
 * Course:        TCNJ - CSC 350 - Computer Graphics
 * Instructor:    Dr. Salgian
 * Project Name:  Homework 2
 * Description:   Homework 2, Exercise 1
 *
 * Filename:      Shape.cpp
 * Description:   OpenGL program to draw a shape using a single triangle strip via triangulation.
 * Last Modified: 2015-9-19
 *
 * Modified from Sumanta Guha.
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

/*
 * Draw routine
 */
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(000.0, 080.0, 0.0);
		glVertex3f(000.0, 100.0, 0.0);
		glVertex3f(020.0, 080.0, 0.0);
		glVertex3f(040.0, 100.0, 0.0);
		glVertex3f(020.0, 000.0, 0.0);
		glVertex3f(040.0, 020.0, 0.0);
		glVertex3f(060.0, 000.0, 0.0);
		glVertex3f(060.0, 020.0, 0.0);
		glVertex3f(080.0, 000.0, 0.0);
		glVertex3f(060.0, 100.0, 0.0);
		glVertex3f(080.0, 080.0, 0.0);
		glVertex3f(100.0, 100.0, 0.0);
		glVertex3f(100.0, 080.0, 0.0);
	glEnd();

	glFlush();
}

/*
 * Init routine
 */
void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);

	/* Make the triangle strip work clockwise. */
	glFrontFace(GL_CW);

	/* Show wireframe */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/*
 * OpenGL resize routine
 */
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Shape.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}