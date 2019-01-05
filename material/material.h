#ifndef MATERIAL_H
#define MATERIAL_H
#include "../camera/ray.h"
#include "../hitable/hitable.h"

vec3 reflect(const vec3& in, const vec3& n){
    // n can points inside or outside, there is no influence. 
    // Because in and its projection on n always points to the same orientation no matter what orientation n is.
    // dot(in, info.n)*info.n = the projection of in on info.n
    // because in and its projection on n points to the same orientation, we need reverse the projection
    // that is why add a minus sign
    return in - 2*dot(in, n)*n;
}

bool refract(const vec3& in, const vec3& n, float ni_over_nt, vec3& refracted){
    vec3 u_in = normalize(in);
    float cosi = -dot(u_in, n);
    float sint2 =  (ni_over_nt * ni_over_nt) * (1 - cosi*cosi);
    if(sint2 >= 1){
        return false;
    }else{
        refracted = -n * sqrt(1 - sint2) + (cosi * n + u_in) * ni_over_nt;
        return true;
    }
}

class material{
public:
    // attenuation is used to adjust the color relected
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) const = 0;
    virtual vec3 emitted(float u, float v, const vec3& p) const {
        return vec3(0, 0, 0);
    }
};
#endif