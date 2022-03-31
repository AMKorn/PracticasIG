// Etapa4.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>
#include <iostream>

const int W_WIDTH = 700; // Tamaño incial de la ventana
const int W_HEIGHT = 700;

// Boolean to state if axes are to be shown. 
const int SHOW_AXES = true;

GLfloat rotate_x, rotate_y;
GLfloat eye_x = 1.0f, eye_y = 1.0f, eye_z = 1.0f;
GLfloat center_x = 0.0f, center_y = 0.0f, center_z = 0.0f;

int camera_mode;

// Funcion que visualiza la escena OpenGL
void Display(void) {
	// Establecemos la escena con profundidad
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(eye_x, eye_y, eye_z,
		center_x, center_y, center_z,
		0.0f, 1.0f, 0.0f);
	// We draw a cube 
	glPushMatrix(); {
		glTranslatef(0.25f, 0.25f, 0.25f);
		glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
		glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
		draw_parall(0.5f, 0.15f, 0.5f);
	}
	glPopMatrix();

	if (SHOW_AXES) {
		print_axes();
	}
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado. Sin usar.
void Idle(void) {
	// Incrementamos el angulo
	rotate_x += 0.03f;
	// Si es mayor que dos pi la decrementamos
	if (rotate_x > 360)
		rotate_x -= 360;
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

	/*gluPerspective(90, (GLfloat)W_WIDTH * scale_w / (GLfloat)W_HEIGHT * scale_h, 0, 100);
	gluLookAt(0.0f, 0.0f, 5.0f,		// eye
			  0.0f, 0.0f, 0.0f,		// center
			  0.0f, 1.0f, 0.0f);	// up */
	glViewport(0.0f, 0.0f, W_WIDTH * scale_w, W_HEIGHT * scale_h);
}

// Función para escuchar las teclas
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		camera_mode = CAM_PAN;
		std::cout << "Camera control mode: panning\n";
		break;
	case GLUT_KEY_F2:
		camera_mode = CAM_MOVE;
		std::cout << "Camera control mode: moving\n";
		break;
	case GLUT_KEY_RIGHT:
		if (camera_mode == CAM_PAN) {
			center_x += 0.05f;
			center_z -= 0.05f;
		} else if (camera_mode == CAM_MOVE) {
			center_x -= 0.05f;
			center_z += 0.05f;
			eye_x -= 0.05f;
			eye_z += 0.05f;
		}
		break;
	case GLUT_KEY_LEFT:
		if (camera_mode == CAM_PAN) {
			center_x -= 0.05f;
			center_z += 0.05f;
		} else if (camera_mode == CAM_MOVE) {
			center_x += 0.05f;
			center_z -= 0.05f;
			eye_x += 0.05f;
			eye_z -= 0.05f;
		}
		break;
	case GLUT_KEY_UP:
		if (camera_mode == CAM_PAN) {
			center_y += 0.05f;
		} else if (camera_mode == CAM_MOVE) {
			center_y -= 0.05f;
			eye_y -= 0.05f;
		}
		break;
	case GLUT_KEY_DOWN:
		if (camera_mode == CAM_PAN) {
			center_y -= 0.05f;
		} else if (camera_mode == CAM_MOVE) {
			center_y += 0.05f;
			eye_y += 0.05f;
		}
		break;
	}

	glutPostRedisplay(); // Solicitar actualización de visualización
}

//void keyboardKeys(unsigned char key, int x, int y) {
//	switch (key) {
//	case 'Q':
//	
//	}
//}

// Funcion principal
int main(int argc, char** argv) {
	// Inicializamos la libreria GLUT
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	// Creamos la nueva ventana
	glutCreateWindow("Etapa 4");

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(Display);
	glutSpecialFunc(specialKeys);
	//glutKeyboardFunc(keyboardKeys);
	//glutIdleFunc(Idle);

	// El color de fondo sera el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);
	//glFrustum(-10, 10, -10, 10, 0, 100);
	gluPerspective(60, (GLfloat)W_WIDTH / (GLfloat)W_HEIGHT, 0.001f, 100.0f);
	//glulookat(eye_x, eye_y, eye_z,
	//	center_x, center_y, center_z,
	//	0.0f, 1.0f, 0.0f);

	glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);

	// Comienza la ejecucion del core de GLUT
	glutMainLoop();
	return 0;
}