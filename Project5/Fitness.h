#pragma once
#ifndef FITNESS_H
#define FITNESS_H

#include <iostream>
#include <vector>
#include <cmath>
#include "Location.h"


class Fitness {
public:

    class City : public Location {
    private:
        int x;
        int y;

    public:
        City(int x, int y);

        bool operator==(const City& other) const;

        double distance(const Location* location) override;

        friend std::ostream& operator<<(std::ostream& os, const City& city);

        int getX();

        int getY();
    };

private:
    std::vector<City> route;
    double distance;
    double fitness;

public:
    Fitness(const std::vector<City>& route);

    double routeDistance();

    double routeFitness();
};

#endif