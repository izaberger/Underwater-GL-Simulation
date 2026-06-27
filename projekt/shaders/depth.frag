#version 330 core

uniform sampler2D opacityTexture;
uniform float textureScale;
uniform float alphaCutoff;
uniform bool flipTextureY;
uniform bool useOpacityTexture;

in vec2 texCoord;

void main()
{
	vec2 fixedTexCoord = texCoord;
	if (flipTextureY)
		fixedTexCoord.y = 1.0 - fixedTexCoord.y;

	if (useOpacityTexture && texture(opacityTexture, fixedTexCoord * textureScale).r < alphaCutoff)
		discard;
}
