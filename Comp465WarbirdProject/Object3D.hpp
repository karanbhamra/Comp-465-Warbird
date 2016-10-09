//Based on the Shape3D 
// We created our own class for handling Location, Rotation, and Scaling operations.


# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define __OBJECT3D__

class Object3D {

private :

  int id;
  glm::mat4 rotationMatrix;
  glm::mat4 scaleMatrix;
  glm::mat4 translationMatrix;
  glm::vec3 rotationAxis; 
  glm::mat4 staticRot;
  float radians; 
  bool orbital;

  Object3D * parentNode;

public:

    Object3D(int number) {

        id = number;  // for debugging
        setupFirst(number, false); 

    }

    Object3D(int number, bool isOrbital) {

        id = number;  // for debugging
        setupFirst(number, isOrbital);

    }

    void setupFirst(int number, bool isOrbital) {

        setScale(1.0f);
        rotationMatrix = glm::mat4();  // no initial orientation
        rotationAxis = glm::vec3(0,  1,  0);
        setRotateBy(0.0f);
        setPosition(0, 0, 0);
        setStaticRotationOffset(PI);
        orbital = isOrbital;

    }

    void setRotationAxis(float x, float y, float z) {
        rotationAxis = glm::vec3(x, y, z);
    }

    void setStaticRotationOffset(float rads) {
        staticRot = glm::rotate(glm::mat4(), rads, glm::vec3(0, 1, 0));
    }

    void setScale(float s) {
        scaleMatrix = glm::scale(glm::mat4(), glm::vec3(s, s, s));
    }

    void setRotateBy(float rads) {
        radians = glm::radians(rads);
    }

    void setRotation(float x, float y, float z, float w) {
        rotationMatrix = glm::rotate(glm::mat4(), w, glm::vec3(x, y, z));
    }

    void setPosition(float x, float y, float z) {
        translationMatrix = glm::translate(glm::mat4(), glm::vec3(x, y, z));
    }

    glm::mat4 getModelMatrix() {
		
        if (orbital) { // orbital rotation

            if (parentNode == NULL) {
                return(rotationMatrix * translationMatrix * staticRot * scaleMatrix);
            } else {

                glm::mat4 prntLocRot = parentNode->getLocRot();
                return(prntLocRot * rotationMatrix * translationMatrix * staticRot * scaleMatrix);
            }
        } else { // center rotation
            return(translationMatrix * rotationMatrix * staticRot * scaleMatrix);
        }

    }

    glm::mat4 getLocRot() {
        return rotationMatrix * translationMatrix;
    }

    void update() {
      rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
	  // float radsNew = radians * 500.0f;
	  // float doubledRads = glm::radians(radsNew);
	  // staticRot = glm::rotate(staticRot, doubledRads, rotationAxis);

    }

    void setupRuber(float boundingRadius) {

        orbital = false;
        setPosition(0, 0, 0);
        setRotateBy(2);
        setScale(2000.0f / boundingRadius);
    }

    void setupUnum(float boundingRadius) {

        orbital = true;
        setPosition(4000, 0, 0);
        setRotateBy(2);
        setStaticRotationOffset(0.0f);
        setScale(200.0f / boundingRadius);
    }

    void setupDuo(float boundingRadius) {

        orbital = true;
        setPosition(9000, 0, 0);
        setRotateBy(4);
        setStaticRotationOffset(0.0f);
        setScale(400.0f / boundingRadius);
    }

    void setupPrimus(float boundingRadius, Object3D * parent) {

        orbital = true;
        setPosition(900, 0, 0);
        setRotateBy(20);
        setStaticRotationOffset(0.0f);
        setRotationAxis(0.0f, 1.0f, 0.0f);
        setScale(100.0f / boundingRadius);
        parentNode = parent;

    }

    void setupSecundus(float boundingRadius, Object3D * parent) {

        orbital = true;
        setPosition(1750, 0, 0);
        setRotateBy(15);
        setStaticRotationOffset(0.0f);
        setRotationAxis(0.0f, 1.0f, 0.0f);
		//setRotationAxis(0.0f, 0.0f, 1.0f);
        setScale(150.0f / boundingRadius);
        parentNode = parent;

    }

    void setupWarbird(float boundingRadius) {

        orbital = true;
        setPosition(5000, 1000, 5000);
        setRotateBy(0);
        setStaticRotationOffset(0.0f);
        setScale(100.0f / boundingRadius);

    }

    void setupStaticMissile(float boundingRadius) {

        orbital = true;
        setPosition(4900, 1000, 4850);
        setRotateBy(0);
        setStaticRotationOffset(0.0f);
        setScale(25.0f / boundingRadius);

    }

};  