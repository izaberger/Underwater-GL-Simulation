#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 transformation;
uniform mat4 modelMatrix;
uniform vec2 currentDir;
uniform float time;
uniform float waveStrength;
uniform float waveSpeed;

out vec3 worldPos;
out float height01;

void main()
{
	height01 = clamp(vertexPosition.y / 1.45, 0.0, 1.0);
	vec3 current = normalize(vec3(currentDir.x, 0.0, currentDir.y));

	float phase = time * waveSpeed + vertexPosition.y * 4.2 + modelMatrix[3].x * 1.7 + modelMatrix[3].z * 0.9;
	float sway = sin(phase) * waveStrength * height01 * height01;
	float smallNoise = sin(phase * 2.31 + vertexTexCoord.x * 3.0) * waveStrength * 0.25 * height01;

	vec3 pos = vertexPosition;
	pos += current * (sway + smallNoise);

	vec4 world = modelMatrix * vec4(pos, 1.0);
	worldPos = world.xyz;
	gl_Position = transformation * vec4(pos, 1.0);
}
