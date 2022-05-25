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

const int DEFAULT = 0;
const int NADIR = 1;
const int LOW_ANGLE = 2;
const int NORMAL = 3;
const int ZENITH = 4;

const int PAUSED = 5;

const int AMBIENT = 0;
const int LIGHT_1 = 1;
const int LIGHT_2 = 2;

// Variables for positions or vectors set as arrays
const int x = 0;
const int y = 1;
const int z = 2;

// Variables for colors set as arrays
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int ALPHA = 3;

const GLfloat AMBIENT_DEF = 0.2f;
const GLfloat DIFFUSE_DEF = 0.8f;
const GLfloat SPECULAR_DEF = 0;
const GLfloat EMISSION_DEF = 0;
const GLfloat SHININESS_DEF = 0;

GLUquadricObj* quadratic = gluNewQuadric();
GLUquadricObj* inv_quadratic = gluNewQuadric();

void setMaterial(GLfloat color[3], GLfloat ambient_level, GLfloat diffuse_level, GLfloat specular_level, GLfloat emission_level, GLfloat shininess) {
	GLfloat red = color[RED];
	GLfloat green = color[GREEN];
	GLfloat blue = color[BLUE];
	GLfloat ambient[] = { red * ambient_level, green * ambient_level, blue * ambient_level };
	GLfloat diffuse[] = { red * diffuse_level, green * diffuse_level, blue * diffuse_level };
	GLfloat specular[] = { red * specular_level, green * specular_level, blue * specular_level };
	GLfloat emission[] = { red * emission_level, green * emission_level, blue * emission_level };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColor3f(red, green, blue);
}

void resetMaterial() {
	setMaterial(new GLfloat[]{ 0,0,0 }, AMBIENT_DEF, DIFFUSE_DEF, SPECULAR_DEF, EMISSION_DEF, SHININESS_DEF);
}

void draw_axes() {
	resetMaterial();

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

// Draws a parallelepiped with the given dimensions with a vertex in 0,0,0 with the current color and material.
void draw_parall_material(GLfloat face_length_x, GLfloat face_length_y, GLfloat face_length_z) {
	// Cara en el plano xy
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, face_length_y, 0);
	glVertex3f(0, face_length_y, 0);
	glEnd();

	// Cara paralela al plano xy
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1.0f);
	glVertex3f(0, 0, face_length_z);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(0, face_length_y, face_length_z);
	glEnd();

	// Cara en el plano zy
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, face_length_z);
	glVertex3f(0, face_length_y, face_length_z);
	glVertex3f(0, face_length_y, 0);
	glEnd();

	// Cara paralela al plano zy
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0, 0);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(face_length_x, face_length_y, 0);
	glEnd();

	// Cara en el plano xz
	glBegin(GL_POLYGON);
	glNormal3f(0, -1.0f, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(0, 0, face_length_z);
	glEnd();

	// Cara paralela al plano xz
	glBegin(GL_POLYGON);
	glNormal3f(0, 1.0f, 0);
	glVertex3f(0, face_length_y, 0);
	glVertex3f(face_length_x, face_length_y, 0);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(0, face_length_y, face_length_z);
	glEnd();
}

