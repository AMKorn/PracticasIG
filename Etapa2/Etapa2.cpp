// Etapa2.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>

const int W_WIDTH = 700; // Tamaño incial de la ventana
const int W_HEIGHT = 700;

// Boolean to state if axes are to be shown. 
const int SHOW_AXES = true;

GLfloat fAnguloBase; // Variable que indica el angulo de rotación del primer "hueso".
GLfloat fAngulo2; // Variable que indica el ángulo de rotación del segundo "hueso".

int directionBase = -1;
int direction2 = -1;

// Funcion que visualiza la escena OpenGL
void display(void) {
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Creamos la base
	glBegin(GL_POLYGON);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.25f, -0.05f, 0.0f);
	glVertex3f(-0.25f, 0.05f, 0.0f);
	glVertex3f(0.25f, 0.05f, 0.0f);
	glVertex3f(0.25f, -0.05f, 0.0f);
	glEnd();

	glPushMatrix();
		// Las siguientes primitivas se rotan: es el primer hueso del brazo 
		// y todas las que estén ancladas a ella
		glRotatef(fAnguloBase, 0.0f, 0.0f, 1.0f);

		glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.7f);
		glVertex3f(-0.05f, 0.0f, 0.0f);
		glVertex3f(0.05f, 0.0f, 0.0f);
		glVertex3f(0.05f, 0.45f, 0.0f);
		glVertex3f(-0.05f, 0.45f, 0.0f);
		glEnd();

		drawEllipse(0.0f, 0.0f, 0.05f, 0.05f, 360);

		glPushMatrix();
			// Lo siguiente se vuelve a rotar: es el segundo "hueso" del brazo.
			glTranslatef(0.0f, 0.45f, 0.0f);
			glRotatef(fAngulo2, 0.0f, 0.0f, 1.0f);

			glBegin(GL_POLYGON);
			glColor3f(0.9f, 0.9f, 0.9f);
			glVertex3f(0.0f, 0.05f, 0.0f);
			glVertex3f(0.0f, -0.05f, 0.0f);
			glVertex3f(0.4f, -0.05f, 0.0f);
			glVertex3f(0.4f, 0.05f, 0.0f);
			glEnd();

			drawEllipse(0.0f, 0.0f, 0.05f, 0.05f, 360);

			glColor3f(0.0f, 0.0f, 0.0f);
			drawEllipse(0.0f, 0.0f, 0.025f, 0.025f, 360);

		glPopMatrix();

		glColor3f(0.0f, 0.0f, 0.0f);
		drawEllipse(0.0f, 0.0f, 0.025f, 0.025f, 360);

	glPopMatrix();

	if (SHOW_AXES) {
		drawAxes();
	}

	glFlush();
	glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void idle(void) {
	// Incrementamos el angulo
	fAnguloBase += 0.01f*directionBase;
	// Queremos que el brazo vaya girando solo 10 grados a cada lado de su origen
	if (fAnguloBase > 10.0f) {
		directionBase = -1;
	} else if (fAnguloBase < -10.0f) {
		directionBase = 1;
	}
	// Incrementamos el angulo 2, este hueso gira 20 grados en cada direccion
	fAngulo2 += 0.03f * direction2;
	if (fAngulo2 > 20.0f) {
		direction2 = -1;
	}
	else if (fAngulo2 < -20.0f) {
		direction2 = 1;
	}
	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

void reshape(int width, int height) {
	const float ar_origin = (float) W_WIDTH / (float) W_HEIGHT;
	const float ar_new = (float) width / (float) height;

	float scale_w = (float) width / (float) W_WIDTH;
	float scale_h = (float) height / (float) W_HEIGHT;
	if (ar_new > ar_origin) {
		scale_w = scale_h;
	} else {
		scale_h = scale_w;
	}

	glViewport(0.0f, 0.0f, W_WIDTH * scale_w, W_HEIGHT * scale_h);
}

// Funcion principal
int main(int argc, char** argv) {
	// Inicializamos la libreria GLUT
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	
	// Creamos la nueva ventana
	glutCreateWindow("Etapa 2");

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	// El color de fondo sera el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);
	glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);

	// Comienza la ejecucion del core de GLUT
	glutMainLoop();
	return 0;
}