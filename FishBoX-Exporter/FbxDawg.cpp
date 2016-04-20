
#include "FbxDawg.h"


//How to organize code for readability:

//
//In h-files:
//- Public before private.
//- Everything only used within the function, should be in private.
//- Have attributes at the top.
//-	Have comments above each function explaining clearly what the parameters and return values are used for.
//	Those comments must be written as /** text **/ and placed above the function. This way the comment
//	will be displayed when hovering pointer over function-name.
//- Organize functions alphabetically, or by frequency of use.
//- Organize attributes by type, alphabetically, or frequency of use.
//- Have the destructors and constructors at the bottom.
//- Come to an agreement on naming standards of functions, booleans, classes, structs, and variables.
//- Come to an agreement on how to write out {}.
//- Generally have short, to-the-point names of functions, variables, and classes.

//In cpp-files:
//- Have the functions appear in the same order as in the header.
//- Have functions be the maximum length of one "screen-height"
//- If some code is within an if-statement: consider making it a function.
//- Have comments describing parts of the code, and what it's used for.

FbxDawg::FbxDawg()
{

}

FbxDawg::~FbxDawg()
{
	
	delete this->FBXIndexArray;
}




void FbxDawg::loadModels(const char* filePath)
{

	FbxManager *SDK_Manager = FbxManager::Create(); //FbxManager is an object that handles memory management.

	FbxIOSettings *ios = FbxIOSettings::Create(SDK_Manager, IOSROOT); //FbxIOSettings is a collection of import/export options
	SDK_Manager->SetIOSettings(ios);

	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	ios->SetBoolProp(IMP_FBX_ANIMATION, true);


	FbxScene* Fbx_Scene = FbxScene::Create(SDK_Manager, "myScene"); //FbxScene contains all the nodes, materials, textures, poses, characters


	FbxImporter * Fbx_Importer = FbxImporter::Create(SDK_Manager, ""); //FbxImporter is to import an FBX file into SDK objects.

	Fbx_Importer->Initialize(filePath, -1, SDK_Manager->GetIOSettings());// eller ios ist�llet f�r SDK_M
	Fbx_Importer->Import(Fbx_Scene);

	FbxNode* FBXRootNode = Fbx_Scene->GetRootNode(); // FbxCamera, FbxLight, FbxMesh, etc... elements organized in a hierarchical tree. Root is the mother and by FbxNode::GetChild() we work our way down

	if (FBXRootNode)
	{
		for (int i = 0; i < FBXRootNode->GetChildCount(); i++)//For each and every childnode...
		{
			FbxNode* FbxChildNode = FBXRootNode->GetChild(i);//... initialize the childnode we are at

			if (FbxChildNode->GetNodeAttribute() == NULL)//... and then check if its an unset attribute. (special node that we dont want now) (A NULL node attribute is set by calling function FbxNode::SetNodeAttribute() with a NULL pointer)
				continue;//tar n�sta child ist�llet

			FbxNodeAttribute::EType AttributeType = FbxChildNode->GetNodeAttribute()->GetAttributeType();//... But if its not unset, we check what type the content is, FbxCamera, FbxSkeleton, FbxMesh, etc...

			if (AttributeType == FbxNodeAttribute::eMesh)// Only meshes allowed to enter check.
				processMesh(FbxChildNode);
		}
		Fbx_Importer->Destroy();
	}
} //end of loader



std::vector<int> FbxDawg::makeIndexList(std::vector<MyVertexStruct> modelVertexList)
{
	if (FBXIndexArray != nullptr)
		delete[] this->FBXIndexArray;

	this->FBXIndexArray = new int[modelVertexList.size()];
	this->sizeOfFBXIndexArray = modelVertexList.size();

	for (int i = 0; i < this->sizeOfFBXIndexArray; i++)
		FBXIndexArray[i] = i;

	for (int vertex = 0; vertex< modelVertexList.size(); vertex++)
	{
		for (int other = vertex + 1; other < modelVertexList.size(); other++)
		{
			if (modelVertexList[vertex].norX == modelVertexList[other].norX &&
				modelVertexList[vertex].norY == modelVertexList[other].norY &&
				modelVertexList[vertex].norZ == modelVertexList[other].norZ
				&&

				modelVertexList[vertex].u == modelVertexList[other].u &&
				modelVertexList[vertex].v == modelVertexList[other].v
				&&

				modelVertexList[vertex].x == modelVertexList[other].x &&
				modelVertexList[vertex].y == modelVertexList[other].y &&
				modelVertexList[vertex].z == modelVertexList[other].z)
			{
			
				FBXIndexArray[other] = FBXIndexArray[vertex]; 
				//Remove that index and replace with earlier.
			}
		}//printf("%d\n", FBXIndexArray[vertex]); //Compared with all other.

	}//All vertexes have been compared

	
	//convert to vector for reasons
	std::vector<int> indexVec;
	indexVec.resize(sizeOfFBXIndexArray);

	for (int i = 0; i < sizeOfFBXIndexArray; i++)
		indexVec[i] = FBXIndexArray[i];

	return indexVec;
}

