#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::vector<int>> readMazeFromFile(const char* _path);

int main()
{
	std::vector<std::vector<int>> mazeMap = readMazeFromFile("../src/Labs15and16TerrainFile1.txt");

	system("pause");
	return 0;
}

std::vector<std::vector<int>> readMazeFromFile(const char* _path)
{
	std::ifstream inFile(_path);

	// Extract maze dimensions
	int width = 0, height = 0;
	inFile >> width >> height;
	std::cout << width << " " << height << std::endl; // Debug output

	// Create & resize 2D maze vector
	std::vector<std::vector<int>> data;
	data.resize(height, std::vector<int>(width));

	// Populate 2D maze
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			inFile >> data[h][w];
			std::cout << data[h][w] << " "; // Debug output
		}
		std::cout << std::endl;
	}

	return data;
}