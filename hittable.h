#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"

class material;

class hit_record{
    public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    double u, v;
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
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;\
    virtual aabb bounding_box() const = 0;
};

#endif