void FbxDawg::makeControlPointMap(FbxMesh* currMesh)
{
	int aids = currMesh->GetControlPointsCount();

	for (int i = 0; i < aids; ++i) {
		sAnimationData iAmHereToFillVector;
		dataPerControlPoint.push_back(&iAmHereToFillVector);
	}
}

void FbxDawg::processMesh(FbxNode * FbxChildNode)
{
	FbxMesh* mesh = (FbxMesh*)FbxChildNode->GetNodeAttribute();//we are sure that there was a mesh that went through, we get the content of the node.


	if (mesh->GetDeformerCount(FbxDeformer::eBlendShape) > 0)
		this->bsLoader(mesh);

	Mesh tempMesh;
	//tempMesh.vertexData.resize(mesh->GetPolygonCount() * 3);

	std::vector<MyIndexStruct> indexData; indexData.resize(mesh->GetPolygonCount() * 3);
	//std::vector<MyVertexStruct> vertData; vertData.resize(mesh->GetPolygonCount() * 3);


	//static int offsets[] = { 1, 0, 2 }; //directX
	static int offsets[] = { 0, 1, 2 }; //opengl

	processVertex(mesh, tempMesh.vertexData, indexData, offsets);
	processNormals(mesh, tempMesh.vertexData, indexData, offsets);
	processUV(mesh, tempMesh.vertexData, indexData, offsets);


	//>>>>>>>>>Texture filepath<<<<<<<<<<<<<<<
	//int material_Count = mesh->GetSrcObjectCount<FbxSurfaceMaterial>();
	int matCount = FbxChildNode->GetMaterialCount();
	for (int m = 0; m < matCount; m++)//.. for every material attached to the mesh
	{
		material tempMat;
		FbxSurfaceMaterial* materialRead = FbxChildNode->GetMaterial(m);
		if (materialRead)//.. if material
		{
			 // make temp material struct for later pushback
			FbxPropertyT<FbxDouble3> double3;
			FbxPropertyT<FbxDouble> double1;

			std::string name = materialRead->GetNameOnly();
			strncpy_s(tempMat.materialName, name.c_str(), sizeof(tempMat.materialName));
			strncpy_s(tempMesh.materialName, name.c_str(), sizeof(tempMesh.materialName));

			
			if (materialRead->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				double3 = ((FbxSurfacePhong *)materialRead)->Ambient;
				tempMat.ambient[0] = double3.Get()[0]; tempMat.ambient[1] = double3.Get()[1]; tempMat.ambient[2] = double3.Get()[2];
				//printf("\n%f, %f, %f\n", tempMat.ambient[0], tempMat.ambient[1], tempMat.ambient[2]);

				double3 = ((FbxSurfacePhong *)materialRead)->Specular;
				tempMat.specular[0] = double3.Get()[0]; tempMat.specular[1] = double3.Get()[1]; tempMat.specular[2] = double3.Get()[2];
				//printf("\n%f, %f, %f\n", tempMat.specular[0], tempMat.specular[1], tempMat.specular[2]);

				double3 = ((FbxSurfacePhong *)materialRead)->Diffuse;
				tempMat.diffuse[0] = double3.Get()[0]; tempMat.diffuse[1] = double3.Get()[1]; tempMat.diffuse[2] = double3.Get()[2];
				//printf("\n%f, %f, %f\n", tempMat.diffuse[0], tempMat.diffuse[1], tempMat.diffuse[2]);

				double1 = ((FbxSurfacePhong *)materialRead)->Shininess;
				tempMat.shinyness = double1.Get();
				//printf("\n%f\n", tempMat.shinyness);
			}
			else if (materialRead->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				double3 = ((FbxSurfacePhong *)materialRead)->Ambient;
				tempMat.ambient[0] = double3.Get()[0]; tempMat.ambient[1] = double3.Get()[1]; tempMat.ambient[2] = double3.Get()[2];
				//printf("\n%f, %f, %f\n", tempMat.ambient[0], tempMat.ambient[1], tempMat.ambient[2]);

				double3 = ((FbxSurfacePhong *)materialRead)->Diffuse;
				tempMat.diffuse[0] = double3.Get()[0]; tempMat.diffuse[1] = double3.Get()[1]; tempMat.diffuse[2] = double3.Get()[2];
				//printf("\n%f, %f, %f\n", tempMat.diffuse[0], tempMat.diffuse[1], tempMat.diffuse[2]);
			}

			FbxProperty prop = materialRead->FindProperty(FbxSurfaceMaterial::sDiffuse);
			int texture_Count = prop.GetSrcObjectCount<FbxTexture>();
			for (int i = 0; i < texture_Count; i++)// how many texturefiles attached to the material
			{
				const FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(i));

				wchar_t* wideName;
				FbxUTF8ToWC(((const FbxFileTexture*)texture)->GetFileName(), wideName);

				std::string filepath = ((const FbxFileTexture*)texture)->GetFileName();

				textureFilepath = wideName;

				strncpy_s(tempMat.textureFilePath, filepath.c_str(), sizeof(tempMat.textureFilePath));

				printf("%s", tempMat.textureFilePath);

				FbxFree(wideName);
			}
		}
		materialVec.push_back(tempMat);
	}

