#include "camera.h"

Camera::Camera(const glm::vec3 & pos, float horizontalA, float verticalA, float fov, float aspect, float zNear, float zFar) {
	position = pos;
	horizontalAngle = horizontalA;
	verticalAngle = verticalA;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	projection = glm::perspective(fov, aspect, zNear, zFar);
}

Camera::Camera(const glm::vec3& position, float fov, float aspect, float zNear, float zFar) {
	this->position = position;
	this->direction = glm::vec3(1.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->projection = glm::perspective(fov, aspect, zNear, zFar);
}

Camera::Camera(const glm::vec3 & pos, glm::vec3 & direction, float fov, float aspect, float zNear, float zFar) {
	position = pos;
	direction = direction;

	// compute the horizontal angle
	/*glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 directionProjectedXZ = glm::vec3(direction.x, 0.0f, direction.z);
	horizontalAngle = acos(directionProjectedXZ.x * xAxis.x / sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z));

	// compute the vertical angle
	glm::mat4 rotationToXY = glm::rotate(glm::mat4(1.0f), horizontalAngle, yAxis);
	glm::vec3 directionRotatedToXY = glm::vec3(rotationToXY * glm::vec4(direction, 1.0f));
	verticalAngle = acos(directionRotatedToXY.x * xAxis.x / sqrtf(directionRotatedToXY.x * directionRotatedToXY.x + directionRotatedToXY.y * directionRotatedToXY.y + directionRotatedToXY.z * directionRotatedToXY.z));

	horizontalAngle += glm::radians(180.0f);*/

	//std::cout << direction.x << " " << direction.y << " " << direction.z << " " << std::endl;
	//std::cout << glm::degrees(horizontalAngle) << std::endl;
	//std::cout << glm::degrees(verticalAngle) << std::endl;

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	projection = glm::perspective(fov, aspect, zNear, zFar);
}

void Camera::updateCameraAngles(float horizontalAngleDelta, float verticalAngleDelta) {
	horizontalAngle -= horizontalAngleDelta;
	// We restrict the camera vertical angle to be in [90°, -90°]. 
	// That means the direction is at most directly up or directly down. Otherwise the scene could turn around.
	if (verticalAngle - verticalAngleDelta < 1.5707f && verticalAngle - verticalAngleDelta > -1.5707f) {
		verticalAngle -= verticalAngleDelta;
	}
}

void Camera::moveCameraRightLeftUpDown(float horizontalDelta, float verticalDelta) {
	glm::vec3 right = glm::cross(direction, up);
	position -= right * horizontalDelta;
	position += up * verticalDelta;
}

void Camera::moveCameraForwardBackward(float delta) {
	position += direction * delta;
}

glm::mat4 Camera::getViewProjection() {
	direction = glm::vec3(cos(horizontalAngle) * cos(verticalAngle),
		sin(verticalAngle),
		-sin(horizontalAngle) * cos(verticalAngle));
	return projection * glm::lookAt(position, position + direction, up);
}

glm::vec3 Camera::getCameraPosition(void) {
	return position;
}