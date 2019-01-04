#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H
#include "texture.h"
#include "../tools/perlin.h"
class noiseTexture : public texture{
public:
    noiseTexture(float scale, vec3 col):sc(scale), color(col){};
    virtual vec3 tex_value(float u, float v, vec3& h_point) const {
        return perlinNoise.noise(h_point*sc) * color;
    }
    perlin perlinNoise;
    float sc;
    vec3 color;
};
#endif