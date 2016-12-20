#version 140

in vec3 position;
in uint index;

const uint boneCount = 43;		// length must be hard-coded

uniform mat4 MVP[boneCount];

void main()
{
	mat4 transform = MVP[index];
	gl_Position = transform * vec4(position, 1.0);
}