#pragma region >>ASSEMBLY OF VERTEXDATA<<
	MyVertexStruct tempVertex;
	MyBSposStruct tempBlendShape;
	MyIndexStruct tempIndex;
	FbxVector4* Vertices;

	Vertices = mesh->GetControlPoints();

	for (int i = 0; i < indexData.size(); i++)
	{
		//printf("pos %d nor %d uv %d\n", indexData[i].posIndex, indexData[i].norIndex, indexData[i].uvIndex);
		FbxVector4 normals = mesh->GetElementNormal()->GetDirectArray().GetAt(indexData[i].norIndex);
		tempVertex.norX = normals[0];
		tempVertex.norY = normals[1];
		tempVertex.norZ = (normals[2]);

		tempVertex.x = (float)Vertices[indexData[i].posIndex].mData[0];
		tempVertex.y = (float)Vertices[indexData[i].posIndex].mData[1];
		tempVertex.z = ((float)Vertices[indexData[i].posIndex].mData[2]);


		FbxVector2 UVValue = indexData[i].UVElement->GetDirectArray().GetAt(indexData[i].uvIndex);
		tempVertex.u = UVValue.mData[0];
		tempVertex.v = UVValue.mData[1];

		tempMesh.vertexData.push_back(tempVertex);
	}

	for (int j = 0; j < bsVert.size(); j++)
		Vertices = bsVert[j];
	for (int i = 0; i < indexData.size(); i++)
	{

		//normals = normalElement->GetDirectArray().GetAt(indexData[i].norIndex);
		//tempVertex.norX = normals[0];
		//tempVertex.norY = normals[1];
		//tempVertex.norZ = (-1)*(normals[2]);

		tempBlendShape.x = (float)Vertices[indexData[i].posIndex].mData[0];
		tempBlendShape.y = (float)Vertices[indexData[i].posIndex].mData[1];
		tempBlendShape.z = -1 * ((float)Vertices[indexData[i].posIndex].mData[2]);

		//FbxVector2 UVValue = indexData[i].UVElement->GetDirectArray().GetAt(indexData[i].uvIndex);
		//tempVertex.u = UVValue.mData[0];
		//tempVertex.v = 1 - UVValue.mData[1];

		this->blendShapes.push_back(tempBlendShape);
	}
	

	this->makeIndexList(tempMesh.vertexData);

	tempMesh.index = this->makeIndexList(tempMesh.vertexData);

	this->MeshVec.push_back(tempMesh);

