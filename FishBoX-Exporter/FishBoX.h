#pragma once
#ifndef FISHBOX_H
#define FISHBOX_H
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

#include "FbxDawg.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>

//OpenGL Includes
#include <glm\glm.hpp>


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
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
	};
	struct index
	{
		unsigned int vertexIndex;
	};
	struct blendShape
	{
		glm::vec3 pos;
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
	struct material
	{
		char materialName[256];
		char textureFilePath[256];
		char normalFilePath[256];

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float shinyness;
	};

public:
	FishBoX();
	~FishBoX();

private:
	FbxDawg FBX;
	fileHeader HEADER;
	std::string newfilename;

	std::vector<std::string> texVector; //old filepaths
	std::vector<std::string> newTexVector; //new filepaths
	mesh * meArray;
	material * maArray;
	directionalLight * dlArray;
	pointLight * plArray;
	areaLight *alArray;

	vertexData ** vArray;
	index ** iArray;
	blendShape *** bsArray;
	

	int fbxFileCount;
	std::string * fbxNames;



public:
	void initialize();
	void initialize(std::string filepath);
	void test();

private:

	void printShit();
	void writeShit(std::string filepath);
	void writeErrorCheck();
};






#endif // !FISHBOX_H
