#pragma once

#include "glew.h"
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "ext.hpp"
#include "gtc/constants.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Texture.h"

#include "imgui.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;
const int BLOOM_BLUR_PASSES = 8;

float aspectRatio = 1280.0f / 720.0f;
int framebufferWidth = 1280;
int framebufferHeight = 720;

float lastTime = -1.0f;
float deltaTime = 0.0f;

Core::Shader_Loader shaderLoader;

GLuint programPBR = 0;
GLuint programSkybox = 0;
GLuint programRefract = 0;
GLuint programSeaweed = 0;
GLuint programDepth = 0;
GLuint programColor = 0;
GLuint programBlur = 0;
GLuint programBloomCombine = 0;

Core::RenderContext sphereContext;
Core::RenderContext cubeContext;
Core::RenderContext rockContext;
std::vector<Core::RenderContext> caveContexts;

GLuint skyboxVAO = 0;
GLuint skyboxVBO = 0;
GLuint skyboxCubemap = 0;

GLuint seaweedVAO = 0;
GLuint seaweedVBO = 0;
GLuint seaweedEBO = 0;
int seaweedIndexCount = 0;

GLuint quadVAO = 0;
GLuint quadVBO = 0;

GLuint depthMapFBO = 0;
GLuint depthMap = 0;

GLuint hdrFBO = 0;
GLuint colorBuffers[2] = { 0, 0 };
GLuint hdrDepthRBO = 0;
GLuint pingpongFBO[2] = { 0, 0 };
GLuint pingpongColorbuffers[2] = { 0, 0 };

bool enableBloom = true;
bool enableShadows = true;
bool enableBubble = true;
bool enableCrystals = true;
bool mouseLook = true;

float bloomExposure = 1.0f;
float crystalGlow = 3.2f;
float bubbleIOR = 1.12f;
float bubbleAlpha = 0.55f;
float seaweedStrength = 0.28f;
float seaweedSpeed = 1.15f;
glm::vec2 currentDirection = glm::normalize(glm::vec2(0.7f, 0.35f));

bool keyCWasDown = false;
bool keyPWasDown = false;

double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool firstMouse = true;

struct Material
{
	GLuint albedo = 0;
	GLuint normal = 0;
	glm::vec3 baseColor = glm::vec3(1.0f);
	float metallic = 0.0f;
	float roughness = 0.5f;
	float ao = 1.0f;
	float textureScale = 1.0f;
	float normalStrength = 0.58f;
	float ambientStrength = 1.0f;
	float fogDensity = 0.075f;
	float fogMax = 0.78f;
};

Material makeMaterial(GLuint albedo, GLuint normal, glm::vec3 baseColor, float metallic, float roughness, float ao)
{
	Material material;
	material.albedo = albedo;
	material.normal = normal;
	material.baseColor = baseColor;
	material.metallic = metallic;
	material.roughness = roughness;
	material.ao = ao;
	return material;
}

namespace texture
{
	GLuint rockAlbedo = 0;
	GLuint rockNormal = 0;
	GLuint rustAlbedo = 0;
	GLuint rustNormal = 0;
	GLuint sandAlbedo = 0;
	GLuint flatNormal = 0;
	GLuint crystalAlbedo = 0;
	GLuint caveAlbedo = 0;
	GLuint caveNormal = 0;
}

Material rockMaterial;
Material rustMaterial;
Material sandMaterial;
Material crystalMaterial;
Material caveMaterial;

glm::vec3 lightPos = glm::vec3(-2.5f, 4.6f, -2.0f);
glm::vec3 lightColor = glm::vec3(4.0f, 5.2f, 6.8f);
glm::vec3 waterFogColor = glm::vec3(0.02f, 0.20f, 0.24f);

float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

struct QuaternionCamera
{
	glm::vec3 position = glm::vec3(0.0f, -0.35f, 0.95f);
	glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	float mouseSensitivity = 0.0022f;

