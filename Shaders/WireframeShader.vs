#version 140

in vec3 position;

uniform mat4 MVP;

out vec4 color;

void main()
{
	
	if (position.z == 1.0) {
	//if (position == vec3(0.0, 1.0, 1.0)) {
		color = vec4(0.0, 1.0, 0.0, 0.0);
	} else {
		color = vec4(1.0, 1.0, 1.0, 0.0);
	}
	//color = vec4(1.0, 1.0, 1.0, 0.0);
	gl_Position = MVP * vec4(position, 1.0);
}
