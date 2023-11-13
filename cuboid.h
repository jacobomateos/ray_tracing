#ifndef CUBOID_H
#define CUBOID_H

#include "hittable.h"
#include "vec3.h"
#include <algorithm> 

class cuboid : public hittable {
    public:
    cuboid(point3 _center, double _height, double _width, double _depth, double _radius, shared_ptr<material> _material) 
        : center(_center), height(_height), width(_width), depth(_depth), radius(_radius), mat(_material) {}
    
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Define the min and max points of the cuboid.
        vec3 min = center - vec3(height/2, width/2, depth/2);
        vec3 max = center + vec3(height/2, width/2, depth/2);

        float tmin = (min.x() - r.origin().x()) / r.direction().x();
        float tmax = (max.x() - r.origin().x()) / r.direction().x();
        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.y() - r.origin().y()) / r.direction().y();
        float tymax = (max.y() - r.origin().y()) / r.direction().y();
        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) return false;

        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        float tzmin = (min.z() - r.origin().z()) / r.direction().z();
        float tzmax = (max.z() - r.origin().z()) / r.direction().z();
        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax)) return false;

        if (tzmin > tmin) tmin = tzmin;
        if (tzmax < tmax) tmax = tzmax;

        // Check if the nearest valid intersection is within the ray interval
        if (!ray_t.surrounds(tmin)) return false;

        rec.t = tmin;
        rec.p = r.at(rec.t);

        // Compute the normal. We need to find out which face was hit.
        vec3 outward_normal = get_normal(rec.p, min, max);
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    vec3 get_normal(const vec3& p, const vec3& min, const vec3& max) const {
        vec3 normal;
        if (p.x() <= min.x() || p.x() >= max.x()) normal = vec3(1, 0, 0);
        else if (p.y() <= min.y() || p.y() >= max.y()) normal = vec3(0, 1, 0);
        else normal = vec3(0, 0, 1);
        return normal;
    }

    private:
    point3 center;
    double height, width, depth, radius;
    shared_ptr<material> mat;
};


#endif