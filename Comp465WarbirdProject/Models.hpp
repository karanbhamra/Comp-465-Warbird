# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define __MODELS__
# include "Object3D.hpp"

class Models {

private:

    // Shapes
static const int nModels = 7;

static const int nShapes = 7;
Object3D * objs[nShapes];

Object3D * unum;
Object3D * duo;
Object3D * lookAtPlanet;


enum ObjectIndex {
    
    Ruber = 0,
    Unum,
    Duo,
    Primus,
    Secundus,
    Ship,
    Missle
    
};

// char * modelFile[nModels] = { "Ruber.tri", "Space_Ship.tri", "Missile.tri" , "Primus.tri", "Secundus.tri", "Duo.tri", "Unum.tri"};
// const GLuint nVertices[nModels] = { 684 * 3,    2544 * 3,       494 * 3,        2610 * 3,        920 * 3,  1516 * 3, 1942 * 3 };  // ruber, ship, missile


char * modelFile[nModels] = { "Ruber.tri", "Space_Ship.tri", "Missile.tri" , "Primus.tri", "Secundus.tri", "Duo.tri", "Unum.tri"};
const GLuint nVertices[nModels] = { 684 * 3,    2544 * 3,       494 * 3,        2610 * 3,        920 * 3,  1516 * 3, 1942 * 3 }; 
float boundingRadius[nModels]; // models bounding radius
float scaleValue[nModels];  // models scaling value


GLuint VAO[nModels];   // vertex array objects
GLuint buffer[nModels];  // vertex buffer objects
GLuint vPosition[nModels];
GLuint vColor[nModels];
GLuint vNormal[nModels];

GLuint MVP; 
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();



public:

    Models() {
        setupLocRotScale();
    }

    void setupModels(GLuint shaderProgram) {

        // generate VAOs and VBOs
        glGenVertexArrays(nModels, VAO);
        glGenBuffers(nModels, buffer);
        // load the buffers from the model files
        for (int i = 0; i < nModels; i++) {
            boundingRadius[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
                                                vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
            // set scale for models given bounding radius
        }

        setupLocRotScale();

    }

    void setupLocRotScale() {

        for (int i = 0; i < nShapes; i++) {
            
            if (i == 0) {
                //Ruber
                objs[i] = new Object3D(i);
                objs[i]->setupRuber(boundingRadius[0]);
            } else if (i == 1) {
                //Unum
                objs[i] = new Object3D(i);
                objs[i]->setupUnum(boundingRadius[6]);
                unum = objs[i];
            } else if (i == 2) {
                //Duo
                objs[i] = new Object3D(i);
                objs[i]->setupDuo(boundingRadius[5]);
                duo = objs[i];
            } else if (i == 3) {
                //Primus
                objs[i] = new Object3D(i);
                objs[i]->setupPrimus(boundingRadius[3], duo);
            } else if (i == 4) {
                //Secundus 
                objs[i] = new Object3D(i);
                objs[i]->setupSecundus(boundingRadius[4], duo);
            } else if (i == 5) {
                //Warbird
                objs[i] = new Object3D(i);
                objs[i]->setupWarbird(boundingRadius[1]);
            } else if (i == 6) {
                //Missile
                objs[i] = new Object3D(i);
                objs[i]->setupStaticMissile(boundingRadius[2]);
            } else {
                
            }
            
        }

    }


    void updateProjectionMatrix(int width, int height) {
        glViewport(0, 0, width, height);
        projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 1.0f, 100000.0f);
    }

    void updateViewMatrix(glm::mat4 newMatrix) {
        viewMatrix = newMatrix;
    }

    void displayUpdate() {

        // for (int m = 0; m < nModels; m++) {  // number of models

        //     modelMatrix = objs[m]->getModelMatrix();
        //     ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        //     glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
        //     glBindVertexArray(VAO[m]);
        //     glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);

        // }
        updateRuber();
        updateUnum();
        updateDuo();
        updatePrimus();
        updateSecundus();
        updateWarbird();
        updateMissile();

    }

    void updateRuber() {
        updateObj(0);
        drawModel(0);
    }

    void updateUnum() {
        updateObj(1);
        drawModel(6);
    }

    void updateDuo() {
        updateObj(2);
        drawModel(5);
    }

    void updatePrimus() {
        updateObj(3);
        drawModel(3);
    }

    void updateSecundus() {
        updateObj(4);
        drawModel(4);
    }

    void updateWarbird() {
        updateObj(5);
        drawModel(1);
    }

    void updateMissile() {
        updateObj(6);
        drawModel(2);
    }

    void updateObj(int objIndex) {

        modelMatrix = objs[objIndex]->getModelMatrix();
        ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

    }

    void clearLookAtPlanet() {
        lookAtPlanet = NULL;
    }

    void setLookAtPlanetToDuo() {
        lookAtPlanet = duo;
    }

    void setLookAtPlanetToUnum() {
        lookAtPlanet = unum;
    }

    bool isLookAtPlanetSet() {
        if (lookAtPlanet == NULL) {
            return false;
        } else {
            return true;
        }
    }

    glm::vec3 getLookAtPlanetPosition() {

        glm::mat4 mtrx = lookAtPlanet->getModelMatrix();
        glm::vec3 pos = getPosition(mtrx);
        return pos;

    }

    void handleKeyPress(unsigned char key) {
        
        clearLookAtPlanet();

        switch (key) {
            case 'u': case 'U':
                setLookAtPlanetToUnum(); 
                break;
            case 'd': case 'D':
                setLookAtPlanetToDuo();
                break;				

        }

    }

    void drawModel(int modelIndex) {
        glBindVertexArray(VAO[modelIndex]);
        glDrawArrays(GL_TRIANGLES, 0, nVertices[modelIndex]);
    }

    void updateObjLocRotScale() {

        for (int i = 0; i < nShapes; i++) {
            objs[i]->update();
        }

    }


};  