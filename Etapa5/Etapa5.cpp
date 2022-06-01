// Etapa5.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>

// **** Constants ****
// Initial window size
const int W_WIDTH = 700;
const int W_HEIGHT = 700;

// Boolean to state if axes are to be shown. 
const bool SHOW_AXES = false;
const bool bIsPaused = true;

// Constant to state the distance a camera jump makes with each input
const GLfloat CAM_JUMP = 0.05f;
const GLfloat CAM_MAX_HEIGHT = 20.0f;

// **** Variables ****
GLfloat rotateX, rotateY; // Variables to manage the object rotation
GLfloat eyeX = 1.0f, eyeY = 1.0f, eyeZ = 1.0f; // Variables to manage the camera
GLfloat centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
GLfloat upX = 0.0f, upY = 1.0f, upZ = 0.0f;

bool bAmbientLightIsOn = true;
bool bLight1IsOn = true;
bool bLight2IsOn = true;
GLfloat ambientLightValue[] = { 0.5f, 0.5f, 0.5f,1 }; // values in RGBA // CON luz = 1,1,1, SIN luz = 0,0,0

//GLfloat light1_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Position of light 1
GLfloat light1Value[] = {1, 1, 1, 1}; // { 0.0f, 1.0f, 1.0f, 1.0f }; // Values of light 1 (RGBA)

GLfloat light2PosX = 0.5f;
GLfloat light2PosY = 0.6f;
GLfloat light2PosZ = 0.25f;
GLfloat light2Ambient[] = { 0,0,0,1 };
GLfloat light2Diffuse[] = { 1,1,1,1 };
GLfloat light2Specular[] = { 1,1,1,1 };

int cameraMode = CAM_PAN; // Variable to state which camera mode is enabled. Panning (F1) is default state.
int shadeModel = 1;

// *** Scene values ***
// * Table values *
GLfloat tableColor[] = {164 / RGB_MAX, 114 / RGB_MAX , 44 / RGB_MAX}; // A sort of brown color
GLfloat lampAccentsColor[] = { 100 / RGB_MAX, 100 / RGB_MAX, 100 / RGB_MAX};
GLfloat lampArmsColor[] = {184 / RGB_MAX, 184 / RGB_MAX, 184 / RGB_MAX};

GLfloat tableHeight = 0.5f;

GLfloat tableSurface[] = { 0.75f, 0.05f, 0.5f };
GLfloat tableLeg[] = { 0.05f, tableHeight - tableSurface[y], 0.05f };

// * Lamp values *
GLfloat lampPosition[] = { tableSurface[x] / 3, tableHeight, tableSurface[z] / 2 };

GLfloat lampBaseRadius = 0.05f, lampBaseHeight = 0.025f;
GLfloat lampArmRadius = lampBaseRadius / 5, lampArmLength = 0.1f;

GLfloat lampConeWidth = lampArmRadius * 4;

GLfloat lampArticulationAngle = 90.0f;
GLfloat MAX_LAMP_ARTICULATION_ANGLE = 90.0f;
GLint rotationDirection = 1;

// * Ball values *
GLfloat ballPosition[] = { tableSurface[x] * 2 / 3, tableHeight, tableSurface[z] / 2 };
GLfloat ballRadius = 0.05f;

