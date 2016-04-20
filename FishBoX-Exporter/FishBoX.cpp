#include "FishBoX.h"

FishBoX::FishBoX()
{

}

FishBoX::~FishBoX()
{
	
}

void FishBoX::initialize()
{
	FBX.loadModels("./tempfish.fbx");

	HEADER.meshCount = FBX.GetMeshVec().size();
	HEADER.materialCount = FBX.GetMaterialVec().size();
	HEADER.directionalLightCount = 0;
	HEADER.pointLightCount = 0;
	HEADER.areaLightCount = 0;
	HEADER.cameraCount = 0;

	printShit();
	writeShit("tempfish.FSH");
}

void FishBoX::initialize(std::string filepath)
{
	FBX.loadModels(filepath.c_str());

	HEADER.meshCount = FBX.GetMeshVec().size();
	HEADER.materialCount = FBX.GetMaterialVec().size();
	HEADER.directionalLightCount = 0;
	HEADER.pointLightCount = 0;
	HEADER.areaLightCount = 0;
	HEADER.cameraCount = 0;
	
	int len = filepath.length();
	
	if (filepath[len - 1] == '\r')
	{
		filepath.replace(len - 1, 1, "");
	}
	if (filepath[len - 1] == '\n')
	{
		filepath.replace(len - 1, 1, "");
	}

	std::string newfilename = filepath.substr(0, filepath.find_last_of('.')) + ".FSH";

	printShit();
	writeShit(newfilename.c_str());
}
//
//void FishBoX::test()
//{
//	char derp[256];
//	std::string string = "hello penis face";
//	//derp = convertString(string);
//}

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
		strncpy_s(meArray[i].materialName, FBX.GetMeshVec()[i].materialName, sizeof(FBX.GetMeshVec()[i].materialName)); //TEMP
		printf("\nMaterialName: %s", meArray[i].materialName); //TEMP
		
		//set vertexcount then print it
		meArray[i].vertexCount = FBX.GetMeshVec()[i].vertexData.size();
		printf("\nVertices: %d", FBX.GetMeshVec()[i].vertexData.size());

		//set blendshapeCount then print it
		meArray[i].blendshapesCount = 0; //TEMP
		printf("\nBlendShapes: %d", meArray[i].blendshapesCount);

		//set indexCount then print it
		meArray[i].indexCount = FBX.GetMeshVec()[i].index.size();
		printf("\nIndices: %d", FBX.GetMeshVec()[i].index.size());

		//initialize the current vertexData array
		vArray[i] = new vertexData[meArray[i].vertexCount];
		//initialize the current index array
		iArray[i] = new index[meArray[i].vertexCount];
		

		for (int j = 0; j < meArray[i].vertexCount; j++) //vertexData
		{

			//printf("\n\nVertex Index: %d\n", FBX.GetMeshVec()[i].index[j]);
			//set pos then print it;
			vArray[i][j].pos[0] = FBX.GetMeshVec()[i].vertexData[j].x;
			vArray[i][j].pos[1] = FBX.GetMeshVec()[i].vertexData[j].y;
			vArray[i][j].pos[2] = FBX.GetMeshVec()[i].vertexData[j].z;

			//printf("Position\n X: %f, Y: %f, Z: %f\n", vArray[i][j].pos[0],
			//vArray[i][j].pos[1],
			//vArray[i][j].pos[2]);

			//set normal then print it
			vArray[i][j].normal[0] = FBX.GetMeshVec()[i].vertexData[j].norX;
			vArray[i][j].normal[1] = FBX.GetMeshVec()[i].vertexData[j].norY;
			vArray[i][j].normal[2] = FBX.GetMeshVec()[i].vertexData[j].norZ;
			/*printf("Normal\n X: %f, Y: %f, Z: %f\n", FBX.GetMeshVec()[i].vertexData[i].norX,
				FBX.GetMeshVec()[i].vertexData[i].norY,
				FBX.GetMeshVec()[i].vertexData[i].norZ);*/

			//SET UV AND MMMMMGUESS WHAT... PRINT IT
			vArray[i][j].uv[0] = FBX.GetMeshVec()[i].vertexData[j].u;
			vArray[i][j].uv[1] = FBX.GetMeshVec()[i].vertexData[j].v;

			//printf("UVs\n U: %f, V: %f\n", FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].u,
			//	FBX.GetMeshVec()[i].vertexData[FBX.GetMeshVec()[i].index[j]].v);
		}

		for (int j = 0; j < FBX.GetMeshVec()[i].index.size(); j++) //KEEP GOING HERE LOAD VERTEXIAN DATA AND INDEXIES
		{
			// SET GOD DAMNED INDIIICEEEEES!½
			iArray[i][j].vertexIndex = FBX.GetMeshVec()[i].index[j];
		}
	}
}



void FishBoX::writeShit(std::string filepath)
{
	std::ofstream outfile(filepath.c_str(), std::ofstream::binary); //filestream

	
	outfile.write((const char*)&HEADER, sizeof(fileHeader)); //write main header


	for (int i = 0; i < HEADER.meshCount; i++) //mesh loop
	{
		outfile.write((const char*)&meArray[i], sizeof(mesh));

		outfile.write((const char*)vArray[i], sizeof(vertexData) * meArray[i].vertexCount);
		outfile.write((const char*)iArray[i], sizeof(index) * meArray[i].indexCount);
	}
	outfile.close();

	//writeErrorCheck();
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



