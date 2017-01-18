#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>

/**
*	Defines the representation of the shaders.
*/

/**
*	Shader object.
*/
class Shader {
	private:
		GLuint vertexShader;
		GLuint fragmentShader;

		/**
		*	Loads a shader from a file to a string.
		*/
		std::string loadShader(const std::string& fileName);

		/**
		*	Checks for the errors occured while linking or validating the shader program.
		*/
		void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

		/**
		*	Creates a vertex or fragment shader.
		*/
		GLuint createShader(const std::string& text, unsigned int type);

		/**
		*	Finishes the creation of the shader used for the joints of the line model.
		*/
		void finishWireframeShaderCreation(void);

		/**
		*	Finishes the creation of the shader used for the bones of the line model.
		*/
		void finishLineBoneShaderCreation(void);

		/**
		*	Finishes the creation of the shader used for the cylindrical model.
		*/
		void finishCylindricalModelShaderCreation(void);
	public:
		GLuint shaderProgram;
		GLint positionLocation;
		GLint normalLocation;
		GLint jointIndexLocation;
		GLint MVPLocation;
		GLint firstMVPLocation;
		GLint secondMVPLocation;
		GLint modelMatrixLocation;				
		GLint normalMatrixLocation;
		GLint pointLight1PositionLocation;
		GLint pointLight2PositionLocation;

		Shader(const std::string& fileName);
		Shader(const std::string vertexShaderName, const std::string fragmentShaderName);
		void setMVPMatrix(glm::mat4 & MVP);
		~Shader();
};
