#pragma once
#include <iostream>

/**
*	Auxiliary methods and defines.
*/

#define PI 3.14159f

float degreesToRadians(float angleDeg) {
	return angleDeg * (PI / 180.0f);
}

/***
*	Prints the given matrix so that it's columns are printed in columns (does not transpose the matrix).
*/
void printGlmMatrixColumnsAsColumns(glm::mat4 & m) {
	std::cout << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << " |" << std::endl;
	std::cout << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << " |" << std::endl;
	std::cout << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << " |" << std::endl;
	std::cout << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << " V" << std::endl;
}
