#ifndef MARBLE1_TEXTURE_H
#define MARBLE1_TEXTURE_H
#include "texture.h"
#include "../tools/perlin.h"

class marble1Texture : public texture{
public:
    marble1Texture(float scale, vec3 col, float att = 1.0):sc(scale), color(col), atten(att){};
    virtual vec3 tex_value(float u, float v, const vec3& h_point) const {
        return perlinNoise.turbo(h_point*sc) * color * atten;
    }
    perlin perlinNoise;
    float sc;
    float atten;
    vec3 color;
};
#endif