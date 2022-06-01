// Etapa6.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <Practicas.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// **** Constants ****
// Initial window size
const int W_WIDTH = 700;
const int W_HEIGHT = 700;

const bool PRINT_CONSOLE = false;

// Constant to state the distance a camera jump makes with each input
const GLfloat CAM_JUMP = 0.05f;
const GLfloat CAM_MAX_HEIGHT = 20.0f;

const GLfloat backgroundColor[] = { 1, 1, 1, 1 };

// **** Variables ****
// Toggleable booleans for the full scene. 
bool bShowAxes = false;
bool bIsPaused = true;
bool bShowFog = false;
bool bSmoothCam = true;

// Variables for perspective
GLfloat eyeX = 1.0f, eyeY = 1.0f, eyeZ = 1.0f;
GLfloat centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
GLfloat upX = 0.0f, upY = 1.0f, upZ = 0.0f;

// Variables for perspective when using smooth_camera
GLfloat realEye[] = { eyeX, eyeY, eyeZ }; 
GLfloat realCenter[] = { centerX, centerY, centerZ };
GLfloat realUp[] = {upX, upY, upZ};

bool bLightIsOn[] = { true, true, false };
GLfloat ambientLightValue[] = { 0.5f, 0.5f, 0.5f, 1 }; // values in RGBA // CON luz = 1,1,1, SIN luz = 0,0,0

//GLfloat light1_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Position of light 1
GLfloat light1Value[] = { 1, 1, 1, 1 }; // { 0.0f, 1.0f, 1.0f, 1.0f }; // Values of light 1 (RGBA)

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
GLfloat tableColor[] = { 164 / RGB_MAX, 114 / RGB_MAX , 44 / RGB_MAX }; // A sort of brown color
GLfloat lampAccentsColor[] = { 100 / RGB_MAX, 100 / RGB_MAX, 100 / RGB_MAX };
GLfloat lampArmsColor[] = { 184 / RGB_MAX, 184 / RGB_MAX, 184 / RGB_MAX };

GLfloat tableHeight = 0.5f;

GLfloat tableSurface[] = { 0.75f, 0.05f, 0.5f };
GLfloat tableLeg[] = { 0.05f, tableHeight - tableSurface[y], 0.05f };

// * Lamp values *
const GLfloat INIT_LAMP_POSITION[] = { tableSurface[x] / 4, tableHeight, tableSurface[z] / 2 };
GLfloat lampPosition[] = { INIT_LAMP_POSITION[x], INIT_LAMP_POSITION[y], INIT_LAMP_POSITION[z] };
const GLfloat INIT_LAMP_DIRECTION = 0;
GLfloat lampDirection = INIT_LAMP_DIRECTION; // Angle to rotate the lamp around its base

GLfloat lampBaseRadius = 0.05f, lampBaseHeight = 0.025f;
GLfloat lampArmRadius = lampBaseRadius / 5, lampArmLength = 0.1f;

GLfloat lampConeWidth = lampArmRadius * 4;
const GLfloat INIT_LAMP_CONE_DIRECTION[] = { 0, 80, 0 };
GLfloat lampConeDirection[] = { INIT_LAMP_CONE_DIRECTION[x], INIT_LAMP_CONE_DIRECTION[y], INIT_LAMP_CONE_DIRECTION[z] };

const GLfloat INIT_LAMP_ARTICULATION_ANGLE = 90.0f;
GLfloat lampArticulationAngle = INIT_LAMP_ARTICULATION_ANGLE;
GLfloat MAX_LAMP_ARTICULATION_ANGLE = 90.0f;
GLint rotationDirection = 1;

// * Ball values *
GLfloat ballPosition[] = { tableSurface[x] * 2 / 3, tableHeight, tableSurface[z] / 2 };
GLfloat ballRadius = 0.05f;

// * Animation values *
const int ANIMATION_LENGTH = 90;
const GLfloat MAX_LAMP_BOUNCE = 0.1f;
int animationTime = 0;
bool bLampIsJumping = true;

