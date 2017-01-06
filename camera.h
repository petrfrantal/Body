#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

struct Camera
{
public:
	Camera(const glm::vec3& position, float fov, float aspect, float zNear, float zFar)
	{
		this->position = position;
		this->direction = glm::vec3(1.0f, 0.0f, 0.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, zNear, zFar);
	}

	Camera(const glm::vec3& position, glm::vec3 & direction, float fov, float aspect, float zNear, float zFar) {
		this->position = position;
		this->direction = direction;

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

		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, zNear, zFar);
	}

	Camera(const glm::vec3 & pos, float horizontalA, float verticalA, float fov, float aspect, float zNear, float zFar) {
		position = pos;
		horizontalAngle = horizontalA;
		verticalAngle = verticalA;
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		projection = glm::perspective(fov, aspect, zNear, zFar);
	}

	inline glm::mat4 getViewProjection() {
		//std::cout << glm::degrees(horizontalAngle) << std::endl;
		//std::cout << glm::degrees(verticalAngle) << std::endl;
		direction = glm::vec3(cos(horizontalAngle) * cos(verticalAngle),
								sin(verticalAngle),
								-sin(horizontalAngle) * cos(verticalAngle));
		//std::cout << direction.x << " " << direction.y << " " << direction.z << " " << std::endl;
		return projection * glm::lookAt(position, position + direction, up);
	}

	//void MoveForward(float amt)
	//{
	//	position += direction * amt;
	//}

	//void MoveRight(float amt)
	//{
	//	position += glm::cross(up, direction) * amt;
	//}

	//void Pitch(float angle)
	//{
	//	glm::vec3 right = glm::normalize(glm::cross(up, direction));

	//	direction = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(direction, 0.0)));
	//	up = glm::normalize(glm::cross(direction, right));
	//}

	//void RotateY(float angle)
	//{
	//	static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

	//	glm::mat4 rotation = glm::rotate(angle, UP);

	//	direction = glm::vec3(glm::normalize(rotation * glm::vec4(direction, 0.0)));
	//	up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	//}

protected:
private:
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	float horizontalAngle;
	float verticalAngle;
};

#endif
