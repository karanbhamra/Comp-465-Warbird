
// define your target operating system: __Windows__  __Linux__  __Mac__
# define __Windows__ 
# include "../includes465/include465.hpp"
# include "Object3D.hpp"



// Shapes
const int nShapes = 5;
Object3D * objs[nShapes];
const int nModels = 3;

// Model for shapes
char * modelFile[nModels] = { "Ruber.tri", "Space_Ship.tri", "Missile.tri" };
float boundingRadius[nModels]; // models bounding radius
float scaleValue[nModels];  // models scaling value
const GLuint nVertices[nModels] = { 684 * 3, 2544 * 3, 494 * 3 };  // ruber, ship, missile

int Index = 0;  // global variable indexing into VBO arrays

				// display state and "state strings" for title display
				// window title strings
char baseStr[50] = "Comp 465 Warbird {f, t, b} : ";
char fpsStr[15], viewStr[15] = " front view";
char titleStr[100], timerStr[20] = " interval timer";


GLuint VAO[nModels];   // vertex array objects
GLuint buffer[nModels];  // vertex buffer objects
GLuint shaderProgram;
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";

GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models

GLuint MVP;  // Model View Projection matrix's handle
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;          // set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();
									 // vectors and values for lookAt
glm::vec3 eye, at, up;

// rotation variables
glm::mat4 identity(1.0f);
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval;
bool idleTimerFlag = false;  // interval or idle timer ?


// quickly sets the 3 camera views on Ruber
void setFrontView() {
	eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // eye is 6000 "out of screen" from origin
	at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
	up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
	strcpy(viewStr, " front view");
}

void setTopView() {
	eye = glm::vec3(0.0f, 20000.0f, 0.0f);   // eye is 3000 up from origin
	at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin  
	up = glm::vec3(0.0f, 0.0f, -1.0f);   // camera's up is looking towards -Z vector
	strcpy(viewStr, " top view");

}

void setBottomView() {
	eye = glm::vec3(0.0f, -20000.0f, 0.0f);   // eye is 1000 right from origin
	at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
	up = glm::vec3(0.0f, 0.0f, 1.0f);   // camera'a up vector
	strcpy(viewStr, " bottom view");
}


void init(void) {
	
	// load vertex shader and fragment shader and use them
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(nModels, VAO);
	glGenBuffers(nModels, buffer);
	// load the buffers from the model files
	for (int i = 0; i < nModels; i++) {
		boundingRadius[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		//scale[i] = glm::vec3(modelSize[i] * 1.0f / boundingRadius[i]);
	}

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

	viewMatrix = glm::lookAt(
		glm::vec3(50.0f, 50.0f, 200.0f),  // eye position
		glm::vec3(0),                   // look at position
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

									  // set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	

	// initially use a front view
	setFrontView();	
	viewMatrix = glm::lookAt(eye, at, up);

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// create shape
	for (int i = 0; i < nShapes; i++) {

		if (i == 0) {
			objs[i] = new Object3D(i, false);
		} else {
			objs[i] = new Object3D(i, true);
		}

		float xPos = 200.0f * (float)i;
		objs[i]->setPosition(xPos, 0, 0);
		float newI = float(i * i) + 1;
		objs[i]->setRotateBy(0.4f * newI);
			
	}
	objs[0]->setScale(250);
		objs[1]->setPosition(4900, 1000, 4850);  // set warbird and 1 missile at (4900,1000,4850)
		objs[1]->setScale(100);
		objs[2]->setPosition(4900, 1000, 4850);  // set warbird and 1 missile at (4900,1000,4850)
		objs[2]->setScale(100);
	
	printf("%d Shapes created \n", nShapes);
	


	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time

}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 1.0f, 100000.0f);
}

// update and display animation state in window title
void updateTitle() {
	strcpy(titleStr, baseStr);
	strcat(titleStr, viewStr);
	strcat(titleStr, fpsStr);
	// printf("title string = %s \n", titleStr);
	glutSetWindowTitle(titleStr);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update model matrix, set MVP, draw
	//for (int i = 0; i < nShapes; i++) {
	//	modelMatrix = objs[i]->getModelMatrix();
	//	ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	//	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
	//	glDrawArrays(GL_TRIANGLES, 0, nVertices[i]);
	//	//glDrawArrays(GL_TRIANGLES, 0, nVertices2);
	//}
	for (int m = 0; m < nModels; m++) {  // number of models
		modelMatrix = objs[m]->getModelMatrix();

		
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[m]);
		glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);
	}



	glutSwapBuffers();
	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000) {
		sprintf(fpsStr, " fps %4d", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}


// Animate scene objects by updating their transformation matrices
// timerDelay = 40, or 25 updates / second
void update(int i) {
	glutTimerFunc(timerDelay, update, 1);
	for (int i = 0; i < nShapes; i++) {
		objs[i]->update();
	}
}

// Quit or set the view
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
	case 'f': case 'F':  // front view
		//eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // eye is 6000 "out of screen" from origin
		//at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
		//up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
		//strcpy(viewStr, " front view");
		setFrontView();
		break;
	case 'b': case 'B':  // bottom view
		//eye = glm::vec3(0.0f, -20000.0f, 0.0f);   // eye is 1000 right from origin
		//at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
		//up = glm::vec3(0.0f, 0.0f, 1.0f);   // camera'a up vector
		//strcpy(viewStr, " right view");
		setBottomView();
		break;
	case 't': case 'T':  // top view
		//eye = glm::vec3(0.0f, 20000.0f, 0.0f);   // eye is 3000 up from origin
		//at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin  
		//up = glm::vec3(0.0f, 0.0f, -1.0f);   // camera's up is looking towards -Z vector
		//strcpy(viewStr, " top view");
		setTopView();
		break;
	}
	viewMatrix = glm::lookAt(eye, at, up);
	updateTitle();
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
	glutCreateWindow("Comp 465 Warbird {f, t, b} : front view");
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
