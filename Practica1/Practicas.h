// Practicas.h
// Header a ser usado por todas las otras etapas del
// proyecto
////////////////////////////////////////////////////

#include <math.h>

// As most color pickers online give the values in
// 0-255 instead of 0-1, which openGL uses, this
// constant could be used to easily convert by doing
// x' = x / RGB_MAX
const float RGB_MAX = 255.0f;

void print_axes() {
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();

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

/**
* Function to create part of a circumference, up to the specified angle
*/
void draw_ellipse(float center_x, float center_y, float radius_x, float radius_y, int drawn_angle) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(center_x, center_y);
	for (int i = 0; i <= drawn_angle; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(360); //get the current angle 
		float x = radius_x * cosf(theta); //calculate the x component 
		float y = radius_y * sinf(theta); //calculate the y component 
		glVertex2f(x + center_x, y + center_y); //output vertex 
	}
	glEnd();
}