#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform samplerCube skybox;
uniform vec3 fogColor;
uniform float time;

in vec3 texCoord;

void main()
{
	vec3 env = texture(skybox, texCoord).rgb;
	vec3 waterTint = vec3(0.02, 0.26, 0.34);
	float upperGlow = smoothstep(-0.2, 0.85, normalize(texCoord).y);
	float rays = pow(max(0.0, sin(texCoord.x * 12.0 + time * 0.4) * 0.5 + 0.5), 16.0) * upperGlow * 0.12;

	vec3 color = mix(fogColor, env * waterTint + vec3(0.0, 0.18, 0.22), 0.35);
	color += vec3(0.10, 0.42, 0.50) * upperGlow * 0.25 + vec3(rays);

	FragColor = vec4(color, 1.0);
	BrightColor = vec4(0.0);
}
