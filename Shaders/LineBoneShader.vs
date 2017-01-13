#version 140

in vec3 position;
in int index;

uniform mat4 firstMVP;
uniform mat4 secondMVP;

void main()
{
	mat4 transform;
	if (index == 1) {
		transform = firstMVP;
	} else {
		transform = secondMVP;
	}
	gl_Position = transform * vec4(position, 1.0);
}
