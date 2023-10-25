#ifndef HITTABLE_H
#define HITTABLE_N

#include "ray.h"

class hit_record{
    public:
    point3 p;
    vec3 normal;
    double t;
};

class hittable {
    public:

    // Virtual classes used to generate specific implementation in derived classes.
    // The ~hittable() function is the destructor function for a class. 
    // Destructors are automatically called when an object of the class is being destroyed.
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif
