#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"
#include "hitable.h"
class material{
public:
    // attenuation is used to adjust the color relected
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) const = 0;
};
#endif