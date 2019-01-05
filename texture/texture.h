#ifndef TEXTURE_H
#define TEXTURE_H
#include "../tools/vec3.h"

class texture{
public:
    virtual vec3 tex_value(float u, float v, const vec3& h_point) const = 0;
};
#endif