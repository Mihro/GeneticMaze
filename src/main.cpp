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
struct Maze
{
	int width;
	int height;
	int start[2] = {};
	int finish[2] = {};
	std::vector<std::vector<int>> terrain;
};

Maze readTerrainFromFile(const char* _path);
std::vector<Chromosome> generatePopulation(int _size, int _chromeLen);
void populationFitness(std::vector<Chromosome>& _pop, Maze& _m);
void chromosomeFitness(Chromosome& _chrome, Maze& _m);

std::default_random_engine randomGenerator;
std::uniform_real_distribution<double> uniformDistribution(0.0, 1.0);

int main()
{
	Maze maze = readTerrainFromFile("../src/Maze1.txt");
	std::vector<Chromosome> population = generatePopulation(10, 16);
	populationFitness(population, maze);

	system("pause");
	return 0;
}

Maze readTerrainFromFile(const char* _path)
{
	std::ifstream inFile(_path);

	Maze maze = {};

	// Extract maze dimensions
	inFile >> maze.width >> maze.height;
	std::cout << maze.width << " " << maze.height << std::endl; // Debug output

	// Create & resize 2D maze vector
	std::vector<std::vector<int>> data;
	data.resize(maze.height, std::vector<int>(maze.width));

	// Populate 2D maze
	for (int h = 0; h < maze.height; h++)
	{
		for (int w = 0; w < maze.width; w++)
		{
			int tile;
			inFile >> tile;
			data[h][w] = tile;

			if (tile == 2)
			{
				maze.start[0] = w;
				maze.start[1] = h;
			}
			else if (tile == 3)
			{
				maze.finish[0] = w;
				maze.finish[1] = h;
			}
			std::cout << tile << " "; // Debug output
		}
		std::cout << std::endl;
	}

	return maze;
}

std::vector<Chromosome> generatePopulation(int _size, int _chromeLen)
{
	std::cout << "\nPopulation size: " << _size << "\tChromosome Length: " << _chromeLen << std::endl;

	std::vector<Chromosome> pop;

	for (int n = 0; n < _size; n++)
	{
		Chromosome member;
		member.id = n + 1;

		for (int length = 0; length < _chromeLen; length++)
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

		std::cout << member.id << ":\t" << member.string << std::endl;
		pop.push_back(member);
	}

	return pop;
}

void populationFitness(std::vector<Chromosome>& _pop, Maze& _m)
{
	std::cout << "\nInstructions: " << std::endl;
	for (int i = 0; i < _pop.size(); i++)
	{
		chromosomeFitness(_pop.at(i), _m);
		std::cout << std::endl;
	}
}

void chromosomeFitness(Chromosome& _chrome, Maze& _m)
{
	int currentPos[2] = {_m.start[0], _m.start[1]};

	std::cout << _chrome.id << ":\t";
	for (int i = 0; i < _chrome.string.length(); i+=2)
	{
		std::string gene = _chrome.string.substr(i, 2);
		std::cout << gene << " ";
	}
}