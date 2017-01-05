#version 140

smooth in vec3 position_v;
smooth in vec3 normal_v;

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

out vec4 color_f;

vec4 sunDiffuseComponent(vec3 normal_v, vec3 materialDiffuse, vec3 sunDiffuse, vec3 sunDirection) {
	vec3 L = normalize(-sunDirection); //smer ke svetlu
	float NLdot = max(0.0f, dot(normal_v, L));
	return vec4(materialDiffuse * sunDiffuse * NLdot, 0.0f);
}

vec4 sunSpecularComponent(vec3 position_v, vec3 normal_v, vec3 cameraPosition, vec3 sunDirection, vec3 materialSpecular, vec3 sunSpecular, float materialShininess) {
	vec3 L = normalize(-sunDirection);
	vec3 R = reflect(-L, normal_v);
	vec3 V = normalize(cameraPosition - position_v);
	float RVdot = max(0.0f, dot(R, V));
	return vec4(materialSpecular * sunSpecular * pow(RVdot, materialShininess), 0.0f);
}

void main()
{
	// we get the coordinates of position and normal in the world coordinates
	vec3 normalWorldSpace = normalize((normalMatrix * vec4(normal_v, 1.0)).xyz);
	vec3 positionWorldSpace = vec3(modelMatrix * vec4(position_v, 1.0));

	vec4 outputColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	// hardcoded light and material
	vec3 materialDiffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 sunDiffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 sunDirection = vec3(10.0f, -10.0f, 0.0f);

	outputColor += sunDiffuseComponent(normalWorldSpace, materialDiffuse, sunDiffuse, sunDirection);
	//outputColor += sunSpecularComponent(positionWorldSpace, normalWorldSpace, cameraPosition, sunDirection, materialSpecular, sunSpecular, materialShininess);

	color_f = outputColor;
	//color_f = vec4(1.0, 0.0, 0.0, 0.0);
}
