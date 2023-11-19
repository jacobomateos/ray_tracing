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

    const interval& axis(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto t0 = fmin((axis(a).min - r.origin()[a]) / r.direction()[a], 
                axis(a).max - r.origin()[a] / r.direction()[a]);
            
            auto t1 = fmax((axis(a).min - r.origin()[a]) / r.direction()[a], 
                axis(a).max - r.origin()[a] / r.direction()[a]);
            ray_t.min = fmax(t0, ray_t.min);
            ray_t.max = fmin(t1, ray_t.max);

            if (ray_t.max <= ray_t.min) return false;
        }
        return true;
    }

};

#endif