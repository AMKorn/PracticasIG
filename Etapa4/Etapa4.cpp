// Etapa4.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>

// **** Constants ****
// Initial window size
const int W_WIDTH = 700;
const int W_HEIGHT = 700;

// Boolean to state if axes are to be shown. 
const int SHOW_AXES = false;

// Constant to state the distance a camera jump makes with each input
const GLfloat CAM_JUMP = 0.05f;
const GLfloat CAM_MAX_HEIGHT = 20.0f;

// **** Variables ****
GLfloat rotateX, rotateY; // Variables to manage the object rotation
GLfloat eyeX = 1.0f, eyeY = 1.0f, eyeZ = 1.0f; // Variables to manage the camera
GLfloat centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
GLfloat upX = 0.0f, upY = 1.0f, upZ = 0.0f;

int cameraMode = CAM_PAN; // Variable to state which camera mode is enabled. Panning (F1) is default state.

// Scene values
GLfloat tableHeight = 0.5f;

GLfloat tableSurface[] = { 0.75f, 0.05f, 0.5f };
GLfloat tableLeg[] = { 0.05f, tableHeight - tableSurface[y], 0.05f};

GLfloat lampPosition[] = {tableSurface[x] / 3, tableHeight, tableSurface[z] / 2};

GLfloat lampBaseRadius = 0.05f, lampBaseHeight = 0.025f;
GLfloat lampArmRadius = lampBaseRadius / 5, lampArmLength = 0.1f;

// Funcion que visualiza la escena OpenGL
void display(void) {
	// Establecemos la escena con profundidad
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix(); {
		gluLookAt(eyeX, eyeY, eyeZ,
			centerX, centerY, centerZ,
			upX, upY, upZ);

		// Start of scene 
		glPushMatrix(); {
			/*glTranslatef(0.25f, 0.25f, 0.25f);
			glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
			glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);*/

			// We draw the table legs
			drawParall(tableLeg[x], tableLeg[y], tableLeg[z]);

			glPushMatrix(); {
				glTranslatef(0, 0, tableSurface[z] - tableLeg[z]);
				drawParall(tableLeg[x], tableLeg[y], tableLeg[z]);
			}
			glPopMatrix();

			glPushMatrix(); {
				glTranslatef(tableSurface[x] - tableLeg[x], 0, 0);
				drawParall(tableLeg[x], tableLeg[y], tableLeg[z]);

				glPushMatrix(); {
					glTranslatef(0, 0, tableSurface[z] - tableLeg[z]);
					drawParall(tableLeg[x], tableLeg[y], tableLeg[z]);
				}
				glPopMatrix();
			}
			glPopMatrix();
			// End of table legs

			// Start of table surface
			glPushMatrix(); {
				glTranslatef(0, tableHeight - tableSurface[y], 0);
				drawParall(tableSurface[x], tableSurface[y], tableSurface[z]);
			}
			glPopMatrix(); // End of table surface

			// Start of lamp 
			glPushMatrix(); {
				glTranslatef(lampPosition[x], lampPosition[y], lampPosition[z]);

				glRotated(-90, 1, 0, 0);
				// Lamp base
				glPushMatrix(); {
					glColor3f(0, 0.5f, 0.5f);
					gluCylinder(quadratic, lampBaseRadius, lampBaseRadius, lampBaseHeight, 32, 32);

					// Start of the top of the lamp base
					glPushMatrix(); {
						glColor3f(0.5f, 0, 0.5f);
						glTranslatef(0, 0, lampBaseHeight);
						drawEllipse(0, 0, lampBaseRadius, lampBaseRadius, 360);
					}
					glPopMatrix(); // End of top of lamp base
				}
				glPopMatrix(); // End of lamp base

				// First bone of the lamp
				glPushMatrix(); {
					glRotated(-30, 0, 1, 0);
					glColor3f(0,1,0);
					gluCylinder(quadratic, lampArmRadius, lampArmRadius, lampArmLength, 32, 32);
				}
				glPopMatrix(); // End of first bone

			}
			glPopMatrix(); // End of lamp

		}
		glPopMatrix(); // End of scene

		if (SHOW_AXES) {
			drawAxes();
		}
	}
	glPopMatrix();

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

	/*gluPerspective(90, (GLfloat)W_WIDTH * scale_w / (GLfloat)W_HEIGHT * scale_h, 0, 100);
	gluLookAt(0.0f, 0.0f, 5.0f,		// eye
			  0.0f, 0.0f, 0.0f,		// center
			  0.0f, 1.0f, 0.0f);	// up */
	glViewport(0.0f, 0.0f, W_WIDTH * scale_w, W_HEIGHT * scale_h);
}

