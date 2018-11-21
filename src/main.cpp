#include <fstream>
#include <iostream>
#include <vector>

int main()
{
	std::ifstream inFile("../src/Labs15and16TerrainFile1.txt");

	int mazeWidth = 0, mazeHeight = 0;
	inFile >> mazeWidth >> mazeHeight;
	std::cout << mazeWidth << " " << mazeHeight << std::endl;

	std::vector<std::vector<int>> mazeData;
	mazeData.resize(mazeHeight, std::vector<int>(mazeWidth));
	for (int h = 0; h < mazeHeight; h++)
	{
		for (int w = 0; w < mazeWidth; w++)
		{
			inFile >> mazeData[h][w];
			std::cout << mazeData[h][w] << " ";
		}
		std::cout << std::endl;
	}
	
	system("pause");
	return 0;
}