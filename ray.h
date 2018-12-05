#ifndef RAY_H
#define RAY_H
#include "vec3.h"
class ray{
public:
    vec3 origin;
    vec3 direction;

    ray(){};
    ray(const vec3& ori, const vec3& dir){origin = ori; direction = dir;}
    vec3 ori() const {return origin;}
    vec3 dir() const {return direction;}
    vec3 point_at_parameter(float t) const {return origin + direction * t;}
};
#endif