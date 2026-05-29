#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 transformation;
uniform mat4 modelMatrix;

out vec3 worldPos;
out vec3 worldNormal;

void main()
{
	vec4 world = modelMatrix * vec4(vertexPosition, 1.0);
	worldPos = world.xyz;
	worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * vertexNormal);
	gl_Position = transformation * vec4(vertexPosition, 1.0);
}
