#pragma once
#ifndef FISHBOX_H
#define FISHBOX_H

#include "FbxDawg.h"

class FishBoX
{
private:
	struct fileHeader
	{
		unsigned int meshCount;
		unsigned int materialCount;
		unsigned int directionalLightCount;
		unsigned int pointLightCount;
		unsigned int areaLightCount;
	};
	struct mesh
	{
		char materialName[256];
		unsigned int vertexCount;
		unsigned int blendshapesCount;
		unsigned int indexCount;
		unsigned int cameraCount;
	};
	struct vertexData
	{
		float pos[3];
		float normal[3];
		float uv[2];
	};
	struct index
	{
		unsigned int vertexIndex;
	};
	struct blendShape
	{
		float pos[3];
	};
	struct material
	{
		char materialName[256];
		char textureFilePath[256];
		char normalFilePath[256];

		float diffuse[3];
		float spec[3];
		float shinyness;
	};
	struct directionalLight
	{
		float intensity;
		float lightColor;
		float direction[3];
	};
	struct pointLight
	{
		float intensity;
		float lightColor;
		float pos[3];
	};
	struct areaLight
	{
		float intensity;
		float lightColor;
		float pos[3];
		float direction[3];
		float scale[3];
	};
	struct camera
	{
		float pos[3];
		float rotation[3];
		float target[3];
		float upVec[3];
	};

public:
	FishBoX();
	~FishBoX();

private:
	FbxDawg FBX;
	fileHeader HEADER;
	mesh * meshes;
	vertexData * vertices;
	index * indices;


public:
	void initialize();


};






#endif // !FISHBOX_H
