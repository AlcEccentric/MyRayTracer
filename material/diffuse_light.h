#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H
#include "../camera/ray.h"
#include "../hitable/hitable.h"
#include "../texture/texture.h"

class diffuseLight: public material{
public:
    // attenuation is used to adjust the color relected
    diffuseLight(texture* t): emit(t) {};
    virtual bool scatter(const ray& r_in, const hit_info& info, vec3& attenuation, ray& scattered) {
        return false;
    };
    virtual vec3 emitted(float u, float v, const vec3& p) const {
        return emit->tex_value(u, v, p);// by default, the constant texture will be used
    }
    texture* emit;
};
#endif