#pragma endregion >>ASSEMBLY OF VERTEXDATA<<

	//>>>>>>>CREATING THE MAP OF CONTROL POINTS<<<<<<<<<
	//________________________________________________
	makeControlPointMap(mesh);//<---------------------
	//________________________________________________

}
void FbxDawg::processVertex(FbxMesh* mesh, std::vector<MyVertexStruct>& vertData, std::vector<MyIndexStruct>& indexData, const int * offsets)
{
#pragma region >>VERTEX POSITION<<

	for (int t = 0; t < mesh->GetPolygonCount(); t++)//
	{
		int totalVertices = mesh->GetPolygonSize(t);//
		assert(totalVertices == 3);//

		for (int v = 0; v < totalVertices; v++)//
		{
			int controlPointIndex = mesh->GetPolygonVertex(t, v);
			indexData[(t * 3) + offsets[v]].posIndex = controlPointIndex; //adding index to a list. To create vertex later.
		}
	}
#pragma endregion >>VERTEX POSITION<<
}
void FbxDawg::processNormals(FbxMesh* mesh, std::vector<MyVertexStruct>& vertData, std::vector<MyIndexStruct>& indexData, const int * offsets)
{
#pragma region >>NORMALS<<
	FbxVector4 normals;
	FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
	if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		FbxVector4 normals;
		int indexByPolygonVertex = 0;
		
		for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++) //For every triangle
		{
			int polygonSize = mesh->GetPolygonSize(polygonIndex);

			for (int i = 0; i < polygonSize; i++) //For every vertex in triangle
			{
				int normalIndex = 0; //reference mode is direct, the normal index is the same as indexByPolygonVertex

				if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
					normalIndex = indexByPolygonVertex;
				}
				if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
					normalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);
				}

				normals = normalElement->GetDirectArray().GetAt(normalIndex);
				indexData[polygonIndex * 3 + offsets[i]].norIndex = normalIndex;
				indexByPolygonVertex++;
			}
		}
	}


	else if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		FbxVector4 normals;
		int indexByPolygonVertex = 0;

		for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++) //For every triangle
		{
			int polygonSize = mesh->GetPolygonSize(polygonIndex);

			for (int i = 0; i < polygonSize; i++) //For every vertex in triangle
			{
				int normalIndex = 0;
				//reference mode is direct, the normal index is the same as indexByPolygonVertex
				if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
					normalIndex = indexByPolygonVertex;
				}
				if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
					normalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);
				}

				normals = normalElement->GetDirectArray().GetAt(normalIndex);
				indexData[polygonIndex * 3 + offsets[i]].norIndex = normalIndex;
				indexByPolygonVertex++;
			}
		}
	}
#pragma endregion >> NORMALS <<
}
void FbxDawg::processUV(FbxMesh* mesh, std::vector<MyVertexStruct>& vertData, std::vector<MyIndexStruct>& indexData, const int * offsets)
{

#pragma region >>UV<<


	FbxStringList UVSetNameList;  //Gets all UV sets
	mesh->GetUVSetNames(UVSetNameList);
	FbxGeometryElementUV* UVElement;


	for (int UVSetIndex = 0; UVSetIndex < UVSetNameList.GetCount(); UVSetIndex++) //Iterates through the UV sets. Meshes can have multiple textures, every texture got a different UV set
	{

		char* UVSetName = UVSetNameList.GetStringAt(UVSetIndex); //Gets name of the current UV set

		UVElement = mesh->GetElementUV(UVSetName); //Gets the UV-element with the name that UVSetName holds

		if (!UVElement) {
			continue;
		}

		//Only supports these two modes.
		if (UVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			UVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;
		bool useIndex = UVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		int indexCount = (useIndex) ? UVElement->GetIndexArray().GetCount() : 0; //<---- what's happening here??? A questionmark? And the : 0 out of nowhere.

		int polyCount = mesh->GetPolygonCount();

		int polyIndexCounter = 0;
		if (UVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int polyIndex = 0; polyIndex < polyCount; polyIndex++)
			{
				//build the max index array that is needed
				int polySize = mesh->GetPolygonSize(polyIndex);
				for (int vertIndex = 0; vertIndex < polySize; ++vertIndex)
				{
					FbxVector2 UVValue;
					//get the index of the current current vertex in the vertex array
					int polyVertIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);
					//the UV index depends on the reference mode
					int UVIndex = useIndex ? UVElement->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;//<----questionmark and : again...

					UVValue = UVElement->GetDirectArray().GetAt(UVIndex);
					indexData[polyIndex*polySize + offsets[vertIndex]].uvIndex = UVIndex;
					indexData[polyIndex*polySize + offsets[vertIndex]].UVSetName = UVSetName;
					indexData[polyIndex*polySize + offsets[vertIndex]].UVElement = UVElement;
					polyIndexCounter++;

				}
			}
		}

		else if (UVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{

			for (int polyIndex = 0; polyIndex < polyCount; ++polyIndex)
			{
				int polySize = mesh->GetPolygonSize(polyIndex);
				for (int vertIndex = 0; vertIndex < polySize; ++vertIndex)
				{
					if (polyIndexCounter < indexCount)
					{
						FbxVector2 UVValue;
						int UVIndex = useIndex ? UVElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;

						UVValue = UVElement->GetDirectArray().GetAt(UVIndex);

						indexData[polyIndex*polySize + offsets[vertIndex]].uvIndex = UVIndex;
						indexData[polyIndex*polySize + offsets[vertIndex]].UVSetName = UVSetName;
						indexData[polyIndex*polySize + offsets[vertIndex]].UVElement = UVElement;
						polyIndexCounter++;

					}

				}
			}
		}


	}//uv


#pragma endregion >>UV<<
}
//for mesh



