#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <ctime>

#define POPULATION_SIZE 100

struct Maze
{
	int width;
	int height;
	std::vector<int> start;
	std::vector<int> finish;
	std::vector<std::vector<int>> terrain;

	Maze()
	{
		start.resize(2);
		finish.resize(2);
	}
	int getCell(int _x, int _y)
	{
		return terrain[_x][_y];
	}
};
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
struct Population
{
	std::vector<Chromosome> list;
	float totalFitness;

	Population() : 
		totalFitness(0.0f) 
	{}
};

Maze readTerrainFromFile(const char* _path);
std::vector<Chromosome> generatePopulation(int _size, int _chromeLen);
bool checkPopulationFitness(Maze& _m, Population& _population);
bool traverseMaze(Maze& _m, Population& _population, Chromosome& _chrome);
bool checkLegalMove(Maze& _m, std::vector<int>& _nextPos);
float calcChromeFitness(Maze& _m, std::vector<int> _finalPos);

std::default_random_engine randomGenerator( time(NULL) );
std::uniform_real_distribution<double> uniformDistribution(0.0, 1.0);

int main()
{
	Maze maze = readTerrainFromFile("../src/Labs15and16TerrainFile1.txt");
	Population population;
	population.list = generatePopulation(POPULATION_SIZE, 16);
	bool success = false;
	success = checkPopulationFitness(maze, population);
	while (!success)
	{
		// Do
			// Select mates
			// Crossover
			// Mutation
			// Add offspring to new population
		// While new population < POPULATION_SIZE
		// Swap old population for new
		// Calculate new fitness
	}
	std::cout << "Success!" << std::endl;
	
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

	// Resize 2D maze terrain
	maze.terrain.resize(maze.width, std::vector<int>(maze.height));

	// Populate 2D maze
	for (int h = 0; h < maze.height; h++)
	{
		for (int w = 0; w < maze.width; w++)
		{
			int tile;
			inFile >> tile;
			maze.terrain[w][h] = tile;

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

	std::vector<Chromosome> population;

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
		population.push_back(member);
	}

	return population;
}

bool checkPopulationFitness(Maze& _m, Population& _population)
{
	bool traversalSuccess = false;
	std::cout << "\nInstructions: " << std::endl;
	for (int i = 0; i < _population.list.size(); i++)
	{
		traversalSuccess = traverseMaze(_m, _population, _population.list.at(i));
		std::cout << std::endl;

		if (traversalSuccess)
		{
			std::cout << "Total Fitness: " << _population.totalFitness << std::endl;
			return true;
		}
	}
	return false;
}

bool traverseMaze(Maze& _m, Population& _population, Chromosome& _chrome)
{
	std::cout << "Chromosome ID: " << _chrome.id << std::endl;
	
	std::cout << "Gene: ";
	
	std::vector<int> currentPos = _m.start;

	for (int i = 0; i < _chrome.string.length(); i+=2)
	{
		std::string gene = _chrome.string.substr(i, 2);
		std::cout << gene << " ";

		std::vector<int> nextPos = currentPos;

		if (gene == "00") // Up
		{
			nextPos[1] -= 1;
		}
		else if (gene == "01") // Right
		{
			nextPos[0] += 1;
		}
		else if (gene == "10") // Down
		{
			nextPos[1] += 1;
		}
		else if (gene == "11") // Left
		{
			nextPos[0] -= 1;
		}
		if (checkLegalMove(_m, nextPos))
		{
			currentPos = nextPos;
		}
	}
	std::cout << std::endl;

	float chromeFitness = calcChromeFitness(_m, currentPos);
	_chrome.fitness = chromeFitness;
	_population.totalFitness += chromeFitness;

	std::cout << "Start (" << _m.start[0] << "," << _m.start[1] << ")";
	std::cout << "\tEnd (" << currentPos[0] << "," << currentPos[1] << ")";
	std::cout << "\tFitness: " << _chrome.fitness << std::endl;

	if (_chrome.fitness == 1.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool checkLegalMove(Maze& _m, std::vector<int>& _nextPos)
{
	if (!(_nextPos[0] >= 0 && _nextPos[0] < _m.width) || // If move-to tile is not in X bounds
		!(_nextPos[1] >= 0 && _nextPos[1] < _m.height))	 // If move-to tile is not in Y bounds
	{
		return false;
	}
	if (_m.getCell(_nextPos[0], _nextPos[1]) == 1)		 // If move-to tile is a wall
	{
		return false;
	}
	return true;
}

float calcChromeFitness(Maze& _m, std::vector<int> _finalPos)
{
	int dx = abs(_finalPos[0] - _m.finish[0]);
	int dy = abs(_finalPos[1] - _m.finish[1]);
	float fitness = (float) 1 / (dx + dy + 1);
	return fitness;
}
