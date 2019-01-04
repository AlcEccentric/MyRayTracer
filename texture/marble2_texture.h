#ifndef MARBLE2_TEXTURE_H
#define MARBLE2_TEXTURE_H
#include "texture.h"
#include "../tools/perlin.h"

class marble2Texture : public texture{
public:
    marble2Texture(float scale, vec3 col, float att = 1.0):sc(scale), color(col), atten(att){};
    virtual vec3 tex_value(float u, float v, vec3& h_point) const {
        return (sin(sc*h_point.z()+10*perlinNoise.turbo(h_point))+1) * color * atten;
    }
    perlin perlinNoise;
    float sc;
    float atten;
    vec3 color;
};
#endif