// Etapa4.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>

// **** Constants ****
// Initial window size
const int W_WIDTH = 700;
const int W_HEIGHT = 700;

// Toggleable booleans for the full scene. 
bool show_axes = false;
bool is_paused = true;
bool show_fog = false;

// Constant to state the distance a camera jump makes with each input
const GLfloat CAM_JUMP = 0.05f;
const GLfloat CAM_MAX_HEIGHT = 20.0f;

const GLfloat background_color[] = { 1, 1, 1, 1 };

// **** Variables ****
GLfloat rotate_x, rotate_y; // Variables to manage the object rotation
GLfloat eye_x = 1.0f, eye_y = 1.0f, eye_z = 1.0f; // Variables to manage the camera
GLfloat center_x = 0.0f, center_y = 0.0f, center_z = 0.0f;
GLfloat up_x = 0.0f, up_y = 1.0f, up_z = 0.0f;

bool light_is_on[] = { true, true, false };
GLfloat ambient_light_value[] = { 0.5f, 0.5f, 0.5f, 1 }; // values in RGBA // CON luz = 1,1,1, SIN luz = 0,0,0

//GLfloat light1_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Position of light 1
GLfloat light1_value[] = { 1, 1, 1, 1 }; // { 0.0f, 1.0f, 1.0f, 1.0f }; // Values of light 1 (RGBA)

GLfloat light2_posx = 0.5f;
GLfloat light2_posy = 0.6f;
GLfloat light2_posz = 0.25f;
GLfloat light2_ambient[] = { 0,0,0,1 };
GLfloat light2_diffuse[] = { 1,1,1,1 };
GLfloat light2_specular[] = { 1,1,1,1 };

int camera_mode = CAM_PAN; // Variable to state which camera mode is enabled. Panning (F1) is default state.
int shade_model = 1;

// *** Scene values ***
// * Table values *
GLfloat table_color[] = { 164 / RGB_MAX, 114 / RGB_MAX , 44 / RGB_MAX }; // A sort of brown color
GLfloat lamp_accents_color[] = { 100 / RGB_MAX, 100 / RGB_MAX, 100 / RGB_MAX };
GLfloat lamp_arms_color[] = { 184 / RGB_MAX, 184 / RGB_MAX, 184 / RGB_MAX };

GLfloat table_height = 0.5f;

GLfloat table_surface[] = { 0.75f, 0.05f, 0.5f };
GLfloat table_leg[] = { 0.05f, table_height - table_surface[y], 0.05f };

// * Lamp values *
const GLfloat INIT_LAMP_POSITION[] = { table_surface[x] / 4, table_height, table_surface[z] / 2 };
GLfloat lamp_position[] = { INIT_LAMP_POSITION[x], INIT_LAMP_POSITION[y], INIT_LAMP_POSITION[z] };
const GLfloat INIT_LAMP_DIRECTION = 0;
GLfloat lamp_direction = INIT_LAMP_DIRECTION; // Angle to rotate the lamp around its base

GLfloat lamp_base_radius = 0.05f, lamp_base_height = 0.025f;
GLfloat lamp_arm_radius = lamp_base_radius / 5, lamp_arm_length = 0.1f;

GLfloat lamp_cone_width = lamp_arm_radius * 4;
const GLfloat INIT_LAMP_CONE_ANGLE[] = { 0, 80, 0 };
GLfloat lamp_cone_angle[] = { INIT_LAMP_CONE_ANGLE[x], INIT_LAMP_CONE_ANGLE[y], INIT_LAMP_CONE_ANGLE[z] };

const GLfloat INIT_LAMP_ARTICULATION_ANGLE = 90.0f;
GLfloat lamp_articulation_angle = INIT_LAMP_ARTICULATION_ANGLE;
GLfloat MAX_LAMP_ARTICULATION_ANGLE = 90.0f;
GLint rotation_direction = 1;

// * Ball values *
GLfloat ball_position[] = { table_surface[x] * 2 / 3, table_height, table_surface[z] / 2 };
GLfloat ball_radius = 0.05f;

