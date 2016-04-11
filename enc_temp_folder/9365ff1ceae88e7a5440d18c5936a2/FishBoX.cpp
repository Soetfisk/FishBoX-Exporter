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

	//listFiles("./");

	HEADER.meshCount = FBX.GetMeshVec().size();
	HEADER.materialCount = 0;
	HEADER.directionalLightCount = 0;
	HEADER.pointLightCount = 0;
	HEADER.areaLightCount = 0;
	HEADER.cameraCount = 0;
	
	
	printShit();
	writeShit();
}

void FishBoX::printShit()
{

	printf("\nHEADER\n");
	printf("Meshes: %d\n", HEADER.meshCount);
	printf("Materials %d\n", HEADER.materialCount);
	printf("Directional Lights: %d\n", HEADER.directionalLightCount);
	printf("Point Lights: %d\n", HEADER.pointLightCount);
	printf("Area Lights: %d\n", HEADER.areaLightCount);
	printf("Cameras: %d\n", HEADER.cameraCount);

	meArray = new mesh[HEADER.meshCount];
	maArray = new material[HEADER.materialCount];
	directionalLight * dlArray = new directionalLight[HEADER.directionalLightCount];
	pointLight * plArray = new pointLight[HEADER.pointLightCount];
	areaLight *alArray = new areaLight[HEADER.areaLightCount];
	
	
	vArray = new vertexData*[HEADER.meshCount];
	iArray = new index*[HEADER.meshCount];


	for (int i = 0; i < HEADER.meshCount; i++)
	{
		//print no of meshes
		printf("\n\nMesh: %d", (i + 1));

		//set material name then print it
		meArray[i].materialName[0] = 't'; meArray[i].materialName[1] = 'e'; meArray[i].materialName[2] = 'm'; meArray[i].materialName[3] = 'p'; //TEMP
		printf("\nMaterialName: %c%c%c%c", meArray[i].materialName[0], meArray[i].materialName[1], meArray[i].materialName[2], meArray[i].materialName[3]); //TEMP
		
		//set vertexcount then print it
		meArray[i].vertexCount = FBX.GetMeshVec()[i].vertexData.size();
		printf("\n\nVertices: %d", FBX.GetMeshVec()[i].vertexData.size());

		//set blendshapeCount then print it
		meArray[i].blendshapesCount = 0; //TEMP
		printf("\nBlendShapes: %d", meArray[i].blendshapesCount);

		//set indexCount then print it
		meArray[i].indexCount = FBX.GetMeshVec()[i].index.size();
		printf("\nIndices: %d", FBX.GetMeshVec()[i].index.size());

		//initialize the current vertexData array
		vArray[i] = new vertexData[FBX.GetMeshVec()[i].vertexData.size()];
		//initialize the current index array
		iArray[i] = new index[FBX.GetMeshVec()[i].index.size()];

		
		
		for (int j = 0; j < FBX.GetMeshVec()[i].vertexData.size(); j++) //vertexData
		{
			printf("\n\nVertex Index: %d\n", FBX.GetMeshVec()[i].index[j]);
			//set pos then print it
			vArray[i][j].pos[0] = FBX.GetMeshVec()[i].vertexData[i].x;
			vArray[i][j].pos[1] = FBX.GetMeshVec()[i].vertexData[i].y;
			vArray[i][j].pos[2] = FBX.GetMeshVec()[i].vertexData[i].z;

			printf("Position\n X: %f, Y: %f, Z: %f\n", vArray[i][j].pos[0],
				vArray[i][j].pos[1],
				vArray[i][j].pos[2]);

			//set normal then print it
			vArray[i][j].normal[0] = FBX.GetMeshVec()[i].vertexData[i].norX;
			vArray[i][j].normal[1] = FBX.GetMeshVec()[i].vertexData[i].norY;
			vArray[i][j].normal[2] = FBX.GetMeshVec()[i].vertexData[i].norZ;
			printf("Normal\n X: %f, Y: %f, Z: %f\n", FBX.GetMeshVec()[i].vertexData[i].norX,
				FBX.GetMeshVec()[i].vertexData[i].norY,
				FBX.GetMeshVec()[i].vertexData[i].norZ);

			//SET UV AND MMMMMGUESS WHAT... PRINT IT
			vArray[i][j].uv[0] = FBX.GetMeshVec()[i].vertexData[i].u;
			vArray[i][j].uv[1] = FBX.GetMeshVec()[i].vertexData[i].v;

			printf("UVs\n U: %f, V: %f\n", FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].u,
				FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].v);
		}

		for (int j = 0; j < FBX.GetMeshVec()[i].index.size(); j++) //KEEP GOING HERE LOAD VERTEXIAN DATA AND INDEXIES
		{
			// SET GOD DAMNED INDIIICEEEEES!½
			iArray[i][j].vertexIndex = FBX.GetMeshVec()[i].index[j];
		}
	}

}

