
#include "FishBoX.h"

FishBoX::FishBoX()
{

}

FishBoX::~FishBoX()
{

}

void FishBoX::initialize()
{
	FBX.loadModels("./TestBox.fbx");

	HEADER.meshCount = FBX.GetMeshVec().size();
	HEADER.materialCount = 0;
	HEADER.directionalLightCount = 0;
	HEADER.pointLightCount = 0;
	HEADER.areaLightCount = 0;


	printf("\nHEADER\n");
	printf("Meshes: %d\n", HEADER.meshCount);
	printf("Materials %d\n", HEADER.materialCount);
	printf("Directional Lights: %d\n", HEADER.directionalLightCount);
	printf("Point Lights: %d\n", HEADER.pointLightCount);
	printf("Area Lights: %d\n", HEADER.areaLightCount);

	for (int i = 0; i < HEADER.meshCount; i++)
	{
		printf("\n\nMesh: %d", (i + 1));
		printf("\n\nVertices: %d", FBX.GetMeshVec()[i].vertexData.size());
		printf("\nIndices: %d", FBX.GetMeshVec()[i].index.size());

		for (int j = 0; j < FBX.GetMeshVec()[i].index.size(); j++)
		{
			printf("\n\nVertex Index: %d\n", FBX.GetMeshVec()[i].index[j]);
			printf("Position\n X: %d, Y: %d, Z: %d\n", FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].x,
													   FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].y,
													   FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].z);
			printf("Normal\n X: %d, Y: %d, Z: %d\n",   FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].norX,
													   FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].norY,
													   FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].norZ);
			printf("UVs\n U: %d, V: %d\n",             FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].u,
													   FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].v);



		}
	}

}
