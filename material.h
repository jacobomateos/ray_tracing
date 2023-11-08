#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

class material;

class hit_record;

class material{
    public:
    virtual ~material() = default;
    virtual bool scatter (
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;

};

class lambertian : public material {
    public:
    lambertian(const color &a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero()){
            scatter_direction = rec.normal;
        }
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

    private:
    color albedo;
};

class metal : public material {
    public:
    metal(const color& a) : albedo(a) {}


    private:
    color albedo;
};

#endif