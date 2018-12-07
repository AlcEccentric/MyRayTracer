#ifndef DIELECT_H
#define DIELECT_H
#include "ray.h"
#include "material.h"
#include <stdlib.h>

vec3 reflect(const vec3& in, const vec3& n);
bool refract(const vec3& in, const vec3& n, float ni_over_nt, vec3& refracted);

// a approximate function to cumpute the 反射比(the intensity of reflection ray/ the intensity of the original ray) compared with Fresnel equation
// please refer to this page to see Fresnel equation
//  折射比 + 反射比 = 1
// https://zhuanlan.zhihu.com/p/31534769
float schlick(float cosinei, float ref_idx){
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0 * r0;
    return r0 + (1-r0)*pow((1-cosinei),5);
}

class dielect : public material{
public:
    float die_ri;
    // no attenuation be cause glass does not absorb
    dielect( float ri) :  die_ri(ri) {};
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) const{
        float cos_in = dot(normalize(r_in.dir()), info.n);
        vec3 ni_normal;
        float ni_over_nt;
        float cosine;
        attenuation = vec3(1.0, 1.0, 1.0);
        if (cos_in > 0) {
            // ray from dielect to air, i is dielect, t is air
            ni_normal =  - info.n;
            ni_over_nt = die_ri / 1;
            cosine = cos_in;
        }else{
            // ray from air to dielect, i is air, t is dielect
            ni_normal = info.n;
            ni_over_nt = 1.0 / die_ri;
            cosine = -cos_in;
        }
        vec3 refracted;
        float ref_ratio;
        if(refract(r_in.dir(), ni_normal, ni_over_nt, refracted)){
            ref_ratio = schlick(cosine, die_ri);
        }else{
            ref_ratio = 1.0;
            // this is a bug, we return false when full reflection
            // which menas there will be no reflection
        }
        if(drand48() < ref_ratio){
            // no refraction, full reflection == ref_ratio = 1
            // or reflection ratio is greater than a random value
            scattered = ray(info.p, reflect(r_in.dir(), info.n));
        }else{
            // there is refraction
            scattered = ray(info.p, refracted);
        }
        return true;
    }  
};
#endif