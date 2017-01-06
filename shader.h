#pragma once

#include <string>
#include <GL/glew.h>
#include "transform.h"

class Shader {
	private:
		static const unsigned int NUM_UNIFORMS = 3;
		
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint m_uniforms[NUM_UNIFORMS];
		std::string loadShader(const std::string& fileName);
		void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
		GLuint createShader(const std::string& text, unsigned int type);
		void finishShaderCreation(void);
		void finishWireframeShaderCreation(void);
		void finishLineBoneShaderCreation(void);
		void finishCylindricalModelShaderCreation(void);
	public:
		GLuint shaderProgram;
		GLint positionLocation;
		GLint normalLocation;
		GLint jointIndexLocation;
		GLint MVPLocation;
		GLint firstMVPLocation;
		GLint secondMVPLocation;
		GLint modelMatrixLocation;		// these two are for calculating of the lighting in the cylindrical model
		GLint normalMatrixLocation;

		Shader(const std::string& fileName);
		Shader(const std::string vertexShaderName, const std::string fragmentShaderName);
		void Bind();
		void Update(const Transform& transform, Camera& camera);
		void setMVPMatrix(glm::mat4 & MVP);
		~Shader();
};
