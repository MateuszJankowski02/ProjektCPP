#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <map>
#include <cmath>
#include <functional>
#include <iterator>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include <conio.h>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <type_traits>
#include "Fitness.h"

using namespace std;

//Tworzy losow¹ trasê na podstawie listy miast.
vector<Fitness::City> createRoute(const vector<Fitness::City>& cityList);

//Tworzy pocz¹tkow¹ populacjê tras.
vector<vector<Fitness::City>> initialPopulation(int popSize, const vector<Fitness::City>& cityList);

//Przypisuje i sortuje trasy wed³ug wartoœci fitnesu.
vector<pair<int, double>> rankRoutes(const vector<vector<Fitness::City>>& population);

//Wybiera trasy do reprodukcji na podstawie wartoœci fitnesu.      
vector<int> selection(const vector<pair<int, double>>& popRanked, int eliteSize);

//Tworzy grupê tras do krzy¿owania.		Wybiera population count najlepszych tras i zapisuje je w wektorze
vector<vector<Fitness::City>> matingPool(const vector<vector<Fitness::City>>& population, const vector<int>& selectionResults);

// Krzy¿uje dwie trasy, tworz¹c potomka.		//krzy¿ówka zwraca jedno dziecko
vector<Fitness::City> breed(const vector<Fitness::City>& parent1, const vector<Fitness::City>& parent2);

//Krzy¿uje populacjê tras.
vector<vector<Fitness::City>> breedPopulation(const vector<vector<Fitness::City>>& matingpool, int eliteSize);

//Modyfikuje trasê w wyniku mutacji. swapuje geny 
vector<Fitness::City> mutate(const vector<Fitness::City>& individual, double mutationRate);

//Mutuje populacjê tras.
vector<vector<Fitness::City>> mutatePopulation(const vector<vector<Fitness::City>>& population, double mutationRate);

//Tworzy kolejn¹ generacjê tras na podstawie aktualnej populacji.
vector<vector<Fitness::City>> nextGeneration(const vector<vector<Fitness::City>>& currentGen, int eliteSize, double mutationRate);

//Implementuje algorytm genetyczny, zwracaj¹c najlepsz¹ trasê.
vector<Fitness::City> geneticAlgorithm(const vector<Fitness::City>& population, int popSize, int eliteSize, double mutationRate, int generations);

vector<Fitness::City> readCitiesFromFile(const string& filename);

#endif
