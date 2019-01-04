#ifndef LERBERTIAN_H
#define LERBERTIAN_H
#include "material.h"
#include "../texture/constant_texture.h"
vec3 randend_in_unit_sphere();

class lambertian: public material{
public:
    texture* albedo;
    lambertian(texture* a) : albedo(a) {};
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) const{
        vec3 randDir = info.n + randend_in_unit_sphere();
        scattered = ray(info.p, randDir, r_in.time());
        vec3 hitPoint = info.p;
        attenuation = albedo->tex_value(info.u, info.v, hitPoint);// diffuse will only have constant texture
        return true;
    };
};

#endif