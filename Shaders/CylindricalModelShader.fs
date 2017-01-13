#version 140

smooth in vec3 position_v;
smooth in vec3 normal_v;

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform vec3 pointLight1Position;
uniform vec3 pointLight2Position;

out vec4 color_f;

vec4 pointLightDiffuseComponent(vec3 position_v, vec3 normal_v, vec3 pointLightPosition, vec3 materialDiffuse, vec3 pointLightDiffuse) {
	vec3 L = normalize(pointLightPosition - position_v);
	float NLdot = max(0.0f, dot(normal_v, L));
	return vec4(materialDiffuse * pointLightDiffuse * NLdot, 0.0f);
}

void main()
{
	// we get the coordinates of position and normal in the world coordinates
	vec3 normalWorldSpace = normalize((normalMatrix * vec4(normal_v, 1.0)).xyz);
	vec3 positionWorldSpace = vec3(modelMatrix * vec4(position_v, 1.0));

	vec4 outputColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	// light and material specification
	vec3 materialDiffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 sunDirection = vec3(10.0f, -10.0f, 0.0f);
	vec3 sunDiffuse = vec3(0.4f, 0.4f, 0.4f);
	vec3 pointLight1Diffuse = vec3(0.5f, 0.5f, 0.5f);
	vec3 pointLight2Diffuse = vec3(0.5f, 0.5f, 0.5f);

	outputColor += pointLightDiffuseComponent(positionWorldSpace, normalWorldSpace, pointLight1Position, materialDiffuse, pointLight1Diffuse);
	outputColor += pointLightDiffuseComponent(positionWorldSpace, normalWorldSpace, pointLight2Position, materialDiffuse, pointLight2Diffuse);

	color_f = outputColor;
}
