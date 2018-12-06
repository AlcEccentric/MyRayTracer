#ifndef LERBERTIAN_H
#define LERBERTIAN_H
#include "material.h"

vec3 randend_in_unit_sphere();

class lambertian: public material{
public:
    vec3 albedo;
    lambertian(const vec3& a) : albedo(a) {};
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) const{
        vec3 randDir = info.n + randend_in_unit_sphere();
        scattered = ray(info.p, randDir);
        attenuation = albedo;
        return true;
    };
};

#endif