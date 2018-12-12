#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <ctime>

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

Maze readTerrainFromFile(const char* _path);
std::vector<Chromosome> generatePopulation(int _size, int _chromeLen);
void populationFitness(std::vector<Chromosome>& _pop, Maze& _m);
bool traverseMaze(Chromosome& _chrome, Maze& _m);
bool checkLegalMove(std::vector<int>& _nextPos, Maze& _m);
float calcChromeFitness(std::vector<int> _finalPos, Maze& _m);

std::default_random_engine randomGenerator( time(NULL) );
std::uniform_real_distribution<double> uniformDistribution(0.0, 1.0);

int main()
{
	Maze maze = readTerrainFromFile("../src/Labs15and16TerrainFile1.txt");
	std::vector<Chromosome> population = generatePopulation(100, 16);
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

void populationFitness(std::vector<Chromosome>& _population, Maze& _m)
{
	bool traversalSuccess = false;
	std::cout << "\nInstructions: " << std::endl;
	for (int i = 0; i < _population.size(); i++)
	{
		traversalSuccess = traverseMaze(_population.at(i), _m);
		std::cout << std::endl;

		if (traversalSuccess)
		{
			std::cout << "Success!" << std::endl;
			break;
		}
	}
}

bool traverseMaze(Chromosome& _chrome, Maze& _m)
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
		if (checkLegalMove(nextPos, _m))
		{
			currentPos = nextPos;
		}
	}
	std::cout << std::endl;

	_chrome.fitness = calcChromeFitness(currentPos, _m);

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

bool checkLegalMove(std::vector<int>& _nextPos, Maze& _m)
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

float calcChromeFitness(std::vector<int> _finalPos, Maze& _m)
{
	int dx = abs(_finalPos[0] - _m.finish[0]);
	int dy = abs(_finalPos[1] - _m.finish[1]);
	float fitness = (float) 1 / (dx + dy + 1);
	return fitness;
}