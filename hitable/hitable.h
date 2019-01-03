#ifndef HITABLE_H
#define HITABLE_H
#include "../camera/ray.h"
#include "../tools/vec3.h"
#include "../tools/AABB.h"

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
    virtual bool bounding_box(float time0, float time1, aabb& box) const = 0;
    aabb surrounding_box(aabb& a, aabb& b) const;
};

aabb hitable::surrounding_box(aabb& a, aabb& b) const{
    
    vec3 minBound(fmin(a.getMin()[0], b.getMin()[0]),
                  fmin(a.getMin()[1], b.getMin()[1]),
                  fmin(a.getMin()[2], b.getMin()[2])
    );

    vec3 maxBound(fmax(a.getMax()[0], b.getMax()[0]),
                  fmax(a.getMax()[1], b.getMax()[1]),
                  fmax(a.getMax()[2], b.getMax()[2])
    );
    return aabb(minBound, maxBound);
}
#endif