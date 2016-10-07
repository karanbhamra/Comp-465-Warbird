/*
manyCubes.cpp
  includes Shape3D.cpp

A modification of CubeTriModel.cpp that uses C and C++.
This file (manyCube.cpp) is a C file -- it defines no classes
but has 1000 instances of Shape3D.

File Shape3D defines a 3D shape that can translate and rotate.
There is only 1 model created from the "Cube2.tri" file.

However 1,000 Shapes3D are drawn as cubes.  
Each has its own position and orientation.
Each cube rotates either orbitally or about its center.
Each cube is updated approximately 25 times per second.
Each frame is drawn whenever glut is idle, see fps value in title.

465 utility include files:  shader465.h, triModel465.h  
Shaders:  simpleVertex.glsl  amd simpleFragment.glsl

User commands:
'f' view from front (0, 0, 3) looking at origin
't' view from above (0, 3, 3) looking at origin
'b' view from below (0, -3, 0) looking at origin

Current state is displayed in the window title.
 
The cube2.tri model uses 12 triangular surfaces to display 12 trianlge surfaces.  
So, there are 36 (3 * 12) vertices.

The triModel465.hpp utility creates colors, and normals for ever vertex.
However the simple*.glsl shaders do not use the normal. 
Since the same color is created for every vertex of every surface,
the object is rendered with flat shading.

A reference for estimating fps using glut functions:
http://mycodelog.com/2010/04/16/fps/

Mike Barnes
8/16/2014
*/


// define your target operating system: __Windows__  __Linux__  __Mac__
# define __Windows__ 
# include "../includes465/include465.hpp"
# include "Shape3D.hpp"

// Shapes
const int nShapes = 5;
Shape3D * shape[nShapes];
// Model for shapes
char * modelFile = "ruber_sphere.tri"; // name of tri model file
const GLuint nVertices = 264 * 3;  // 3 vertices per line (surface) of model file  
float boundingRadius;  // modelFile's bounding radius
int Index =  0;  // global variable indexing into VBO arrays

// display state and "state strings" for title display
// window title strings
char baseStr[50] =    "Comp 465 Warbird {f, t, r} : ";
char fpsStr[15], viewStr[15] =    " front view";
char titleStr [100]; 

GLuint vao;  // VertexArrayObject
GLuint buffer;
GLuint shaderProgram; 
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint MVP ;  // Model View Projection matrix's handle
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;          // set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();


// vectors and values for lookAt
glm::vec3 eye, at, up;

// vectors for "model"
glm::vec4 vertex[nVertices];
glm::vec3 normal[nVertices];
glm::vec4 diffuseColorMaterial[nVertices];

// rotation variables
glm::mat4 identity(1.0f); 
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval; 


void init (void) {
  boundingRadius = loadTriModel(modelFile, nVertices, vertex, diffuseColorMaterial, normal);
  if (boundingRadius == -1.0f) {
    printf("loadTriModel error:  returned -1.0f \n");
    exit(1); }
    else
      printf("loaded %s model with %7.2f bounding radius \n", modelFile, boundingRadius);

  shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
  glUseProgram(shaderProgram);

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initialize a buffer object
  // GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertex) + sizeof(diffuseColorMaterial) + sizeof(normal), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertex), sizeof(diffuseColorMaterial), diffuseColorMaterial );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertex) + sizeof(diffuseColorMaterial), sizeof(normal), normal );


  // set up vertex arrays (after shaders are loaded)
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertex)) );

  // Normal code is here for later use with lighting and shaders
  GLuint vNormal = glGetAttribLocation( shaderProgram, "vNormal" );
  glEnableVertexAttribArray( vNormal);
  glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertex) + sizeof(diffuseColorMaterial)) );

  MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
  
// initially use a front view
  eye = glm::vec3(0.0f, 0.0f, 400.0f);   // eye is 1000 "out of screen" from origin ( CAMERA DISTANCE)
  at  = glm::vec3(0.0f, 0.0f,    0.0f);   // looking at origin
  up  = glm::vec3(0.0f, 1.0f,    0.0f);   // camera'a up vector
  viewMatrix = glm::lookAt(eye, at, up);
  
  // set render state values
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

  // create shape
  for(int i = 0; i < nShapes; i++) shape[i] = new Shape3D(i);
  printf("%d Shapes created \n", nShapes);

  lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
  }

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat) width /  (GLfloat) height, 1.0f, 10000.0f); 
  }

// update and display animation state in window title
void updateTitle() {
  strcpy(titleStr, baseStr);
  strcat(titleStr, viewStr);
  strcat(titleStr, fpsStr);
  // printf("title string = %s \n", titleStr);
  glutSetWindowTitle( titleStr);
  }

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // update model matrix, set MVP, draw
  for(int i = 0; i < nShapes; i++) { 
    modelMatrix = shape[i]->getModelMatrix(); 
    ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix; 
    glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix)); 
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
    }
  glutSwapBuffers();
  frameCount++;
  // see if a second has passed to set estimated fps information
  currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
  timeInterval = currentTime - lastTime;
  if ( timeInterval >= 1000) {
    sprintf(fpsStr, " fps %4d", (int) (frameCount / (timeInterval / 1000.0f)) );
    lastTime = currentTime;
    frameCount = 0;
    updateTitle();
    }
  }


// Animate scene objects by updating their transformation matrices
// timerDelay = 40, or 25 updates / second
void update (int i) { 
  glutTimerFunc(timerDelay, update, 1);
  for(int i = 0; i < nShapes; i++) shape[i] -> update();  
  }

// Quit or set the view
void keyboard (unsigned char key, int x, int y) {
  switch(key) {
    case 033 : case 'q' :  case 'Q' : exit(EXIT_SUCCESS); break;
    case 'f' : case 'F' :  // front view
        eye = glm::vec3(0.0f, 0.0f, 5000.0f);   // eye is 2000 "out of screen" from origin
        at  = glm::vec3(0.0f, 0.0f,    0.0f);   // looking at origin
        up  = glm::vec3(0.0f, 1.0f,    0.0f);   // camera'a up vector
        strcpy(viewStr, " front view"); break;
    case 'r' : case 'R' :  // bottom view
        eye = glm::vec3(1000.0f, 0.0f, 0.0f);   // eye is 1000 right from origin
        at  = glm::vec3(   0.0f, 0.0f, 0.0f);   // looking at origin
        up  = glm::vec3(   0.0f, 1.0f, 0.0f);   // camera'a up vector
        strcpy(viewStr, " right view"); break;
    case 't' : case 'T' :  // top view
        eye = glm::vec3(0.0f, 3000.0f,  0.0f);   // eye is 3000 up from origin
        at  = glm::vec3(0.0f,    0.0f,  0.0f);   // looking at origin  
        up  = glm::vec3(0.0f,    0.0f, -1.0f);   // camera's up is looking towards -Z vector
        strcpy(viewStr, " top view"); break;
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
  

