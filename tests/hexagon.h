
#ifndef HEXAGON_H
#define HEXAGON_H
#include <GL/gl.h>

#define GLCOLOR1 0.91f, 0.128f, 0.64f, 1.0f
#define GLCOLOR2 0.0f, 1.0f, 0.0f, 0.5f
#define GLCOLOR3 0.0f, 0.0f, 1.0f, 0.5f

void drawHexagon()
{
	glPushMatrix();
	// Object
	/*
	 * faster and less tedious than defining them with quads and 
	 * triangles
	 */
	glBegin(GL_TRIANGLE_FAN);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor4f(GLCOLOR1);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor4f(GLCOLOR3);
		glVertex3f(0.86f, 0.5f, 0.0f);
		glColor4f(GLCOLOR2);
		glVertex3f(0.86f, -0.5f, 0.0f);
		glColor4f(GLCOLOR1);
		glVertex3f(0.0f, -1.0f, 0.0f);
		glColor4f(GLCOLOR3);
		glVertex3f(-0.86f, -0.5f, 0.0f);
		glColor4f(GLCOLOR2);
		glVertex3f(-0.86f, 0.5f, 0.0f);
		glColor4f(GLCOLOR1);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glPopMatrix();
}
#endif
