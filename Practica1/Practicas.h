// Practicas.h
// Header a ser usado por todas las otras etapas del
// proyecto
////////////////////////////////////////////////////
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <iostream>

// As most color pickers online give the values in
// 0-255 instead of 0-1, which openGL uses, this
// constant could be used to easily convert by doing
// x' = x / RGB_MAX
const float RGB_MAX = 255.0f;

// Modes for the camera control from etapa 4 and onwards
const int CAM_PAN = 0;
const int CAM_MOVE = 1;

// Variables for positions or vectors set as arrays
const int x = 0;
const int y = 1;
const int z = 2;

GLUquadricObj* quadratic = gluNewQuadric();

void print_axes() {
	// x-axis
	glPushMatrix();
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		gluCylinder(quadratic, 0.005f, 0.005f, 0.5f, 32, 32);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.5f);
			glutSolidCone(0.01f, 0.1f, 32, 32);
		glPopMatrix();
	glPopMatrix();

	// y-axis
	glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		gluCylinder(quadratic, 0.005f, 0.005f, 0.5f, 32, 32);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.5f);
			glutSolidCone(0.01f, 0.1f, 32, 32);
		glPopMatrix();
	glPopMatrix();

	// z-axis
	glPushMatrix();
		//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		gluCylinder(quadratic, 0.005f, 0.005f, 0.5f, 32, 32);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.5f);
			glutSolidCone(0.01f, 0.1f, 32, 32);
		glPopMatrix();
	glPopMatrix();
}

// Draws a parallelepiped with the given dimensions with a vertex in 0,0,0 and colors the faces.
void draw_parall(GLfloat face_length_x, GLfloat face_length_y, GLfloat face_length_z) {

	GLfloat half_length = 0.25f;

	// Cara en el plano xy
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, face_length_y, 0);
	glVertex3f(0, face_length_y, 0);
	glEnd();

	// Cara paralela al plano xy
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0, 0, face_length_z);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(0, face_length_y, face_length_z);
	glEnd();

	// Cara en el plano zy
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, face_length_z);
	glVertex3f(0, face_length_y, face_length_z);
	glVertex3f(0, face_length_y, 0);
	glEnd();

	// Cara paralela al plano zy
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(face_length_x, face_length_y, 0);
	glEnd();

	// Cara en el plano xz
	glBegin(GL_POLYGON);
	glNormal3f(0, -1.0f, 0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(0, 0, face_length_z);
	glEnd();

	// Cara paralela al plano xz
	glBegin(GL_POLYGON);
	glNormal3f(0, 1.0f, 0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, face_length_y, 0);
	glVertex3f(face_length_x, face_length_y, 0);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(0, face_length_y, face_length_z);
	glEnd();
}

// Draws a rectangle on the plane xy, given the dimensions.
void draw_rectangle(GLfloat dim_x, GLfloat dim_y) {
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(dim_x, 0, 0);
	glVertex3f(dim_x, dim_y, 0);
	glVertex3f(0, dim_y, 0);
	glEnd();
}

/**
* Function to create part of a circumference, up to the specified angle in degrees
*/
void draw_ellipse(float center_x, float center_y, float radius_x, float radius_y, int drawn_angle) {
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 0, 1);
	glVertex2f(center_x, center_y);
	for (int i = 0; i <= drawn_angle; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(360); //get the current angle 
		float x = radius_x * cosf(theta); //calculate the x component 
		float y = radius_y * sinf(theta); //calculate the y component 
		glVertex2f(x + center_x, y + center_y); //output vertex 
	}
	glEnd();
}