void FbxDawg::getJointData(FbxNode* rootNode, FbxScene* Fbx_Scene)
{
	//Explanation of FBX-terminology regarding Animation-Related things:
	//___________________________________________________________________________________________________________________________
	//Deformers are ways of deforming your mesh. Joints are deformers, but constraints like parent constraints are also deformers.
	//Therefore, one can think of a skeleton as a bunch of deformers of joint-type.
	//Inside each Deformer is a Cluster. Clusters contain "links" to the actual type of deformer. 
	//For a jointDeformer, it's Cluster would contain a link to the actual joint.
	//___________________________________________________________________________________________________________________________

	FbxMesh* currMesh = rootNode->GetMesh();
	unsigned int deformerCount = currMesh->GetDeformerCount();

	for (unsigned int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
			continue;

		unsigned int numberOfClusters = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numberOfClusters; ++clusterIndex) 
		{
			FbxCluster* currentCluster = currSkin->GetCluster(clusterIndex);
			const char* currentJointName = currentCluster->GetName();
			unsigned int currentJointIndex = findJointIndexByName(currentJointName);
			
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currentCluster->GetTransformMatrix(transformMatrix); //The transform of the MESH at bind-time
			currentCluster->GetTransformLinkMatrix(transformLinkMatrix); //The transformation of the cluster(joint) in bindtime from localspace to worldspace 
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix; 

			//Update the information in skeleton
			skeleton.joints[currentJointIndex]->globalBindPoseInverse = &globalBindposeInverseMatrix;
			skeleton.joints[currentJointIndex]->jointNode = currentCluster->GetLink();

			//Associate each joint with the control points it affects
			unsigned int numOfIndices = currentCluster->GetControlPointIndicesCount();

			for (unsigned int i = 0; i < numOfIndices; ++i) 
			{
				//currentCluster->GetControlPointWe
				sBlendingIndexWeightPair currBlendingIndexWeightPair;
				currBlendingIndexWeightPair.affectedJointIndex = currentJointIndex;
				currBlendingIndexWeightPair.blendingWeight = currentCluster->GetControlPointWeights()[i];
				//FUUUUCK what am I supposed to do next? Our code diverges here...
				//currCluster->GetControlPointIndices()[i] gets the returned value at index i. Meaning that the returned item is a list.
				//Uhmm... i need to have all of the control points somewhere... So that I can map the cluster's "affected Control Points"
				//somewhere. OR... Hmm... Can I map them here? Can I get all of the indices of the model? Do I have them already?
				dataPerControlPoint[currentCluster->GetControlPointIndices()[i]]->weightData.push_back(&currBlendingIndexWeightPair);
				
			}

			//Now start getting animation information
			//--> site says "Now only supports one take" in comment here.
			//stack, according to the "comment" in FbxAnimStack describes a stack as a 
			//collection of "animation layers". The Fbx-file can have one or more stacks.
			int count = 0;
			FbxAnimStack* aids;
			int numStacks = Fbx_Scene->GetSrcObjectCount<FbxAnimStack>();
			for (unsigned int stackIndex = 0; stackIndex < numStacks; ++stackIndex) 
			{

				FbxAnimStack* currentAnimStack = Fbx_Scene->GetSrcObject<FbxAnimStack>(stackIndex);

				int numAnimLayers = currentAnimStack->GetMemberCount<FbxAnimLayer>();

				for (unsigned int animLayerCounter = 0; animLayerCounter < numAnimLayers; ++animLayerCounter)
				{
					FbxAnimLayer* currAnimLayer = currentAnimStack->GetMember<FbxAnimLayer>();
					//FbxAnimCurve* curveRotation = skeleton.joints[currentJointIndex]->jointNode->LclTranslation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_TRANSFORM, false);
					//FbxAnimCurve* curveRotation = skeleton.joints[currentJointIndex]->jointNode->LclRotation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_ROTATION, false);


					//get the Translation and Rotation XYZ component curves. Also the scaling curve to "normalize" the scale.
					FbxAnimCurve* translationCurve_X = skeleton.joints[currentJointIndex]->jointNode->LclTranslation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					translationCurve_X->KeyGetCount();

					FbxAnimEvaluator* animEvaluator = Fbx_Scene->GetAnimationEvaluator();
					
					FbxString animStackName = currentAnimStack->GetName();
					FbxString temp = animStackName.Buffer();
					animationName.push_back(&temp);

					FbxTakeInfo* takeInfo = Fbx_Scene->GetTakeInfo(animStackName);
					FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
					FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
					animationLength.push_back(end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1);

					//I need to loop through all keyframes, get their respective time-values, and input them into the animEvaluator
					for (long keyIndex = 0; keyIndex < translationCurve_X->KeyGetCount(); ++keyIndex)
					{
						FbxAnimCurveKey* currKey = &translationCurve_X->KeyGet(keyIndex);
						
						FbxNode::Pivots aids = skeleton.joints[currentJointIndex]->jointNode->GetPivots();
						//skeleton.joints[currentJointIndex]->jointNode->GetPivots;
						FbxNode::Pivot alba;

						//FIGURE OUT HOW TO GET LOCAL TRANSFORMS FROM FbxAnimEvaluator !!!

						//FbxNode::EPivotSet lol = skeleton.joints[currentJointIndex]->jointNode->GetPivots();
						
						//animEvaluator->GetNodeLocalTransform(skeleton.joints[currentJointIndex]->jointNode, currKey->GetTime(), skeleton.joints[currentJointIndex]->jointNode->GetPivots, false, false);
					}
					
					//Now get the animation curves that determine the rotation and translation of this joint at different times
					//FbxAnimCurve* yolo = skeleton.joints[currentJointIndex]->jointNode->LclTranslation.GetCurve();

				}
				//THIS IS THE EASIER WAY BUT LENDS LESS CONTROL
				
				//"Official Way" to "Evaluate" animationdata. Which means that the right "settings" are 
				//used to get the local or global transformationmatrices for a PyNode (of joint type)
				
				
				
				
			}
			
			
		}
	}
}

