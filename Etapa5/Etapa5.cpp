// Etapa4.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>

// **** Constants ****
// Initial window size
const int W_WIDTH = 700;
const int W_HEIGHT = 700;

// Boolean to state if axes are to be shown. 
const bool SHOW_AXES = true;
const bool IS_IDLE = true;

// Constant to state the distance a camera jump makes with each input
const GLfloat CAM_JUMP = 0.05f;
const GLfloat CAM_MAX_HEIGHT = 20.0f;

// **** Variables ****
GLfloat rotate_x, rotate_y; // Variables to manage the object rotation
GLfloat eye_x = 1.0f, eye_y = 1.0f, eye_z = 1.0f; // Variables to manage the camera
GLfloat center_x = 0.0f, center_y = 0.0f, center_z = 0.0f;
GLfloat up_x = 0.0f, up_y = 1.0f, up_z = 0.0f;

bool ambient_light_is_on = true;
bool light1_is_on = true;
GLfloat ambient_light_value[] = {0.5f,0.5f,0.5f,1}; // values in RGBA // CON luz = 1,1,1, SIN luz = 0,0,0

//GLfloat light1_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Position of light 1
GLfloat light1_value[] = {1, 1, 1, 1}; // 0.0f, 1.0f, 1.0f, 1.0f }; // Values of light 1 (RGBA)

int camera_mode = CAM_PAN; // Variable to state which camera mode is enabled. Panning (F1) is default state.

// Scene values
GLfloat table_color[] = {164 / RGB_MAX, 114 / RGB_MAX , 44 / RGB_MAX}; // A sort of brown color
GLfloat lamp_accents_color[] = { 100 / RGB_MAX, 100 / RGB_MAX, 100 / RGB_MAX};
GLfloat lamp_arms_color[] = {184 / RGB_MAX, 184 / RGB_MAX, 184 / RGB_MAX};

GLfloat table_height = 0.5f;

GLfloat table_surface[] = { 0.75f, 0.05f, 0.5f };
GLfloat table_leg[] = { 0.05f, table_height - table_surface[y], 0.05f };

GLfloat lamp_position[] = { table_surface[x] / 3, table_height, table_surface[z] / 2 };

GLfloat lamp_base_radius = 0.05f, lamp_base_height = 0.025f;
GLfloat lamp_arm_radius = lamp_base_radius / 5, lamp_arm_length = 0.1f;

GLfloat lamp_cone_width = lamp_arm_radius * 4;

GLfloat lamp_angle = 90.0f;
GLfloat MAX_LAMP_ANGLE = 90.0f;

