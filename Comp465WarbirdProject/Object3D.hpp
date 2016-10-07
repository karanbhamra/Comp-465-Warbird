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

public:

    Object3D(int number, bool isOrbital) {

        id = number;  // for debugging

        // Scale =======================================
        //==============================================

        scaleMatrix = glm::scale(glm::mat4(), glm::vec3(20, 20, 20));

        // Rotation ====================================
        //==============================================

        rotationMatrix = glm::mat4();  // no initial orientation
        //set cube's  rotation axis and rotation radians
        rotationAxis = glm::vec3(0,  1,  0);
        // rotate between 0.1 and 2.0 degrees
        radians = glm::radians(0.8f);

		staticRot = glm::rotate(glm::mat4(), PI / 2.0f , glm::vec3(0, 1, 0));
        // Translation =================================
        //==============================================

        translationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));

        // determine rotation type
        orbital = isOrbital;

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
            return(rotationMatrix * translationMatrix * staticRot * scaleMatrix);
        } else { // center rotation
            return(translationMatrix * rotationMatrix * staticRot * scaleMatrix);
        }

    }

    void update() {
      rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
      //translationMatrix = glm::translate(translationMatrix, translation);
    }  

};  