// Funcion que visualiza la escena OpenGL
void display(void) {
	// Establecemos la escena con profundidad y con iluminación
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (shadeModel == 1) {
		glShadeModel(GL_SMOOTH);
	} else {
		glShadeModel(GL_FLAT);
	}

	if (bAmbientLightIsOn) {
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightValue);
	} else glDisable(GL_LIGHT0);

	if (bLight1IsOn) {
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Value);
		glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10);
		//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
		//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
		//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0);
	} else glDisable(GL_LIGHT1);

	glPushMatrix(); {
		gluLookAt(eyeX, eyeY, eyeZ,
			centerX, centerY, centerZ,
			upX, upY, upZ);

		// Free light 
		if (bLight2IsOn) {
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_AMBIENT, light2Ambient);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, light2Diffuse);
			glLightfv(GL_LIGHT2, GL_SPECULAR, light2Specular);

			glPushMatrix(); {
				glTranslatef(light2PosX, light2PosY, light2PosZ);

				glLightfv(GL_LIGHT2, GL_POSITION, new GLfloat[]{ 0, 0, 0, 1 });

				setMaterial(new GLfloat[]{ 1,1,0 }, 0, 0, 0, 1, 50);
				gluSphere(quadratic, 0.02f, 32, 32);
				resetMaterial();
			}
			glPopMatrix();

		} else glDisable(GL_LIGHT2);

		// Start of scene 
		glPushMatrix(); {

			// We draw the table legs
			setMaterial(tableColor, 0.5f, 1.0f, 1.0f, 0.0f, 50.0f);
			drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z]);

			glPushMatrix(); {
				glTranslatef(0, 0, tableSurface[z] - tableLeg[z]);
				drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z]);
			}
			glPopMatrix();

			glPushMatrix(); {
				glTranslatef(tableSurface[x] - tableLeg[x], 0, 0);
				drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z]);

				glPushMatrix(); {
					glTranslatef(0, 0, tableSurface[z] - tableLeg[z]);
					drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z]);
				}
				glPopMatrix();
			}
			glPopMatrix();
			// End of table legs

			// Start of table surface
			glPushMatrix(); {
				glTranslatef(0, tableHeight - tableSurface[y], 0);
				drawParallMaterial(tableSurface[x], tableSurface[y], tableSurface[z], 128);
			}
			glPopMatrix(); // End of table surface

			// Start of lamp 
			glPushMatrix(); {
				glTranslatef(lampPosition[x], lampPosition[y], lampPosition[z]);

				glRotated(-90, 1, 0, 0);
				// Lamp base
				glPushMatrix(); {
					//glColor3f(0, 0.5f, 0.5f);
					setMaterial(lampAccentsColor, 1, 1, 0.5f, 0, 0.5f);
					gluCylinder(quadratic, lampBaseRadius, lampBaseRadius, lampBaseHeight, 32, 1);

					// Start of the top of the lamp base
					glPushMatrix(); {
						glTranslatef(0, 0, lampBaseHeight);
						drawEllipse(0, 0, lampBaseRadius, lampBaseRadius, 360);

						glTranslatef(lampBaseRadius / 3, 0, 0);
						glColor3f(0,0,0);
						GLfloat switch_height = 0.01f;
						if (bLight1IsOn) switch_height /= 3;

						gluCylinder(quadratic, 0.005f, 0.005f, switch_height, 16, 1);
						glTranslatef(0, 0, switch_height);
						glutSolidSphere(0.005f, 16, 16);
					}
					glPopMatrix(); // End of top of lamp base
				}
				glPopMatrix(); // End of lamp base

				// First bone of the lamp
				glPushMatrix(); {
					glRotated(-30, 0, 1, 0);
					setMaterial(lampArmsColor, 1, 1, 0.5f, 0, 0.5f);
					gluCylinder(quadratic, lampArmRadius, lampArmRadius, lampArmLength, 32, 1);

					// First joint
					glPushMatrix(); {
						glTranslatef(0,0,lampArmLength);
						setMaterial(lampAccentsColor, 1, 1, 0.5f, 0, 0.5f);
						glutSolidSphere(lampArmRadius * 3/2, 32, 32);

						// Second bone of the lamp
						glPushMatrix(); {
							glRotated(lampArticulationAngle, 0, 1, 0);
							setMaterial(lampArmsColor, 1, 1, 0.5f, 0, 0.5f);
							gluCylinder(quadratic, lampArmRadius, lampArmRadius, lampArmLength, 32, 1);

							// Lamp cone + lightbulb
							glPushMatrix(); {
								glTranslatef(0, 0, lampArmLength);
								glRotated(lampArticulationAngle - 10, 0, 1, 0);
								setMaterial(lampAccentsColor, 1, 1, 0.5f, 0, 0.5f);
								glutSolidSphere(lampArmRadius * 3 / 2, 32, 32);
								gluCylinder(quadratic, lampArmRadius, lampConeWidth, lampArmLength / 2, 32, 32);
								gluCylinder(invQuadratic, lampArmRadius-0.001f, lampConeWidth- 0.001f, lampArmLength / 2, 32, 32);

								resetMaterial();
								glTranslatef(0, 0, 0.035f);
								setMaterial(new GLfloat[]{1,1,0}, 0, 0, 0, 1, 50);
								glutSolidSphere(0.02f, 32, 32);

								glLightfv(GL_LIGHT1, GL_POSITION, new GLfloat[]{0, 0, 0});
								glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, new GLfloat[]{0,0,1});
							}
							glPopMatrix(); // End of lamp cone

						} 
						glPopMatrix();// End of second bone

					}
					glPopMatrix(); // End of first joint
				}
				glPopMatrix(); // End of first bone

			}
			glPopMatrix(); // End of lamp

			//// Start of ball
			//glPushMatrix(); {
			//	glTranslatef(ball_position[x], ball_position[y] + ball_radius, ball_position[z]);
			//	resetMaterial();
			//	glColor3f(0,1,0);
			//	glutSolidSphere(ball_radius, 32, 32);
			//}
			//glPopMatrix(); // End of ball

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
	if (!bIsPaused) {
		lampArticulationAngle += 0.03f * rotationDirection;
		// Si es mayor que dos pi la decrementamos
		if (lampArticulationAngle > MAX_LAMP_ARTICULATION_ANGLE) {
			rotationDirection = -1;
		}
		else if (lampArticulationAngle < 0) {
			rotationDirection = 1;
		}
	}
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
		} 
		else if (cameraMode == CAM_MOVE) {
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
		}
		else if (cameraMode == CAM_MOVE) {
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
		}
		else if (cameraMode == CAM_MOVE) {
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
		}
		else if (cameraMode == CAM_MOVE) {
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

void keyboardKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'l':
		bAmbientLightIsOn = !bAmbientLightIsOn;
		break;
	case '1':
		bLight1IsOn = !bLight1IsOn;
		break;
	case '2':
		bLight2IsOn = !bLight2IsOn;
		break;
	case ' ':
		shadeModel = (shadeModel + 1) % 2; // toggle shade model
		break;
	case 'e':
		light2PosY += 0.01f;
		break;
	case 'q':
		light2PosY -= 0.01f;
		break;
	case 'a':
		light2PosX -= 0.01f;
		break;
	case 'd':
		light2PosX += 0.01f;
		break;
	case 'w':
		light2PosZ -= 0.01f;
		break;
	case 's':
		light2PosZ += 0.01f;
		break;
	}
	glutPostRedisplay();
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
	glutCreateWindow("Etapa 5");

	std::cout << "Welcome! \n"
		<< "Use the following controls to move the camera : \n"
		<< " F1 : Camera panning[default]\n F2 : Move camera\n F3 : Nadir\n"
		<< " F4 : low angle view\n F5 : Normal\n F6 : high angle view[default]\n F7 : Zenith\n";
	std::cout << "Use 2 to enable/disable the free light, WASD to move it horizontally. Q and E to move it vertically \n";

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboardKeys);
	glutIdleFunc(idle);

	// Establecemos que inv_quadratic tiene que tener las normales para el interior
	gluQuadricOrientation(invQuadratic, GLU_INSIDE);

	// El color de fondo sera el blanco (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	gluPerspective(60, (GLfloat)W_WIDTH / (GLfloat)W_HEIGHT, 0.001f, 100.0f);

	glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);

	// Comienza la ejecucion del core de GLUT
	glutMainLoop();
	return 0;
}