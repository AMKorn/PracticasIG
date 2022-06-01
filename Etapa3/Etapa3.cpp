// Etapa3.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>

const int W_WIDTH = 700; // Tamaño incial de la ventana
const int W_HEIGHT = 700;

// Boolean to state if axes are to be shown. 
const int SHOW_AXES = true;

GLfloat rotateX, rotateY;

// Funcion que visualiza la escena OpenGL
void display(void) {
	// Establecemos la escena con profundidad
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// We draw a cube 
	glPushMatrix(); {
		//glTranslatef(0.25f, 0.25f, 0.25f);
		glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
		glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
		drawParall(0.5f, 0.15f, 0.5f);
	}
	glPopMatrix();

	if (SHOW_AXES) {
		drawAxes();
	}

	glFlush();
	glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado. Sin usar.
void idle(void) {
	// Incrementamos el angulo
	rotateX += 0.03f;
	// Si es mayor que dos pi la decrementamos
	if (rotateX > 360)
		rotateX -= 360;
	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

void reshape(int width, int height) {
	const float ar_origin = (float)W_WIDTH / (float)W_HEIGHT;
	const float ar_new = (float)width / (float)height;

	float scale_w = (float)width / (float)W_WIDTH;
	float scale_h = (float)height / (float)W_HEIGHT;
	if (ar_new > ar_origin) {
		scale_w = scale_h;
	}
	else {
		scale_h = scale_w;
	}

	glViewport(0.0f, 0.0f, W_WIDTH * scale_w, W_HEIGHT * scale_h);
}

// Función para escuchar las teclas
void specialKeys(int key, int x, int y) {
	switch (key){
	case GLUT_KEY_RIGHT:
		rotateY -= 5;
		break;
	case GLUT_KEY_LEFT:
		rotateY += 5;
		break;
	case GLUT_KEY_UP:
		rotateX += 5;
		break;
	case GLUT_KEY_DOWN:
		rotateX -= 5;
		break;
	}

	glutPostRedisplay(); // Solicitar actualización de visualización
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
	glutCreateWindow("Etapa 3");

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	//glutIdleFunc(Idle);

	// El color de fondo sera el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);
	glFrustum(-10, 10, -10, 10, 0, 100);
	glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);

	// Comienza la ejecucion del core de GLUT
	glutMainLoop();
	return 0;
}