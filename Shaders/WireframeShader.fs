#version 140

in vec4 color;
out vec4 color_f;

void main()
{
	color_f = color;
	//color_f = vec4(1.0, 1.0, 1.0, 0.0);
}
