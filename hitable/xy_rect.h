#ifndef XY_RECT_H
#define XY_RECT_H
#include "hitable.h"
#include "../material/material.h"
class xyRect: public hitable{
public:
    float x0, x1, y0, y1, z;
    material* mtr;
    xyRect(float _x0, float _x1, float _y0, float _y1, float _z, material* m):x0(_x0), x1(_x1), y0(_y0), y1(_y1), z(_z), mtr(m){};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        box = aabb(vec3(x0, y0, z - 0.001), vec3(x1, y1, z + 0.001));
        return true;
    };
};

bool xyRect::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const{
    float t = (z-r.ori().z()) / r.dir().z();
    if (t < t_min || t > t_max)
        return false;
    float x = r.ori().x() + t*r.dir().x();
    float y = r.ori().y() + t*r.dir().y();
    if (x < x0 || x > x1 || y < y0 || y > y1) 
        return false;
    h_info.u = (x-x0)/(x1-x0);
    h_info.v = (y-y0)/(y1-y0); 
    h_info.t = t;
    h_info.mat_ptr = mtr;
    h_info.p = r.point_at_parameter(t);
    h_info.n = vec3(0, 0, 1);
    return true;
}

#endif