

// define your target operating system: __Windows__  __Linux__  __Mac__
//# define __Mac__ 
//# include "includes465/include465.hpp"
# define __Windows__
# include "../includes465/include465.hpp"

# include "Camera.hpp"
# include "Models.hpp"
# include "TopBar.hpp"

Camera * cam;
Models * models;
TopBar * bar;

// rotation variables
glm::mat4 identity(1.0f);

// A delay of 40 milliseconds is 25 updates per second
int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval;

// Cam
int camCount = 0;
int numCams = 4;


void init(void) {

	cam = new Camera();
	models = new Models();
	bar = new TopBar();
	models->setupModels(cam->shaderProgram);

	cam->setToFront();
	glm::mat4 viewMatrixNew = cam->updateViewMatrix();
	models->updateViewMatrix(viewMatrixNew);
	bar->updateTitle();

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time

}



void reshape(int width, int height) {
	models->updateProjectionMatrix(width, height);
}


bool isFirstRun = true;
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	models->displayUpdate();

	if (isFirstRun) {
		isFirstRun = false;
		cam->setToFront();
	}

	glutSwapBuffers();

	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;

	if (timeInterval >= 1000) {

		bar->displayUpdate(frameCount, timeInterval);
		lastTime = currentTime;
		frameCount = 0;

	}

}






void update(int i) {

	glutTimerFunc(timerDelay, update, 1);
	models->updateObjLocRotScale();

	//glm::vec3 getLookAtPlanetPosition()
	if (models->isLookAtPlanetSet()) {
		glm::vec3 planetPosition = models->getLookAtPlanetPosition();
		glm::mat4 viewMatrixNew = cam->updateViewMatrix(planetPosition);
		models->updateViewMatrix(viewMatrixNew);
	}

}


char views[4] = {'f', 't', 'u', 'd'};

void keyboard(unsigned char key, int x, int y) {

	if ((key == 033) || (key == 'q') || (key == 'Q')) {

		exit(EXIT_SUCCESS);

	} else if (key == 'v' || key == 'V' || key == 'x' || key == 'X' ) {

		printf("v pressed\n");
		printf("%d camCount", camCount);

		if (key == 'v' || key == 'V') {
			camCount++;
			if (camCount > 3) {
				camCount = 0;
			}
		} else {
			camCount--;
			if (camCount < 0) {
				camCount = 3;
			}
		}
		
		int viewIndex = camCount % numCams;
		char selectedView = views[viewIndex];
		models->handleKeyPress(selectedView);	//camera views
		cam->handleKeyPress(selectedView);
		bar->handleKeyPress(selectedView);

	} else {

		models->handleKeyPress(key);
		cam->handleKeyPress(key);
		bar->handleKeyPress(key);

	}

	glm::mat4 viewMatrixNew = cam->updateViewMatrix();
	models->updateViewMatrix(viewMatrixNew);
	bar->updateTitle();

}





int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
# ifdef __Mac__
	// Can't change the version in the GLUT_3_2_CORE_PROFILE
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
# endif
# ifndef __Mac__
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
# endif
	glutInitWindowSize(800, 600);
	// set OpenGL and GLSL versions to 3.3 for Comp 465/L, comment to see highest versions
# ifndef __Mac__
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
# endif
	glutCreateWindow("Comp 465 Warbird {f, t, r} : front view");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene
	init();
	// set glut callback functions


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(timerDelay, update, 1);
	glutIdleFunc(display);
	glutMainLoop();
	printf("done\n");
	return 0;
}


