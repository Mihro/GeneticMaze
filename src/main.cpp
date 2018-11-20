#include <fstream>
#include <iostream>

int main()
{
	std::ifstream inFile("../src/Labs15and16TerrainFile1.txt");

	int mazeWidth = 0, mazeHeight = 0;
	inFile >> mazeWidth >> mazeHeight;
	std::cout << mazeWidth << " " << mazeHeight << std::endl;

	system("pause");
	return 0;
}