void FishBoX::listFiles(const char* path)
{
	struct _finddata_t dirFile;
	long hFile;

	bool gIgnoreHidden = true;

	if ((hFile = _findfirst(path, &dirFile)) != -1)
	{
		do
		{
			if (!strcmp(dirFile.name, ".")) continue;
			if (!strcmp(dirFile.name, "..")) continue;
			if (gIgnoreHidden)
			{
				if (dirFile.attrib & _A_HIDDEN) continue;
				if (dirFile.name[0] == '.') continue;
			}

			// dirFile.name is the name of the file. Do whatever string comparison 
			// you want here. Something like:
			if (strstr(dirFile.name, ".fbx"))
				printf("found a .txt file: %s", dirFile.name);

		} while (_findnext(hFile, &dirFile) == 0);
		_findclose(hFile);
	}
}

void FishBoX::writeShit()
{
	std::ofstream outfile("testBin.FSH", std::ofstream::binary); //filestream

	
	outfile.write((const char*)&HEADER, sizeof(fileHeader)); //write main header


	for (int i = 0; i < HEADER.meshCount; i++) //mesh loop
	{
		outfile.write((const char*)&meArray[i], sizeof(mesh));

		outfile.write((const char*)vArray[i], sizeof(vertexData) * meArray[i].vertexCount);
		outfile.write((const char*)iArray[i], sizeof(index) * meArray[i].indexCount);
	}
	outfile.close();

	writeErrorCheck();
}

void FishBoX::writeErrorCheck() //check if written file matches file in memory
{
	fileHeader tHEADER;
	mesh * tMesh;
	vertexData ** tVertex;
	index ** tIndex;

	std::ifstream infile("testBin.FSH", std::ifstream::binary);

	infile.read((char*)&tHEADER, sizeof(fileHeader));

	printf("\nHEADER\n");
	printf("Meshes: %d\n", tHEADER.meshCount);
	printf("Materials %d\n", tHEADER.materialCount);
	printf("Directional Lights: %d\n", tHEADER.directionalLightCount);
	printf("Point Lights: %d\n", tHEADER.pointLightCount);
	printf("Area Lights: %d\n", tHEADER.areaLightCount);
	printf("Cameras: %d\n", tHEADER.cameraCount);

	tMesh = new mesh[tHEADER.meshCount];
	tVertex = new vertexData*[tHEADER.meshCount];
	tIndex = new index*[tHEADER.meshCount];

	for (int i = 0; i < tHEADER.meshCount; i++) //mesh loop
	{
		infile.read((char*)&tMesh[i], sizeof(mesh));

		tVertex[i] = new vertexData[tMesh[i].vertexCount];
		tIndex[i] = new index[tMesh[i].indexCount];

		printf("\n\nMesh: %d", (i + 1));
		printf("\nMaterialName: %c%c%c%c", tMesh[i].materialName[0], tMesh[i].materialName[1], tMesh[i].materialName[2], tMesh[i].materialName[3]); //TEMP
		printf("\n\nVertices: %d", tMesh[i].vertexCount);
		printf("\nBlendShapes: %d", tMesh[i].blendshapesCount);
		printf("\nIndices: %d", tMesh[i].indexCount);

		infile.read((char*)tVertex[i], sizeof(vertexData) * tMesh[i].vertexCount);
		infile.read((char*)tIndex[i], sizeof(index) * tMesh[i].indexCount);

		for (int j = 0; j < tMesh[i].vertexCount; j++)
		{
			printf("\n%f, %f, %f", tVertex[i][j].pos[0], tVertex[i][j].pos[1], tVertex[i][j].pos[2]);
		}

		int result = memcmp(tVertex[i], vArray[i], tMesh[i].vertexCount*sizeof(vertexData));



		printf("\nresult: %d\n", result);
	}

	infile.close();

}


