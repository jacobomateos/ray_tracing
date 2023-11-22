#ifndef QUAD_H
#define QUAD_H

#include "rtweekend.h"
#include "hittable.h"

class quat : public hittable {
    public:
    quat( const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> m) : Q(_Q), u(_u), v(_v), mat(m) {
        set_bounding_box();
    }

    virtual void set_bounding_box() {
        bbox = aabb(Q,Q+u+v).pad();
    }

    aabb bounding_box() const override { return bbox; }

    bool hit (const ray& r, interval ray_t, hit_record& rec) const override {
        return false; // To implement
    }

    private:
    point3 Q; //Quadrilateral point of reference
    vec3 u, v; // Vectors setting sides 
    shared_ptr<material> mat;
    aabb bbox;
};

#endif