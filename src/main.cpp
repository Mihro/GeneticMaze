#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::vector<int>> readTerrainFromFile(const char* _path);

int main()
{
	std::vector<std::vector<int>> terrain = readTerrainFromFile("../src/Maze1.txt");

	system("pause");
	return 0;
}

std::vector<std::vector<int>> readTerrainFromFile(const char* _path)
{
	std::ifstream inFile(_path);

	// Extract maze dimensions
	int sizeX = 0, sizeY = 0;
	inFile >> sizeX >> sizeY;
	std::cout << sizeX << " " << sizeY << std::endl; // Debug output

	// Create & resize 2D maze vector
	std::vector<std::vector<int>> data;
	data.resize(sizeY, std::vector<int>(sizeX));

	// Populate 2D maze
	for (int h = 0; h < sizeY; h++)
	{
		for (int w = 0; w < sizeX; w++)
		{
			inFile >> data[h][w];
			std::cout << data[h][w] << " "; // Debug output
		}
		std::cout << std::endl;
	}

	return data;
}