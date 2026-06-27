#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;
uniform bool useVertexWave;
uniform float waveTime;
uniform float waveStrength;
uniform float waveSpeed;
uniform float wavePhase;
uniform vec2 waveDirection;

out vec2 texCoord;

void main()
{
	vec3 animatedPosition = vertexPosition;
	if (useVertexWave)
	{
		float height01 = clamp(vertexPosition.y * 0.5 + 0.5, 0.0, 1.0);
		float swayMask = height01 * height01;
		vec2 dir = normalize(waveDirection + vec2(0.0001, 0.0));
		float wave = sin(waveTime * waveSpeed + wavePhase + vertexPosition.y * 2.6 + vertexPosition.x * 1.7);
		float detailWave = sin(waveTime * waveSpeed * 1.37 + wavePhase * 0.73 + vertexPosition.z * 2.1) * 0.35;
		animatedPosition.xz += dir * (wave + detailWave) * waveStrength * swayMask;
	}

	texCoord = vertexTexCoord;
	gl_Position = lightSpaceMatrix * modelMatrix * vec4(animatedPosition, 1.0);
}
