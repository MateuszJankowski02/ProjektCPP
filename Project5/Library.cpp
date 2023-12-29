#include "library.h"


// create a route from a list of cities
vector<Fitness::City> createRoute(const vector<Fitness::City>& cityList) {
	vector<Fitness::City> route = cityList;
	random_device rd;
	mt19937 generator(rd());
	shuffle(route.begin(), route.end(), generator);
	return route;
}

// initialize a population of cities
vector<vector<Fitness::City>> initialPopulation(int popSize, const vector<Fitness::City>& cityList) {
	vector<vector<Fitness::City>> population;

	for (int i = 0; i < popSize; i++) {
		population.push_back(createRoute(cityList));
	}

	return population;
}


// rank the routes in the population
vector<pair<int, double>> rankRoutes(const vector<vector<Fitness::City>>& population) {
	map<int, double> fitnessResults;

	for (int i = 0; i < population.size(); i++) {
		fitnessResults[i] = Fitness(population[i]).routeFitness();
	}

	vector<pair<int, double>> sortedResults;
	copy(fitnessResults.begin(), fitnessResults.end(), back_inserter(sortedResults));

	sort(sortedResults.begin(), sortedResults.end(),
		[](const pair<int, double>& a, const pair<int, double>& b) {			
			return a.second > b.second;
		});

	return sortedResults;
}


// select the routes that will be used for mating
vector<int> selection(const vector<pair<int, double>>& popRanked, int eliteSize) {
	vector<int> selectionResults;
	vector<double> cumPerc;	
	double fitnessSum = 0.0;

	for (const auto& ranked : popRanked) {
		fitnessSum += ranked.second;
	}

	double cumSum = 0.0;	
	for (const auto& ranked : popRanked) {
		cumSum += ranked.second;
		cumPerc.push_back(100.0 * cumSum / fitnessSum);
	}

	for (int i = 0; i < eliteSize; i++) {
		selectionResults.push_back(popRanked[i].first);
	}

	random_device rd;
	mt19937 generator(rd());
	// Czemu selekcja przebiega w ten sposób?			
	for (int i = 0; i < popRanked.size() - eliteSize; i++) {
		double pick = 100.0 * generate_canonical<double, 10>(generator);	//czym jest pick
		for (int j = 0; j < popRanked.size(); j++) {
			if (pick <= cumPerc[j]) {
				selectionResults.push_back(popRanked[j].first);
				break;
			}
		}
	}


	return selectionResults;
}


// create a mating pool
vector<vector<Fitness::City>> matingPool(const vector<vector<Fitness::City>>& population, const vector<int>& selectionResults) {
	vector<vector<Fitness::City>> matingpool;

	for (int i = 0; i < selectionResults.size(); i++) {
		int index = selectionResults[i];
		matingpool.push_back(population[index]);
	}

	return matingpool;
}


// breed two parents to create a child
vector<Fitness::City> breed(const vector<Fitness::City>& parent1, const vector<Fitness::City>& parent2) {
	vector<Fitness::City> child;
	vector<Fitness::City> childP1;
	vector<Fitness::City> childP2;

	int geneA = static_cast<int>(rand() % parent1.size());
	int geneB = static_cast<int>(rand() % parent1.size());

	int startGene = min(geneA, geneB);
	int endGene = max(geneA, geneB);

	for (int i = startGene; i < endGene; i++) {
		childP1.push_back(parent1[i]);
	}

	for (const auto& item : parent2) {
		if (find(childP1.begin(), childP1.end(), item) == childP1.end()) {
			childP2.push_back(item);
		}
	}

	child.insert(child.end(), childP1.begin(), childP1.end());
	child.insert(child.end(), childP2.begin(), childP2.end());

	return child;
}


// create a new generation
vector<vector<Fitness::City>> breedPopulation(const vector<vector<Fitness::City>>& matingpool, int eliteSize) {
	vector<vector<Fitness::City>> children;
	int length = matingpool.size() - eliteSize;
	vector<vector<Fitness::City>> pool = matingpool;
	random_device rd;
	mt19937 generator(rd());
	shuffle(pool.begin(), pool.end(), generator);

	for (int i = 0; i < eliteSize; i++) {
		children.push_back(matingpool[i]);
	}

	for (int i = 0; i < length; i++) {
		vector<Fitness::City> child = breed(pool[i], pool[matingpool.size() - i - 1]);
		children.push_back(child);
	}

	return children;
}


