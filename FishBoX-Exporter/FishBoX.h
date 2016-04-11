#pragma once
#ifndef FISHBOX_H
#define FISHBOX_H
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

#include "FbxDawg.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>



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
		unsigned int cameraCount;
	};
	struct mesh
	{
		char materialName[256];
		unsigned int vertexCount;
		unsigned int blendshapesCount;
		unsigned int indexCount;
		
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


	mesh * meArray;
	material * maArray;
	directionalLight * dlArray;
	pointLight * plArray;
	areaLight *alArray;

	vertexData ** vArray;
	index ** iArray;

	int fbxFileCount;
	std::string * fbxNames;


public:
	void initialize();

private:
	void listFiles(const char* path);
	void printShit();
	void writeShit();
	void writeErrorCheck();
};






#endif // !FISHBOX_H
