#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
    public:
    interval x, y, z;
    aabb () {} // Default AABB is empty, like intervals by default.
    aabb(const interval& ix, const interval& iy, const interval& iz) 
        : x(ix), y(iy), z(iz) {}
    
    aabb(const point3& a, const point3& b){
        // Treat the two points as extrems of the bounding box.
        x = interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
        y = interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
        z = interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
    }

    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    const interval& axis(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto invert_dir = 1/r.direction()[a];
            auto orig = r.origin()[a];

            auto t0 = (axis(a).min - orig) * invert_dir;
            auto t1 = (axis(a).max - orig) * invert_dir;

            // If invert_dir is negative, the ray direction along this axis is negative
            //  and the ray will hit the far side of the AABB before it hits the near side.
            if (invert_dir < 0) std::swap(t0, t1);

            if(t0 > ray_t.min) ray_t.min = t0;
            if(t1 < ray_t.max) ray_t.max = t1;
            
            if (ray_t.max <= ray_t.min) return false;
        }
        return true;
    }

}; 

#endif