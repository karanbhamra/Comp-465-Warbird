# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# include "Object3D.hpp"

class Missile : public Object3D {
private:
	glm::mat4 targetLocation;			// the location of missiles target

	glm::vec3 initialTranslationPosition;	// the x,y,z intial position of the object

	glm::vec3 distance;		// the vector the missile will travel about

	glm::vec3 targetVector;	// the vector containing the targets location

	glm::vec3 missileVector;	// the vector containing the missiles location

	bool active;				// indicates if the missile is active

	bool fired;				// indicates if the missile has been fired

	bool target;				// indicates if the missile has a target

	float speed;				// the amount the missile moves every update

	float rotationAxisDirection;		// the axis of rotation's direction, either positive of negative

	int numberOfUpdates;		// indicates the number of updates the missile has gone through

	int maximumNumberOfUpdates;	// The number of Updates the Missile can be active

public:

	Missile(float modelSize, float modelBoundingRadius)
		: Object3D(modelSize, modelBoundingRadius) {

		// initially inactive
		active = false;

		// initially not fired
		fired = false;

		// no initial target
		target = false;

		// initial speed
		speed = 20.0f;

		// intial update count is 0
		numberOfUpdates = 0;

		// The maximum number of updates the missile can be active
		maximumNumberOfUpdates = 2000;

		// no initial rotation axis direction
		rotationAxisDirection = 0;
	}


	/* Method to get the targets orientation matrix
	*/
	glm::mat4 getTargetLocation() {
		return targetLocation;
	}


	/* Method to get the current number of updates
	*/
	int getNumberOfUpdates() {
		return numberOfUpdates;
	}


	/* Method returns true if the missile has a target, else false.
	The variabele target is set to true when the missiles target
	orientation matrix is set.
	*/
	bool hasTarget() {
		return target;
	}


	/* Method returns true if the missile has fired, false
	otherwise.
	*/
	bool hasFired() {
		return fired;
	}


	/* Method returns true if the missile is active, false
	otherwise.
	*/
	bool activated() {
		return active;
	}


	/* Method to set the target's orientation matrix for the missile
	targeting system.
	*/
	void setTargetLocation(glm::mat4 newLocation) {
		targetLocation = newLocation;
		if (target == false) {
			target = true;
		}
	}


	/*  Method to fire the missile.
	*/
	void launchMissile() {
		fired = true;
	}


	/* Method sets an inactive missile to an active missile.
	*/
	void activate() {
		active = true;
	}


	/* Mehtod to return the missile to its default state
	of when it was created
	*/
	void destroy() {
		active = false;
		fired = false;
		target = false;
		numberOfUpdates = 0;
	}

	/* Method to update the location of an object by translating and
	rotating the object by a given amount in radians
	*/
	void update() {

		// Initialy the missile does not rotate or translate
		rotationMatrix = identityMatrix;
		translationMatrix = identityMatrix;

		// If the missile has fired, start moving the missile and check its lifespan
		if (fired == true)
		{
			// Keep Count of the Number of Updates
			numberOfUpdates = numberOfUpdates + 1;

			// The distance the missile will travel
			distance = glm::vec3(0, 0, -speed);
			translationMatrix = glm::translate(identityMatrix, distance);

			// If the Missile exceeds its lifespan, destroy it
			if (numberOfUpdates > maximumNumberOfUpdates) {
				destroy();
			}

			// The Missile will only attempt to reorient itself if it has a target and is active
			if (active && target) {

				// Get the Missiles at Vector and the Vector from the missile to the target
				targetVector = getPosition(targetLocation) - getPosition(staticRot);
				missileVector = getIn(staticRot);

				// Normalize the vectors
				targetVector = glm::normalize(targetVector);
				missileVector = glm::normalize(missileVector);

				// If the two vectors are colinear, do not attempt to do any rotations
				if (!(colinear(missileVector, targetVector, 0.1) || glm::distance(targetVector, glm::vec3(0, 0, 0)) == 0)) {

					// The rotation axis the missile will be rotating about
					rotationAxis = glm::cross(missileVector, targetVector);
					rotationAxisDirection = rotationAxis.x + rotationAxis.y + rotationAxis.z;

					// Get the rotation Amount of the Missile and Determine the Direction of Rotation.
					// This equation allows to get the angle of rotation between the two vectors,
					// The dot product of two vectors equals |A|*|B|*cos(angle), so to get the angle in between
					// divide by |A|*|B|.
					if (rotationAxisDirection > 0) {
						radians = -glm::acos(glm::dot(missileVector, targetVector) /
							(glm::abs(glm::distance(targetVector, glm::vec3(0, 0, 0))) * glm::abs(glm::distance(missileVector, glm::vec3(0, 0, 0)))));
					}
					else
					{
						radians = glm::acos(glm::dot(missileVector, targetVector) /
							(glm::abs(glm::distance(targetVector, glm::vec3(0, 0, 0))) * glm::abs(glm::distance(missileVector, glm::vec3(0, 0, 0)))));
					}

					// Only rotate the Missile only a 4th of the rotation amount, 
					// this allows for smoother rotations in the simulation.
					rotationMatrix = glm::rotate(identityMatrix, radians / 4, rotationAxis);

				}
			}
		}

		// Update the orientation of the missile
		staticRot = staticRot * translationMatrix * rotationMatrix;
	}
};