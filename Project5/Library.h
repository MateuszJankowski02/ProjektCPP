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

//Tworzy losow� tras� na podstawie listy miast.
vector<Fitness::City> createRoute(const vector<Fitness::City>& cityList);

//Tworzy pocz�tkow� populacj� tras.
vector<vector<Fitness::City>> initialPopulation(int popSize, const vector<Fitness::City>& cityList);

//Przypisuje i sortuje trasy wed�ug warto�ci fitnesu.
vector<pair<int, double>> rankRoutes(const vector<vector<Fitness::City>>& population);

//Wybiera trasy do reprodukcji na podstawie warto�ci fitnesu.      
vector<int> selection(const vector<pair<int, double>>& popRanked, int eliteSize);

//Tworzy grup� tras do krzy�owania.		Wybiera population count najlepszych tras i zapisuje je w wektorze
vector<vector<Fitness::City>> matingPool(const vector<vector<Fitness::City>>& population, const vector<int>& selectionResults);

// Krzy�uje dwie trasy, tworz�c potomka.		//krzy��wka zwraca jedno dziecko
vector<Fitness::City> breed(const vector<Fitness::City>& parent1, const vector<Fitness::City>& parent2);

//Krzy�uje populacj� tras.
vector<vector<Fitness::City>> breedPopulation(const vector<vector<Fitness::City>>& matingpool, int eliteSize);

//Modyfikuje tras� w wyniku mutacji. swapuje geny 
vector<Fitness::City> mutate(const vector<Fitness::City>& individual, double mutationRate);

//Mutuje populacj� tras.
vector<vector<Fitness::City>> mutatePopulation(const vector<vector<Fitness::City>>& population, double mutationRate);

//Tworzy kolejn� generacj� tras na podstawie aktualnej populacji.
vector<vector<Fitness::City>> nextGeneration(const vector<vector<Fitness::City>>& currentGen, int eliteSize, double mutationRate);

//Implementuje algorytm genetyczny, zwracaj�c najlepsz� tras�.
vector<Fitness::City> geneticAlgorithm(const vector<Fitness::City>& population, int popSize, int eliteSize, double mutationRate, int generations);

vector<Fitness::City> readCitiesFromFile(const string& filename);

#endif
