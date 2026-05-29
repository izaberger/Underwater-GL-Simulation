#version 330 core

out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

in vec2 texCoord;

void main()
{
	vec3 hdrColor = texture(scene, texCoord).rgb;
	vec3 bloomColor = texture(bloomBlur, texCoord).rgb;
	if (bloom)
		hdrColor += bloomColor;

	vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
	mapped = pow(mapped, vec3(1.0 / 2.2));
	FragColor = vec4(mapped, 1.0);
}