// * Animation values *
const int ANIMATION_LENGTH = 90;
const GLfloat MAX_LAMP_BOUNCE = 0.1f;
int animation_time = 0;
bool lamp_is_jumping = true;

// Funcion que visualiza la escena OpenGL
void Display(void) {
	// Establecemos la escena con profundidad y con iluminación
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (shade_model == 1) {
		glShadeModel(GL_SMOOTH);
	} else {
		glShadeModel(GL_FLAT);
	}

	if (light_is_on[LIGHT_AMBIENT]) {
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light_value);
	} else glDisable(GL_LIGHT0);

	if (light_is_on[LIGHT_1]) {
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_value);
		glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10);
	} else glDisable(GL_LIGHT1);

	if (show_fog) {
		glEnable(GL_FOG);

		glFogfv(GL_FOG_COLOR, background_color);
		glFogi(GL_FOG_MODE, GL_LINEAR);

		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 1.5f);
	} else glDisable(GL_FOG);

	glPushMatrix(); {
		gluLookAt(eye_x, eye_y, eye_z,
			center_x, center_y, center_z,
			up_x, up_y, up_z);

		// Free light 
		if (light_is_on[LIGHT_2]) {
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
			glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

			glPushMatrix(); {
				glTranslatef(light2_posx, light2_posy, light2_posz);

				glLightfv(GL_LIGHT2, GL_POSITION, new GLfloat[]{ 0, 0, 0, 1 });

				setMaterial(new GLfloat[]{ 1, 1, 0 }, 0, 0, 0, 1, 50);
				gluSphere(quadratic, 0.02f, 32, 32);
				resetMaterial();
			}
			glPopMatrix();

		} else glDisable(GL_LIGHT2);

		// Start of scene 
		glPushMatrix(); {

			// We draw the table legs
			setMaterial(table_color, 0.5f, 0.05f, 1.0f, 0.0f, 5.0f);
			draw_parall_material(table_leg[x], table_leg[y], table_leg[z], 128);

			glPushMatrix(); {
				glTranslatef(0, 0, table_surface[z] - table_leg[z]);
				draw_parall_material(table_leg[x], table_leg[y], table_leg[z], 128);
			}
			glPopMatrix();

			glPushMatrix(); {
				glTranslatef(table_surface[x] - table_leg[x], 0, 0);
				draw_parall_material(table_leg[x], table_leg[y], table_leg[z], 128);

				glPushMatrix(); {
					glTranslatef(0, 0, table_surface[z] - table_leg[z]);
					draw_parall_material(table_leg[x], table_leg[y], table_leg[z], 128);
				}
				glPopMatrix();
			}
			glPopMatrix();
			// End of table legs

			// Start of table surface
			glPushMatrix(); {
				glTranslatef(0, table_height - table_surface[y], 0);
				draw_parall_material(table_surface[x], table_surface[y], table_surface[z], 128);
			}
			glPopMatrix(); // End of table surface

			// Start of lamp 
			glPushMatrix(); {
				glTranslatef(lamp_position[x], lamp_position[y], lamp_position[z]);

				glRotated(-90, 1, 0, 0);
				glRotated(lamp_direction, 0, 0, 1);
				// Lamp base
				glPushMatrix(); {
					//glColor3f(0, 0.5f, 0.5f);
					setMaterial(lamp_accents_color, 1, 1, 0.5f, 0, 0.5f);
					gluCylinder(quadratic, lamp_base_radius, lamp_base_radius, lamp_base_height, 32, 1);

					glNormal3f(0, 0, -1);
					draw_ellipse(0, 0, lamp_base_radius, lamp_base_radius, 360);

					// Start of the top of the lamp base
					glPushMatrix(); {
						glTranslatef(0, 0, lamp_base_height);
						glNormal3f(0, 0, 1);
						draw_ellipse(0, 0, lamp_base_radius, lamp_base_radius, 360);

						glTranslatef(lamp_base_radius / 3, 0, 0);
						glColor3f(0, 0, 0);
						GLfloat switch_height = 0.01f;
						if (light_is_on[LIGHT_1]) switch_height /= 3;

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
					setMaterial(lamp_arms_color, 1, 1, 0.5f, 0, 0.5f);
					gluCylinder(quadratic, lamp_arm_radius, lamp_arm_radius, lamp_arm_length, 32, 1);

					// First joint
					glPushMatrix(); {
						glTranslatef(0, 0, lamp_arm_length);
						setMaterial(lamp_accents_color, 1, 1, 0.5f, 0, 0.5f);
						glutSolidSphere(lamp_arm_radius * 3 / 2, 32, 32);

						// Second bone of the lamp
						glPushMatrix(); {
							glRotated(lamp_articulation_angle, 0, 1, 0);
							setMaterial(lamp_arms_color, 1, 1, 0.5f, 0, 0.5f);
							gluCylinder(quadratic, lamp_arm_radius, lamp_arm_radius, lamp_arm_length, 32, 1);

							// Lamp cone + lightbulb
							glPushMatrix(); {
								glTranslatef(0, 0, lamp_arm_length);
								glRotatef(lamp_cone_angle[y], 0, 1, 0);
								glRotatef(lamp_cone_angle[x], 1, 0, 0);
								//glRotatef(lamp_cone_angle[z], 0, 0, 1);
								setMaterial(lamp_accents_color, 1, 1, 0.5f, 0, 0.5f);
								glutSolidSphere(lamp_arm_radius * 3 / 2, 32, 32);
								gluCylinder(quadratic, lamp_arm_radius, lamp_cone_width, lamp_arm_length / 2, 32, 32);
								gluCylinder(inv_quadratic, lamp_arm_radius - 0.001f, lamp_cone_width - 0.001f, lamp_arm_length / 2, 32, 32);

								resetMaterial();
								glTranslatef(0, 0, 0.035f);
								// Set the material with emission to 1 or to 0 depending on if the light is on
								if (light_is_on[LIGHT_1]) {
									setMaterial(new GLfloat[]{ 1,1,0 }, 0, 0, 0, 1, 50);
								} else {
									setMaterial(new GLfloat[]{ 1,1,1 }, 0, 0, 0, 0, 50);
								}
								glutSolidSphere(0.02f, 32, 32);

								glLightfv(GL_LIGHT1, GL_POSITION, new GLfloat[]{ 0, 0, 0 });
								glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, new GLfloat[]{ 0, 0, 1 });
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

		if (show_axes) {
			draw_axes();
		}
	}
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void reset_animation() {
	lamp_position[x] = INIT_LAMP_POSITION[x];
	lamp_position[y] = INIT_LAMP_POSITION[y];
	lamp_position[z] = INIT_LAMP_POSITION[z];

	lamp_cone_angle[x] = INIT_LAMP_CONE_ANGLE[x];
	lamp_cone_angle[y] = INIT_LAMP_CONE_ANGLE[y];
	lamp_cone_angle[z] = INIT_LAMP_CONE_ANGLE[z];

	lamp_direction = INIT_LAMP_DIRECTION;

	lamp_articulation_angle = INIT_LAMP_ARTICULATION_ANGLE;

	animation_time = 0;
}

// Funcion que se ejecuta cuando el sistema no esta ocupado. Sin usar.
void Idle(void) {
	// Animation
	if (!is_paused) {
		if (animation_time <= 30) {
			// From 0 -> 30: Lamp starts jumping
			lamp_position[x] += 0.01f;
			GLfloat lamp_height = sinf(M_PI * (float)animation_time / 15);
			lamp_position[y] = table_height + abs(lamp_height * MAX_LAMP_BOUNCE);
			lamp_articulation_angle += 2 * lamp_height;
		} else if(animation_time <= 35) {
			// From 30 -> 35: Lamp looks back
			lamp_cone_angle[x] += 20;
		} else if (animation_time <= 45) {
			// From 35 -> 45
			// Lamp turns around. Should turn around exactly 180 degrees in 10 seconds. That means 18 degrees per second: d=18 * (t - to)
			// where to is the starting time of the animation (in this case, 35).
			lamp_direction = - 18 * (animation_time - 35);
			lamp_cone_angle[x] -= 10;
			if (animation_time <= 40) lamp_position[y] += 0.02f;
			else lamp_position[y] -= 0.02f;
		} else if(animation_time <= 75) {
			// From 45 -> 75: Lamp jumps back to starting position
			lamp_position[x] -= 0.01f;
			GLfloat lamp_height = sinf(M_PI * (float)(animation_time-45) / 15);
			lamp_position[y] = table_height + abs(lamp_height * MAX_LAMP_BOUNCE);
			lamp_articulation_angle += 2 * lamp_height;
		} else if (animation_time <= 80) {
			// From 75 -> 80: Lamp looks back again
			lamp_cone_angle[x] += 20;
		} else {
			// From 80 -> 90
			// Lamp turns around. Should turn around exactly 180 degrees in 10 seconds with a starting value of 180. That means 18 degrees per second: d=180 - 18 * (t - to) 
			// where to is the starting time of the animation (in this case, 80), but it starts with a value of 180.
			lamp_direction = 180 - 18 * (animation_time - 80);
			lamp_cone_angle[x] -= 10;
			if (animation_time <= 85) lamp_position[y] += 0.02f;
			else lamp_position[y] -= 0.02f;
		}

		animation_time++;
		if (animation_time >= ANIMATION_LENGTH) {
			reset_animation();
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
	} else {
		scale_h = scale_w;
	}

	glViewport(0.0f, 0.0f, W_WIDTH * scale_w, W_HEIGHT * scale_h);
}

// This functions modifies perspective and whether the animation is paused or not. It also functions as a menu listener, 
// but it has this very cool name as artistic liberty because it is also called in other places of the code in which 
// it would not make much sense to call it menuListener.
void setTimeAndSpace(int value) {
	switch (value) {
	case PERSPECTIVE_NADIR:
		eye_x = table_surface[x] / 2;
		eye_y = -1.0f;
		eye_z = table_surface[z] / 2;
		center_x = table_surface[x] / 2;
		center_y = 0.0f;
		center_z = table_surface[z] / 2;
		up_x = 0.0f;
		up_y = 0.0f;
		up_z = 1.0f;
		break;
	case PERSPECTIVE_LOW_ANGLE:
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
	case PERSPECTIVE_NORMAL:
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
	case PERSPECTIVE_ZENITH:
		eye_x = table_surface[x] / 2;
		eye_y = 1.5f;
		eye_z = table_surface[z] / 2;
		center_x = table_surface[x] / 2;
		center_y = 0.0f;
		center_z = table_surface[z] / 2;
		up_x = 0.0f;
		up_y = 0.0f;
		up_z = -1.0f;
		break;
	case ANIMATION_PAUSE:
		is_paused = !is_paused;
		break;
	case ANIMATION_RESET:
		reset_animation();
		break;
	case AXES_TOGGLE:
		show_axes = !show_axes;
		break;
	case FOG_TOGGLE:
		show_fog = !show_fog;
		break;
	default:
		eye_x = 1.0f;
		eye_y = 1.0f;
		eye_z = 1.0f;
		center_x = 0.0f;
		center_y = 0.0f;
		center_z = 0.0f;
		up_x = 0.0f;
		up_y = 1.0f;
		up_z = 0.0f;
	}
}

void toggleLights(int light) {
	light_is_on[light] = !light_is_on[light];
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
		setTimeAndSpace(PERSPECTIVE_NADIR);
		break;
	case GLUT_KEY_F4:
		std::cout << "Set: Low angle view\n";
		setTimeAndSpace(PERSPECTIVE_LOW_ANGLE);
		break;
	case GLUT_KEY_F5:
		std::cout << "Set: Normal view\n";
		setTimeAndSpace(PERSPECTIVE_NORMAL);
		break;
	case GLUT_KEY_F6:
		std::cout << "Set: High angle view\n";
		setTimeAndSpace(PERSPECTIVE_DEFAULT);
		break;
	case GLUT_KEY_F7:
		std::cout << "Set: Zenith view\n [Camera move not supported]\n";
		setTimeAndSpace(PERSPECTIVE_ZENITH);
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
		} else if (camera_mode == CAM_MOVE) {
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
		} else if (camera_mode == CAM_MOVE) {
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
		} else if (camera_mode == CAM_MOVE) {
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
		} else if (camera_mode == CAM_MOVE) {
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
		toggleLights(LIGHT_AMBIENT);
		break;
	case '1':
		toggleLights(LIGHT_1);
		break;
	case '2':
		toggleLights(LIGHT_2);
		break;
	case ' ':
		shade_model = (shade_model + 1) % 2; // toggle shade model
		break;
	case 'e':
		light2_posy += 0.01f;
		break;
	case 'q':
		light2_posy -= 0.01f;
		break;
	case 'a':
		light2_posx -= 0.01f;
		break;
	case 'd':
		light2_posx += 0.01f;
		break;
	case 'w':
		light2_posz -= 0.01f;
		break;
	case 's':
		light2_posz += 0.01f;
		break;
	case 'p':
		setTimeAndSpace(ANIMATION_PAUSE);
		break;
	case 'f':
		setTimeAndSpace(FOG_TOGGLE);
		break;
	}
	glutPostRedisplay();
}

void createMenu() {
	int idMenuDefault = glutCreateMenu(setTimeAndSpace);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutAddMenuEntry("Default perspective", PERSPECTIVE_DEFAULT);

	int idMenuNonDefault = glutCreateMenu(setTimeAndSpace);
	glutAddMenuEntry("Nadir", PERSPECTIVE_NADIR);
	glutAddMenuEntry("Low angle", PERSPECTIVE_LOW_ANGLE);
	glutAddMenuEntry("Normal", PERSPECTIVE_NORMAL);
	glutAddMenuEntry("Zenith", PERSPECTIVE_ZENITH);

	glutSetMenu(idMenuDefault);
	glutAddSubMenu("Other perspectives", idMenuNonDefault);

	int idMenuLights = glutCreateMenu(toggleLights);
	glutAddMenuEntry("Ambient light", LIGHT_AMBIENT);
	glutAddMenuEntry("Lamp light", LIGHT_1);
	glutAddMenuEntry("Free light", LIGHT_2);

	glutSetMenu(idMenuDefault);
	glutAddSubMenu("Toggle lights", idMenuLights);

	glutAddMenuEntry("Toggle fog", FOG_TOGGLE);

	glutAddMenuEntry("Toggle animation", ANIMATION_PAUSE);
	glutAddMenuEntry("Restart animation", ANIMATION_RESET);

	glutAddMenuEntry("Toggle axes", AXES_TOGGLE);
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
	glutCreateWindow("Etapa 6");

	std::cout << "Welcome! \n"
		<< "Use the following controls to move the camera : \n"
		<< " F1 : Camera panning[default]\n F2 : Move camera\n F3 : Nadir\n"
		<< " F4 : low angle view\n F5 : Normal\n F6 : high angle view[default]\n F7 : Zenith\n";
	std::cout << "Use F to enable or disable fog.\n";
	std::cout << "Use 2 to enable/disable the free light, WASD to move it horizontally. Q and E to move it vertically.\n";
	std::cout << "Press P to pause or play the animation.\n";

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(Display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboardKeys);
	glutIdleFunc(Idle);
	createMenu();

	// Establecemos que inv_quadratic tiene que tener las normales para el interior
	gluQuadricOrientation(inv_quadratic, GLU_INSIDE);

	// El color de fondo sera el blanco (RGBA, RGB + Alpha channel)
	glClearColor(background_color[RED], background_color[GREEN], background_color[BLUE], background_color[ALPHA]);
	gluPerspective(60, (GLfloat)W_WIDTH / (GLfloat)W_HEIGHT, 0.001f, 100.0f);

	glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);

	// Comienza la ejecucion del core de GLUT
	glutMainLoop();
	return 0;
}