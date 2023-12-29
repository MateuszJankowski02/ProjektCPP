#pragma once
#ifndef LOCATION_H
#define LOCATION_H

class Location {
public:

    virtual double distance(const Location* location) = 0;

    virtual ~Location() = default;
};

#endif