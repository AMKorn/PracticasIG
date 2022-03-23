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

void DrawCircle(float cx, float cy, float r, int num_segments) {
	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 
		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
}