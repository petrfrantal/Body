#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

struct Camera
{
	private:
		glm::mat4 projection;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 up;
		float horizontalAngle;
		float verticalAngle;
	public:
		Camera(const glm::vec3& position, float fov, float aspect, float zNear, float zFar);
		Camera(const glm::vec3& position, glm::vec3 & direction, float fov, float aspect, float zNear, float zFar);
		Camera(const glm::vec3 & pos, float horizontalA, float verticalA, float fov, float aspect, float zNear, float zFar);
		void updateCameraAngles(float horizontalAngleDelta, float verticalAngleDelta);
		void moveCameraRightLeftUpDown(float horizontalDelta, float verticalDelta);
		void moveCameraForwardBackward(float delta);
		glm::vec3 getCameraPosition(void);
		glm::mat4 getViewProjection(void);
};
#endif