// * Texture values * 
int woodWidth, woodHeight, nrChannels;
unsigned char* data = stbi_load("wood.jpg", &woodWidth, &woodHeight, &nrChannels, 0);
unsigned int woodTexture;

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

	if (bLightIsOn[LIGHT_AMBIENT]) {
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightValue);
	} else glDisable(GL_LIGHT0);

	if (bLightIsOn[LIGHT_1]) {
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Value);
		glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10);
	} else glDisable(GL_LIGHT1);

	if (bShowFog) {
		glEnable(GL_FOG);

		glFogfv(GL_FOG_COLOR, backgroundColor);
		glFogi(GL_FOG_MODE, GL_LINEAR);

		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 1.5f);
	} else glDisable(GL_FOG);

	glPushMatrix(); {
		if (bSmoothCam)	{
			gluLookAt(realEye[x], realEye[y], realEye[z],
				realCenter[x], realCenter[y], realCenter[z],
				realUp[x], realUp[y], realUp[z]);
		} else {
			gluLookAt(eyeX, eyeY, eyeZ,
				centerX, centerY, centerZ,
				upX, upY, upZ);
		}

		// Free light 
		if (bLightIsOn[LIGHT_2]) {
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_AMBIENT, light2Ambient);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, light2Diffuse);
			glLightfv(GL_LIGHT2, GL_SPECULAR, light2Specular);

			glPushMatrix(); {
				glTranslatef(light2PosX, light2PosY, light2PosZ);

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
			setMaterial(tableColor, 0.5f, 0.05f, 1.0f, 0.0f, 5.0f);
			drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z], 128);

			glPushMatrix(); {
				glTranslatef(0, 0, tableSurface[z] - tableLeg[z]);
				drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z], 128);
			}
			glPopMatrix();

			glPushMatrix(); {
				glTranslatef(tableSurface[x] - tableLeg[x], 0, 0);
				drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z], 128);

				glPushMatrix(); {
					glTranslatef(0, 0, tableSurface[z] - tableLeg[z]);
					drawParallMaterial(tableLeg[x], tableLeg[y], tableLeg[z], 128);
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
				glRotated(lampDirection, 0, 0, 1);
				// Lamp base
				glPushMatrix(); {
					//glColor3f(0, 0.5f, 0.5f);
					setMaterial(lampAccentsColor, 1, 1, 0.5f, 0, 0.5f);
					gluCylinder(quadratic, lampBaseRadius, lampBaseRadius, lampBaseHeight, 32, 1);

					glNormal3f(0, 0, -1);
					drawEllipse(0, 0, lampBaseRadius, lampBaseRadius, 360);

					// Start of the top of the lamp base
					glPushMatrix(); {
						glTranslatef(0, 0, lampBaseHeight);
						glNormal3f(0, 0, 1);
						drawEllipse(0, 0, lampBaseRadius, lampBaseRadius, 360);

						glTranslatef(lampBaseRadius / 3, 0, 0);
						glColor3f(0, 0, 0);
						GLfloat switchHeight = 0.01f;
						if (bLightIsOn[LIGHT_1]) switchHeight /= 3;

						gluCylinder(quadratic, 0.005f, 0.005f, switchHeight, 16, 1);
						glTranslatef(0, 0, switchHeight);
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
						glTranslatef(0, 0, lampArmLength);
						setMaterial(lampAccentsColor, 1, 1, 0.5f, 0, 0.5f);
						glutSolidSphere(lampArmRadius * 3 / 2, 32, 32);

						// Second bone of the lamp
						glPushMatrix(); {
							glRotated(lampArticulationAngle, 0, 1, 0);
							setMaterial(lampArmsColor, 1, 1, 0.5f, 0, 0.5f);
							gluCylinder(quadratic, lampArmRadius, lampArmRadius, lampArmLength, 32, 1);

							// Lamp cone + lightbulb
							glPushMatrix(); {
								glTranslatef(0, 0, lampArmLength);
								glRotatef(lampConeDirection[y], 0, 1, 0);
								glRotatef(lampConeDirection[x], 1, 0, 0);
								setMaterial(lampAccentsColor, 1, 1, 0.5f, 0, 0.5f);
								glutSolidSphere(lampArmRadius * 3 / 2, 32, 32);
								gluCylinder(quadratic, lampArmRadius, lampConeWidth, lampArmLength / 2, 32, 32);
								gluCylinder(invQuadratic, lampArmRadius - 0.001f, lampConeWidth - 0.001f, lampArmLength / 2, 32, 32);

								resetMaterial();
								glTranslatef(0, 0, 0.035f);
								// Set the material with emission to 1 or to 0 depending on if the light is on
								if (bLightIsOn[LIGHT_1]) {
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

		if (bShowAxes) {
			drawAxes();
		}
	}
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void resetAnimation() {
	lampPosition[x] = INIT_LAMP_POSITION[x];
	lampPosition[y] = INIT_LAMP_POSITION[y];
	lampPosition[z] = INIT_LAMP_POSITION[z];

	lampConeDirection[x] = INIT_LAMP_CONE_DIRECTION[x];
	lampConeDirection[y] = INIT_LAMP_CONE_DIRECTION[y];
	lampConeDirection[z] = INIT_LAMP_CONE_DIRECTION[z];

	lampDirection = INIT_LAMP_DIRECTION;

	lampArticulationAngle = INIT_LAMP_ARTICULATION_ANGLE;

	animationTime = 0;
}

// Funcion que se ejecuta cuando el sistema no esta ocupado.
void idle(void) {
	// Animation
	if (!bIsPaused) {
		if (animationTime <= 30) {
			// From 0 -> 30: Lamp starts jumping
			lampPosition[x] += 0.01f;
			GLfloat lampHeight = sinf(M_PI * (float)animationTime / 15);
			lampPosition[y] = tableHeight + abs(lampHeight * MAX_LAMP_BOUNCE);
			lampArticulationAngle += 2 * lampHeight;
		} else if(animationTime <= 35) {
			// From 30 -> 35: Lamp looks back
			lampConeDirection[x] += 20;
		} else if (animationTime <= 45) {
			// From 35 -> 45
			// Lamp turns around. Should turn around exactly 180 degrees in 10 seconds. That means 18 degrees per second: d=18 * (t - to)
			// where to is the starting time of the animation (in this case, 35).
			lampDirection = - 18 * (animationTime - 35);
			lampConeDirection[x] -= 10;
			if (animationTime <= 40) lampPosition[y] += 0.02f;
			else lampPosition[y] -= 0.02f;
		} else if(animationTime <= 75) {
			// From 45 -> 75: Lamp jumps back to starting position
			lampPosition[x] -= 0.01f;
			GLfloat lampHeight = sinf(M_PI * (float)(animationTime-45) / 15);
			lampPosition[y] = tableHeight + abs(lampHeight * MAX_LAMP_BOUNCE);
			lampArticulationAngle += 2 * lampHeight;
		} else if (animationTime <= 80) {
			// From 75 -> 80: Lamp looks back again
			lampConeDirection[x] += 20;
		} else {
			// From 80 -> 90
			// Lamp turns around. Should turn around exactly 180 degrees in 10 seconds with a starting value of 180. That means 18 degrees per second: d=180 - 18 * (t - to) 
			// where to is the starting time of the animation (in this case, 80), but it starts with a value of 180.
			lampDirection = 180 - 18 * (animationTime - 80);
			lampConeDirection[x] -= 10;
			if (animationTime <= 85) lampPosition[y] += 0.02f;
			else lampPosition[y] -= 0.02f;
		}

		animationTime++;
		if (animationTime >= ANIMATION_LENGTH) {
			resetAnimation();
		}
	}

	if (bSmoothCam) {
		smoothTransition(&realCenter[x], centerX, 0.15f);
		smoothTransition(&realCenter[y], centerY, 0.15f);
		smoothTransition(&realCenter[z], centerZ, 0.15f);
		smoothTransition(&realEye[x], eyeX, 0.15f);
		smoothTransition(&realEye[y], eyeY, 0.15f);
		smoothTransition(&realEye[z], eyeZ, 0.15f);
		smoothTransition(&realUp[x], upX, 0.15f);
		smoothTransition(&realUp[y], upY, 0.15f);
		smoothTransition(&realUp[z], upZ, 0.15f);
	}

	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

void reshape(int width, int height) {
	const float originalAspectRatio = (float)W_WIDTH / (float)W_HEIGHT;
	const float newAspectRatio = (float)width / (float)height;

	float widthScale = (float)width / (float)W_WIDTH;
	float heightScale = (float)height / (float)W_HEIGHT;
	if (newAspectRatio > originalAspectRatio) {
		widthScale = heightScale;
	} else {
		heightScale = widthScale;
	}

	glViewport(0.0f, 0.0f, W_WIDTH * widthScale, W_HEIGHT * heightScale);
}

// This functions modifies perspective and whether the animation is paused or not. It also functions as a menu listener, 
// but it has this very cool name as artistic liberty because it is also called in other places of the code in which 
// it would not make much sense to call it menuListener.
void setTimeAndSpace(int value) {
	switch (value) {
	case PERSPECTIVE_NADIR:
		eyeX = tableSurface[x] / 2;
		eyeY = -1.0f;
		eyeZ = tableSurface[z] / 2;
		centerX = tableSurface[x] / 2;
		centerY = 0.0f;
		centerZ = tableSurface[z] / 2;
		upX = 0.0f;
		upY = 0.0f;
		upZ = 1.0f;
		break;
	case PERSPECTIVE_LOW_ANGLE:
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
	case PERSPECTIVE_NORMAL:
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
	case PERSPECTIVE_ZENITH:
		eyeX = tableSurface[x] / 2;
		eyeY = 1.5f;
		eyeZ = tableSurface[z] / 2;
		centerX = tableSurface[x] / 2;
		centerY = 0.0f;
		centerZ = tableSurface[z] / 2;
		upX = 0.0f;
		upY = 0.0f;
		upZ = -1.0f;
		break;
	case ANIMATION_PAUSE:
		bIsPaused = !bIsPaused;
		break;
	case ANIMATION_RESET:
		resetAnimation();
		break;
	case AXES_TOGGLE:
		bShowAxes = !bShowAxes;
		break;
	case FOG_TOGGLE:
		bShowFog = !bShowFog;
		break;
	case SMOOTH_CAM:
		bSmoothCam = !bSmoothCam;
		break;
	default:
		eyeX = 1.0f;
		eyeY = 1.0f;
		eyeZ = 1.0f;
		centerX = 0.0f;
		centerY = 0.0f;
		centerZ = 0.0f;
		upX = 0.0f;
		upY = 1.0f;
		upZ = 0.0f;
	}
}

void toggleLights(int light) {
	bLightIsOn[light] = !bLightIsOn[light];
}

// Función para escuchar las teclas
void specialKeys(int key, int x, int y) {
	GLfloat modulus; // Modulus vector
	GLfloat unitaryX, unitaryY, unitaryZ; // Unitary vector.

	switch (key) {
	case GLUT_KEY_F1:
		cameraMode = CAM_PAN;
		if(PRINT_CONSOLE) std::cout << "Camera control mode: panning\n";
		break;
	case GLUT_KEY_F2:
		cameraMode = CAM_MOVE;
		if (PRINT_CONSOLE) std::cout << "Camera control mode: moving\n";
		break;
	case GLUT_KEY_F3:
		if (PRINT_CONSOLE) std::cout << "Set: Nadir plane\n [Camera move not supported]\n";
		setTimeAndSpace(PERSPECTIVE_NADIR);
		break;
	case GLUT_KEY_F4:
		if (PRINT_CONSOLE) std::cout << "Set: Low angle view\n";
		setTimeAndSpace(PERSPECTIVE_LOW_ANGLE);
		break;
	case GLUT_KEY_F5:
		if (PRINT_CONSOLE) std::cout << "Set: Normal view\n";
		setTimeAndSpace(PERSPECTIVE_NORMAL);
		break;
	case GLUT_KEY_F6:
		if (PRINT_CONSOLE) std::cout << "Set: High angle view\n";
		setTimeAndSpace(PERSPECTIVE_DEFAULT);
		break;
	case GLUT_KEY_F7:
		if (PRINT_CONSOLE) std::cout << "Set: Zenith view\n [Camera move not supported]\n";
		setTimeAndSpace(PERSPECTIVE_ZENITH);
		break;
	case GLUT_KEY_RIGHT:
		if (cameraMode == CAM_PAN) {
			// Camera should rotate around itself counter-clockwise
			// We will use the same idea as a rotation transformation to move the camera center around 
			// the axis parallel to the y-axis and that passes through the camera eye
			centerX -= eyeX;
			centerZ -= eyeZ; // We move it to where it would be if eye was in (0,y,0)

			centerX = centerX * cosf(-CAM_JUMP) + centerZ * sinf(-CAM_JUMP);
			centerZ = centerZ * cosf(-CAM_JUMP) - centerX * sinf(-CAM_JUMP);

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

			centerX = centerX * cosf(CAM_JUMP) + centerZ * sinf(CAM_JUMP);
			centerZ = centerZ * cosf(CAM_JUMP) - centerX * sinf(CAM_JUMP);

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

	glutAddMenuEntry("Toggle smooth camera", SMOOTH_CAM);

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

	if (PRINT_CONSOLE) {
		std::cout << "Welcome! \n"
			<< "Use the following controls to move the camera : \n"
			<< " F1 : Camera panning[default]\n F2 : Move camera\n F3 : Nadir\n"
			<< " F4 : low angle view\n F5 : Normal\n F6 : high angle view[default]\n F7 : Zenith\n";
		std::cout << "Use F to enable or disable fog.\n";
		std::cout << "Use 2 to enable/disable the free light, WASD to move it horizontally. Q and E to move it vertically.\n";
		std::cout << "Press P to pause or play the animation.\n";
	}

	// Indicamos cuales son las funciones de redibujado e idle
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboardKeys);
	glutIdleFunc(idle);
	createMenu();

	// Establecemos que inv_quadratic tiene que tener las normales para el interior
	gluQuadricOrientation(invQuadratic, GLU_INSIDE);

	// El color de fondo sera el blanco (RGBA, RGB + Alpha channel)
	glClearColor(backgroundColor[RED], backgroundColor[GREEN], backgroundColor[BLUE], backgroundColor[ALPHA]);
	gluPerspective(60, (GLfloat)W_WIDTH / (GLfloat)W_HEIGHT, 0.001f, 100.0f);

	glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);

	// Comienza la ejecucion del core de GLUT
	glutMainLoop();
	return 0;
}