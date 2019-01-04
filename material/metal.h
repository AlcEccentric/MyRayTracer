#ifndef METAL_H
#define METAL_H
#include "../camera/ray.h"
#include "material.h"

class metal: public material{
public:
    texture* albedo;
    float fuzz;
    metal(texture* a, float f):albedo(a) {if(f < 1) fuzz = f; else fuzz = 1;};
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) const{
        
        scattered = ray(info.p, reflect(r_in.dir(), info.n) + fuzz * randend_in_unit_sphere());
        vec3 hitPoint = info.p;
        attenuation = albedo->tex_value(0, 0, hitPoint);// metal will only have constant texture
        // only when r_in.dir points in the tangent surface, the r_in will be scatter by the surface
        // "r_in.dir points in"   
        // <=> "the angle between r_in.dir and info.n greater than 90 degree"
        // <=> "the angle between scattered and info.n smaller than 90 degree"
        return dot(scattered.dir(), info.n) > 0;
    };
};
#endif