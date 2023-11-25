#ifndef QUAD_H
#define QUAD_H

#include "rtweekend.h"
#include "hittable.h"

class quad : public hittable {
    public:
    quad( const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> m) : Q(_Q), u(_u), v(_v), mat(m) {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
        set_bounding_box();
    }

    virtual void set_bounding_box() {
        bbox = aabb(Q,Q+u+v).pad();
    }

    aabb bounding_box() const override { return bbox; }

    bool hit (const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane
        if(fabs(denom) < 1e-8) 
            return false;
        
        // Return false if the hit point parameter t is outside the ray interval
        auto t = (D - dot(normal,r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        auto intersection = r.at(t);

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);
        
        return true;
    }

    private:
    point3 Q; //Quadrilateral point of reference
    vec3 u, v; // Vectors setting sides 
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D; // Ax + By + Cz = D. 
    vec3 w;
};

#endif