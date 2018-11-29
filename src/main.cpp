#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <string>

struct Chromosome {
	int id;
	std::string string;
	float fitness;

	Chromosome() : 
		id(NULL),
		string(""), 
		fitness(0.0f) 
	{}
	Chromosome(int _i, std::string _s, float _f) :
		id(_i),
		string(_s),
		fitness(_f)
	{}
};

std::vector<std::vector<int>> readTerrainFromFile(const char* _path);
std::vector<Chromosome> generatePopulation(int _size, int _chromosomeLength);

std::default_random_engine randomGenerator;
std::uniform_real_distribution<double> uniformDistribution(0.0, 1.0);

int main()
{
	std::vector<std::vector<int>> terrain = readTerrainFromFile("../src/Maze1.txt");
	std::vector<Chromosome> population = generatePopulation(50, 16);

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

std::vector<Chromosome> generatePopulation(int _size, int _chromosomeLength)
{
	std::cout << "\nPopulation size: " << _size << std::endl;

	std::vector<Chromosome> pop;

	for (int n = 0; n < _size; n++)
	{
		Chromosome member;
		member.id = n + 1;

		for (int length = 0; length < _chromosomeLength; length++)
		{
			double r = uniformDistribution(randomGenerator);
			if (r <= 0.5)
			{
				member.string.append("0");
			}
			else
			{
				member.string.append("1");
			}
		}

		std::cout << member.string << std::endl;
		pop.push_back(member);
	}

	return pop;
}