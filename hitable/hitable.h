#ifndef HITABLE_H
#define HITABLE_H
#include "../camera/ray.h"
#include "../tools/vec3.h"
class material;
struct hit_info{
    float t; 
    vec3 p; //hit point
    vec3 n; //normal at the point
    material* mat_ptr; // material of the hit point
};
class hitable{
public:
    // pure virtual function
    // can only be inherited, not allowed to be instantiated
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const = 0;
};
#endif