//GRAVEYARD
//THIS IS THE HARD WAY THAT GIVES THE USER MORE CONTROL
//In this for-loop I am trying to figure out how animation curves work...
//
//for (unsigned int animLayerCounter = 0; animLayerCounter < numAnimLayers; ++animLayerCounter)
//{
//	FbxAnimLayer* currAnimLayer = currentAnimStack->GetMember<FbxAnimLayer>();
//	//FbxAnimCurve* curveRotation = skeleton.joints[currentJointIndex]->jointNode->LclTranslation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_TRANSFORM, false);
//	//FbxAnimCurve* curveRotation = skeleton.joints[currentJointIndex]->jointNode->LclRotation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_ROTATION, false);
//
//
//	//get the Translation and Rotation XYZ component curves. Also the scaling curve to "normalize" the scale.
//	FbxAnimCurve* translationCurve_X = skeleton.joints[currentJointIndex]->jointNode->LclTranslation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
//	FbxAnimCurve* translationCurve_Y = skeleton.joints[currentJointIndex]->jointNode->LclTranslation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//	FbxAnimCurve* translationCurve_Z = skeleton.joints[currentJointIndex]->jointNode->LclTranslation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
//
//	FbxAnimCurve* rotationCurve_X = skeleton.joints[currentJointIndex]->jointNode->LclRotation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
//	FbxAnimCurve* rotationCurve_Y = skeleton.joints[currentJointIndex]->jointNode->LclRotation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//	FbxAnimCurve* rotationCurve_Z = skeleton.joints[currentJointIndex]->jointNode->LclRotation.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
//
//	FbxAnimCurve* scaleCurve_X = skeleton.joints[currentJointIndex]->jointNode->LclScaling.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
//	FbxAnimCurve* scaleCurve_Y = skeleton.joints[currentJointIndex]->jointNode->LclScaling.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//	FbxAnimCurve* scaleCurve_Z = skeleton.joints[currentJointIndex]->jointNode->LclScaling.GetCurve(currAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
//	//currAnimCurve->
//	//KeyGet, KeyFind, etc are functions belonging to an animationcurve. Keys are "animation graph keys" that you can find in maya. 
//	//The only thing they contain is a time (x-axis in graph), a value (y-axis in graph), and some info about tangents.
//	//The tangents tells us of how the interpolation behaves between keys.
//
//	//Hmm... But the value held by a key might be relevant, if we've a curve for each individual XYZ curve! HMM!
//	//This is the way to go. Oh my god so EZ!
//}

