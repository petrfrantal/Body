#version 140

in vec3 position;
in int index;

uniform mat4 firstMVP;
uniform mat4 secondMVP;

out vec4 color;

void main()
{
	mat4 transform;
	if (index == 1) {
		transform = firstMVP;
		//color = vec4(0.0, 0.0, 1.0, 0.0);
	} else {
		transform = secondMVP;
		//color = vec4(1.0, 0.0, 0.0, 0.0);
	}
	color = vec4(1.0, 1.0, 1.0, 0.0);
	gl_Position = transform * vec4(position, 1.0);
}
