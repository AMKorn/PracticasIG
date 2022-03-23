// Practicas.h
// Header to be used by all other stages of the
// project
////////////////////////////////////////////////////

// As most color pickers online give the values in
// 0-255 instead of 0-1, which openGL uses, this
// constant could be used to easily convert by doing
// x' = x / RGB_MAX
const int RGB_MAX = 255;

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