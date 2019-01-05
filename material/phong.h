#ifndef PHONG_H
#define PHONG_H
#include "material.h"
#include "../camera/ray.h"
#include "../texture/constant_texture.h"

class phong: public material{
public:
    texture* albedo;
    float lightDist;
    vec3 lightDir;
    vec3 viewDir;
    float intensity;
    float Kd;
    float n;
    phong(texture* a, vec3 light_dir, vec3 view_dir, float _Kd = 0.8, float _n = 10, const float& light_dist = 5, const float light_intense = 1) : albedo(a), lightDist(light_dist), intensity(light_intense){
        lightDir = normalize(light_dir);
        viewDir = normalize(view_dir);
        if(_n < 0)
            n = 10;
        else
            n = _n;
        
        if(_Kd > 1 || _Kd < 0)
            Kd = 0.8;
        else
            Kd = _Kd;

    };
    virtual bool isPhong(){ return true; };
    bool phongScatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& detect) {
        attenuation = albedo->tex_value(info.u, info.v, info.p);
        vec3 bias = info.n * 0.0001;
        detect = ray(info.p + bias, -lightDir, r_in.time());
        return true;
    }

    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) {  
        return true;
    };
};

#endif