# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define __MODELS__
# include "Object3D.hpp"
# include "Missile.hpp"
# include "MissileSite.hpp"


class Models {

private:

    // Shapes
static const int nModels = 8;

static const int nShapes = 10; //5 planets + 1 warbird + 2 Missiles + 2 MissileSites

static const int numMissiles = 3; //1 Warbird missile, 2 MissileSite missiles
static const int numMissileSites = 2; //1 Warbird missile, 1 MissileSite missile 


Object3D * objs[nShapes];

Object3D * unum;
Object3D * duo;
Object3D * lookAtPlanet;

//Missiles
Missile * missiles[numMissiles]; //[0] ship's missile, [1] MissileSite missile
MissileSite * missileSites[numMissileSites];
MissileSite * shipTarget; //missile site closest to the active missile fired by the ship.
glm::mat4 missileLoc, targetLoc;
glm::vec3 missilePos, targetPos;
const int missileDetectionRadius = 5000;
int updateCount = 0;	//for determining when the missile will be active
float dist;			//distance between missile's position and its target



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


char * modelFile[nModels] = { "Ruber.tri", "Space_Ship.tri", "Missile.tri" , "Primus.tri", "Secundus.tri", "Duo.tri", "Unum.tri", "MissileSite.tri"};
const GLuint nVertices[nModels] = { 684 * 3,    2544 * 3,       494 * 3,        2610 * 3,        920 * 3,  1516 * 3, 1942 * 3,      651 * 3 }; 
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
				//Missile 1
				objs[i] = new Object3D(i);
				objs[i]->setupMissile(boundingRadius[2]);
			} else if (i == 7) {
				//Missile 2
				objs[i] = new Object3D(i);
				objs[i]->setupMissile(boundingRadius[2]);
            } else if (i == 8) {
				//MissileSite on Unum
				objs[i] = new Object3D(i);
				objs[i]->setupMissileSite(boundingRadius[7], 200.0f);
			} else if (i == 9) {
				//MissileSite on Secundus
				objs[i] = new Object3D(i);
				objs[i]->setupMissileSite(boundingRadius[7], 150.0f);
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
			case 'f': //can be moved to ship's class if created
				if (!missiles[0]->hasFired()) { //TODO: also check if the ship has missiles left to fire (9 total).
					missiles[0]->launchMissile();
					//TODO: Deduct ship's missile count
				}
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

		updateMissiles();
    }

	void updateMissiles() {

		// Increase the number of updates for each missile if it has been fired.
		// If the number of updates is greater than 200, activate the missile.
		for (int i = 0; i < numMissiles; i++)
		{
			if (missiles[i]->hasFired())
			{
				updateCount = missiles[i]->getNumberOfUpdates();
				if (updateCount > 200)
				{
					missiles[i]->activate();
				}
			}
		}

		// The missile attempts to find a target if it has been fired,
		// and if it has been activated.
		if (missiles[0]->hasFired()) {

			if (missiles[0]->activated()) {
				// Check of the missile has a target. If the missile does not
				// have a target, it will try to find one. Otherwise, update the
				// missiles knowledge of the missiles target's location.
				if (!missiles[0]->hasTarget()) {

					// Determine the closest target for the warbirds missile
					missileLoc = missiles[0]->getStaticRotMatrix();
					targetLoc = missileSites[0]->getStaticRotMatrix();
					missilePos = getPosition(missileLoc);
					targetPos = getPosition(targetLoc);
					dist = distance(missilePos, targetPos);

					targetLoc = missileSites[1]->getStaticRotMatrix();
					targetPos = getPosition(targetLoc);

					// The target will be one of the missile sites, the closest
					// one to the missile that is within the missiles detection range.
					if (distance(missilePos, targetPos) > dist) {
						shipTarget = missileSites[0];
					}
					else {
						dist = distance(missilePos, targetPos);
						shipTarget = missileSites[1];
					}

					// Check to make sure the target is within the missile's range
					if (dist < missileDetectionRadius) {
						missiles[0]->setTargetLocation(shipTarget->getStaticRotMatrix());

					}
				}

				// Update the missiles knowledge of its targets location.
				else {
					missiles[0]->setTargetLocation(shipTarget->getStaticRotMatrix());
				}
			}
		}

		// If the missile has not been fired, then set its location to warbirds location.
		else {
			missiles[0]->setStaticRot(glm::translate(warbird->getStaticRotMatrix(), glm::vec3(-33, 0, -30)));
		}

		// The missile attempts to detect the warbird to determine if the warbird is 
		// within firing range. If the warbird is within firing range, the missile targets
		// the warbird and is fired. But the missile does not seek the warbird until it is active.
		if (!missiles[1]->hasFired()) {

			// Set the location of the missile to its designated missile Site if it has not fired
			missiles[1]->setStaticRot(glm::rotate(missileSites[0]->getStaticRotMatrix(), PI / 2, glm::vec3(1, 0, 0)));

			// Attempt to detect if the warbird is within the detection radius
			missileLoc = missiles[1]->getStaticRotMatrix();
			targetLoc = warbird->getOrientationMatrix();
			missilePos = getPosition(missileLoc);
			targetPos = getPosition(targetLoc);
			dist = distance(missilePos, targetPos);

			// If warbird has been detected, set the missiles target, and fire the missile
			if (dist < missileDetectionRadius) {

				missiles[1]->launchMissile();
				missiles[1]->setTargetLocation(warbird->getOrientationMatrix());
				missileSites[0]->reduceMissileCount();
			}
		}

		// If the missile is active, have it seek its target
		if (missiles[1]->activated()) {
			missiles[1]->setTargetLocation(warbird->getOrientationMatrix());
		}

		// The missile attempts to detect the warbird to determine if the warbird is 
		// within firing range. If the warbird is within firing range, the missile targets
		// the warbird and is fired. But the missile does not seek the warbird until it is active.
		if (!missiles[2]->hasFired()) {

			missiles[2]->setStaticRot(
				glm::rotate(missileSites[1]->getStaticRotMatrix(), PI / 2, glm::vec3(1, 0, 0)));

			// Attempt to detect if the warbird is within the detection radius
			missileLoc = missiles[2]->getStaticRotMatrix();
			targetLoc = warbird->getOrientationMatrix();
			missilePos = getPosition(missileLoc);
			targetPos = getPosition(targetLoc);
			dist = distance(missilePos, targetPos);

			// If warbird has been detected, set the missiles target, and fire the missile
			if (dist < missileDetectionRadius) {

				missiles[2]->launchMissile();
				missiles[2]->setTargetLocation(warbird->getOrientationMatrix());
				missileSites[1]->reduceMissileCount();
			}
		}

		// If the missile is active, have it seek its target
		if (missiles[2]->activated()) {
			missiles[2]->setTargetLocation(warbird->getOrientationMatrix());
		}

		// Update the missiles orientation
		for (int i = 0; i < numMissiles; i++) {
			missiles[i]->update();
		}
	}
};  