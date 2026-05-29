#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform float ior;
uniform float alphaValue;

in vec3 worldPos;
in vec3 worldNormal;

void main()
{
	vec3 N = normalize(worldNormal);
	vec3 I = normalize(worldPos - cameraPos);

	vec3 reflected = texture(skybox, reflect(I, N)).rgb;
	vec3 refracted = texture(skybox, refract(I, N, 1.0 / ior)).rgb;
	float fresnel = pow(1.0 - max(dot(-I, N), 0.0), 5.0);

	vec3 color = mix(refracted, reflected, fresnel);
	color = mix(fogColor, color * vec3(0.55, 0.90, 1.0), 0.72);
	color += vec3(0.12, 0.65, 0.95) * fresnel * 1.8;

	FragColor = vec4(color, alphaValue);
	BrightColor = vec4(vec3(fresnel * 0.8), 1.0);
}
