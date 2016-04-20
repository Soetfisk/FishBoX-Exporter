// FishBoX-Exporter.cpp : Defines the entry point for the console application.
//


#include "FishBoX.h"
#include <iostream>

void run()
{
	_finddata_t data;
	std::vector<std::string> filenames;

	printf("####### - WELCOME TO THE FISHBOX - ########\n");

	int first = _findfirst("*.fbx", &data);
	int next = 0;
	while (next != -1)
	{
		std::string temp;
		temp = data.name;
		filenames.push_back(temp.c_str());

		next = _findnext(first, &data);
	}

	for (int i = 0; i < filenames.size(); i++)
	{
		printf("\n\nProcessing file: ");
		printf("%s \n", filenames.at(i).data());

		FishBoX FISHBOX;
		FISHBOX.initialize(filenames.at(i).data());
	}

	printf("\n\n\nDONE!\nHere be dragons... fishes i mean");

	getchar();
	getchar();
}



int main()
{
	run();
	

	FishBoX fishbox;

	

    return 0;
}

