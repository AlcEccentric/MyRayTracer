#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H
#include "texture.h"
class checkerTexture: public texture{
public: 
    texture * a;
    texture * b;
    float sc;
    checkerTexture(){};
    checkerTexture(texture * x, texture * y, float scale = 1): a(x), b(y), sc(scale) {};
    virtual vec3 tex_value(float u, float v, const vec3& hitPoint) const;
};

vec3 checkerTexture::tex_value(float u, float v, const vec3& hitPoint) const{
    float sine3 = sin(hitPoint.x() * sc) * sin(hitPoint.y() * sc) * sin(hitPoint.z() * sc);
    if(sine3 > 0)
        return a->tex_value(u, v, hitPoint);
    else 
        return b->tex_value(u, v, hitPoint);
}

#endif