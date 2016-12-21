#version 140

in vec3 position;
in int index;

const uint boneCount = 43;		// length must be hard-coded

uniform mat4 MVP[boneCount];

uniform mat4 firstMVP;
uniform mat4 secondMVP;

out vec4 color;

void main()
{
	mat4 transform;
	if (index == 0) {
		transform = firstMVP;
		color = vec4(0.0, 0.0, 1.0, 0.0);
	} else {
		transform = secondMVP;
		color = vec4(1.0, 0.0, 0.0, 0.0);
	}
	gl_Position = transform * vec4(position, 1.0);
}
