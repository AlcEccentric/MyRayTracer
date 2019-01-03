#ifndef AABB_H
#define AABB_H
#include "vec3.h"
#include <stdlib.h>
#include "../camera/ray.h"
class aabb{
public:
    vec3 _min, _max;
    aabb(){};
    aabb(const vec3& minBounds, const vec3& maxBounds){ _min = minBounds; _max = maxBounds;}
    vec3 getMin() const{ return _min;}
    vec3 getMax() const{ return _max;}
    bool hit(const ray& r, float t_min, float t_max) const;
};

bool aabb::hit(const ray& r, float t_min, float t_max) const{
    for(int i = 0; i < 3; i++){
        float invDet = 1 / r.dir()[i];
        float t0 = (getMin()[i] - r.ori()[i]) * invDet;
        float t1 = (getMax()[i] - r.ori()[i]) * invDet;
        if(invDet < 0)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if(t_max <= t_min)
            return false;
    }
    return true;
}
#endif