#include "shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string vertexShaderName, const std::string fragmentShaderName) {
	shaderProgram = glCreateProgram();
	vertexShader = createShader(loadShader(vertexShaderName + ".vs"), GL_VERTEX_SHADER);
	fragmentShader = createShader(loadShader(fragmentShaderName + ".fs"), GL_FRAGMENT_SHADER);
	//finishShaderCreation();
	//finishWireframeShaderCreation();
	finishLineBoneShaderCreation();
}

Shader::Shader(const std::string & fileName)
{
	shaderProgram = glCreateProgram();
	vertexShader = createShader(loadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	fragmentShader = createShader(loadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
	if (fileName == "./Shaders/WireframeShader") {
		finishWireframeShaderCreation();
	} else if (fileName == "./Shaders/CylindricalModelShader") {
		finishCylindricalModelShaderCreation();
	} else {
		finishShaderCreation();
	}
}

void Shader::finishCylindricalModelShaderCreation(void) {
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Error linking shader program");
	glValidateProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Invalid shader program");
	positionLocation = glGetAttribLocation(shaderProgram, "position");
	MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	normalLocation = glGetAttribLocation(shaderProgram, "normal");
	modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");
	pointLight1PositionLocation = glGetUniformLocation(shaderProgram, "pointLight1Position");
	pointLight2PositionLocation = glGetUniformLocation(shaderProgram, "pointLight2Position");
}

void Shader::finishLineBoneShaderCreation(void) {
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Error linking shader program");
	glValidateProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Invalid shader program");
	positionLocation = glGetAttribLocation(shaderProgram, "position");
	jointIndexLocation = glGetAttribLocation(shaderProgram, "index");
	firstMVPLocation = glGetUniformLocation(shaderProgram, "firstMVP");
	secondMVPLocation = glGetUniformLocation(shaderProgram, "secondMVP");
}

void Shader::finishWireframeShaderCreation(void) {
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Error linking shader program");
	glValidateProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Invalid shader program");
	positionLocation = glGetAttribLocation(shaderProgram, "position");
	MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
}

void Shader::finishShaderCreation(void) {
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindAttribLocation(shaderProgram, 0, "position");
	glBindAttribLocation(shaderProgram, 1, "texCoord");
	glBindAttribLocation(shaderProgram, 2, "normal");

	glLinkProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(shaderProgram);
	checkShaderError(shaderProgram, GL_LINK_STATUS, true, "Invalid shader program");

	m_uniforms[0] = glGetUniformLocation(shaderProgram, "MVP");
	m_uniforms[1] = glGetUniformLocation(shaderProgram, "Normal");
	m_uniforms[2] = glGetUniformLocation(shaderProgram, "lightDirection");
}

Shader::~Shader()
{
    glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

void Shader::Bind()
{
	glUseProgram(shaderProgram);
}

void Shader::Update(const Transform& transform, Camera & camera)
{
	glm::mat4 MVP = transform.GetMVP(camera);
	glm::mat4 Normal = transform.GetModel();

	glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
	glUniform3f(m_uniforms[2], 0.0f, 0.0f, 1.0f);
}

void Shader::setMVPMatrix(glm::mat4 & MVP) {
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);
}

std::string Shader::loadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

GLuint Shader::createShader(const std::string& text, unsigned int type)
{
    GLuint shader = glCreateShader(type);

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

    return shader;
}
