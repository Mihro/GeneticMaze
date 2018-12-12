#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <ctime>

#define POPULATION_SIZE 10
#define CROSSOVER_RATE 0.7f
#define MUTATION_RATE 0.001f

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

void geneticAlgorithm(Maze& _m);
std::vector<Chromosome> generatePopulation(int _size, int _chromeLen);
bool checkPopulationFitness(Maze& _m, Population& _population);
bool traverseMaze(Maze& _m, Population& _population, Chromosome& _chrome);
bool checkLegalMove(Maze& _m, std::vector<int>& _nextPos);
float calcChromeFitness(Maze& _m, std::vector<int>& _finalPos);
std::vector<Chromosome> selectChromeMates(Population& _population);
Chromosome selectSingleWeightedChrome(Population& _population);
void crossoverChromes(std::vector<Chromosome>& _mates);
void mutateChromes(std::vector<Chromosome>& _pair);
void mutateChromeGene(char& _gene);
void appendNewPopulationChromes(Population& _newPopulation, std::vector<Chromosome>& _chromes, int& _newGenId);

std::default_random_engine randomGenerator( time(NULL) );
std::uniform_real_distribution<double> uniformRealDistribution(0.0, 1.0);

int main()
{
	Maze maze = readTerrainFromFile("../src/Labs15and16TerrainFile2.txt");

	geneticAlgorithm(maze);
	
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

void geneticAlgorithm(Maze& _m)
{
	Population population = {};
	population.list = generatePopulation(POPULATION_SIZE, 16);
	Population newPopulation = {};

	int generationCount = 1;

	bool success = false;
	std::cout << "\nCurrent generation: " << generationCount << std::endl;
	success = checkPopulationFitness(_m, population);
	while (!success)
	{
		std::cout << "\nNew Generation" << std::endl;

		int newGenChromeId = 0;
		// Do
		do
		{
			// Select mates
			std::vector<Chromosome> chromePair = selectChromeMates(population);
			// Crossover
			crossoverChromes(chromePair);
			// Mutation
			mutateChromes(chromePair);
			// Add offspring to new population
			appendNewPopulationChromes(newPopulation, chromePair, newGenChromeId);
		}
		// While new population < POPULATION_SIZE
		while (newPopulation.list.size() < POPULATION_SIZE);
		// Swap old population for new
		population = newPopulation;
		newPopulation = {};
		generationCount++;
		// Calculate new fitness
		std::cout << "\nCurrent generation: " << generationCount << std::endl;
		success = checkPopulationFitness(_m, population);
	}
	std::cout << "\nCurrent generation: " << generationCount << std::endl;
	std::cout << "Success!" << std::endl;

}

std::vector<Chromosome> generatePopulation(int _size, int _chromeLen)
{
	std::cout << "\nInitial Generation" << std::endl;
	std::cout << "Population size: " << _size << "\tChromosome Length: " << _chromeLen << std::endl;

	std::vector<Chromosome> population;

	for (int n = 0; n < _size; n++)
	{
		Chromosome member;
		member.id = n + 1;

		for (int length = 0; length < _chromeLen; length++)
		{
			double r = uniformRealDistribution(randomGenerator);
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
			return true;
		}
	}
	std::cout << "Total Fitness: " << _population.totalFitness << std::endl;
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

float calcChromeFitness(Maze& _m, std::vector<int>& _finalPos)
{
	int dx = abs(_finalPos[0] - _m.finish[0]);
	int dy = abs(_finalPos[1] - _m.finish[1]);
	float fitness = (float) 1 / (dx + dy + 1);
	return fitness;
}

std::vector<Chromosome> selectChromeMates(Population& _population)
{
	std::vector<Chromosome> pair;

	Chromosome chrome1 = selectSingleWeightedChrome(_population);
	pair.push_back(chrome1);
	Chromosome chrome2 = selectSingleWeightedChrome(_population);
	pair.push_back(chrome2);

	std::cout << "\nParents:" << std::endl;
	std::cout << "ID: " << chrome1.id << "\tString: " << chrome1.string << "\tFitness: " << chrome1.fitness << std::endl;
	std::cout << "ID: " << chrome2.id << "\tString: " << chrome2.string << "\tFitness: " << chrome2.fitness << std::endl;

	return pair;
}

Chromosome selectSingleWeightedChrome(Population& _population)
{
	float random = uniformRealDistribution(randomGenerator) * _population.totalFitness;

	for (int i = 0; i < _population.list.size(); i++)
	{
		if (random < _population.list[i].fitness)
		{
			return _population.list[i];
		}
		random -= _population.list[i].fitness;
	}
}

void crossoverChromes(std::vector<Chromosome>& _mates)
{
	float crossover = uniformRealDistribution(randomGenerator);
	if (crossover <= CROSSOVER_RATE)
	{
		int chromeLength = _mates[0].string.length();

		std::uniform_int_distribution<int> uniformIntDistribution(1, chromeLength - 1);
		int crossoverPoint = uniformIntDistribution(randomGenerator);
		int size = chromeLength - crossoverPoint;

		std::string& chrome1String = _mates[0].string;
		std::string& chrome2String = _mates[1].string;

		std::string swapPart1 = chrome1String.substr(crossoverPoint, size);
		std::string swapPart2 = chrome2String.substr(crossoverPoint, size);
		chrome1String.replace(crossoverPoint, size, swapPart2);
		chrome2String.replace(crossoverPoint, size, swapPart1);

		std::cout << "Crossover point: " << crossoverPoint << std::endl;
		std::cout << "Child 1 string: " << chrome1String << std::endl;
		std::cout << "Child 2 string: " << chrome2String << std::endl;
	}
	else
	{
		std::cout << "No crossover" << std::endl;
	}
}

void mutateChromes(std::vector<Chromosome>& _pair)
{
	for (int i = 0; i < _pair[0].string.length(); i++)
	{
		mutateChromeGene(_pair[0].string[i]);
		mutateChromeGene(_pair[1].string[i]);
	}
	std::cout << "After mutation:" << std::endl;
	std::cout << "Child 1 string: " << _pair[0].string << std::endl;
	std::cout << "Child 2 string: " << _pair[1].string << std::endl;
}

void mutateChromeGene(char& _gene)
{
	float mutation = uniformRealDistribution(randomGenerator);
	if (mutation <= MUTATION_RATE)
	{
		if (_gene == '0')
		{
			_gene = '1';
		}
		else if (_gene == '1')
		{
			_gene = '0';
		}
	}
}

void appendNewPopulationChromes(Population& _newPopulation, std::vector<Chromosome>& _chromes, int& _newGenId)
{
	_chromes[0].id = _newGenId++;
	_chromes[1].id = _newGenId++;
	_newPopulation.list.push_back(_chromes[0]);
	_newPopulation.list.push_back(_chromes[1]);
}