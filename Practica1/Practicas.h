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
	// x-axis
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, 0.0f, 0.0f);
	glVertex3f(0.9f, 0.01f, 0.0f);
	glVertex3f(0.0f, 0.01f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.01f, 0.0f);
	glVertex3f(0.9f, 0.02f, 0.0f);
	glVertex3f(0.9f, -0.01f, 0.0f);
	glEnd();

	// y-axis
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.01f, 0.9f, 0.0f);
	glVertex3f(0.01f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.01f, 1.0f, 0.0f);
	glVertex3f(0.02f, 0.9f, 0.0f);
	glVertex3f(-0.01f, 0.9f, 0.0f);
	glEnd();
}

/**
* Function to create part of a circumference, up to the specified angle
*/
void DrawEllipse(float center_x, float center_y, float radius_x, float radius_y, int drawn_angle) {
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