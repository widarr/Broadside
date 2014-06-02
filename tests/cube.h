
#ifndef CUBE_H
#define CUBE_H
#include <GL/gl.h>

#define GLCOLOR1 0.91f, 0.128f, 0.64f, 1.0f
#define GLCOLOR2 0.0f, 1.0f, 0.0f, 0.5f
#define GLCOLOR3 0.0f, 0.0f, 1.0f, 0.5f

void drawCube()
{
	glPushMatrix();
	
	GLfloat n[6][3] = { /* Normals for the 6 faces of a cube. */
		{-1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}
	};
	
	GLfloat v[8][3] = { /* Corners of the 8 vertices of the cube. */
		{-2,2,-2},
		{2,2,-2},
		{2,-2,-2},
		{-2,-2,-2},
		{-2,2,2},
		{2,2,2},
		{2,-2,2},
		{-2,-2,2}
	};

	GLuint f[6][4] = { /* 6 faces of the cube */
		{0,1,2,3},
		{4,5,6,7},
		{8,9,10,11},
		{12,13,14,15},
		{16,17,18,19},
		{20,21,22,23}
	};

	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		glNormal3fv(&n[i][0]);
		glColor4f(GLCOLOR2);
		glVertex3fv(&v[f[i][0]][0]);
		glColor4f(GLCOLOR1);
		glVertex3fv(&v[f[i][1]][0]);
		glColor4f(GLCOLOR3);
		glVertex3fv(&v[f[i][2]][0]);
		glColor4f(GLCOLOR2);
		glVertex3fv(&v[f[i][3]][0]);
	}
	glEnd();



	glPopMatrix();
}

#endif