	glm::vec3 forward() const
	{
		return glm::normalize(orientation * glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 right() const
	{
		return glm::normalize(orientation * glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 up() const
	{
		return glm::normalize(orientation * glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::mat4 viewMatrix() const
	{
		glm::mat4 rotation = glm::toMat4(glm::conjugate(orientation));
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), -position);
		return rotation * translation;
	}

	void rotate(float dx, float dy)
	{
		glm::quat yaw = glm::angleAxis(-dx * mouseSensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
		orientation = glm::normalize(yaw * orientation);

		glm::quat pitch = glm::angleAxis(-dy * mouseSensitivity, right());
		orientation = glm::normalize(pitch * orientation);
	}
};

QuaternionCamera camera;

struct PTFrame
{
	glm::vec3 p;
	glm::vec3 t;
	glm::vec3 n;
	glm::vec3 b;
};

std::vector<glm::vec3> caveRimPath = {
	glm::vec3(-3.8f, 0.10f, -4.6f),
	glm::vec3(-3.1f, 1.80f, -4.8f),
	glm::vec3(-1.2f, 2.75f, -4.9f),
	glm::vec3( 1.4f, 2.65f, -4.8f),
	glm::vec3( 3.5f, 0.35f, -4.6f),
	glm::vec3( 3.2f, 1.40f,  4.4f),
	glm::vec3( 1.1f, 2.45f,  4.8f),
	glm::vec3(-1.6f, 2.55f,  4.7f),
	glm::vec3(-3.4f, 0.55f,  4.4f)
};

std::vector<PTFrame> transportFrames;

float clampFloat(float v, float lo, float hi)
{
	return std::max(lo, std::min(v, hi));
}

glm::mat4 createPerspectiveMatrix()
{
	return glm::perspective(glm::radians(60.0f), aspectRatio, 0.005f, 80.0f);
}

glm::mat4 makeModel(glm::vec3 position, glm::vec3 rotationDeg, glm::vec3 scale)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::rotate(model, glm::radians(rotationDeg.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotationDeg.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotationDeg.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);
	return model;
}

glm::mat4 caveModelMatrix()
{
	const glm::vec3 importedCenter = glm::vec3(0.012875f, 0.014385f, -0.885280f);
	const float scale = 2.2f;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), -importedCenter * scale);
	model = glm::scale(model, glm::vec3(scale));
	return model;
}

glm::mat4 lightSpaceMatrix()
{
	glm::mat4 lightProjection = glm::ortho(-11.0f, 11.0f, -8.0f, 8.0f, 1.0f, 24.0f);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, -0.2f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return lightProjection * lightView;
}

GLuint createSolidTexture(unsigned char r, unsigned char g, unsigned char b)
{
	GLuint id;
	unsigned char pixel[] = { r, g, b, 255 };
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return id;
}

void setTextureForModelAsset(GLuint textureID)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void loadModelToContext(std::string path, Core::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || scene->mNumMeshes == 0)
	{
		std::cout << "ERROR::ASSIMP:: " << path << " " << import.GetErrorString() << std::endl;
		return;
	}

	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void loadModelToContexts(std::string path, std::vector<Core::RenderContext>& contexts)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || scene->mNumMeshes == 0)
	{
		std::cout << "ERROR::ASSIMP:: " << path << " " << import.GetErrorString() << std::endl;
		return;
	}

	contexts.clear();
	contexts.resize(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		contexts[i].initFromAssimpMesh(scene->mMeshes[i]);
}

void initSkybox()
{
	const char* faces[6] = {
		"../project_assets/skybox/_px.jpg",
		"../project_assets/skybox/_nx.jpg",
		"../project_assets/skybox/_py.jpg",
		"../project_assets/skybox/_ny.jpg",
		"../project_assets/skybox/_pz.jpg",
		"../project_assets/skybox/_nz.jpg"
	};

	skyboxCubemap = Core::LoadCubemap(faces);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

void initShadowMap()
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void createBloomBuffers(int width, int height)
{
	width = std::max(width, 1);
	height = std::max(height, 1);

	if (hdrFBO != 0)
	{
		glDeleteFramebuffers(1, &hdrFBO);
		glDeleteTextures(2, colorBuffers);
		glDeleteRenderbuffers(1, &hdrDepthRBO);
		glDeleteFramebuffers(2, pingpongFBO);
		glDeleteTextures(2, pingpongColorbuffers);
	}

	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glGenTextures(2, colorBuffers);

	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	glGenRenderbuffers(1, &hdrDepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrDepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrDepthRBO);

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "HDR framebuffer is not complete" << std::endl;

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

struct SeaweedVertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

void initSeaweedMesh()
{
	std::vector<SeaweedVertex> vertices;
	std::vector<unsigned int> indices;
	const int segments = 7;
	const float height = 1.45f;

	for (int i = 0; i <= segments; i++)
	{
		float v = i / float(segments);
		float y = v * height;
		float width = 0.16f * (1.0f - 0.75f * v);

		vertices.push_back({ glm::vec3(-width, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, v) });
		vertices.push_back({ glm::vec3( width, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, v) });
	}

	for (int i = 0; i < segments; i++)
	{
		unsigned int base = i * 2;
		indices.push_back(base);
		indices.push_back(base + 1);
		indices.push_back(base + 2);
		indices.push_back(base + 1);
		indices.push_back(base + 3);
		indices.push_back(base + 2);
	}

	seaweedIndexCount = int(indices.size());

	glGenVertexArrays(1, &seaweedVAO);
	glGenBuffers(1, &seaweedVBO);
	glGenBuffers(1, &seaweedEBO);

	glBindVertexArray(seaweedVAO);
	glBindBuffer(GL_ARRAY_BUFFER, seaweedVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SeaweedVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, seaweedEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SeaweedVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SeaweedVertex), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SeaweedVertex), (void*)(2 * sizeof(glm::vec3)));
	glBindVertexArray(0);
}

void initScreenQuad()
{
	float quadVertices[] = {
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}

glm::vec3 sampleClosedPath(float x)
{
	int count = int(caveRimPath.size());
	x = x - std::floor(x);
	float scaled = x * count;
	int i = int(std::floor(scaled));
	float t = scaled - i;

	glm::vec3 p0 = caveRimPath[(i - 1 + count) % count];
	glm::vec3 p1 = caveRimPath[i % count];
	glm::vec3 p2 = caveRimPath[(i + 1) % count];
	glm::vec3 p3 = caveRimPath[(i + 2) % count];

	float t2 = t * t;
	float t3 = t2 * t;
	return 0.5f * ((2.0f * p1) +
		(-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
}

void buildTransportFrames()
{
	transportFrames.clear();
	const int samples = 160;
	std::vector<glm::vec3> points;
	points.reserve(samples);

	for (int i = 0; i < samples; i++)
		points.push_back(sampleClosedPath(i / float(samples)));

	std::vector<glm::vec3> tangents(samples);
	for (int i = 0; i < samples; i++)
	{
		glm::vec3 prev = points[(i - 1 + samples) % samples];
		glm::vec3 next = points[(i + 1) % samples];
		tangents[i] = glm::normalize(next - prev);
	}

	glm::vec3 n = glm::cross(tangents[0], glm::vec3(0.0f, 1.0f, 0.0f));
	if (glm::length(n) < 0.01f)
		n = glm::vec3(1.0f, 0.0f, 0.0f);
	else
		n = glm::normalize(n);
	glm::vec3 b = glm::normalize(glm::cross(tangents[0], n));
	n = glm::normalize(glm::cross(b, tangents[0]));

	transportFrames.push_back({ points[0], tangents[0], n, b });

	for (int i = 1; i < samples; i++)
	{
		glm::vec3 previousT = tangents[i - 1];
		glm::vec3 currentT = tangents[i];
		glm::vec3 axis = glm::cross(previousT, currentT);
		float axisLen = glm::length(axis);

		if (axisLen > 0.0001f)
		{
			axis /= axisLen;
			float angle = std::acos(clampFloat(glm::dot(previousT, currentT), -1.0f, 1.0f));
			glm::quat q = glm::angleAxis(angle, axis);
			n = glm::normalize(q * n);
		}

		b = glm::normalize(glm::cross(currentT, n));
		n = glm::normalize(glm::cross(b, currentT));
		transportFrames.push_back({ points[i], currentT, n, b });
	}
}

PTFrame getFrameAt(float x)
{
	if (transportFrames.empty())
		return { glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) };

	x = x - std::floor(x);
	int id = int(x * transportFrames.size()) % int(transportFrames.size());
	return transportFrames[id];
}

glm::mat4 frameMatrix(const PTFrame& frame, glm::vec3 position)
{
	glm::mat4 m(1.0f);
	m[0] = glm::vec4(frame.t, 0.0f);
	m[1] = glm::vec4(frame.n, 0.0f);
	m[2] = glm::vec4(frame.b, 0.0f);
	m[3] = glm::vec4(position, 1.0f);
	return m;
}

void drawDepth(Core::RenderContext& context, glm::mat4 modelMatrix)
{
	glUseProgram(programDepth);
	glm::mat4 lightSpace = lightSpaceMatrix();
	glUniformMatrix4fv(glGetUniformLocation(programDepth, "lightSpaceMatrix"), 1, GL_FALSE, (float*)&lightSpace);
	glUniformMatrix4fv(glGetUniformLocation(programDepth, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	Core::DrawContext(context);
}

void drawPBR(Core::RenderContext& context, glm::mat4 modelMatrix, const Material& mat, glm::vec3 emissiveColor, float emissiveStrength)
{
	glUseProgram(programPBR);

	glm::mat4 view = camera.viewMatrix();
	glm::mat4 projection = createPerspectiveMatrix();
	glm::mat4 transformation = projection * view * modelMatrix;
	glm::mat4 lightSpace = lightSpaceMatrix();

	glUniformMatrix4fv(glGetUniformLocation(programPBR, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(programPBR, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(programPBR, "lightSpaceMatrix"), 1, GL_FALSE, (float*)&lightSpace);
	glUniform3fv(glGetUniformLocation(programPBR, "cameraPos"), 1, (float*)&camera.position);
	glUniform3fv(glGetUniformLocation(programPBR, "lightPos"), 1, (float*)&lightPos);
	glUniform3fv(glGetUniformLocation(programPBR, "lightColor"), 1, (float*)&lightColor);
	glUniform3fv(glGetUniformLocation(programPBR, "baseColor"), 1, (float*)&mat.baseColor);
	glUniform1f(glGetUniformLocation(programPBR, "metallic"), mat.metallic);
	glUniform1f(glGetUniformLocation(programPBR, "roughness"), mat.roughness);
	glUniform1f(glGetUniformLocation(programPBR, "ao"), mat.ao);
	glUniform1f(glGetUniformLocation(programPBR, "textureScale"), mat.textureScale);
	glUniform1f(glGetUniformLocation(programPBR, "normalStrength"), mat.normalStrength);
	glUniform1f(glGetUniformLocation(programPBR, "ambientStrength"), mat.ambientStrength);
	glUniform1f(glGetUniformLocation(programPBR, "fogDensity"), mat.fogDensity);
	glUniform1f(glGetUniformLocation(programPBR, "fogMax"), mat.fogMax);
	glUniform3fv(glGetUniformLocation(programPBR, "emissiveColor"), 1, (float*)&emissiveColor);
	glUniform1f(glGetUniformLocation(programPBR, "emissiveStrength"), emissiveStrength);
	glUniform3fv(glGetUniformLocation(programPBR, "fogColor"), 1, (float*)&waterFogColor);
	glUniform1i(glGetUniformLocation(programPBR, "enableShadows"), enableShadows ? 1 : 0);

	Core::SetActiveTexture(mat.albedo, "colorTexture", programPBR, 0);
	Core::SetActiveTexture(mat.normal, "normalMap", programPBR, 1);
	glUniform1i(glGetUniformLocation(programPBR, "shadowMap"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	Core::DrawContext(context);
}

void drawColor(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color, float emissiveStrength)
{
	glUseProgram(programColor);

	glm::mat4 view = camera.viewMatrix();
	glm::mat4 projection = createPerspectiveMatrix();
	glm::mat4 transformation = projection * view * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(programColor, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(programColor, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3fv(glGetUniformLocation(programColor, "cameraPos"), 1, (float*)&camera.position);
	glUniform3fv(glGetUniformLocation(programColor, "lightPos"), 1, (float*)&lightPos);
	glUniform3fv(glGetUniformLocation(programColor, "lightColor"), 1, (float*)&lightColor);
	glUniform3fv(glGetUniformLocation(programColor, "color"), 1, (float*)&color);
	glUniform1f(glGetUniformLocation(programColor, "emissiveStrength"), emissiveStrength);
	glUniform3fv(glGetUniformLocation(programColor, "fogColor"), 1, (float*)&waterFogColor);

	Core::DrawContext(context);
}

void drawSeaweed(glm::mat4 modelMatrix, float time)
{
	glUseProgram(programSeaweed);

	glm::mat4 view = camera.viewMatrix();
	glm::mat4 projection = createPerspectiveMatrix();
	glm::mat4 transformation = projection * view * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(programSeaweed, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(programSeaweed, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3fv(glGetUniformLocation(programSeaweed, "cameraPos"), 1, (float*)&camera.position);
	glUniform1f(glGetUniformLocation(programSeaweed, "time"), time);
	glUniform1f(glGetUniformLocation(programSeaweed, "waveStrength"), seaweedStrength);
	glUniform1f(glGetUniformLocation(programSeaweed, "waveSpeed"), seaweedSpeed);
	glUniform2fv(glGetUniformLocation(programSeaweed, "currentDir"), 1, (float*)&currentDirection);
	glUniform3fv(glGetUniformLocation(programSeaweed, "fogColor"), 1, (float*)&waterFogColor);

	glBindVertexArray(seaweedVAO);
	glDrawElements(GL_TRIANGLES, seaweedIndexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void drawSkybox()
{
	glDepthFunc(GL_LEQUAL);
	glUseProgram(programSkybox);

	glm::mat4 view = glm::mat4(glm::mat3(camera.viewMatrix()));
	glm::mat4 projection = createPerspectiveMatrix();

	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "view"), 1, GL_FALSE, (float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "projection"), 1, GL_FALSE, (float*)&projection);
	glUniform3fv(glGetUniformLocation(programSkybox, "fogColor"), 1, (float*)&waterFogColor);
	glUniform1f(glGetUniformLocation(programSkybox, "time"), float(glfwGetTime()));
	glUniform1i(glGetUniformLocation(programSkybox, "skybox"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);
	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void drawRefractiveBubble(glm::mat4 modelMatrix)
{
	glUseProgram(programRefract);

	glm::mat4 view = camera.viewMatrix();
	glm::mat4 projection = createPerspectiveMatrix();
	glm::mat4 transformation = projection * view * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(programRefract, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(programRefract, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3fv(glGetUniformLocation(programRefract, "cameraPos"), 1, (float*)&camera.position);
	glUniform3fv(glGetUniformLocation(programRefract, "fogColor"), 1, (float*)&waterFogColor);
	glUniform1f(glGetUniformLocation(programRefract, "ior"), bubbleIOR);
	glUniform1f(glGetUniformLocation(programRefract, "alphaValue"), bubbleAlpha);
	glUniform1i(glGetUniformLocation(programRefract, "skybox"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	Core::DrawContext(sphereContext);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void drawSceneObject(bool depthOnly, Core::RenderContext& context, glm::mat4 model, const Material& material, glm::vec3 emissiveColor, float emissiveStrength)
{
	if (depthOnly)
		drawDepth(context, model);
	else
		drawPBR(context, model, material, emissiveColor, emissiveStrength);
}

void drawCaveRock(bool depthOnly, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 model = makeModel(position, rotation, scale);
	drawSceneObject(depthOnly, rockContext, model, rockMaterial, glm::vec3(0.0f), 0.0f);
}

void drawSmallCrystal(bool depthOnly, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 glowColor, float glowMultiplier)
{
	glm::mat4 model = makeModel(position, rotation, scale);
	float glow = enableCrystals ? crystalGlow * glowMultiplier : 0.0f;
	drawSceneObject(depthOnly, cubeContext, model, crystalMaterial, glowColor, glow);
}

void drawCrystalCluster(bool depthOnly, glm::vec3 basePosition, float yaw, float clusterScale, glm::vec3 glowColor)
{
	glm::vec3 offsets[] = {
		glm::vec3( 0.00f, 0.00f,  0.00f),
		glm::vec3( 0.16f, 0.02f,  0.04f),
		glm::vec3(-0.14f, 0.03f, -0.05f),
		glm::vec3( 0.07f, 0.08f, -0.16f),
		glm::vec3(-0.06f, 0.05f,  0.15f)
	};
	glm::vec3 rotations[] = {
		glm::vec3(-14.0f, yaw,  8.0f),
		glm::vec3(-26.0f, yaw + 26.0f, -14.0f),
		glm::vec3(-18.0f, yaw - 22.0f, 16.0f),
		glm::vec3(-38.0f, yaw + 8.0f, 28.0f),
		glm::vec3(-12.0f, yaw - 35.0f, -22.0f)
	};
	glm::vec3 scales[] = {
		glm::vec3(0.055f, 0.42f, 0.055f),
		glm::vec3(0.045f, 0.30f, 0.045f),
		glm::vec3(0.050f, 0.34f, 0.050f),
		glm::vec3(0.040f, 0.24f, 0.040f),
		glm::vec3(0.036f, 0.22f, 0.036f)
	};

	for (int i = 0; i < 5; i++)
	{
		drawSmallCrystal(
			depthOnly,
			basePosition + offsets[i] * clusterScale,
			rotations[i],
			scales[i] * clusterScale,
			glowColor,
			0.55f + 0.12f * i
		);
	}
}

void renderCaveRings(bool depthOnly)
{
	float zLayers[] = { -5.05f, -3.35f, -1.55f, 0.25f, 2.15f, 4.55f };
	float angles[] = { 18.0f, 38.0f, 60.0f, 84.0f, 109.0f, 134.0f, 157.0f, 204.0f, 336.0f };

	for (int layer = 0; layer < 6; layer++)
	{
		float z = zLayers[layer];
		float rx = 3.90f + 0.18f * std::sinf(layer * 1.7f);
		float ry = 2.25f + 0.16f * std::cosf(layer * 1.3f);
		float centerY = 0.18f + 0.08f * std::sinf(float(layer));

		for (int i = 0; i < 9; i++)
		{
			float a = glm::radians(angles[i] + 4.0f * std::sinf(layer + i * 0.7f));
			float x = std::cosf(a) * rx;
			float y = centerY + std::sinf(a) * ry;
			y = std::max(y, -0.95f);

			glm::vec3 pos = glm::vec3(x, y, z + 0.18f * std::sinf(i * 1.31f));
			glm::vec3 rot = glm::vec3(13.0f * layer, 21.0f * i, angles[i] - 90.0f);
			glm::vec3 scale = glm::vec3(
				0.72f + 0.11f * ((i + layer) % 3),
				0.54f + 0.08f * (i % 2),
				0.66f + 0.10f * (layer % 2)
			);

			if (i == 3 || i == 4)
				scale *= glm::vec3(1.15f, 0.85f, 1.10f);

			drawCaveRock(depthOnly, pos, rot, scale);
		}
	}

	glm::vec3 extraRocks[] = {
		glm::vec3(-4.25f, -0.80f, -2.15f),
		glm::vec3( 4.10f, -0.82f, -2.65f),
		glm::vec3(-4.35f, -0.82f,  2.25f),
		glm::vec3( 4.00f, -0.80f,  2.75f),
		glm::vec3(-2.20f,  2.70f, -0.20f),
		glm::vec3( 2.10f,  2.62f,  0.75f)
	};

	for (int i = 0; i < 6; i++)
	{
		drawCaveRock(
			depthOnly,
			extraRocks[i],
			glm::vec3(17.0f * i, 31.0f * i, -9.0f * i),
			glm::vec3(0.78f + 0.05f * i, 0.48f, 0.72f)
		);
	}
}

void renderCoreObjects(bool depthOnly)
{
	glm::mat4 floorModel = makeModel(glm::vec3(0.0f, -1.18f, 0.0f), glm::vec3(0.0f), glm::vec3(8.6f, 0.18f, 10.2f));
	glm::mat4 leftSoftWall = makeModel(glm::vec3(-4.85f, 0.60f, 0.0f), glm::vec3(0.0f, 0.0f, -8.0f), glm::vec3(0.25f, 2.4f, 9.4f));
	glm::mat4 rightSoftWall = makeModel(glm::vec3(4.85f, 0.65f, 0.0f), glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.25f, 2.4f, 9.4f));
	glm::mat4 ceilingSoftWall = makeModel(glm::vec3(0.0f, 2.95f, 0.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(7.1f, 0.22f, 9.4f));

	drawSceneObject(depthOnly, cubeContext, floorModel, sandMaterial, glm::vec3(0.0f), 0.0f);
	drawSceneObject(depthOnly, cubeContext, leftSoftWall, rockMaterial, glm::vec3(0.0f), 0.0f);
	drawSceneObject(depthOnly, cubeContext, rightSoftWall, rockMaterial, glm::vec3(0.0f), 0.0f);
	drawSceneObject(depthOnly, cubeContext, ceilingSoftWall, rockMaterial, glm::vec3(0.0f), 0.0f);

	renderCaveRings(depthOnly);

	drawCrystalCluster(depthOnly, glm::vec3(-3.15f, -0.88f, -4.15f), 18.0f, 1.05f, glm::vec3(1.00f, 0.18f, 0.78f));
	drawCrystalCluster(depthOnly, glm::vec3(-2.55f, 1.60f, -4.55f), -18.0f, 0.78f, glm::vec3(1.00f, 0.20f, 0.86f));
	drawCrystalCluster(depthOnly, glm::vec3(2.65f, 1.75f, -4.35f), 24.0f, 0.82f, glm::vec3(0.95f, 0.25f, 1.00f));
	drawCrystalCluster(depthOnly, glm::vec3(3.25f, -0.92f, 2.85f), -35.0f, 0.92f, glm::vec3(0.95f, 0.18f, 0.85f));
	drawCrystalCluster(depthOnly, glm::vec3(-3.45f, 1.12f, 3.80f), 32.0f, 0.74f, glm::vec3(0.70f, 0.95f, 1.00f));
	drawCrystalCluster(depthOnly, glm::vec3(1.10f, -0.96f, -3.25f), -10.0f, 0.66f, glm::vec3(0.55f, 0.90f, 1.00f));
}

void renderSeaweedField(float time)
{
	glm::vec3 positions[] = {
		glm::vec3(-3.9f, -1.05f, -4.2f),
		glm::vec3(-3.3f, -1.05f, -3.4f),
		glm::vec3(-4.1f, -1.05f,  2.6f),
		glm::vec3(-3.4f, -1.05f,  3.6f),
		glm::vec3( 3.4f, -1.05f, -4.0f),
		glm::vec3( 4.0f, -1.05f, -2.8f),
		glm::vec3( 3.6f, -1.05f,  2.9f),
		glm::vec3( 2.9f, -1.05f,  3.8f),
		glm::vec3( 0.8f, -1.05f, -4.5f)
	};

	for (int i = 0; i < 9; i++)
	{
		glm::mat4 model = makeModel(
			positions[i],
			glm::vec3(0.0f, 37.0f * i, 0.0f),
			glm::vec3(0.75f + 0.08f * (i % 3))
		);
		drawSeaweed(model, time + 0.37f * i);
	}
}

void renderTransportCrystalDust(float time)
{
	for (int i = 0; i < int(transportFrames.size()); i += 6)
	{
		const PTFrame& frame = transportFrames[i];
		float flicker = 0.75f + 0.25f * std::sin(time * 2.6f + i * 0.41f);
		glm::vec3 offset = frame.n * (0.08f + 0.05f * std::sin(i)) + frame.b * (0.05f * std::cos(i * 1.7f));
		glm::vec3 color = (i % 2 == 0) ? glm::vec3(1.0f, 0.18f, 0.82f) : glm::vec3(0.48f, 0.95f, 1.0f);
		glm::mat4 marker = glm::translate(glm::mat4(1.0f), frame.p + offset)
			* glm::scale(glm::mat4(1.0f), glm::vec3(0.020f + 0.010f * (i % 3)));
		drawColor(sphereContext, marker, color, enableCrystals ? 0.75f * flicker : 0.0f);
	}
}

void renderDepthPass()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderSceneToHDR(GLFWwindow* window)
{
	glViewport(0, 0, framebufferWidth, framebufferHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	glClearColor(0.05f, 0.01f, 0.10f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSkybox();
	for (unsigned int i = 0; i < caveContexts.size(); i++)
	{
		drawPBR(caveContexts[i], caveModelMatrix(), caveMaterial, glm::vec3(0.0f), 0.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderQuad()
{
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void renderBloomResult()
{
	bool horizontal = true;
	bool firstIteration = true;

	glDisable(GL_DEPTH_TEST);
	glUseProgram(programBlur);
	for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal ? 1 : 0]);
		glUniform1i(glGetUniformLocation(programBlur, "horizontal"), horizontal ? 1 : 0);
		glUniform1i(glGetUniformLocation(programBlur, "image"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, firstIteration ? colorBuffers[1] : pingpongColorbuffers[horizontal ? 0 : 1]);
		renderQuad();
		horizontal = !horizontal;
		if (firstIteration)
			firstIteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programBloomCombine);
	glUniform1i(glGetUniformLocation(programBloomCombine, "scene"), 0);
	glUniform1i(glGetUniformLocation(programBloomCombine, "bloomBlur"), 1);
	glUniform1i(glGetUniformLocation(programBloomCombine, "bloom"), enableBloom ? 1 : 0);
	glUniform1f(glGetUniformLocation(programBloomCombine, "exposure"), bloomExposure);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[horizontal ? 0 : 1]);
	renderQuad();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void renderGui()
{
	ImGui::SetNextWindowSize(ImVec2(260, 190), ImGuiCond_FirstUseEver);
	ImGui::Begin("GRK underwater");
	ImGui::Checkbox("Bloom", &enableBloom);
	ImGui::SliderFloat("Exposure", &bloomExposure, 0.2f, 2.5f);
	ImGui::Checkbox("Bubble A13", &enableBubble);
	ImGui::SliderFloat("Bubble IOR", &bubbleIOR, 0.92f, 1.35f);
	ImGui::SliderFloat("Bubble alpha", &bubbleAlpha, 0.15f, 0.85f);
	ImGui::Text("Mouse rotate, C");
	ImGui::End();
}

bool pressedOnce(GLFWwindow* window, int key, bool& previous)
{
	bool now = glfwGetKey(window, key) == GLFW_PRESS;
	bool result = now && !previous;
	previous = now;
	return result;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (pressedOnce(window, GLFW_KEY_C, keyCWasDown))
	{
		mouseLook = !mouseLook;
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, mouseLook ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		float calibrationSpeed = 1.2f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.position += camera.forward() * calibrationSpeed;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.position -= camera.forward() * calibrationSpeed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.position -= camera.right() * calibrationSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.position += camera.right() * calibrationSpeed;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.position -= camera.up() * calibrationSpeed;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.position += camera.up() * calibrationSpeed;
	}

	if (pressedOnce(window, GLFW_KEY_P, keyPWasDown))
	{
		std::cout << "camera.position = glm::vec3("
			<< camera.position.x << "f, "
			<< camera.position.y << "f, "
			<< camera.position.z << "f);" << std::endl;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!mouseLook || ImGui::GetIO().WantCaptureMouse)
	{
		firstMouse = true;
		return;
	}

	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float dx = float(xpos - lastMouseX);
	float dy = float(ypos - lastMouseY);
	lastMouseX = xpos;
	lastMouseY = ypos;
	camera.rotate(dx, dy);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	framebufferWidth = std::max(width, 1);
	framebufferHeight = std::max(height, 1);
	aspectRatio = framebufferWidth / float(framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);
	createBloomBuffers(framebufferWidth, framebufferHeight);
}

void updateDeltaTime(float time)
{
	if (lastTime < 0.0f)
	{
		lastTime = time;
		deltaTime = 0.016f;
		return;
	}

	deltaTime = time - lastTime;
	if (deltaTime > 0.1f)
		deltaTime = 0.1f;
	lastTime = time;
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	programPBR = shaderLoader.CreateProgram("shaders/pbr.vert", "shaders/pbr.frag");
	programSkybox = shaderLoader.CreateProgram("shaders/skybox.vert", "shaders/skybox.frag");
	programRefract = shaderLoader.CreateProgram("shaders/refract.vert", "shaders/refract.frag");
	programSeaweed = shaderLoader.CreateProgram("shaders/seaweed.vert", "shaders/seaweed.frag");
	programDepth = shaderLoader.CreateProgram("shaders/depth.vert", "shaders/depth.frag");
	programColor = shaderLoader.CreateProgram("shaders/color.vert", "shaders/color.frag");
	programBlur = shaderLoader.CreateProgram("shaders/blur.vert", "shaders/blur.frag");
	programBloomCombine = shaderLoader.CreateProgram("shaders/bloom_final.vert", "shaders/bloom_final.frag");

	loadModelToContexts("../project_assets/cave/source/GRUTA_BASE.OBJ", caveContexts);

	texture::rockAlbedo = createSolidTexture(110, 132, 136);
	texture::rockNormal = createSolidTexture(128, 128, 255);
	texture::rustAlbedo = createSolidTexture(135, 112, 96);
	texture::rustNormal = createSolidTexture(128, 128, 255);
	texture::sandAlbedo = createSolidTexture(92, 112, 114);
	texture::flatNormal = createSolidTexture(128, 128, 255);
	texture::crystalAlbedo = createSolidTexture(80, 220, 255);
	texture::caveAlbedo = Core::LoadTexture("../project_assets/cave/textures/GRUTA_BASE_defaultMat_BaseColor.png");
	texture::caveNormal = Core::LoadTexture("../project_assets/cave/textures/GRUTA_BASE_defaultMat_Normal.png");
	setTextureForModelAsset(texture::caveAlbedo);
	setTextureForModelAsset(texture::caveNormal);

	rockMaterial = makeMaterial(texture::rockAlbedo, texture::rockNormal, glm::vec3(0.55f, 0.70f, 0.76f), 0.0f, 0.82f, 1.0f);
	rustMaterial = makeMaterial(texture::rustAlbedo, texture::rustNormal, glm::vec3(0.82f, 0.78f, 0.72f), 0.65f, 0.58f, 1.0f);
	sandMaterial = makeMaterial(texture::sandAlbedo, texture::flatNormal, glm::vec3(0.45f, 0.60f, 0.62f), 0.0f, 0.9f, 1.0f);
	crystalMaterial = makeMaterial(texture::crystalAlbedo, texture::flatNormal, glm::vec3(0.2f, 0.9f, 1.0f), 0.0f, 0.18f, 1.0f);
	caveMaterial = makeMaterial(texture::caveAlbedo, texture::caveNormal, glm::vec3(0.98f, 0.96f, 1.0f), 0.0f, 0.82f, 1.0f);
	caveMaterial.normalStrength = 0.25f;
	caveMaterial.ambientStrength = 2.6f;
	caveMaterial.fogDensity = 0.026f;
	caveMaterial.fogMax = 0.48f;

	initSkybox();
	initShadowMap();
	initSeaweedMesh();
	initScreenQuad();
	createBloomBuffers(framebufferWidth, framebufferHeight);
	buildTransportFrames();
}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(programPBR);
	shaderLoader.DeleteProgram(programSkybox);
	shaderLoader.DeleteProgram(programRefract);
	shaderLoader.DeleteProgram(programSeaweed);
	shaderLoader.DeleteProgram(programDepth);
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programBlur);
	shaderLoader.DeleteProgram(programBloomCombine);

	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteVertexArrays(1, &seaweedVAO);
	glDeleteBuffers(1, &seaweedVBO);
	glDeleteBuffers(1, &seaweedEBO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

void renderLoop(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		updateDeltaTime(float(glfwGetTime()));
		processInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// UI is hidden during the asset import test, so the cave model fills the view.

		renderDepthPass();
		renderSceneToHDR(window);
		renderBloomResult();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
