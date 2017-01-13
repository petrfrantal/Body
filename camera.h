#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#define RIGHTANGLERADIANS 1.5707f

/**
*	Camera definition. Used to obtain the view and projection transformation matrix. The camera is movable.
*/

/**
*	Camera object.
*/
class Camera {
	private:
		glm::mat4 projection;		// projection matrix
		glm::vec3 position;
		glm::vec3 direction;		// direction of the camera, is computed from the camera angles
		glm::vec3 up;				// up vector
		float horizontalAngle;		// angles used for determining of the camera's direction
		float verticalAngle;
	public:
		/**
		*	Constructs the camera object based on the given position, angles and other variables.
		*/
		Camera(const glm::vec3 & pos, float horizontalA, float verticalA, float fov, float aspect, float zNear, float zFar);

		/**
		*	Updates the camera angles which makes the camera rotate.
		*/
		void updateCameraAngles(float horizontalAngleDelta, float verticalAngleDelta);

		/**
		*	Moves the camera to it's sides (left/right) or up and down.
		*/
		void moveCameraRightLeftUpDown(float horizontalDelta, float verticalDelta);

		/**
		*	Moves the camera forwards of backwards (i.e. adds the camera direction vector to it's position).
		*/
		void moveCameraForwardBackward(float delta);

		/**
		*	Returns the vector of a camera's position.
		*/
		glm::vec3 getCameraPosition(void);

		/**
		*	Returns the multiplied projection * view matrix (multiplication order from right to left according to OpenGL).
		*/
		glm::mat4 getViewProjection(void);
};
#endif
