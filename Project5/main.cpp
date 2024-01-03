#include "Library.h"
#include "Fitness.h"


using namespace std;

template<typename T>
// function that receives input from the user and checks if it's correct
void getInput(T& variable, T defaultValue) {
	if (is_same<T, int>::value || is_same<T, double>::value)
		
		while (true) {
			
			if (cin.peek() == '\n') {
				variable = defaultValue;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			else if (!(cin >> variable)) {
				cout << "Incorrect input! Try again...\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			else {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
}

// main function
int main() {
	// main loop
	while (true) {
		// variables
		vector<Fitness::City> cityList;
		int popSize, eliteSize, generations, numberOfCities{}, inputMethod, inputX{}, inputY{};
		string fileName;
		bool validInput = false;
		double mutationRate{};

		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<mt19937::result_type> dist(100, 800);

		cout << "Traveling Salesman Problem\n\n";

		cout << "Enter the following data:\n";
		
		// get input about the population size, elite size, number of generations and mutation rate
		cout << "Population count [default = 100]:\t";
		getInput(popSize, 100);
		cout << "Elite population count [default = 20]:\t";
		getInput(eliteSize, 20);
		cout << "Number of generations [default = 500]:\t";
		getInput(generations, 500);
		cout << "Mutation frequency [default = 0.1]:\t";
		getInput(mutationRate, 0.1);

		// get input about the way of generating cities
		cout << "\nChoose your preferred way of generating cities:\n";
		cout << "1.Randomized points\n2.From the terminal\n3.From a file\n";
		cin >> inputMethod;
		// get input about the number of cities if the user chose the first or second option
		if (inputMethod == 1 || inputMethod == 2) {
			cout << "Cities count [default = 100]:\t";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getInput<int>(numberOfCities, 100);
		}
		// loop that checks if the input is correct
		while (!validInput)
			// switch that chooses the way of generating cities
			switch (inputMethod) {
			// generate cities randomly
			case 1:

				for (int i = 0; i < numberOfCities; i++) {
					int x = static_cast<int>(dist(generator));
					int y = static_cast<int>(dist(generator));
					cityList.push_back(Fitness::City(x, y));
				}
				validInput = !validInput;
				break;
			// get input about the coordinates of cities from the user
			case 2:
				cout << "Enter the coordinates for " << numberOfCities << " cities in the following format: <X> <Y>.\n";
				for (int i = 0; i < numberOfCities; i++) {
					cout << "Enter the coordinates of citi number " << i + 1 << ":\t";
					cin >> inputX >> inputY;
					if (cin) {
						int x = static_cast<int>(inputX);
						int y = static_cast<int>(inputY);
						cityList.push_back(Fitness::City(x, y));
					}
					else {
						cout << "Incorrect input! Try again...\n";
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						i--;
					}
				}
				validInput = !validInput;
				break;
            // get input about the name of the file with cities	
			case 3:
				cout << "Enter the name of the file without '.txt' extension: ";
				cin >> fileName;
				try {
					cityList = readCitiesFromFile(fileName);
				}
				catch (const exception& e) {
					cerr << "Error: " << e.what() << endl;
					_getch();
					return 1;
				}
				validInput = !validInput;
				break;
			// if the input is incorrect, clear the input buffer and ask the user to enter the input again
			default:
				cout << "Incorrect choice. Try again...\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin >> inputMethod;
				break;
			}

		// run the genetic algorithm
		vector<Fitness::City> bestRoute = geneticAlgorithm(cityList, popSize, eliteSize, mutationRate, generations);

		// print the results
		cout << "The shortest found route goes through the following cities:\n";
		for (int i = 0; i < bestRoute.size(); i++) {
			Location* fromCity = new Fitness::City(bestRoute[i].getX(), bestRoute[i].getY());
			Location* toCity = (i + 1 < bestRoute.size()) ? new Fitness::City(bestRoute[i + 1].getX(), bestRoute[i + 1].getY())
				: new Fitness::City(bestRoute[0].getX(), bestRoute[0].getY());
			cout << *dynamic_cast<Fitness::City*>(fromCity) << " - " << (fromCity->distance(toCity)) << " km - ";
			if (i % 2 == 1)
				cout << endl;
		}
		cout << endl;

		//write a results to a file
		ofstream outputFile("./cities/bestRouteCities.txt");
		if (outputFile.is_open()) {
			for (int i = 0; i < bestRoute.size() - 1; i++) {
				outputFile << bestRoute[i].getX() << " " << bestRoute[i].getY() << "\n";
			}
			outputFile << bestRoute[bestRoute.size() - 1].getX() << " " << bestRoute[bestRoute.size() - 1].getY();
			outputFile.close();
			cout << "Results written to bestRouteCities.txt" << endl;
		}
		else {
			cout << "Couldn't open the file" << endl;
		}

		cout << "Press ESC key to exit the program ...";
		if (_getch() == 27)
		{
			exit(0);
		}

		system("cls");
		
	}
	return 0;
}
