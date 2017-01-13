#pragma once

#define _SCL_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "obj_loader.h"
#include "shader.h"
#include "Cylinder.h"
#include "Sphere.h"

/**
*	Defines the structures used for the representation of the meshes and OpenGL buffer objects (VAOs, VBOs, EBOs).
*/

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3* getPos() { return &pos; }
	glm::vec2* getTexCoord() { return &texCoord; }
	glm::vec3* getNormal() { return &normal; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

class Mesh {
	private:
		static const unsigned int NUM_BUFFERS = 4;
		GLuint m_vertexArrayObject;
		GLuint m_vertexArrayBuffers[NUM_BUFFERS];
		GLuint cylinderVertexArrayObject;	// VAO for cylinder model
		GLuint sphereVertexArrayObject;		// VAO for sphere model
		GLuint jointVertexArrayObject;		// VAO for joints as points
		GLuint lineVertexArrayObject;		// VAO for bones as lines
		GLuint cylinderVertexBufferObject;	// VBO for cylinder model
		GLuint cylinderElementBufferObject;	// EBO for cylinder model
		GLuint sphereVertexBufferObject;	// VBO for sphere model
		GLuint sphereElementBufferObject;	// EBO for sphere model
		GLuint jointVertexBufferObject;		// VBO for joints as points
		GLuint lineVertexAttrBufferObject;	// VBO for bones as lines
		GLuint lineIndexAttrBufferObject;	// second VBO for bones as lines: contains indices of the joint in bone
		unsigned int cylinderTriangleCount;
		unsigned int sphereTriangleCount;

		unsigned int m_numIndices;
		std::vector<glm::vec3> positions;
		void initMesh(const IndexedModel& model);
	public:
		Mesh(const std::string& fileName);
		Mesh(WireframeModel * wireframeModel, Shader * shader);
		Mesh(WireframeModel * wireframeModel, Shader * jointShader, Shader * lineBoneShader);
		Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
		Mesh(Shader * shader);
		GLuint getCylinderVAO(void);
		GLuint getSphereVAO(void);
		GLuint getLineBoneVAO(void);
		GLuint getPointJointVAO(void);
		unsigned int getCylinderTrinagleCount(void);
		unsigned int getSphereTriangleCount(void);
		void draw();
		~Mesh();
};
