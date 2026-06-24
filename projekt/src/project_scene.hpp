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
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/quaternion.hpp"

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Texture.h"
#include "SOIL/SOIL.h"

#include "imgui.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <cmath>
#include <cfloat>
#include <cstdlib>
#include <fstream>
#include <iomanip>
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
std::vector<Core::RenderContext> crystalContexts;
std::vector<Core::RenderContext> crystalOrangeContexts;
std::vector<Core::RenderContext> plantContexts;
std::vector<Core::RenderContext> seaweedGrassContexts;
std::vector<Core::RenderContext> seaweedPinkContexts;

GLuint skyboxVAO = 0;
GLuint skyboxVBO = 0;
GLuint skyboxCubemap = 0;

GLuint groundPlaneVAO = 0;
GLuint groundPlaneVBO = 0;
int groundPlaneVertexCount = 0;

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
GLuint refractionSceneTexture = 0;
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
float bubbleRefractionStrength = 0.085f;
float seaweedStrength = 0.08f;
float seaweedSpeed = 0.65f;
glm::vec2 currentDirection = glm::normalize(glm::vec2(0.7f, 0.35f));

bool keyCWasDown = false;
bool keyPWasDown = false;
bool keyFWasDown = false;
bool keyTabWasDown = false;
bool keyMWasDown = false;
bool keyDeleteWasDown = false;
bool enablePulsatingGlow = false;
bool keyGWasDown = false;

double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool firstMouse = true;

void renderProceduralSeaweed(float time);
void renderSeaweedField(float time);     

struct Material
{
	GLuint albedo = 0;
	GLuint normal = 0;
	GLuint opacity = 0;
	glm::vec3 baseColor = glm::vec3(1.0f);
	float metallic = 0.0f;
	float roughness = 0.5f;
	float ao = 1.0f;
	float textureScale = 1.0f;
	float normalStrength = 0.58f;
	float ambientStrength = 1.0f;
	float fogDensity = 0.075f;
	float fogMax = 0.78f;
	bool flipTextureY = true;
	bool useOpacityTexture = false;
	float alphaCutoff = 0.35f;
};

enum class ObjectType
{
	Crystal,
	CrystalOrange,
	Plant,
	SeaweedGrass,
	SeaweedPink,
	Bubble
};

struct SceneObject
{
	ObjectType type = ObjectType::Crystal;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
};

std::vector<SceneObject> sceneObjects;
int selectedSceneObject = 0;

#include "../generated_scene_layout.hpp"
const char* objectTypeName(ObjectType type)
{
	switch (type)
	{
	case ObjectType::Crystal:
		return "Crystal";
	case ObjectType::CrystalOrange:
		return "Crystal Orange";
	case ObjectType::Plant:
		return "Plant 1";
	case ObjectType::SeaweedGrass:
		return "Seaweed grass";
	case ObjectType::SeaweedPink:
		return "Seaweed pink";
	case ObjectType::Bubble:
		return "Bubble";
	default:
		return "Unknown";
	}
}

const char* objectTypeCodeName(ObjectType type)
{
	switch (type)
	{
	case ObjectType::Crystal:
		return "Crystal";
	case ObjectType::CrystalOrange:
		return "CrystalOrange";
	case ObjectType::Plant:
		return "Plant";
	case ObjectType::SeaweedGrass:
		return "SeaweedGrass";
	case ObjectType::SeaweedPink:
		return "SeaweedPink";
	case ObjectType::Bubble:
		return "Bubble";
	default:
		return "Crystal";
	}
}

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
	GLuint crystalOrangeAlbedo = 0;
	GLuint plantAlbedo = 0;
	GLuint seaweedGrassAlbedo = 0;
	GLuint seaweedGrassOpacity = 0;
	GLuint seaweedPinkAlbedo = 0;
	GLuint caveAlbedo = 0;
	GLuint caveNormal = 0;
}

Material rockMaterial;
Material rustMaterial;
Material sandMaterial;
Material crystalMaterial;
Material crystalOrangeMaterial;
Material plantMaterial;
Material seaweedGrassMaterial;
Material seaweedPinkMaterial;
Material caveMaterial;
Material groundPlaneMaterial;

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

glm::mat4 importedModelMatrix(glm::vec3 position, glm::vec3 importedCenter, glm::vec3 rotationDeg, float scale)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::rotate(model, glm::radians(rotationDeg.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotationDeg.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotationDeg.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale));
	model = glm::translate(model, -importedCenter);
	return model;
}

glm::mat4 crystalModelMatrix()
{
	const glm::vec3 importedCenter = glm::vec3(-1.524054f, 2.864295f, 0.508725f);
	return importedModelMatrix(
		glm::vec3(1.65f, -0.55f, -2.15f),
		importedCenter,
		glm::vec3(0.0f, -18.0f, 0.0f),
		0.14f
	);
}

glm::mat4 plantModelMatrix()
{
	const glm::vec3 importedCenter = glm::vec3(0.014267f, 0.182588f, 0.009282f);
	return importedModelMatrix(
		glm::vec3(-1.45f, -0.62f, -2.05f),
		importedCenter,
		glm::vec3(0.0f, 24.0f, 0.0f),
		0.65f
	);
}

glm::mat4 seaweedGrassModelMatrix()
{
	const glm::vec3 importedCenter = glm::vec3(2.192963f, 48.910385f, 3.925060f);
	return importedModelMatrix(
		glm::vec3(0.20f, -0.75f, -2.85f),
		importedCenter,
		glm::vec3(0.0f, -12.0f, 0.0f),
		0.012f
	);
}

glm::mat4 seaweedPinkModelMatrix()
{
	return importedModelMatrix(
		glm::vec3(2.25f, -0.75f, -1.35f),
		glm::vec3(0.0f),
		glm::vec3(0.0f, -30.0f, 0.0f),
		0.52f
	);
}

glm::mat4 bubbleModelMatrix()
{
	return makeModel(
		glm::vec3(-0.15f, 0.68f, -1.55f),
		glm::vec3(0.0f),
		glm::vec3(0.46f)
	);
}

