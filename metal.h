#ifndef METAL_H
#define METAL_H
#include "ray.h"
#include "material.h"

vec3 reflect(const vec3& in, const vec3& n);
class metal: public material{
public:
    vec3 albedo;
    metal(const vec3& a):albedo(a) {};
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) const{
        scattered = ray(info.p, reflect(r_in.dir(), info.n));
        attenuation = albedo;
        // only when r_in.dir points in the tangent surface, the r_in will be scatter by the surface
        // "r_in.dir points in"   
        // <=> "the angle between r_in.dir and info.n greater than 90 degree"
        // <=> "the angle between scattered and info.n smaller than 90 degree"
        return dot(scattered.dir(), info.n) > 0;
    };
};
#endif