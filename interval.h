#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class interval{
    public:
    double min, max; 

    interval() : min(+infinity), max(-infinity) {} // Default interval is empty.
    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const {
        return min <= x && max >= x;
    }

    bool surrounds(double x) const {
        return min < x && max > x;
    }

    static const interval empty, universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif