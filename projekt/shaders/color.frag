#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 color;
uniform vec3 fogColor;
uniform float emissiveStrength;

in vec3 worldPos;
in vec3 worldNormal;

void main()
{
	vec3 N = normalize(worldNormal);
	vec3 L = normalize(lightPos - worldPos);
	float diff = max(dot(N, L), 0.0);
	vec3 lit = color * (0.22 + diff * 0.8) + color * emissiveStrength;

	float dist = length(cameraPos - worldPos);
	float fog = 1.0 - exp(-dist * 0.08);
	lit = mix(lit, fogColor, clamp(fog, 0.0, 0.7));

	FragColor = vec4(lit, 1.0);
	BrightColor = emissiveStrength > 0.1 ? vec4(lit, 1.0) : vec4(0.0);
}