// Función para escuchar las teclas
void specialKeys(int key, int x, int y) {
	GLfloat modulus;
	GLfloat unitaryX, unitaryY, unitaryZ;

	switch (key) {
	case GLUT_KEY_F1:
		cameraMode = CAM_PAN;
		std::cout << "Camera control mode: panning\n";
		break;
	case GLUT_KEY_F2:
		cameraMode = CAM_MOVE;
		std::cout << "Camera control mode: moving\n";
		break;
	case GLUT_KEY_F3:
		std::cout << "Set: Nadir plane\n [Camera move not supported]\n";
		eyeX = 0.5f;
		eyeY = -1.0f;
		eyeZ = 0.5f;
		centerX = 0.5f;
		centerY = 0.0f;
		centerZ = 0.5f;
		upX = 0.0f;
		upY = 0.0f;
		upZ = 1.0f;
		break;
	case GLUT_KEY_F4:
		std::cout << "Set: Low angle view\n";
		eyeX = 1.0f;
		eyeY = 0.0f;
		eyeZ = 1.0f;
		centerX = 0.5f;
		centerY = 0.5f;
		centerZ = 0.5f;
		upX = 0.0f;
		upY = 1.0f;
		upZ = 0.0f;
		break;
	case GLUT_KEY_F5:
		std::cout << "Set: Normal view\n";
		eyeX = 1.0f;
		eyeY = 0.5f;
		eyeZ = 1.0f;
		centerX = 0.5f;
		centerY = 0.5f;
		centerZ = 0.5f;
		upX = 0.0f;
		upY = 1.0f;
		upZ = 0.0f;
		break;
	case GLUT_KEY_F6:
		std::cout << "Set: High angle view\n";
		eyeX = 1.0f;
		eyeY = 1.0f;
		eyeZ = 1.0f;
		centerX = 0.0f;
		centerY = 0.0f;
		centerZ = 0.0f;
		upX = 0.0f;
		upY = 1.0f;
		upZ = 0.0f;
		break;
	case GLUT_KEY_F7:
		std::cout << "Set: Zenith view\n [Camera move not supported]\n";
		eyeX = 0.5f;
		eyeY = 1.0f;
		eyeZ = 0.5f;
		centerX = 0.5f;
		centerY = 0.0f;
		centerZ = 0.5f;
		upX = 0.0f;
		upY = 0.0f;
		upZ = 1.0f;
		break;
	case GLUT_KEY_RIGHT:
		if (cameraMode == CAM_PAN) {
			// Camera should rotate around itself clockwise
			// We will use the same idea as a rotation transformation to move the camera center around 
			// the axis parallel to the y-axis and that passes through the camera eye
			centerX -= eyeX;
			centerZ -= eyeZ; // We move it to where it would be if eye was in (0,y,0)

			centerX = centerX * cosf(CAM_JUMP) + centerZ * sinf(CAM_JUMP);
			centerZ = centerZ * cosf(CAM_JUMP) - centerX * sinf(CAM_JUMP);

			centerX += eyeX;
			centerZ += eyeZ;
		} else if (cameraMode == CAM_MOVE) {
			// Camera should move to its right
			
			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unitaryX = centerX - eyeX;
			unitaryY = centerY - eyeY;
			unitaryZ = centerZ - eyeZ;

			modulus = sqrt(unitaryX * unitaryX + unitaryY * unitaryY + unitaryZ * unitaryZ);
			unitaryX /= modulus;
			unitaryY /= modulus;
			unitaryZ /= modulus;

			// The right vector is calculated with vectorial product of unit x up;
			// right_x = -unit_z;
			// right_y = 0;
			// right_z = unit_x;
			// After that, the new center and eye position equal:
			// center += right; eye += right;
			centerX -= unitaryZ * CAM_JUMP;
			eyeX -= unitaryZ * CAM_JUMP;
			centerZ += unitaryX * CAM_JUMP;
			eyeZ += unitaryX * CAM_JUMP;
		}
		break;
	case GLUT_KEY_LEFT:
		if (cameraMode == CAM_PAN) {
			centerX -= eyeX;
			centerZ -= eyeZ; // We move it to where it would be if eye was in (0,y,0)

			centerX = centerX * cosf(-CAM_JUMP) + centerZ * sinf(-CAM_JUMP);
			centerZ = centerZ * cosf(-CAM_JUMP) - centerX * sinf(-CAM_JUMP);

			centerX += eyeX;
			centerZ += eyeZ;
		} else if (cameraMode == CAM_MOVE) {
			// Camera should move to its left

			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unitaryX = centerX - eyeX;
			unitaryY = centerY - eyeY;
			unitaryZ = centerZ - eyeZ;

			modulus = sqrt(unitaryX * unitaryX + unitaryY * unitaryY + unitaryZ * unitaryZ);
			unitaryX /= modulus;
			unitaryY /= modulus;
			unitaryZ /= modulus;

			// The right vector is calculated with vectorial product of unit x up;
			// right_x = -unit_z;
			// right_y = 0;
			// right_z = unit_x;
			// After that, the new center and eye position equal:
			// center -= right; eye -= right;
			centerX += unitaryZ * CAM_JUMP;
			eyeX += unitaryZ * CAM_JUMP;
			centerZ -= unitaryX * CAM_JUMP;
			eyeZ -= unitaryX * CAM_JUMP;
		}
		break;
	case GLUT_KEY_UP:
		if (cameraMode == CAM_PAN && centerY < CAM_MAX_HEIGHT) {
			centerY += CAM_JUMP;
		} else if (cameraMode == CAM_MOVE) {
			// UP KEY -> move forward
			
			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unitaryX = centerX - eyeX;
			unitaryY = centerY - eyeY;
			unitaryZ = centerZ - eyeZ;

			modulus = sqrt(unitaryX * unitaryX + unitaryY * unitaryY + unitaryZ * unitaryZ);
			unitaryX /= modulus;
			unitaryY /= modulus;
			unitaryZ /= modulus;

			centerX += unitaryX * CAM_JUMP;
			centerY += unitaryY * CAM_JUMP;
			centerZ += unitaryZ * CAM_JUMP;
			eyeX += unitaryX * CAM_JUMP;
			eyeY += unitaryY * CAM_JUMP;
			eyeZ += unitaryZ * CAM_JUMP;

		}
		break;
	case GLUT_KEY_DOWN:
		if (cameraMode == CAM_PAN && centerY > -CAM_MAX_HEIGHT) {
			centerY -= CAM_JUMP;
		} else if (cameraMode == CAM_MOVE) {
			// DOWN KEY -> move backwards

			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unitaryX = centerX - eyeX;
			unitaryY = centerY - eyeY;
			unitaryZ = centerZ - eyeZ;

			modulus = sqrt(unitaryX * unitaryX + unitaryY * unitaryY + unitaryZ * unitaryZ);
			unitaryX /= modulus;
			unitaryY /= modulus;
			unitaryZ /= modulus;

			centerX -= unitaryX * CAM_JUMP;
			centerY -= unitaryY * CAM_JUMP;
			centerZ -= unitaryZ * CAM_JUMP;
			eyeX -= unitaryX * CAM_JUMP;
			eyeY -= unitaryY * CAM_JUMP;
			eyeZ -= unitaryZ * CAM_JUMP;

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

	printf("Welcome! \n Use the following controls to move the camera:\n F1: Camera panning [default]\n F2: Move camera\n F3: Nadir\n F4: low angle view\n F5: Normal\n F6: high angle view [default]\n F7: Zenith\n");

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	// Creamos la nueva ventana
	glutCreateWindow("Etapa 4");

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
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