unsigned int FbxDawg::findJointIndexByName(const char* jointName)
{
	try
	{
		for (unsigned int i = 0; i < skeleton.joints.size(); ++i)
		{
			int compareValue = std::strcmp(jointName, skeleton.joints[i]->name);
			if (compareValue == 0)
				return skeleton.joints[i]->parentIndex + 1; //parentIndex + 1 gets the index of this joint.
		}
	}

	catch (const std::exception&)
	{
		printf("Error in FbxDawg::findJointIndexByName(const char* jointName): cannot find matching joint name\n");
	}
}

void FbxDawg::processJointHierarchy(FbxNode * inRootNode)
{
	for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex) {
		FbxNode* currNode = inRootNode->GetChild(childIndex);
		recursiveJointHierarchyTraversal(currNode, 0, -1); 
	}
}

void FbxDawg::recursiveJointHierarchyTraversal(FbxNode * inNode, int currentIndex, int inNodeParentIndex)
{
	//every node will only know about it's direct parent's index. 
	if (inNode->GetNodeAttribute() && inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		sJoint currJoint;
		currJoint.parentIndex = inNodeParentIndex;
		currJoint.name = inNode->GetName();
		this->skeleton.joints.push_back(&currJoint);
	}
	for (int i = 0; i < inNode->GetChildCount(); i++) {
		//currentIndex becomes the "old index". And the size of the joint-hierarchy-list "becomes" the currentIndex instead
		//We process each and every child of this node, we search for children of AttributeType eSkeleton to add to the list of joints.
		recursiveJointHierarchyTraversal(inNode->GetChild(i), skeleton.joints.size(), currentIndex); 
	}
}

void FbxDawg::bsLoader(FbxMesh * mesh)
{
	int bShapeCount = mesh->GetDeformerCount(FbxDeformer::eBlendShape);

	if (bShapeCount > 0)
	{
		printf("model has %d BlendShapes\n", bShapeCount);
		for (int bsIndex = 0; bsIndex < bShapeCount; bsIndex++)
		{
			FbxBlendShape* lBlendShape = (FbxBlendShape*)mesh->GetDeformer(bsIndex, FbxDeformer::eBlendShape);

			int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
			printf("bShape has %d BlendShape Channels\n", lBlendShapeChannelCount);

			for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; lChannelIndex++)
			{
				FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);

				int lShapesCount = lChannel->GetTargetShapeCount();

				for (int lShapeIndex = 0; lShapeIndex < lShapesCount; lShapeIndex++)
				{
					FbxShape * lShape = lChannel->GetTargetShape(lShapeIndex);
					bsVert.push_back(lShape->GetControlPoints());
				}
			}
		}
	}
}

std::vector<Mesh> FbxDawg::GetMeshVec()
{
	return MeshVec;
}

std::vector<material> FbxDawg::GetMaterialVec()
{
	return materialVec;
}




