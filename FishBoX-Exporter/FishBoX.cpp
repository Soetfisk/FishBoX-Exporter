#include "FishBoX.h"

FishBoX::FishBoX()
{

}

FishBoX::~FishBoX()
{
	
}

void FishBoX::initialize() //DEPRICATED
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
	HEADER.directionalLightCount = FBX.GetDirectionalLightVec().size();
	HEADER.pointLightCount = 0;
	HEADER.areaLightCount = 0;
	HEADER.cameraCount = FBX.GetCameraVec().size(); //make function;
	
	int len = filepath.length();
	
	if (filepath[len - 1] == '\r')
	{
		filepath.replace(len - 1, 1, "");
	}
	if (filepath[len - 1] == '\n')
	{
		filepath.replace(len - 1, 1, "");
	}

	newfilename = filepath.substr(0, filepath.find_last_of('.')) + ".FSH";

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
	
	pointLight * plArray = new pointLight[HEADER.pointLightCount];
	areaLight *alArray = new areaLight[HEADER.areaLightCount];
	
	
	vArray = new vertexData*[HEADER.meshCount];
	iArray = new index*[HEADER.meshCount];
	bsArray = new blendShape**[HEADER.meshCount];
	caArray = new camera[HEADER.cameraCount];
	dlArray = new directionalLight[HEADER.directionalLightCount];


	for (int i = 0; i < HEADER.meshCount; i++)
	{
		//print no of meshes
		printf("\n\nMesh: %d", (i + 1));

		//set material name then print it
		strncpy_s(meArray[i].materialName, FBX.GetMeshVec()[i].materialName, sizeof(FBX.GetMeshVec()[i].materialName));
		printf("\nMaterialName: %s", meArray[i].materialName); 
		
		//set vertexcount then print it
		meArray[i].vertexCount = FBX.GetMeshVec()[i].vertexData.size();
		printf("\nVertices: %d", FBX.GetMeshVec()[i].vertexData.size());

		//set blendshapeCount then print it
		meArray[i].blendshapesCount = FBX.GetBSCount()[i];
		printf("\nBlendShapes: %d", meArray[i].blendshapesCount);

		//set indexCount then print it
		meArray[i].indexCount = FBX.GetMeshVec()[i].index.size();
		printf("\nIndices: %d", FBX.GetMeshVec()[i].index.size());

		
		//initialize the current vertexData array
		vArray[i] = new vertexData[meArray[i].vertexCount];
		//initialize the current index array
		iArray[i] = new index[meArray[i].vertexCount];
		//initialize the current blendShape array
		bsArray[i] = new blendShape*[meArray[i].blendshapesCount];
		printf("\n");
		for (int j = 0; j < meArray[i].vertexCount; j++) //vertexData
		{

			//printf("\n\nVertex Index: %d\n", FBX.GetMeshVec()[i].index[j]);
			//set pos then print it;
			vArray[i][j].pos[0] = FBX.GetMeshVec()[i].vertexData[j].x;
			vArray[i][j].pos[1] = FBX.GetMeshVec()[i].vertexData[j].y;
			vArray[i][j].pos[2] = FBX.GetMeshVec()[i].vertexData[j].z;


			if (j == 0)
				printf("\n Written nonbs vert%d: %f, %f, %f\n\n", j, vArray[i][j].pos[0],
					vArray[i][j].pos[1],
					vArray[i][j].pos[2]);
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
			//for (int k = 0; k < meArray[i].blendshapesCount; k++)
			//{
			//	bsArray[i][j+k].pos[0] = FBX.GetBSVec()[i][j + k].x;
			//	bsArray[i][j+k].pos[1] = FBX.GetBSVec()[i][j + k].y;
			//	bsArray[i][j+k].pos[2] = FBX.GetBSVec()[i][j + k].z;

			//	if (j == 0)
			//		printf("\n Written bs%d vert%d: %f, %f, %f", k+1, j, bsArray[i][j + k].pos[0], bsArray[i][j + k].pos[1], bsArray[i][j + k].pos[2]);
			//	if (j == 1)
			//		printf("\n Written bs%d vert%d: %f, %f, %f", k + 1, j, bsArray[i][j + k].pos[0], bsArray[i][j + k].pos[1], bsArray[i][j + k].pos[2]);
			//		
			//	if (j == 2)
			//		printf("\n Written bs%d vert%d: %f, %f, %f", k + 1, j, bsArray[i][j + k].pos[0], bsArray[i][j + k].pos[1], bsArray[i][j + k].pos[2]);
			//}		
			//if (j == 0)
			//	printf("\n");
			//if (j == 1)
			//	printf("\n");
			//if (j == 2)
			//	printf("\n");
		}

		for (int j = 0; j <meArray[i].blendshapesCount; j++)
		{
			bsArray[i][j] = new blendShape[meArray[i].vertexCount];
			for (int k = 0; k < meArray[i].vertexCount; k++)
			{
				bsArray[i][j][k].pos[0] = FBX.GetBSVec()[i][j][k].x;
				bsArray[i][j][k].pos[1] = FBX.GetBSVec()[i][j][k].y;
				bsArray[i][j][k].pos[2] = FBX.GetBSVec()[i][j][k].z;

				printf("\n Written bs%d: %f, %f, %f", k, bsArray[i][j][k].pos[0], bsArray[i][j][k].pos[1], bsArray[i][j][k].pos[2]);


			}
			printf("\n");
		}

		for (int j = 0; j < FBX.GetMeshVec()[i].index.size(); j++) //KEEP GOING HERE LOAD VERTEXIAN DATA AND INDEXIES
		{
			// SET GOD DAMNED INDIIICEEEEES!½
			iArray[i][j].vertexIndex = FBX.GetMeshVec()[i].index[j];
		}
	}
	for (int  i = 0; i < HEADER.materialCount; i++)
	{
		std::string tempTexFP;

		if (!(std::find(texVector.begin(), texVector.end(), FBX.GetMaterialVec()[i].textureFilePath) != texVector.end()))
		{//check if we have the texture filepath already
			texVector.push_back(FBX.GetMaterialVec()[i].textureFilePath); //if not, append it
			tempTexFP = FBX.GetMaterialVec()[i].textureFilePath;
			tempTexFP = "Textures" + tempTexFP.substr(tempTexFP.find_last_of('/'));
			tempTexFP = tempTexFP.substr(0, tempTexFP.find_last_of('.')) + ".FST";
			newTexVector.push_back(tempTexFP);
		}
		//read material data and print it
		strncpy_s(maArray[i].materialName, FBX.GetMaterialVec()[i].materialName, sizeof(FBX.GetMaterialVec()[i].materialName));
		printf("\n\nMATERIAL %d: %s", i, maArray[i].materialName);

		//format a string for the new texture filepath and name


		strncpy_s(maArray[i].textureFilePath, tempTexFP.c_str(), sizeof(maArray[i].textureFilePath));
		printf("\nTexture: %s", maArray[i].textureFilePath);

		strncpy_s(maArray[i].normalFilePath, FBX.GetMaterialVec()[i].normalFilePath, sizeof(FBX.GetMaterialVec()[i].normalFilePath));
		printf("\nNormal map: %s", maArray[i].normalFilePath);

		maArray[i].ambient[0] = FBX.GetMaterialVec()[i].ambient[0];
		maArray[i].ambient[1] = FBX.GetMaterialVec()[i].ambient[1];
		maArray[i].ambient[2] = FBX.GetMaterialVec()[i].ambient[2];
		printf("\nAmbient: %f, %f, %f", maArray[i].ambient[0], maArray[i].ambient[1], maArray[i].ambient[2]);

		maArray[i].diffuse[0] = FBX.GetMaterialVec()[i].diffuse[0];
		maArray[i].diffuse[1] = FBX.GetMaterialVec()[i].diffuse[1];
		maArray[i].diffuse[2] = FBX.GetMaterialVec()[i].diffuse[2];
		printf("\ndiffuse: %f, %f, %f", maArray[i].diffuse[0], maArray[i].diffuse[1], maArray[i].diffuse[2]);

		maArray[i].specular[0] = FBX.GetMaterialVec()[i].specular[0];
		maArray[i].specular[1] = FBX.GetMaterialVec()[i].specular[1];
		maArray[i].specular[2] = FBX.GetMaterialVec()[i].specular[2];
		printf("\nspecular: %f, %f, %f", maArray[i].specular[0], maArray[i].specular[1], maArray[i].specular[2]);
		
		maArray[i].shinyness = FBX.GetMaterialVec()[i].shinyness;
		printf("\nShinyness: %f", maArray[i].shinyness);
	}

	for (size_t i = 0; i < HEADER.directionalLightCount ; i++)
	{
		dlArray[i].intensity = FBX.GetDirectionalLightVec()[i].intensity;
		printf("\n\nDirectionalLightIntensity: %f", dlArray[i].intensity);
		for (size_t j = 0; j < 3; j++)
		{
			dlArray[i].lightColor[j] = FBX.GetDirectionalLightVec()[i].lightColor[j];
			dlArray[i].direction[j] = FBX.GetDirectionalLightVec()[i].direction[j];
		}
		

		printf("\nDirectionalLightColor: %f, %f, %f", dlArray[i].lightColor[0], dlArray[i].lightColor[1], dlArray[i].lightColor[2]);
		printf("\nDirectionalLightDirection: %f, %f, %f\n", dlArray[i].direction[0], dlArray[i].direction[1], dlArray[i].direction[2]);
	} 

	//Camera here
	printf("\n\nCAMERA: ");
	for (int i = 0; i < HEADER.cameraCount; i++)
	{
		caArray[i].pos[0] = FBX.GetCameraVec()[i].pos[0];
		caArray[i].pos[1] = FBX.GetCameraVec()[i].pos[1];
		caArray[i].pos[2] = FBX.GetCameraVec()[i].pos[2];
		for (size_t j = 0; j < 3; j++)
		{
			printf("\npos: %f", caArray[i].pos[j]);
		}

		caArray[i].roll = FBX.GetCameraVec()[i].roll;
		printf("\nRoll: %f", caArray[i].roll);

		caArray[i].target[0] = FBX.GetCameraVec()[i].target[0];
		caArray[i].target[1] = FBX.GetCameraVec()[i].target[1];
		caArray[i].target[2] = FBX.GetCameraVec()[i].target[2];
		for (size_t j = 0; j < 3; j++)
		{
			printf("\ntarget: %f", caArray[i].target[j]);
		}
		

		caArray[i].upVec[0] = FBX.GetCameraVec()[i].upVec[0];
		caArray[i].upVec[1] = FBX.GetCameraVec()[i].upVec[1];
		caArray[i].upVec[2] = FBX.GetCameraVec()[i].upVec[2];
		for (size_t j = 0; j < 3; j++)
		{
			printf("\nUPVEC: %f", caArray[i].upVec[j]);
		}

		caArray[i].farPlane = FBX.GetCameraVec()[i].farPlane;
		caArray[i].nearPlane = FBX.GetCameraVec()[i].nearPlane;

		printf("\nfarPlane: %f", caArray[i].farPlane);

		caArray[i].pixelRatio = FBX.GetCameraVec()[i].pixelRatio;

		printf("\npixelRatio: %f", caArray[i].pixelRatio);
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
		for (int j = 0; j < meArray[i].blendshapesCount; j++)
			outfile.write((const char*)bsArray[i][j], sizeof(blendShape) * meArray[i].vertexCount);
	}
	for (int i = 0; i < HEADER.materialCount; i++)
	{
		outfile.write((const char*)&maArray[i], sizeof(material));
	}

	//camera write
	for (int i = 0; i < HEADER.cameraCount; i++)
	{
		outfile.write((const char*)&caArray[i], sizeof(camera));
	}

	//DirectionalLight write
	for (int i = 0; i < HEADER.directionalLightCount; i++)
	{
		outfile.write((const char*)&dlArray[i], sizeof(directionalLight));
	}



	//lightwrite

	outfile.close();
	
	for (int i = 0; i < texVector.size(); i++)
	{
		long size = 0;

		std::ifstream texIn(texVector.at(i).c_str(), std::ofstream::binary);

		std::ofstream texOut(newTexVector.at(i).c_str(), std::ofstream::binary);
		
		texIn.seekg(0, texIn.end);
		size = texIn.tellg();
		texIn.seekg(0);

		char* buffer = new char[size];

		texIn.read(buffer, size);

		//texOut.write((char*)&size, sizeof(long)); 
		texOut.write(buffer, size);

		delete[] buffer;

		texOut.close();
		texIn.close();
	}


	//writeErrorCheck();
}

