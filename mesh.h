#pragma once

#define _SCL_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "shader.h"
#include "Cylinder.h"
#include "Sphere.h"

/**
*	Defines the structures used for the representation of the meshes and OpenGL buffer objects (VAOs, VBOs, EBOs).
*/

struct WireframeModel {
	std::vector<float> vertices;
	std::vector<float> boneVertices;
	std::vector<unsigned int> boneIndices;
};


/**
*	Contains the OpenGL buffers to draw the animation.
*/
class Mesh {
	private:
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
	public:
		Mesh(WireframeModel * wireframeModel, Shader * shader);
		Mesh(WireframeModel * wireframeModel, Shader * jointShader, Shader * lineBoneShader);
		Mesh(Shader * shader);
		GLuint getCylinderVAO(void);
		GLuint getSphereVAO(void);
		GLuint getLineBoneVAO(void);
		GLuint getPointJointVAO(void);
		unsigned int getCylinderTrinagleCount(void);
		unsigned int getSphereTriangleCount(void);
		~Mesh();
};