// mutate a single route
vector<Fitness::City> mutate(const vector<Fitness::City>& individual, double mutationRate) {
	vector<Fitness::City> mutatedIndividual = individual;
	int Method = rand() % 2;


	if (Method == 0) {
		for (int swapped = 0; swapped < individual.size(); swapped++) {
		
				if (rand() < mutationRate) {					//Czemu nie randomowe double by³o by czêœciej tak to ma 1/ INT_MAX szans
					int swapWith = rand() % individual.size();

					Fitness::City city1 = mutatedIndividual[swapped];
					Fitness::City city2 = mutatedIndividual[swapWith];

					mutatedIndividual[swapped] = city2;
					mutatedIndividual[swapWith] = city1;
				}
		}
	}
	else {
		if (rand() < mutationRate) {					//Czemu nie randomowe double by³o by czêœciej tak to ma 1/ INT_MAX szans
			
			int subVectorSize = 1 + rand() % (individual.size() / 2);
			int subVectorStart = rand() % (individual.size() - subVectorSize + 1);
			vector<Fitness::City> subvector(mutatedIndividual.begin() + subVectorStart, mutatedIndividual.begin() + subVectorStart + subVectorSize);
			int swapWith = rand() % (individual.size() - subVectorSize + 1);
			mutatedIndividual.erase(mutatedIndividual.begin() + subVectorStart, mutatedIndividual.begin() + subVectorStart + subVectorSize);
			mutatedIndividual.insert(mutatedIndividual.begin() + swapWith, subvector.begin(), subvector.end());
		
		}
	}

	return mutatedIndividual;
}


// mutate the population
vector<vector<Fitness::City>> mutatePopulation(const vector<vector<Fitness::City>>& population, double mutationRate) {
	vector<vector<Fitness::City>> mutatedPop;

	for (int ind = 0; ind < population.size(); ind++) {
		vector<Fitness::City> mutatedInd = mutate(population[ind], mutationRate);
		mutatedPop.push_back(mutatedInd);
	}

	return mutatedPop;
}


// create the next generation
vector<vector<Fitness::City>> nextGeneration(const vector<vector<Fitness::City>>& currentGen, int eliteSize, double mutationRate) {
	vector<pair<int, double>> popRanked = rankRoutes(currentGen);
	vector<int> selectionResults = selection(popRanked, eliteSize);
	vector<vector<Fitness::City>> matingpool = matingPool(currentGen, selectionResults);
	vector<vector<Fitness::City>> children = breedPopulation(matingpool, eliteSize);
	vector<vector<Fitness::City>> nextGeneration = mutatePopulation(children, mutationRate);
	return nextGeneration;
}


// run the genetic algorithm
vector<Fitness::City> geneticAlgorithm(const vector<Fitness::City>& population, int popSize, int eliteSize, double mutationRate, int generations) {
	vector<vector<Fitness::City>> pop = initialPopulation(popSize, population);
	double previousDistance = 1 / rankRoutes(pop)[0].second;
	double currentDistance{};
	cout << "Initial route: " << previousDistance << endl;				
	cout << "Press any key to initialize the algorithm...\n";
	_getch();

	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < generations; i++) {
		pop = nextGeneration(pop, eliteSize, mutationRate);
		currentDistance = 1 / rankRoutes(pop)[0].second;
		if (currentDistance < previousDistance) {
			cout << "\t" << currentDistance << endl;
			previousDistance = currentDistance;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> duration = end - start;
	cout << "Duration: " << duration.count() << "s" << endl;
	cout << "Final shortest route found: " << currentDistance << endl;
	int bestRouteIndex = rankRoutes(pop)[0].first;
	vector<Fitness::City> bestRoute = pop[bestRouteIndex];
	return bestRoute;
}


// read cities from a file
vector<Fitness::City> readCitiesFromFile(const string& filename) {
	vector<Fitness::City> cityList;
	ifstream inputFile("cities/" + filename + ".txt");
	if (!inputFile.is_open()) {
		throw runtime_error("Couldn't open the file: " + filename);
		_getch();
	}

	string line;
	int lineNumber = 1;
	while (getline(inputFile, line)) {
		istringstream iss(line);
		int x, y;
		if (!(iss >> x >> y)) {
			throw runtime_error("Incorrect data in row number " + to_string(lineNumber) + ": " + line);
			_getch();
		}
		cityList.push_back({ x, y });
		lineNumber++;
	}

	inputFile.close();
	return cityList;
}