glm::mat4 importedSceneObjectMatrix(const SceneObject& object, glm::vec3 importedCenter)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), object.position);
	model = glm::rotate(model, glm::radians(object.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(object.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(object.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, object.scale);
	model = glm::translate(model, -importedCenter);
	return model;
}

glm::mat4 sceneObjectModelMatrix(const SceneObject& object)
{
	switch (object.type)
	{
	case ObjectType::Crystal:
		return importedSceneObjectMatrix(object, glm::vec3(-1.524054f, 2.864295f, 0.508725f));
	case ObjectType::CrystalOrange:
		return importedSceneObjectMatrix(object, glm::vec3(-1.524054f, 2.864295f, 0.508725f));
	case ObjectType::Plant:
		return importedSceneObjectMatrix(object, glm::vec3(0.014267f, 0.182588f, 0.009282f));
	case ObjectType::SeaweedGrass:
		return importedSceneObjectMatrix(object, glm::vec3(2.192963f, 48.910385f, 3.925060f));
	case ObjectType::SeaweedPink:
		return importedSceneObjectMatrix(object, glm::vec3(0.0f));
	case ObjectType::Bubble:
		return makeModel(object.position, object.rotation, object.scale);
	default:
		return glm::mat4(1.0f);
	}
}

void initSceneLayout()
{
    // To ładuje wszystkie obiekty z Twojego pliku generated_scene_layout.hpp
    sceneObjects = generatedSceneLayout(); 
    
    // Resetuje zaznaczenie na pierwszy obiekt
    selectedSceneObject = 0;
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

GLuint createTextureFromRGBA(unsigned char* pixels, int width, int height)
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

GLuint loadEmbeddedAlbedoTexture(std::string modelPath, glm::vec3 fallbackColor)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate);
	if (!scene || scene->mNumMaterials == 0 || scene->mNumTextures == 0)
	{
		std::cout << "No embedded texture found in " << modelPath << std::endl;
		return createSolidTexture(
			(unsigned char)(fallbackColor.r * 255.0f),
			(unsigned char)(fallbackColor.g * 255.0f),
			(unsigned char)(fallbackColor.b * 255.0f)
		);
	}

	aiTextureType types[] = { aiTextureType_BASE_COLOR, aiTextureType_DIFFUSE, aiTextureType_EMISSIVE };
	for (unsigned int m = 0; m < scene->mNumMaterials; m++)
	{
		for (unsigned int t = 0; t < 3; t++)
		{
			aiString texturePath;
			if (scene->mMaterials[m]->GetTexture(types[t], 0, &texturePath) != AI_SUCCESS)
				continue;

			std::string ref = texturePath.C_Str();
			if (ref.empty() || ref[0] != '*')
				continue;

			int textureIndex = std::atoi(ref.c_str() + 1);
			if (textureIndex < 0 || textureIndex >= int(scene->mNumTextures))
				continue;

			aiTexture* texture = scene->mTextures[textureIndex];
			if (texture->mHeight == 0)
			{
				int width = 0;
				int height = 0;
				unsigned char* image = SOIL_load_image_from_memory(
					reinterpret_cast<unsigned char*>(texture->pcData),
					texture->mWidth,
					&width,
					&height,
					0,
					SOIL_LOAD_RGBA
				);
				if (image)
				{
					GLuint id = createTextureFromRGBA(image, width, height);
					SOIL_free_image_data(image);
					return id;
				}
			}
			else
			{
				std::vector<unsigned char> pixels(texture->mWidth * texture->mHeight * 4);
				for (unsigned int i = 0; i < texture->mWidth * texture->mHeight; i++)
				{
					pixels[i * 4 + 0] = texture->pcData[i].r;
					pixels[i * 4 + 1] = texture->pcData[i].g;
					pixels[i * 4 + 2] = texture->pcData[i].b;
					pixels[i * 4 + 3] = texture->pcData[i].a;
				}
				return createTextureFromRGBA(pixels.data(), texture->mWidth, texture->mHeight);
			}
		}
	}

	std::cout << "Embedded albedo texture not found in " << modelPath << std::endl;
	return createSolidTexture(
		(unsigned char)(fallbackColor.r * 255.0f),
		(unsigned char)(fallbackColor.g * 255.0f),
		(unsigned char)(fallbackColor.b * 255.0f)
	);
}

bool isLargeCrystalBaseMesh(aiMesh* mesh)
{
	std::string meshName = mesh->mName.C_Str();
	if (meshName.find("Plane") != std::string::npos)
		return true;

	glm::vec3 minPoint(FLT_MAX);
	glm::vec3 maxPoint(-FLT_MAX);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 p(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		minPoint = glm::min(minPoint, p);
		maxPoint = glm::max(maxPoint, p);
	}

	glm::vec3 extent = maxPoint - minPoint;
	return extent.x > 20.0f && extent.z > 20.0f && extent.y < 5.0f;
}

void loadCrystalModelToContexts(std::string path, std::vector<Core::RenderContext>& contexts)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || scene->mNumMeshes == 0)
	{
		std::cout << "ERROR::ASSIMP:: " << path << " " << import.GetErrorString() << std::endl;
		return;
	}

	contexts.clear();
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		if (isLargeCrystalBaseMesh(scene->mMeshes[i]))
			continue;

		Core::RenderContext context;
		context.initFromAssimpMesh(scene->mMeshes[i]);
		contexts.push_back(context);
	}
}

void initSphereContext(Core::RenderContext& context)
{
	const unsigned int rings = 32;
	const unsigned int segments = 64;

	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<float> tangents;
	std::vector<float> bitangents;
	std::vector<unsigned int> indices;

	for (unsigned int y = 0; y <= rings; y++)
	{
		float v = float(y) / float(rings);
		float phi = v * glm::pi<float>();

		for (unsigned int x = 0; x <= segments; x++)
		{
			float u = float(x) / float(segments);
			float theta = u * glm::two_pi<float>();

			float sinPhi = std::sin(phi);
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);
			glm::vec3 normal = glm::normalize(glm::vec3(
				sinPhi * cosTheta,
				std::cos(phi),
				sinPhi * sinTheta
			));
			glm::vec3 tangent = glm::normalize(glm::vec3(-sinTheta, 0.0f, cosTheta));
			glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));

			positions.push_back(normal.x);
			positions.push_back(normal.y);
			positions.push_back(normal.z);
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
			texCoords.push_back(u);
			texCoords.push_back(v);
			tangents.push_back(tangent.x);
			tangents.push_back(tangent.y);
			tangents.push_back(tangent.z);
			bitangents.push_back(bitangent.x);
			bitangents.push_back(bitangent.y);
			bitangents.push_back(bitangent.z);
		}
	}

	for (unsigned int y = 0; y < rings; y++)
	{
		for (unsigned int x = 0; x < segments; x++)
		{
			unsigned int i0 = y * (segments + 1) + x;
			unsigned int i1 = i0 + 1;
			unsigned int i2 = i0 + segments + 1;
			unsigned int i3 = i2 + 1;

			indices.push_back(i0);
			indices.push_back(i2);
			indices.push_back(i1);
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
		}
	}
	unsigned int vertexDataBufferSize = (unsigned int)(sizeof(float) * positions.size());
	unsigned int vertexNormalBufferSize = (unsigned int)(sizeof(float) * normals.size());
	unsigned int vertexTexBufferSize = (unsigned int)(sizeof(float) * texCoords.size());
	unsigned int vertexTangentBufferSize = (unsigned int)(sizeof(float) * tangents.size());
	unsigned int vertexBiTangentBufferSize = (unsigned int)(sizeof(float) * bitangents.size());
	unsigned int vertexElementBufferSize = (unsigned int)(sizeof(unsigned int) * indices.size());

	context.size = int(indices.size());
	glGenVertexArrays(1, &context.vertexArray);
	glBindVertexArray(context.vertexArray);

	glGenBuffers(1, &context.vertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.vertexIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexElementBufferSize, indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &context.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, context.vertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize + vertexBiTangentBufferSize,
		NULL,
		GL_STATIC_DRAW
	);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataBufferSize, positions.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize, vertexNormalBufferSize, normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize, vertexTexBufferSize, texCoords.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize, vertexTangentBufferSize, tangents.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize, vertexBiTangentBufferSize, bitangents.data());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize));
	glBindVertexArray(0);
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

