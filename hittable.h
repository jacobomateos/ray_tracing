#ifndef HITTABLE_H
#define HITTABLE_N

#include "ray.h"

class hit_record{
    public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal){
        // Sets the hit record normal vector.
        // Note: the parameter 'outward normal' is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal; 
    }
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
