#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform sampler2D colorTexture;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform sampler2D opacityTexture;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 baseColor;
uniform vec3 emissiveColor;
uniform vec3 fogColor;

uniform float metallic;
uniform float roughness;
uniform float ao;
uniform float emissiveStrength;
uniform float textureScale;
uniform float normalStrength;
uniform float ambientStrength;
uniform float fogDensity;
uniform float fogMax;
uniform bool enableShadows;
uniform bool flipTextureY;
uniform bool useOpacityTexture;
uniform float alphaCutoff;
uniform float crystalAlpha; 

in vec3 worldPos;
in vec2 texCoord;
in mat3 TBN;
in vec4 fragPosLightSpace;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float r)
{
	float a = r * r;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return a2 / max(denom, 0.0001);
}

float geometrySchlickGGX(float NdotV, float r)
{
	float k = ((r + 1.0) * (r + 1.0)) / 8.0;
	return NdotV / max(NdotV * (1.0 - k) + k, 0.0001);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float r)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	return geometrySchlickGGX(NdotV, r) * geometrySchlickGGX(NdotL, r);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float shadowCalculation(vec4 fragPosLight, vec3 N, vec3 L)
{
	vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0)
		return 0.0;

	float bias = max(0.006 * (1.0 - dot(N, L)), 0.0015);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9.0;
}

void main()
{
	vec2 fixedTexCoord = texCoord;
	if (flipTextureY)
		fixedTexCoord.y = 1.0 - fixedTexCoord.y;

	vec2 scaledTexCoord = fixedTexCoord * textureScale;
	if (useOpacityTexture && texture(opacityTexture, scaledTexCoord).r < alphaCutoff)
		discard;

	vec3 albedo = pow(texture(colorTexture, scaledTexCoord).rgb * baseColor, vec3(2.2));

	vec3 tangentNormal = texture(normalMap, scaledTexCoord).rgb;
	tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
	tangentNormal.xy *= normalStrength;
	tangentNormal = normalize(tangentNormal);
	vec3 N = normalize(TBN * tangentNormal);
	vec3 V = normalize(cameraPos - worldPos);
	if (dot(N, V) < 0.0)
		N = -N;
	vec3 L = normalize(lightPos - worldPos);
	vec3 H = normalize(V + L);

	float distanceToLight = length(lightPos - worldPos);
	float attenuation = 1.0 / max(distanceToLight * distanceToLight * 0.12, 1.0);
	vec3 radiance = lightColor * attenuation;

	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	float r = clamp(roughness, 0.04, 1.0);
	float NDF = distributionGGX(N, H, r);
	float G = geometrySmith(N, V, L, r);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
	float NdotL = max(dot(N, L), 0.0);
	float shadow = enableShadows ? shadowCalculation(fragPosLightSpace, N, L) : 0.0;

	vec3 ambient = vec3(0.02, 0.12, 0.14) * albedo * ao * ambientStrength;
	vec3 color = ambient + (1.0 - shadow) * (kD * albedo / PI + specular) * radiance * NdotL;
	color += emissiveColor * emissiveStrength;

	float dist = length(cameraPos - worldPos);
	float fog = 1.0 - exp(-dist * fogDensity);
	color = mix(color, fogColor, clamp(fog, 0.0, fogMax));
	
	float finalAlpha = 1.0;
	vec3 finalColor = color;
	vec3 bloomContrib = vec3(0.0);

	if (emissiveStrength > 0.0) 
	{
		// Fresnel na kontury kryształu 
		float fresnelEdge = pow(1.0 - max(dot(N, V), 0.0), 3.0);
		vec3 contour = emissiveColor * fresnelEdge * 2.5; 
		
		finalColor += contour;
		finalAlpha = 0.55; 
		
		// czyste swiatlo do emisji bufora
		bloomContrib = (emissiveColor * emissiveStrength) + contour;
	}
	else
	{
		// nieswiecace obiekty
		if (dot(finalColor, vec3(0.2126, 0.7152, 0.0722)) > 1.0)
			bloomContrib = finalColor;
	}

	// poprawa blendingu
	FragColor = vec4(finalColor * finalAlpha, finalAlpha);

	if (emissiveStrength > 0.0) {
		// blask do bufora bloom
		BrightColor = vec4(bloomContrib, 0.0);
	} else {
		// skaly i dno
		BrightColor = vec4(bloomContrib, 1.0);
	}
}