void initGroundPlane()
{
	// Create a 10x10 ground plane centered at origin, Y = 0
	float planeSize = 5.0f; // Will result in 10x10 plane
	std::vector<float> positions = {
		-planeSize, 0.0f, -planeSize,
		 planeSize, 0.0f, -planeSize,
		 planeSize, 0.0f,  planeSize,
		-planeSize, 0.0f, -planeSize,
		 planeSize, 0.0f,  planeSize,
		-planeSize, 0.0f,  planeSize
	};

	std::vector<float> normals = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	std::vector<float> texCoords = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	std::vector<float> tangents = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	std::vector<float> bitangents = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	unsigned int vertexDataBufferSize = (unsigned int)(sizeof(float) * positions.size());
	unsigned int vertexNormalBufferSize = (unsigned int)(sizeof(float) * normals.size());
	unsigned int vertexTexBufferSize = (unsigned int)(sizeof(float) * texCoords.size());
	unsigned int vertexTangentBufferSize = (unsigned int)(sizeof(float) * tangents.size());
	unsigned int vertexBiTangentBufferSize = (unsigned int)(sizeof(float) * bitangents.size());

	groundPlaneVertexCount = (int)positions.size() / 3;

	glGenVertexArrays(1, &groundPlaneVAO);
	glBindVertexArray(groundPlaneVAO);

	glGenBuffers(1, &groundPlaneVBO);
	glBindBuffer(GL_ARRAY_BUFFER, groundPlaneVBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize + vertexBiTangentBufferSize,
		NULL,
		GL_STATIC_DRAW
	);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataBufferSize, positions.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize, vertexNormalBufferSize, normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize, vertexTexBufferSize, texCoords.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize, vertexTangentBufferSize, tangents.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize, vertexBiTangentBufferSize, bitangents.data());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize));
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
		glDeleteTextures(1, &refractionSceneTexture);
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

	glGenTextures(1, &refractionSceneTexture);
	glBindTexture(GL_TEXTURE_2D, refractionSceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
	const int segments = 40;
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
	glUniform1i(glGetUniformLocation(programPBR, "flipTextureY"), mat.flipTextureY ? 1 : 0);
	glUniform1i(glGetUniformLocation(programPBR, "useOpacityTexture"), mat.useOpacityTexture ? 1 : 0);
	glUniform1f(glGetUniformLocation(programPBR, "alphaCutoff"), mat.alphaCutoff);
	glUniform3fv(glGetUniformLocation(programPBR, "emissiveColor"), 1, (float*)&emissiveColor);
	glUniform1f(glGetUniformLocation(programPBR, "emissiveStrength"), emissiveStrength);
	glUniform3fv(glGetUniformLocation(programPBR, "fogColor"), 1, (float*)&waterFogColor);
	glUniform1i(glGetUniformLocation(programPBR, "enableShadows"), enableShadows ? 1 : 0);

	Core::SetActiveTexture(mat.albedo, "colorTexture", programPBR, 0);
	Core::SetActiveTexture(mat.normal, "normalMap", programPBR, 1);
	glUniform1i(glGetUniformLocation(programPBR, "shadowMap"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(glGetUniformLocation(programPBR, "opacityTexture"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mat.opacity);

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

void drawGroundPlane()
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.8f, 0.0f));
	
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
	glUniform3fv(glGetUniformLocation(programPBR, "baseColor"), 1, (float*)&groundPlaneMaterial.baseColor);
	glUniform1f(glGetUniformLocation(programPBR, "metallic"), groundPlaneMaterial.metallic);
	glUniform1f(glGetUniformLocation(programPBR, "roughness"), groundPlaneMaterial.roughness);
	glUniform1f(glGetUniformLocation(programPBR, "ao"), groundPlaneMaterial.ao);
	glUniform1f(glGetUniformLocation(programPBR, "textureScale"), groundPlaneMaterial.textureScale);
	glUniform1f(glGetUniformLocation(programPBR, "normalStrength"), groundPlaneMaterial.normalStrength);
	glUniform1f(glGetUniformLocation(programPBR, "ambientStrength"), groundPlaneMaterial.ambientStrength);
	glUniform1f(glGetUniformLocation(programPBR, "fogDensity"), groundPlaneMaterial.fogDensity);
	glUniform1f(glGetUniformLocation(programPBR, "fogMax"), groundPlaneMaterial.fogMax);
	glUniform1i(glGetUniformLocation(programPBR, "flipTextureY"), groundPlaneMaterial.flipTextureY ? 1 : 0);
	glUniform1i(glGetUniformLocation(programPBR, "useOpacityTexture"), groundPlaneMaterial.useOpacityTexture ? 1 : 0);
	glUniform1f(glGetUniformLocation(programPBR, "alphaCutoff"), groundPlaneMaterial.alphaCutoff);
	glm::vec3 emissiveColor = glm::vec3(0.0f);
	glUniform3fv(glGetUniformLocation(programPBR, "emissiveColor"), 1, (float*)&emissiveColor);
	glUniform1f(glGetUniformLocation(programPBR, "emissiveStrength"), 0.0f);
	glUniform3fv(glGetUniformLocation(programPBR, "fogColor"), 1, (float*)&waterFogColor);
	glUniform1i(glGetUniformLocation(programPBR, "enableShadows"), enableShadows ? 1 : 0);

	Core::SetActiveTexture(groundPlaneMaterial.albedo, "colorTexture", programPBR, 0);
	Core::SetActiveTexture(groundPlaneMaterial.normal, "normalMap", programPBR, 1);
	glUniform1i(glGetUniformLocation(programPBR, "shadowMap"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(glGetUniformLocation(programPBR, "opacityTexture"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, groundPlaneMaterial.opacity);

	glBindVertexArray(groundPlaneVAO);
	glDrawArrays(GL_TRIANGLES, 0, groundPlaneVertexCount);
	glBindVertexArray(0);
}

void drawRefractiveBubble(glm::mat4 modelMatrix)
{
	glUseProgram(programRefract);

	glm::mat4 view = camera.viewMatrix();
	glm::mat4 projection = createPerspectiveMatrix();
	glm::mat4 transformation = projection * view * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(programRefract, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(programRefract, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(programRefract, "viewMatrix"), 1, GL_FALSE, (float*)&view);
	glUniform3fv(glGetUniformLocation(programRefract, "cameraPos"), 1, (float*)&camera.position);
	glUniform3fv(glGetUniformLocation(programRefract, "fogColor"), 1, (float*)&waterFogColor);
	glUniform1f(glGetUniformLocation(programRefract, "ior"), bubbleIOR);
	glUniform1f(glGetUniformLocation(programRefract, "alphaValue"), bubbleAlpha);
	glUniform1f(glGetUniformLocation(programRefract, "refractionStrength"), bubbleRefractionStrength);
	glUniform2f(glGetUniformLocation(programRefract, "viewportSize"), float(framebufferWidth), float(framebufferHeight));
	glUniform1i(glGetUniformLocation(programRefract, "skybox"), 0);
	glUniform1i(glGetUniformLocation(programRefract, "sceneTexture"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionSceneTexture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	Core::DrawContext(sphereContext);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void copySceneForBubbleRefraction()
{
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindTexture(GL_TEXTURE_2D, refractionSceneTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, framebufferWidth, framebufferHeight);
}

void drawEditableSceneObject(const SceneObject& object)
{
	glm::mat4 model = sceneObjectModelMatrix(object);

	switch (object.type)
	{
	case ObjectType::Crystal:
	{
		float pulsingGlow = 0.0f;
		if (enableCrystals) {
			if (enablePulsatingGlow) {
				float time = float(glfwGetTime());
				float phaseOffset = object.position.x * 2.1f + object.position.y * 1.3f + object.position.z * 0.8f;
				pulsingGlow = 2.5f + 1.2f * std::sinf(time * 1.5f + phaseOffset);
			}
			else {
				pulsingGlow = 2.0f;
			}
		}
		for (unsigned int i = 0; i < crystalContexts.size(); i++)
			drawPBR(crystalContexts[i], model, crystalMaterial, glm::vec3(1.0f, 0.16f, 0.82f), pulsingGlow);
		break;
	}
	case ObjectType::CrystalOrange:
		for (unsigned int i = 0; i < crystalOrangeContexts.size(); i++)
			drawPBR(crystalOrangeContexts[i], model, crystalOrangeMaterial, glm::vec3(0.0f), 0.0f);
		break;
	case ObjectType::Plant:
		for (unsigned int i = 0; i < plantContexts.size(); i++)
			drawPBR(plantContexts[i], model, plantMaterial, glm::vec3(0.0f), 0.0f);
		break;
	case ObjectType::SeaweedGrass:
		for (unsigned int i = 0; i < seaweedGrassContexts.size(); i++)
			drawPBR(seaweedGrassContexts[i], model, seaweedGrassMaterial, glm::vec3(0.0f), 0.0f);
		break;
	case ObjectType::SeaweedPink:
		for (unsigned int i = 0; i < seaweedPinkContexts.size(); i++)
			drawPBR(seaweedPinkContexts[i], model, seaweedPinkMaterial, glm::vec3(0.22f, 0.02f, 0.18f), 0.22f);
		break;
	case ObjectType::Bubble:
		if (enableBubble)
			drawRefractiveBubble(model);
		break;
	}
}

float selectedMarkerLift(const SceneObject& object)
{
	switch (object.type)
	{
	case ObjectType::Crystal:
		return 0.82f;
	case ObjectType::Plant:
		return 0.52f;
	case ObjectType::SeaweedGrass:
		return 1.35f;
	case ObjectType::SeaweedPink:
		return 0.82f;
	case ObjectType::Bubble:
		return object.scale.y + 0.20f;
	default:
		return 0.5f;
	}
}

void drawSelectedObjectMarker()
{
	if (sceneObjects.empty())
		return;

	selectedSceneObject = std::max(0, std::min(selectedSceneObject, int(sceneObjects.size()) - 1));
	const SceneObject& object = sceneObjects[selectedSceneObject];
	glm::vec3 markerPos = object.position + glm::vec3(0.0f, selectedMarkerLift(object), 0.0f);
	glm::mat4 markerModel = makeModel(markerPos, glm::vec3(0.0f), glm::vec3(0.055f));
	drawColor(sphereContext, markerModel, glm::vec3(1.0f, 0.92f, 0.18f), 2.2f);
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
    
    for (unsigned int i = 0; i < crystalContexts.size(); i++) {
        drawSceneObject(depthOnly, crystalContexts[i], model, crystalMaterial, glowColor, glow);
    }
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

	// Główne skupiska kryształów
	drawCrystalCluster(depthOnly, glm::vec3(-3.15f, -0.68f, -4.15f), 18.0f, 1.05f, glm::vec3(1.00f, 0.18f, 0.78f));
	drawCrystalCluster(depthOnly, glm::vec3(-2.55f, -0.55f, -4.55f), -18.0f, 0.78f, glm::vec3(1.00f, 0.20f, 0.86f));
	drawCrystalCluster(depthOnly, glm::vec3(2.65f, -0.50f, -4.35f), 24.0f, 0.82f, glm::vec3(0.95f, 0.25f, 1.00f));
	drawCrystalCluster(depthOnly, glm::vec3(3.25f, -0.70f, 2.85f), -35.0f, 0.92f, glm::vec3(0.95f, 0.18f, 0.85f));
	drawCrystalCluster(depthOnly, glm::vec3(-3.45f, -0.60f, 3.80f), 32.0f, 0.74f, glm::vec3(0.70f, 0.95f, 1.00f));
	drawCrystalCluster(depthOnly, glm::vec3(1.10f, -0.72f, -3.25f), -10.0f, 0.66f, glm::vec3(0.55f, 0.90f, 1.00f));
	
	// Mniejsze rozproszone kryształki - grupa lewostronna
	drawCrystalCluster(depthOnly, glm::vec3(-4.20f, -0.65f, -2.00f), 45.0f, 0.42f, glm::vec3(0.88f, 0.22f, 0.92f));
	drawCrystalCluster(depthOnly, glm::vec3(-3.80f, -0.68f, -0.50f), -22.0f, 0.38f, glm::vec3(0.85f, 0.30f, 0.95f));
	drawCrystalCluster(depthOnly, glm::vec3(-4.65f, -0.70f, 1.20f), 55.0f, 0.35f, glm::vec3(0.92f, 0.25f, 0.88f));
	drawCrystalCluster(depthOnly, glm::vec3(-3.25f, -0.62f, 1.90f), -40.0f, 0.40f, glm::vec3(0.78f, 0.92f, 1.00f));
	
	// Mniejsze rozproszone kryształki - grupa prawostronna
	drawCrystalCluster(depthOnly, glm::vec3(4.10f, -0.68f, -3.20f), -25.0f, 0.40f, glm::vec3(1.00f, 0.28f, 0.75f));
	drawCrystalCluster(depthOnly, glm::vec3(3.70f, -0.65f, -1.50f), 30.0f, 0.38f, glm::vec3(0.92f, 0.32f, 0.85f));
	drawCrystalCluster(depthOnly, glm::vec3(4.50f, -0.70f, 0.80f), -50.0f, 0.36f, glm::vec3(0.88f, 0.28f, 0.90f));
	drawCrystalCluster(depthOnly, glm::vec3(3.90f, -0.63f, 2.50f), 20.0f, 0.42f, glm::vec3(0.65f, 0.88f, 1.00f));
	
	// Bardzo małe dekoracyjne kryształki - środek
	drawCrystalCluster(depthOnly, glm::vec3(0.50f, -0.72f, -2.80f), 15.0f, 0.32f, glm::vec3(0.95f, 0.22f, 0.82f));
	drawCrystalCluster(depthOnly, glm::vec3(-0.80f, -0.68f, 0.50f), -38.0f, 0.30f, glm::vec3(0.75f, 0.92f, 1.00f));
	drawCrystalCluster(depthOnly, glm::vec3(1.80f, -0.70f, 1.20f), 42.0f, 0.31f, glm::vec3(0.90f, 0.25f, 0.88f));
	drawCrystalCluster(depthOnly, glm::vec3(-0.30f, -0.65f, -0.80f), -12.0f, 0.29f, glm::vec3(0.82f, 0.88f, 0.98f));
}

void renderSeaweedField(float time)
{
	// Główne skupiska wodorostów
	glm::vec3 positions[] = {
		// Lewy tylny róg
		glm::vec3(-3.9f, -1.15f, -4.2f),
		glm::vec3(-3.3f, -1.15f, -3.4f),
		glm::vec3(-4.5f, -1.15f, -3.8f),
		glm::vec3(-3.7f, -1.15f, -4.6f),
		glm::vec3(-4.1f, -1.15f, -3.1f),
		// Lewy przedni róg
		glm::vec3(-4.1f, -1.15f,  2.6f),
		glm::vec3(-3.4f, -1.15f,  3.6f),
		glm::vec3(-4.7f, -1.15f,  3.1f),
		glm::vec3(-3.8f, -1.15f,  2.0f),
		glm::vec3(-4.3f, -1.15f,  4.0f),
		// Prawy tylny róg
		glm::vec3( 3.4f, -1.15f, -4.0f),
		glm::vec3( 4.0f, -1.15f, -2.8f),
		glm::vec3( 4.6f, -1.15f, -3.5f),
		glm::vec3( 3.8f, -1.15f, -4.5f),
		glm::vec3( 4.3f, -1.15f, -3.0f),
		// Prawy przedni róg
		glm::vec3( 3.6f, -1.15f,  2.9f),
		glm::vec3( 2.9f, -1.15f,  3.8f),
		glm::vec3( 4.3f, -1.15f,  3.4f),
		glm::vec3( 3.2f, -1.15f,  2.2f),
		glm::vec3( 4.1f, -1.15f,  4.2f),
		
		glm::vec3( 4.8f, -1.15f, -1.5f),
		glm::vec3( 4.5f, -1.15f, -1.0f),
		glm::vec3( 4.1f, -1.15f,  0.2f),
		glm::vec3( 4.6f, -1.15f,  0.8f),
		glm::vec3( 4.5f, -1.15f, -0.5f)
	};

	for (int i = 0; i < 25; i++)
	{
		float thickness = 0.5f + 0.15f * (i % 3);
		float height = 0.6f + 0.25f * (i % 4);

		glm::mat4 model = makeModel(
			positions[i],
			glm::vec3(0.0f, 37.0f * i + 15.0f * std::sinf(i * 0.5f), 0.0f),
			glm::vec3(thickness, height, thickness)
		);
		drawSeaweed(model, time + 0.37f * i + 0.15f * std::cosf(i * 0.8f));
		
	}
	renderProceduralSeaweed(time);
	
}

void renderProceduralSeaweed(float time) {

	srand(4321);

    glm::vec3 clusters[] = {
        glm::vec3(-3.5f, -1.15f, -2.0f), // Lewy bok
        glm::vec3( 3.5f, -1.15f,  2.0f), // Prawy bok
        glm::vec3(-3.6f, -1.15f,  1.5f), // Lewy bok
        glm::vec3( 3.6f, -1.15f, -1.5f), // Prawy bok
        glm::vec3( 0.0f, -1.15f, -2.5f), // Środek tył
        glm::vec3( 0.0f, -1.15f,  1.5f)  // Środek przód
    };

	for (int c = 0; c < 4; c++) {
        for (int i = 0; i < 5; i++) {
            float x = clusters[c].x + (float)(rand() % 40 - 20) / 50.0f;
            float z = clusters[c].z + (float)(rand() % 40 - 20) / 50.0f;
            
			float height = 0.60f + (float)(rand() % 50) / 100.0f;
            float thickness = 0.40f + (float)(rand() % 30) / 100.0f;
			float randomYaw = (float)(rand() % 360);

            glm::mat4 model1 = makeModel(glm::vec3(x, clusters[c].y, z), glm::vec3(0.0f, randomYaw, 0.0f), glm::vec3(thickness, height, thickness));
			drawSeaweed(model1, time + (float)i * 0.3f);

			glm::mat4 model2 = makeModel(glm::vec3(x, clusters[c].y, z), glm::vec3(0.0f, randomYaw + 90.0f, 0.0f), glm::vec3(thickness, height, thickness));
			drawSeaweed(model2, time + (float)i * 0.3f);
    
        }
    }  
}

void renderTransportCrystalDust(float time)
{
    srand(12345); 

    glm::vec3 clusterCenters[] = {
        glm::vec3(-4.0f, 0.5f, -2.0f), glm::vec3(4.0f, 0.5f, -2.0f), 
        glm::vec3(-3.8f, 1.8f, 0.5f),  glm::vec3(3.8f, 1.8f, 0.5f),  
        glm::vec3(-4.2f, 0.2f, 1.5f),  glm::vec3(4.2f, 0.2f, 1.5f),  
        glm::vec3(-4.0f, 2.5f, -3.0f), glm::vec3(4.0f, 2.5f, -3.0f), 
        glm::vec3(-3.5f, 1.0f,  3.0f), glm::vec3(3.5f, 1.0f,  3.0f), 
        glm::vec3(0.0f, 2.5f, -1.0f),
		glm::vec3(-2.0f, 1.2f, 4.2f), 
        glm::vec3(0.0f, 1.6f, 4.5f), 
        glm::vec3(2.0f, 1.2f, 4.2f)
    };

    for (int c = 0; c < 14; c++) 
    {
        for (int i = 0; i < 800; i++) 
        {
            float ox = (float)(rand() % 500 - 250) / 100.0f;
            float oy = (float)(rand() % 500 - 250) / 100.0f;
            float oz = (float)(rand() % 500 - 250) / 100.0f;
            
            glm::vec3 pos = clusterCenters[c] + glm::vec3(ox, oy, oz);

            bool naDnie = (pos.y < -0.4f);
            bool naWyjsciu = (pos.z > 2.8f);
            bool srodekJaskini = (pos.x > -2.0f && pos.x < 2.0f && pos.y < 2.0f);
            bool nadRoslinami = (pos.z > 2.0f && pos.y > 0.2f);
            
			bool pozaJaskinia = (pos.x < -4.6f || pos.x > 4.6f || pos.y > 2.8f || pos.z < -4.8f || pos.z > 2.8f);

            if (naDnie || naWyjsciu || (srodekJaskini && !nadRoslinami)) {
                continue; 
            }

            glm::mat4 marker = glm::translate(glm::mat4(1.0f), pos)
                             * glm::scale(glm::mat4(1.0f), glm::vec3(0.008f));

			glm::vec3 particleColor;
            int colorType = rand() % 3; 
            
            if (colorType == 0) {
                particleColor = glm::vec3(0.9f, 0.2f, 0.8f); 
            } else if (colorType == 1) {
                particleColor = glm::vec3(0.4f, 0.0f, 0.4f); 
            } else {
                particleColor = glm::vec3(0.79f, 0.02f, 0.53f);
            }

            // krysztal z wylosowanym kolorem
            drawColor(sphereContext, marker, particleColor, 2.0f);
            
        }
	}
}

void renderDepthPass()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// nowa funkcja generujaca krysztaly
void renderWallCrystals(bool depthOnly)
{
    srand(999); 

    for (int i = 0; i < 100; i++) 
    {
        float t = (float)i;
        float z = -8.0f + 16.0f * std::abs(std::sinf(t * 11.23f)); 
        
        bool isLeft = std::cosf(t * 22.34f) > 0.0f;
        float xOffset = std::sinf(t * 33.45f) * 0.8f;
        float x = isLeft ? (-4.2f + xOffset) : (4.2f - xOffset);
        
        float y = -0.8f + std::abs(std::cosf(t * 44.56f)) * 4.5f;
        float yaw = std::sinf(t * 55.67f) * 180.0f;
        float scale = 0.2f + std::abs(std::sinf(t * 66.78f)) * 0.4f;
       
        glm::vec3 randomColor = glm::vec3(
            0.5f + 0.5f * std::sinf(t * 0.1f), 
            0.5f + 0.5f * std::sinf(t * 0.2f + 2.0f), 
            0.5f + 0.5f * std::sinf(t * 0.3f + 4.0f)
        );
        
        drawCrystalCluster(depthOnly, glm::vec3(x, y, z), yaw, scale, randomColor);
    }
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
	
	for (unsigned int i = 0; i < sceneObjects.size(); i++) {
        if (sceneObjects[i].type != ObjectType::Bubble) {
		    drawEditableSceneObject(sceneObjects[i]);
        }
    }

	drawSelectedObjectMarker();

	// renderSeaweedField(float(glfwGetTime()));
	renderTransportCrystalDust(float(glfwGetTime()));

	copySceneForBubbleRefraction();

	// obiekty przezroczyste
	for (unsigned int i = 0; i < sceneObjects.size(); i++) {
        if (sceneObjects[i].type == ObjectType::Bubble) {
		    drawEditableSceneObject(sceneObjects[i]);
        }
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

void writeVec3Code(std::ofstream& file, const glm::vec3& value)
{
	file << "glm::vec3(" << value.x << "f, " << value.y << "f, " << value.z << "f)";
}

bool writeSceneLayoutFile(const std::string& path)
{
	std::ofstream file(path.c_str());
	if (!file.is_open())
		return false;

	file << "#pragma once\n\n";
	file << "// Generated with the temporary scene layout editor. Include this after SceneObject/ObjectType declarations.\n";
	file << "static std::vector<SceneObject> generatedSceneLayout()\n";
	file << "{\n";
	file << "\treturn {\n";
	file << std::fixed << std::setprecision(4);
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		const SceneObject& object = sceneObjects[i];
		file << "\t\t{ ObjectType::" << objectTypeCodeName(object.type) << ", ";
		writeVec3Code(file, object.position);
		file << ", ";
		writeVec3Code(file, object.rotation);
		file << ", ";
		writeVec3Code(file, object.scale);
		file << " }";
		if (i + 1 < sceneObjects.size())
			file << ",";
		file << "\n";
	}
	file << "\t};\n";
	file << "}\n";
	return true;
}

void saveSceneLayout()
{
	const std::string path = "generated_scene_layout.hpp";
	if (writeSceneLayoutFile(path))
	{
		std::cout << "Scene layout saved to " << path << std::endl;
		return;
	}

	std::cout << "Could not save scene layout to " << path << std::endl;
}

void selectNextSceneObject()
{
	if (sceneObjects.empty())
		return;

	selectedSceneObject = (selectedSceneObject + 1) % int(sceneObjects.size());
	std::cout << "Selected object " << selectedSceneObject + 1 << "/" << sceneObjects.size()
		<< ": " << objectTypeName(sceneObjects[selectedSceneObject].type) << std::endl;
}

void duplicateSelectedSceneObject()
{
	if (sceneObjects.empty())
		return;

	selectedSceneObject = std::max(0, std::min(selectedSceneObject, int(sceneObjects.size()) - 1));
	SceneObject copy = sceneObjects[selectedSceneObject];
	copy.position += glm::vec3(0.28f, 0.0f, 0.18f);
	sceneObjects.push_back(copy);
	selectedSceneObject = int(sceneObjects.size()) - 1;
	std::cout << "Duplicated " << objectTypeName(copy.type) << ", objects: " << sceneObjects.size() << std::endl;
}

void deleteSelectedSceneObject()
{
	if (sceneObjects.size() <= 1)
		return;

	selectedSceneObject = std::max(0, std::min(selectedSceneObject, int(sceneObjects.size()) - 1));
	std::cout << "Deleted " << objectTypeName(sceneObjects[selectedSceneObject].type) << std::endl;
	sceneObjects.erase(sceneObjects.begin() + selectedSceneObject);
	selectedSceneObject = std::min(selectedSceneObject, int(sceneObjects.size()) - 1);
}

void editSelectedSceneObject(GLFWwindow* window)
{
	if (sceneObjects.empty() || ImGui::GetIO().WantCaptureKeyboard)
		return;

	selectedSceneObject = std::max(0, std::min(selectedSceneObject, int(sceneObjects.size()) - 1));
	SceneObject& object = sceneObjects[selectedSceneObject];

	bool ctrlDown = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
	bool shiftDown = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
	bool altDown = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;

	float moveSpeed = (altDown ? 0.22f : 0.82f) * deltaTime;
	float rotateSpeed = (altDown ? 24.0f : 72.0f) * deltaTime;
	float scaleSpeed = (altDown ? 0.35f : 0.85f) * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		object.position.x -= moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		object.position.x += moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (shiftDown)
			object.position.y += moveSpeed;
		else
			object.position.z -= moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (shiftDown)
			object.position.y -= moveSpeed;
		else
			object.position.z += moveSpeed;
	}

	float rotationDirection = shiftDown ? -1.0f : 1.0f;
	if (!ctrlDown && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		object.rotation.x += rotateSpeed * rotationDirection;
	if (!ctrlDown && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		object.rotation.y += rotateSpeed * rotationDirection;
	if (!ctrlDown && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		object.rotation.z += rotateSpeed * rotationDirection;

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		object.scale = glm::max(object.scale * (1.0f - scaleSpeed), glm::vec3(0.001f));
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		object.scale *= (1.0f + scaleSpeed);
}

void renderGui()
{
	ImGui::SetNextWindowSize(ImVec2(330, 260), ImGuiCond_FirstUseEver);
	ImGui::Begin("GRK underwater");
	ImGui::Checkbox("Bloom", &enableBloom);
	ImGui::SliderFloat("Exposure", &bloomExposure, 0.2f, 2.5f);
	ImGui::Checkbox("Pulsing Glow (G)", &enablePulsatingGlow);
	ImGui::Checkbox("Bubble A13", &enableBubble);
	ImGui::SliderFloat("Bubble IOR", &bubbleIOR, 0.92f, 1.35f);
	ImGui::SliderFloat("Bubble alpha", &bubbleAlpha, 0.15f, 0.85f);
	ImGui::SliderFloat("Bubble refract", &bubbleRefractionStrength, 0.0f, 0.22f);
	ImGui::Separator();
	if (!sceneObjects.empty())
	{
		selectedSceneObject = std::max(0, std::min(selectedSceneObject, int(sceneObjects.size()) - 1));
		const SceneObject& object = sceneObjects[selectedSceneObject];
		ImGui::Text("Selected %d/%d: %s", selectedSceneObject + 1, int(sceneObjects.size()), objectTypeName(object.type));
		ImGui::Text("Pos: %.2f %.2f %.2f", object.position.x, object.position.y, object.position.z);
		ImGui::Text("Rot: %.1f %.1f %.1f", object.rotation.x, object.rotation.y, object.rotation.z);
		ImGui::Text("Scale: %.3f %.3f %.3f", object.scale.x, object.scale.y, object.scale.z);
	}
	ImGui::Text("F/TAB object, arrows move, Shift+Up/Down Y");
	ImGui::Text("Q/E/R rotate XYZ, Shift reverses, Z/X scale");
	ImGui::Text("P copy, Del remove, M save");
	ImGui::Text("C mouse, Ctrl+WASD camera");
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

	bool ctrlDown = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

	if (pressedOnce(window, GLFW_KEY_C, keyCWasDown))
	{
		mouseLook = !mouseLook;
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, mouseLook ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	if (ctrlDown)
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

	if (pressedOnce(window, GLFW_KEY_F, keyFWasDown) || pressedOnce(window, GLFW_KEY_TAB, keyTabWasDown))
		selectNextSceneObject();

	if (pressedOnce(window, GLFW_KEY_M, keyMWasDown))
		saveSceneLayout();

	if (pressedOnce(window, GLFW_KEY_DELETE, keyDeleteWasDown))
		deleteSelectedSceneObject();
	if (pressedOnce(window, GLFW_KEY_G, keyGWasDown))
		enablePulsatingGlow = !enablePulsatingGlow;

	if (pressedOnce(window, GLFW_KEY_G, keyGWasDown))
		enablePulsatingGlow = !enablePulsatingGlow;

	if (pressedOnce(window, GLFW_KEY_P, keyPWasDown))
	{
		if (ctrlDown)
		{
			std::cout << "camera.position = glm::vec3("
				<< camera.position.x << "f, "
				<< camera.position.y << "f, "
				<< camera.position.z << "f);" << std::endl;
		}
		else
		{
			duplicateSelectedSceneObject();
		}
	}

	editSelectedSceneObject(window);
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
	// Initialize seed for procedural generation (deterministic but looks random)
	srand(42);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	programPBR = shaderLoader.CreateProgram("../projekt/shaders/pbr.vert", "../projekt/shaders/pbr.frag");
	programSkybox = shaderLoader.CreateProgram("../projekt/shaders/skybox.vert", "../projekt/shaders/skybox.frag");
	programRefract = shaderLoader.CreateProgram("../projekt/shaders/refract.vert", "../projekt/shaders/refract.frag");
	programSeaweed = shaderLoader.CreateProgram("../projekt/shaders/seaweed.vert", "../projekt/shaders/seaweed.frag");
	programDepth = shaderLoader.CreateProgram("../projekt/shaders/depth.vert", "../projekt/shaders/depth.frag");
	programColor = shaderLoader.CreateProgram("../projekt/shaders/color.vert", "../projekt/shaders/color.frag");
	programBlur = shaderLoader.CreateProgram("../projekt/shaders/blur.vert", "../projekt/shaders/blur.frag");
	programBloomCombine = shaderLoader.CreateProgram("../projekt/shaders/bloom_final.vert", "../projekt/shaders/bloom_final.frag");

	loadModelToContexts("../project_assets/cave/source/GRUTA_BASE.OBJ", caveContexts);
	loadCrystalModelToContexts("../project_assets/cristal/cristal.obj", crystalContexts);
	loadCrystalModelToContexts("../project_assets/cristal/crystal_orange.obj", crystalOrangeContexts);
	loadModelToContexts("../project_assets/plant_1/source/anemone on reef L.glb", plantContexts);
	loadModelToContexts("../project_assets/seaweed_grass/source/seaweed02.fbx", seaweedGrassContexts);
	loadModelToContexts("../project_assets/seaweed_pink/seaweed_asset.glb", seaweedPinkContexts);

	texture::rockAlbedo = createSolidTexture(110, 132, 136);
	texture::rockNormal = createSolidTexture(128, 128, 255);
	texture::rustAlbedo = createSolidTexture(135, 112, 96);
	texture::rustNormal = createSolidTexture(128, 128, 255);
	texture::sandAlbedo = createSolidTexture(92, 112, 114);
	texture::flatNormal = createSolidTexture(128, 128, 255);
	texture::crystalAlbedo = Core::LoadTexture("../project_assets/cristal/textures/textures/Cristal Base Color.001");
	texture::crystalOrangeAlbedo = Core::LoadTexture("../project_assets/cristal/crystal+orange/Water_Pool_Light.jpg");
	texture::plantAlbedo = Core::LoadTexture("../project_assets/plant_1/textures/gltf_embedded_0.jpeg");
	texture::seaweedGrassAlbedo = Core::LoadTexture("../project_assets/seaweed_grass/textures/seaweed_colour.png");
	texture::seaweedGrassOpacity = Core::LoadTexture("../project_assets/seaweed_grass/textures/seaweed_opacity.png");
	texture::seaweedPinkAlbedo = loadEmbeddedAlbedoTexture("../project_assets/seaweed_pink/seaweed_asset.glb", glm::vec3(0.9f, 0.24f, 0.72f));
	texture::caveAlbedo = Core::LoadTexture("../project_assets/cave/textures/GRUTA_BASE_defaultMat_BaseColor.png");
	texture::caveNormal = Core::LoadTexture("../project_assets/cave/textures/GRUTA_BASE_defaultMat_Normal.png");
	setTextureForModelAsset(texture::crystalAlbedo);
	setTextureForModelAsset(texture::crystalOrangeAlbedo);
	setTextureForModelAsset(texture::plantAlbedo);
	setTextureForModelAsset(texture::seaweedGrassAlbedo);
	setTextureForModelAsset(texture::seaweedGrassOpacity);
	setTextureForModelAsset(texture::seaweedPinkAlbedo);
	setTextureForModelAsset(texture::caveAlbedo);
	setTextureForModelAsset(texture::caveNormal);

	rockMaterial = makeMaterial(texture::rockAlbedo, texture::rockNormal, glm::vec3(0.55f, 0.70f, 0.76f), 0.0f, 0.82f, 1.0f);
	rustMaterial = makeMaterial(texture::rustAlbedo, texture::rustNormal, glm::vec3(0.82f, 0.78f, 0.72f), 0.65f, 0.58f, 1.0f);
	sandMaterial = makeMaterial(texture::sandAlbedo, texture::flatNormal, glm::vec3(0.45f, 0.60f, 0.62f), 0.0f, 0.9f, 1.0f);
	crystalMaterial = makeMaterial(texture::crystalAlbedo, texture::flatNormal, glm::vec3(1.0f, 0.82f, 1.0f), 0.0f, 0.24f, 1.0f);
	crystalMaterial.normalStrength = 0.0f;
	crystalMaterial.ambientStrength = 2.3f;
	crystalMaterial.fogDensity = 0.018f;
	crystalMaterial.fogMax = 0.38f;
	crystalOrangeMaterial = makeMaterial(texture::crystalOrangeAlbedo, texture::flatNormal, glm::vec3(0.839216f, 0.313725f, 0.874510f), 0.0f, 0.35f, 1.0f);
	crystalOrangeMaterial.normalStrength = 0.0f;
	crystalOrangeMaterial.ambientStrength = 1.5f;
	crystalOrangeMaterial.fogDensity = 0.025f;
	crystalOrangeMaterial.fogMax = 0.45f;
	plantMaterial = makeMaterial(texture::plantAlbedo, texture::flatNormal, glm::vec3(1.0f), 0.0f, 0.72f, 1.0f);
	plantMaterial.normalStrength = 0.0f;
	plantMaterial.ambientStrength = 2.15f;
	plantMaterial.fogDensity = 0.020f;
	plantMaterial.fogMax = 0.42f;
	plantMaterial.flipTextureY = false;
	seaweedGrassMaterial = makeMaterial(texture::seaweedGrassAlbedo, texture::flatNormal, glm::vec3(0.82f, 1.0f, 0.92f), 0.0f, 0.78f, 1.0f);
	seaweedGrassMaterial.opacity = texture::seaweedGrassOpacity;
	seaweedGrassMaterial.useOpacityTexture = true;
	seaweedGrassMaterial.alphaCutoff = 0.42f;
	seaweedGrassMaterial.normalStrength = 0.0f;
	seaweedGrassMaterial.ambientStrength = 2.1f;
	seaweedGrassMaterial.fogDensity = 0.020f;
	seaweedGrassMaterial.fogMax = 0.42f;
	seaweedGrassMaterial.flipTextureY = false;
	seaweedPinkMaterial = makeMaterial(texture::seaweedPinkAlbedo, texture::flatNormal, glm::vec3(1.0f, 0.78f, 0.96f), 0.0f, 0.66f, 1.0f);
	seaweedPinkMaterial.normalStrength = 0.0f;
	seaweedPinkMaterial.ambientStrength = 2.25f;
	seaweedPinkMaterial.fogDensity = 0.020f;
	seaweedPinkMaterial.fogMax = 0.42f;
	seaweedPinkMaterial.flipTextureY = false;
	caveMaterial = makeMaterial(texture::caveAlbedo, texture::caveNormal, glm::vec3(0.98f, 0.96f, 1.0f), 0.0f, 0.82f, 1.0f);
	caveMaterial.normalStrength = 0.25f;
	caveMaterial.ambientStrength = 2.6f;
	caveMaterial.fogDensity = 0.026f;
	caveMaterial.fogMax = 0.48f;

	groundPlaneMaterial = makeMaterial(texture::sandAlbedo, texture::flatNormal, glm::vec3(0.40f, 0.55f, 0.58f), 0.0f, 0.80f, 0.6f);
	groundPlaneMaterial.normalStrength = 0.15f;
	groundPlaneMaterial.ambientStrength = 1.8f;
	groundPlaneMaterial.fogDensity = 0.030f;
	groundPlaneMaterial.fogMax = 0.50f;

	initSceneLayout();
	initSkybox();
	initGroundPlane();
	initSphereContext(sphereContext);
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
	glDeleteVertexArrays(1, &groundPlaneVAO);
	glDeleteBuffers(1, &groundPlaneVBO);
	glDeleteVertexArrays(1, &sphereContext.vertexArray);
	glDeleteBuffers(1, &sphereContext.vertexBuffer);
	glDeleteBuffers(1, &sphereContext.vertexIndexBuffer);
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
		renderGui();

		renderDepthPass();
		renderSceneToHDR(window);
		renderBloomResult();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