// Funcion que visualiza la escena OpenGL
void Display(void) {
	// Establecemos la escena con profundidad y con iluminación
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	if (ambient_light_is_on) {
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light_value);
	} else glDisable(GL_LIGHT0);

	if (light1_is_on) {
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_value);
	} else glDisable(GL_LIGHT1);

	glPushMatrix(); {
		gluLookAt(eye_x, eye_y, eye_z,
			center_x, center_y, center_z,
			up_x, up_y, up_z);

		// Start of scene 
		glPushMatrix(); {

			// We draw the table legs
			setMaterial(table_color, 0.5f, 1.0f, 1.0f, 0.0f, 50.0f);
			draw_parall_material(table_leg[x], table_leg[y], table_leg[z]);

			glPushMatrix(); {
				glTranslatef(0, 0, table_surface[z] - table_leg[z]);
				draw_parall_material(table_leg[x], table_leg[y], table_leg[z]);
			}
			glPopMatrix();

			glPushMatrix(); {
				glTranslatef(table_surface[x] - table_leg[x], 0, 0);
				draw_parall_material(table_leg[x], table_leg[y], table_leg[z]);

				glPushMatrix(); {
					glTranslatef(0, 0, table_surface[z] - table_leg[z]);
					draw_parall_material(table_leg[x], table_leg[y], table_leg[z]);
				}
				glPopMatrix();
			}
			glPopMatrix();
			// End of table legs

			// Start of table surface
			glPushMatrix(); {
				glTranslatef(0, table_height - table_surface[y], 0);
				draw_parall_material(table_surface[x], table_surface[y], table_surface[z]);
			}
			glPopMatrix(); // End of table surface

			// Start of lamp 
			glPushMatrix(); {
				glTranslatef(lamp_position[x], lamp_position[y], lamp_position[z]);

				glRotated(-90, 1, 0, 0);
				// Lamp base
				glPushMatrix(); {
					//glColor3f(0, 0.5f, 0.5f);
					setMaterial(lamp_accents_color, 1, 1, 0.5f, 0, 0.5f);
					gluCylinder(quadratic, lamp_base_radius, lamp_base_radius, lamp_base_height, 32, 1);

					// Start of the top of the lamp base
					glPushMatrix(); {
						glTranslatef(0, 0, lamp_base_height);
						draw_ellipse(0, 0, lamp_base_radius, lamp_base_radius, 360);
					}
					glPopMatrix(); // End of top of lamp base
				}
				glPopMatrix(); // End of lamp base

				// First bone of the lamp
				glPushMatrix(); {
					glRotated(-30, 0, 1, 0);
					setMaterial(lamp_arms_color, 1, 1, 0.5f, 0, 0.5f);
					gluCylinder(quadratic, lamp_arm_radius, lamp_arm_radius, lamp_arm_length, 32, 1);

					// First joint
					glPushMatrix(); {
						glTranslatef(0,0,lamp_arm_length);
						setMaterial(lamp_accents_color, 1, 1, 0.5f, 0, 0.5f);
						glutSolidSphere(lamp_arm_radius * 3/2, 32, 32);

						// Second bone of the lamp
						glPushMatrix(); {
							glRotated(lamp_angle, 0, 1, 0);
							setMaterial(lamp_arms_color, 1, 1, 0.5f, 0, 0.5f);
							gluCylinder(quadratic, lamp_arm_radius, lamp_arm_radius, lamp_arm_length, 32, 1);

							// Lamp cone
							glPushMatrix(); {
								glTranslatef(0, 0, lamp_arm_length);
								glRotated(lamp_angle - 10, 0, 1, 0);
								setMaterial(lamp_accents_color, 1, 1, 0.5f, 0, 0.5f);
								glutSolidSphere(lamp_arm_radius * 3 / 2, 32, 32);
								gluCylinder(quadratic, lamp_arm_radius, lamp_cone_width, lamp_arm_length / 2, 32, 32);

								glColor3f(1, 1, 1);
								GLfloat white[] = { 1,1,0 };
								setMaterial(white, 0, 0, 0, 1, 50);
								glutSolidSphere(0.01f, 32, 32);

								glLightiv(GL_LIGHT1, GL_POSITION, new GLint[]{0, 0, 0, 1});
								glLightiv(GL_LIGHT1, GL_SPOT_DIRECTION, new GLint[]{ 0, 0, 1 });
								glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
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

		}
		glPopMatrix(); // End of scene

		if (SHOW_AXES) {
			print_axes();
		}
	}
	glPopMatrix();

	resetMaterial();

	glFlush();
	glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado. Sin usar.
void Idle(void) {
	// Incrementamos el angulo
	lamp_angle += 0.03f;
	// Si es mayor que dos pi la decrementamos
	if (lamp_angle > 360)
		lamp_angle -= 360;
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
	GLfloat module;
	GLfloat unit_x, unit_y, unit_z;

	switch (key) {
	case GLUT_KEY_F1:
		camera_mode = CAM_PAN;
		std::cout << "Camera control mode: panning\n";
		break;
	case GLUT_KEY_F2:
		camera_mode = CAM_MOVE;
		std::cout << "Camera control mode: moving\n";
		break;
	case GLUT_KEY_F3:
		std::cout << "Set: Nadir plane\n [Camera move not supported]\n";
		eye_x = 0.5f;
		eye_y = -1.0f;
		eye_z = 0.5f;
		center_x = 0.5f;
		center_y = 0.0f;
		center_z = 0.5f;
		up_x = 0.0f;
		up_y = 0.0f;
		up_z = 1.0f;
		break;
	case GLUT_KEY_F4:
		std::cout << "Set: Low angle view\n";
		eye_x = 1.0f;
		eye_y = 0.0f;
		eye_z = 1.0f;
		center_x = 0.5f;
		center_y = 0.5f;
		center_z = 0.5f;
		up_x = 0.0f;
		up_y = 1.0f;
		up_z = 0.0f;
		break;
	case GLUT_KEY_F5:
		std::cout << "Set: Normal view\n";
		eye_x = 1.0f;
		eye_y = 0.5f;
		eye_z = 1.0f;
		center_x = 0.5f;
		center_y = 0.5f;
		center_z = 0.5f;
		up_x = 0.0f;
		up_y = 1.0f;
		up_z = 0.0f;
		break;
	case GLUT_KEY_F6:
		std::cout << "Set: High angle view\n";
		eye_x = 1.0f;
		eye_y = 1.0f;
		eye_z = 1.0f;
		center_x = 0.0f;
		center_y = 0.0f;
		center_z = 0.0f;
		up_x = 0.0f;
		up_y = 1.0f;
		up_z = 0.0f;
		break;
	case GLUT_KEY_F7:
		std::cout << "Set: Zenith view\n [Camera move not supported]\n";
		eye_x = 0.5f;
		eye_y = 1.0f;
		eye_z = 0.5f;
		center_x = 0.5f;
		center_y = 0.0f;
		center_z = 0.5f;
		up_x = 0.0f;
		up_y = 0.0f;
		up_z = 1.0f;
		break;
	case GLUT_KEY_RIGHT:
		if (camera_mode == CAM_PAN) {
			// Camera should rotate around itself clockwise
			// We will use the same idea as a rotation transformation to move the camera center around 
			// the axis parallel to the y-axis and that passes through the camera eye
			center_x -= eye_x;
			center_z -= eye_z; // We move it to where it would be if eye was in (0,y,0)

			center_x = center_x * cosf(CAM_JUMP) + center_z * sinf(CAM_JUMP);
			center_z = center_z * cosf(CAM_JUMP) - center_x * sinf(CAM_JUMP);

			center_x += eye_x;
			center_z += eye_z;
		}
		else if (camera_mode == CAM_MOVE) {
			// Camera should move to its right

			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unit_x = center_x - eye_x;
			unit_y = center_y - eye_y;
			unit_z = center_z - eye_z;

			module = sqrt(unit_x * unit_x + unit_y * unit_y + unit_z * unit_z);
			unit_x /= module;
			unit_y /= module;
			unit_z /= module;

			// The right vector is calculated with vectorial product of unit x up;
			// right_x = -unit_z;
			// right_y = 0;
			// right_z = unit_x;
			// After that, the new center and eye position equal:
			// center += right; eye += right;
			center_x -= unit_z * CAM_JUMP;
			eye_x -= unit_z * CAM_JUMP;
			center_z += unit_x * CAM_JUMP;
			eye_z += unit_x * CAM_JUMP;
		}
		break;
	case GLUT_KEY_LEFT:
		if (camera_mode == CAM_PAN) {
			center_x -= eye_x;
			center_z -= eye_z; // We move it to where it would be if eye was in (0,y,0)

			center_x = center_x * cosf(-CAM_JUMP) + center_z * sinf(-CAM_JUMP);
			center_z = center_z * cosf(-CAM_JUMP) - center_x * sinf(-CAM_JUMP);

			center_x += eye_x;
			center_z += eye_z;
		}
		else if (camera_mode == CAM_MOVE) {
			// Camera should move to its left

			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unit_x = center_x - eye_x;
			unit_y = center_y - eye_y;
			unit_z = center_z - eye_z;

			module = sqrt(unit_x * unit_x + unit_y * unit_y + unit_z * unit_z);
			unit_x /= module;
			unit_y /= module;
			unit_z /= module;

			// The right vector is calculated with vectorial product of unit x up;
			// right_x = -unit_z;
			// right_y = 0;
			// right_z = unit_x;
			// After that, the new center and eye position equal:
			// center -= right; eye -= right;
			center_x += unit_z * CAM_JUMP;
			eye_x += unit_z * CAM_JUMP;
			center_z -= unit_x * CAM_JUMP;
			eye_z -= unit_x * CAM_JUMP;
		}
		break;
	case GLUT_KEY_UP:
		if (camera_mode == CAM_PAN && center_y < CAM_MAX_HEIGHT) {
			center_y += CAM_JUMP;
		}
		else if (camera_mode == CAM_MOVE) {
			// UP KEY -> move forward

			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unit_x = center_x - eye_x;
			unit_y = center_y - eye_y;
			unit_z = center_z - eye_z;

			module = sqrt(unit_x * unit_x + unit_y * unit_y + unit_z * unit_z);
			unit_x /= module;
			unit_y /= module;
			unit_z /= module;

			center_x += unit_x * CAM_JUMP;
			center_y += unit_y * CAM_JUMP;
			center_z += unit_z * CAM_JUMP;
			eye_x += unit_x * CAM_JUMP;
			eye_y += unit_y * CAM_JUMP;
			eye_z += unit_z * CAM_JUMP;

		}
		break;
	case GLUT_KEY_DOWN:
		if (camera_mode == CAM_PAN && center_y > -CAM_MAX_HEIGHT) {
			center_y -= CAM_JUMP;
		}
		else if (camera_mode == CAM_MOVE) {
			// DOWN KEY -> move backwards

			// We need to get the unit vector of the vector eye -> center
			// u = v / module(v);
			unit_x = center_x - eye_x;
			unit_y = center_y - eye_y;
			unit_z = center_z - eye_z;

			module = sqrt(unit_x * unit_x + unit_y * unit_y + unit_z * unit_z);
			unit_x /= module;
			unit_y /= module;
			unit_z /= module;

			center_x -= unit_x * CAM_JUMP;
			center_y -= unit_y * CAM_JUMP;
			center_z -= unit_z * CAM_JUMP;
			eye_x -= unit_x * CAM_JUMP;
			eye_y -= unit_y * CAM_JUMP;
			eye_z -= unit_z * CAM_JUMP;

		}
		break;
	}

	glutPostRedisplay(); // Solicitar actualización de visualización
}

void keyboardKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'l':
		ambient_light_is_on = !ambient_light_is_on;
		break;
	case '1':
		light1_is_on = !light1_is_on;
		break;
	}


	glutPostRedisplay();
}

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
	glutCreateWindow("Etapa 5");

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(Display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboardKeys);
	if(!IS_IDLE) glutIdleFunc(Idle);

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