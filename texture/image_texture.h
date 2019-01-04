#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H
#include "texture.h"
void clamp(int& n, int min, int max) {
    if(n < min) n = min;
    else if(n > max) n = max;
    else n = n;
}
class imageTexture: public texture{
public:
    unsigned char* data;
    int nx, ny;
    imageTexture(){};
    imageTexture(unsigned char* pix, int width, int height):data(pix), nx(width), ny(height){};

    virtual vec3 tex_value(float u, float v, vec3& p) const;
};

vec3 imageTexture::tex_value(float u, float v, vec3& p) const{
    int i = u*nx;
    int j = (1-v)*ny - 0.001;
    clamp(i, 0, nx-1);
    clamp(j, 0, ny-1);
    float r = int(data[3*j*nx + 3*i]) / 255.0;
    float g = int(data[3*j*nx + 3*i + 1]) / 255.0;
    float b = int(data[3*j*nx + 3*i + 2]) / 255.0;
    return vec3(r, g, b);
}

#endif