void FishBoX::writeErrorCheck() //check if written file matches file in memory
{
	fileHeader tHEADER;
	mesh * tMesh;
	material * tMaterial;
	vertexData ** tVertex;
	index ** tIndex;
	

	std::ifstream infile(newfilename, std::ifstream::binary);

	infile.read((char*)&tHEADER, sizeof(fileHeader));

	tMesh = new mesh[tHEADER.meshCount];
	tMaterial = new material[tHEADER.materialCount];
	tVertex = new vertexData*[tHEADER.meshCount];
	tIndex = new index*[tHEADER.meshCount];
	

	int result;

	printf("\n\n\n### ERROR CHECKING ###\n");

	printf("\nFILEHEADER ERROR CHECK:");
	result = memcmp(&tHEADER, &HEADER, sizeof(fileHeader));
	printf("\nFileheader data error check result: ");
	if (result == 0)
		printf("%s", "SAFE");
	else printf("%s", "FAILED");

	printf("\n");

	for (int i = 0; i < tHEADER.meshCount; i++) //mesh loop
	{
		int result;
		infile.read((char*)&tMesh[i], sizeof(mesh));

		tVertex[i] = new vertexData[tMesh[i].vertexCount];
		tIndex[i] = new index[tMesh[i].indexCount];

		//printf("\n\nMesh: %d", (i + 1));
		//printf("\nMaterialName: %c%c%c%c", tMesh[i].materialName[0], tMesh[i].materialName[1], tMesh[i].materialName[2], tMesh[i].materialName[3]); //TEMP
		//printf("\n\nVertices: %d", tMesh[i].vertexCount);
		//printf("\nBlendShapes: %d", tMesh[i].blendshapesCount);
		//printf("\nIndices: %d", tMesh[i].indexCount);

		infile.read((char*)tVertex[i], sizeof(vertexData) * tMesh[i].vertexCount);
		infile.read((char*)tIndex[i], sizeof(index) * tMesh[i].indexCount);

		//for (int j = 0; j < tMesh[i].vertexCount; j++)
		//{
		//	printf("\n%f, %f, %f", tVertex[i][j].pos[0], tVertex[i][j].pos[1], tVertex[i][j].pos[2]);
		//}

		printf("\nMESH %d ERROR CHECK:" , i);
		result = memcmp(&tMesh[i], &meArray[i], sizeof(mesh));
		printf("\nMesh data error check result: ");
		if (result == 0)
			printf("%s", "SAFE");
		else printf("%s", "FAILED");

		result = memcmp(tVertex[i], vArray[i], tMesh[i].vertexCount*sizeof(vertexData));
		printf("\nVertex error check result: ");
		if (result == 0)
			printf("%s", "SAFE");
		else printf("%s", "FAILED");

		result = memcmp(tIndex[i], iArray[i], tMesh[i].indexCount*sizeof(index));
		printf("\nIndex error check result: ");
		if (result == 0)
			printf("%s", "SAFE");
		else printf("%s", "FAILED");

		printf("\n");

	}
	infile.close();
}



