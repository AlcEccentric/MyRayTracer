#ifndef XZ_RECT_H
#define XZ_RECT_H
#include "hitable.h"
#include "../material/material.h"
class xzRect: public hitable{
public:
    float x0, x1, z0, z1, y;
    material* mtr;
    xzRect(float _x0, float _x1, float _z0, float _z1, float _y, material* m):x0(_x0), x1(_x1), z0(_z0), z1(_z1), y(_y), mtr(m){};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        box = aabb(vec3(x0, y - 0.001, z0), vec3(x1, y + 0.001, z1));
        return true;
    };
};

bool xzRect::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const{
    float t = (y-r.ori().y()) / r.dir().y();
    if (t < t_min || t > t_max)
        return false;
    float x = r.ori().x() + t*r.dir().x();
    float z = r.ori().z() + t*r.dir().z();
    if (x < x0 || x > x1 || z < z0 || z > z1) 
        return false;
    h_info.u = (x-x0)/(x1-x0);
    h_info.v = (z-z0)/(z1-z0); 
    h_info.t = t;
    h_info.mat_ptr = mtr;
    h_info.p = r.point_at_parameter(t);
    h_info.n = vec3(0, 1, 0);
    return true;
}

#endif