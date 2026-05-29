#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform vec3 cameraPos;
uniform vec3 fogColor;

in vec3 worldPos;
in float height01;

void main()
{
	vec3 base = mix(vec3(0.02, 0.20, 0.08), vec3(0.06, 0.48, 0.20), height01);
	float fakeLight = 0.45 + 0.45 * height01;
	vec3 color = base * fakeLight;

	float dist = length(cameraPos - worldPos);
	float fog = 1.0 - exp(-dist * 0.08);
	color = mix(color, fogColor, clamp(fog, 0.0, 0.72));

	FragColor = vec4(color, 1.0);
	BrightColor = vec4(0.0);
}
