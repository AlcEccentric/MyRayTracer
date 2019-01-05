#ifndef MOSAIC_TEXTURE_H
#define MOSAIC_TEXTURE_H
#include "texture.h"
#include "../tools/perlin.h"
class mosaicTexture : public texture{
public:
    mosaicTexture(float scale, vec3 col):sc(scale), color(col){};
    virtual vec3 tex_value(float u, float v, const vec3& h_point) const {
        return perlinNoise.mosaic_noise(h_point*sc) * color;
    }
    perlin perlinNoise;
    float sc;
    vec3 color;
};
#endif