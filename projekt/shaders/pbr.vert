#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

uniform mat4 transformation;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

out vec3 worldPos;
out vec2 texCoord;
out mat3 TBN;
out vec4 fragPosLightSpace;

void main()
{
	vec4 world = modelMatrix * vec4(vertexPosition, 1.0);
	worldPos = world.xyz;
	texCoord = vertexTexCoord;
	fragPosLightSpace = lightSpaceMatrix * world;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 N = normalize(normalMatrix * vertexNormal);
	vec3 T = normalize(normalMatrix * vertexTangent);
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(N, T));
	if (length(vertexBitangent) > 0.001)
		B = normalize(normalMatrix * vertexBitangent);

	TBN = mat3(T, B, N);
	gl_Position = transformation * vec4(vertexPosition, 1.0);
}
