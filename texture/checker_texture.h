#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H
#include "texture.h"
class checkerTexture: public texture{
public: 
    texture * a;
    texture * b;
    checkerTexture(){};
    checkerTexture(texture * x, texture * y): a(x), b(y) {};
    virtual vec3 tex_value(float u, float v, vec3& hitPoint) const;
};

vec3 checkerTexture::tex_value(float u, float v, vec3& hitPoint) const{
    float sine3 = sin(hitPoint.x() * 10) * sin(hitPoint.y() * 10) * sin(hitPoint.z() * 10);
    if(sine3 > 0)
        return a->tex_value(u, v, hitPoint);
    else 
        return b->tex_value(u, v, hitPoint);
}

#endif