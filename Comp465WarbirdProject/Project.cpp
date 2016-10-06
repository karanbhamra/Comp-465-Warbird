/*
Empty source file for 465L projects.
Mike Barnes 7/28/2016
*/

# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"

// declare variables
GLfloat aspectRatio;
int timerDelay = 40; // 40 millisecond delay approximates 35 fps

// define functions

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
	}

void init(void) {
	// put your initializations here
	// ...
	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	}

// set viewport and projectionMatrix on window resize events
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	aspectRatio = (GLfloat)width / (GLfloat)height;
	// set projection matrix
	}
	
// Use with Idle and intervalTimer functions 
void update(void) {
	glutPostRedisplay();
	}

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer(int i) {
	glutTimerFunc(timerDelay, intervalTimer, 1); // reset timerDelay
	update();  // fixed interval timer
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	// Uncomment the following line to force OpenGL & GLSL 3.3
	// glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Starter source file for 465L");
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
	// initialize scene here
	init();
	// set glut callback functions here
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(timerDelay, intervalTimer, 1);  // keep the window up
	glutMainLoop();
	printf("done\n");
	return 0;
}