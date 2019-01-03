#ifndef RAY_H
#define RAY_H
#include "../tools/vec3.h"
class ray{
public:
    vec3 origin;
    vec3 direction;
    float _time;

    ray(){};
    ray(const vec3& ori, const vec3& dir, const float& ti = 0.0f){origin = ori; direction = dir; _time = ti;}
    vec3 ori() const {return origin;}
    vec3 dir() const {return direction;}
    float time() const {return _time;}
    vec3 point_at_parameter(float t) const {return origin + direction * t;}
};
#endif