#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform samplerCube skybox;
uniform sampler2D sceneTexture;
uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform float ior;
uniform float alphaValue;
uniform float refractionStrength;
uniform float cubemapStrength;
uniform float fresnelStrength;
uniform vec2 viewportSize;

in vec3 worldPos;
in vec3 worldNormal;
in vec3 viewNormal;

vec3 sampleSoftSkybox(vec3 direction, float lod)
{
	return textureLod(skybox, normalize(direction), lod).rgb;
}

void main()
{
	vec3 N = normalize(worldNormal);
	vec3 I = normalize(worldPos - cameraPos);

	vec3 reflectDir = reflect(I, N);
	vec3 refractDir = refract(I, N, 1.0 / ior);
	if (length(refractDir) < 0.001)
		refractDir = reflectDir;

	// A13: cubemap reflection/refraction mixed by Schlick Fresnel.
	vec3 reflected = sampleSoftSkybox(reflectDir, 1.4) * vec3(0.82, 1.05, 1.16);
	vec3 refracted = sampleSoftSkybox(refractDir, 3.2) * vec3(0.48, 0.88, 1.08);
	float facing = max(dot(-I, N), 0.0);
	float f0 = pow((1.0 - ior) / (1.0 + ior), 2.0);
	float fresnel = f0 + (1.0 - f0) * pow(1.0 - facing, 5.0);
	fresnel = clamp(fresnel * fresnelStrength, 0.0, 1.0);
	float rim = smoothstep(0.12, 0.95, 1.0 - facing);

	vec2 screenUv = gl_FragCoord.xy / viewportSize;
	vec2 bendDirection = normalize(viewNormal.xy + vec2(0.0001));
	float normalBend = length(viewNormal.xy);
	float iorBend = clamp((ior - 1.0) * 1.8, 0.08, 0.42);
	vec2 bendOffset = bendDirection * normalBend * refractionStrength * iorBend * (0.45 + rim * 1.10);
	vec2 refractedUv = clamp(screenUv + bendOffset, vec2(0.001), vec2(0.999));
	vec2 reverseUv = clamp(screenUv - bendOffset * 0.55, vec2(0.001), vec2(0.999));
	vec3 sceneBent = texture(sceneTexture, refractedUv).rgb;
	vec3 sceneOpposite = texture(sceneTexture, reverseUv).rgb;
	sceneBent = mix(sceneBent, sceneOpposite, 0.18);

	vec3 envColor = mix(refracted, reflected, fresnel);
	vec3 waterShell = fogColor + vec3(0.08, 0.32, 0.38);
	vec3 refractedScene = mix(sceneBent, sceneBent * vec3(0.78, 1.02, 1.16) + waterShell * 0.34, 0.38);
	float envAmount = cubemapStrength * mix(0.16, 0.78, rim);
	vec3 color = mix(refractedScene, envColor, clamp(envAmount, 0.0, 1.0));
	float softRim = pow(rim, 1.6);
	float shellGlow = smoothstep(0.05, 0.85, normalBend);
	float glint = pow(max(dot(normalize(viewNormal), normalize(vec3(-0.35, 0.62, 0.70))), 0.0), 18.0);
	color += reflected * fresnel * cubemapStrength * 0.34;
	color += vec3(0.12, 0.48, 0.58) * softRim * 0.62;
	color += vec3(0.10, 0.28, 0.32) * shellGlow * 0.16;
	color += vec3(0.78, 1.0, 1.0) * glint * 0.55;

	float alpha = mix(max(alphaValue, 0.84), 0.96, rim);
	FragColor = vec4(color, alpha);
	BrightColor = vec4(0.0);
}
