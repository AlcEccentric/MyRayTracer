#ifndef CONSTANT_TEXTURE_H
#define CONSTANT_TEXTURE_H
#include "texture.h"
class constantTexture: public texture{
public:
    vec3 col;
    constantTexture(){};
    constantTexture(const vec3& color){
        col = color;
    };

    virtual vec3 tex_value(float u, float v, vec3& p) const;
};

vec3 constantTexture::tex_value(float u, float v, vec3& p) const{
    return col;
}

#endif