// Draws a parallelepiped with the given dimensions with a vertex in 0,0,0 with the current color and material. 
// Each face is comprised of smaller squares equal to the parameter slices. This is to improve lighting.
void draw_parall_material(GLfloat face_length_x, GLfloat face_length_y, GLfloat face_length_z, GLint face_polygons) {
	// Cara en el plano xy
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, -1);
	for (int i = 0; i < face_polygons; i++) {
		for (int j = 0; j < face_polygons; j++) {
			glVertex3f(i * face_length_x / face_polygons, j * face_length_y / face_polygons, 0);
			glVertex3f(i * face_length_x / face_polygons, (j + 1) * face_length_y / face_polygons, 0);
			glVertex3f((i + 1) * face_length_x / face_polygons, j * face_length_y / face_polygons, 0);
			glVertex3f((i + 1) * face_length_x / face_polygons, (j + 1) * face_length_y / face_polygons, 0);
		}
	}

	//// Cara paralela al plano xy
	/*glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1.0f);
	glVertex3f(0, 0, face_length_z);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(0, face_length_y, face_length_z);
	glEnd();*/
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, 1);
	for (int i = 0; i < face_polygons; i++) {
		for (int j = 0; j < face_polygons; j++) {
			glVertex3f(i * face_length_x / face_polygons, j * face_length_y / face_polygons, face_length_z);
			glVertex3f(i * face_length_x / face_polygons, (j + 1) * face_length_y / face_polygons, face_length_z);
			glVertex3f((i + 1) * face_length_x / face_polygons, j * face_length_y / face_polygons, face_length_z);
			glVertex3f((i + 1) * face_length_x / face_polygons, (j + 1) * face_length_y / face_polygons, face_length_z);
		}
	}

	// Cara en el plano zy
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(-1, 0, 0);
	for (int i = 0; i < face_polygons; i++) {
		for (int j = 0; j < face_polygons; j++) {
			glVertex3f(0, i * face_length_y / face_polygons, j * face_length_z / face_polygons);
			glVertex3f(0, i * face_length_y / face_polygons, (j + 1) * face_length_z / face_polygons);
			glVertex3f(0, (i + 1) * face_length_y / face_polygons, j * face_length_z / face_polygons);
			glVertex3f(0, (i + 1) * face_length_y / face_polygons, (j + 1) * face_length_z / face_polygons);
		}
	}

	// Cara paralela al plano zy
	/*glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0, 0);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(face_length_x, face_length_y, face_length_z);
	glVertex3f(face_length_x, face_length_y, 0);
	glEnd();*/
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(1, 0, 0);
	for (int i = 0; i < face_polygons; i++) {
		for (int j = 0; j < face_polygons; j++) {
			glVertex3f(face_length_x, i * face_length_y / face_polygons, j * face_length_z / face_polygons);
			glVertex3f(face_length_x, i * face_length_y / face_polygons, (j + 1) * face_length_z / face_polygons);
			glVertex3f(face_length_x, (i + 1) * face_length_y / face_polygons, j * face_length_z / face_polygons);
			glVertex3f(face_length_x, (i + 1) * face_length_y / face_polygons, (j + 1) * face_length_z / face_polygons);
		}
	}

	// Cara en el plano xz
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, -1, 0);
	for (int i = 0; i < face_polygons; i++) {
		for (int j = 0; j < face_polygons; j++) {
			glVertex3f(i * face_length_x / face_polygons, 0, j * face_length_z / face_polygons);
			glVertex3f(i * face_length_x / face_polygons, 0, (j + 1) * face_length_z / face_polygons);
			glVertex3f((i + 1) * face_length_x / face_polygons, 0, j * face_length_z / face_polygons);
			glVertex3f((i + 1) * face_length_x / face_polygons, 0, (j + 1) * face_length_z / face_polygons);
		}
	}
	glEnd();
	/*glBegin(GL_POLYGON);
	glNormal3f(0, -1.0f, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(face_length_x, 0, 0);
	glVertex3f(face_length_x, 0, face_length_z);
	glVertex3f(0, 0, face_length_z);
	glEnd();*/

	// Cara paralela al plano xz
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < face_polygons; i++) {
		for (int j = 0; j < face_polygons; j++) {
			glVertex3f(i * face_length_x / face_polygons, face_length_y, j * face_length_z / face_polygons);
			glVertex3f(i * face_length_x / face_polygons, face_length_y, (j + 1) * face_length_z / face_polygons);
			glVertex3f((i + 1) * face_length_x / face_polygons, face_length_y, j * face_length_z / face_polygons);
			glVertex3f((i + 1) * face_length_x / face_polygons, face_length_y, (j + 1) * face_length_z / face_polygons);
		}
	}
	glEnd();

}

// Draws a parallelepiped with the given dimensions with a vertex in 0,0,0 and colors the faces red, green and blue.
void draw_parall(GLfloat face_length_x, GLfloat face_length_y, GLfloat face_length_z) {
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