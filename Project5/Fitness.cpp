#include "Fitness.h"

// document all the functions and classes in this file see examples in library.cpp or main.cpp


// constructor of the Fitness class
Fitness::Fitness(const std::vector<City>& route) {
    this->route = route;
    this->distance = 0.0;
    this->fitness = 0.0;
}

// method that calculates the distance of the route
double Fitness::routeDistance() {
    if (distance == 0.0) {
        double pathDistance = 0.0;
        
        for (size_t i = 0; i < route.size(); i++) {
            //Polimorfizm
            Location* fromCity = new City(route[i].getX(), route[i].getY());
            Location* toCity = (i + 1 < route.size()) ? new City(route[i + 1].getX(), route[i + 1].getY()) 
                : new City(route[0].getX(), route[0].getY());
            //City& fromCity = route[i];
            //City& toCity = (i + 1 < route.size()) ? route[i + 1] : route[0];           
            pathDistance += fromCity->distance(toCity);     //Wywo³anie polimorficzne 
        }
        distance = pathDistance;
    }
    return distance;
}

// method that calculates the fitness of the route
double Fitness::routeFitness() {
    if (fitness == 0.0) {
        fitness = 1.0 / routeDistance();
    }
    return fitness;
}

// constructor of the City class
Fitness::City::City(int x, int y) : x(x), y(y) {}

// operator that checks if two cities are equal
bool Fitness::City::operator==(const City& other) const {
    return (x == other.x) && (y == other.y);
}

// method that calculates the distance between two cities
double Fitness::City::distance(const Location* location) {
    const City* city = dynamic_cast<const City*>(location);
    int xDis = abs(this->x - city->x);
    int yDis = abs(this->y - city->y);
    double distance = sqrt(pow(xDis, 2) + pow(yDis, 2));
    return distance;
}

// operator that prints the city
std::ostream& operator<<(std::ostream& os, const Fitness::City& city) {
    os << "(" << city.x << "," << city.y << ")";
    return os;
}

// method that returns the x coordinate of the city
int Fitness::City::getX(){
    return this->x;
}

// method that returns the y coordinate of the city
int Fitness::City::getY(){
    return this->y;
}