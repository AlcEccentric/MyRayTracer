#ifndef YZ_RECT_H
#define YZ_RECT_H
#include "hitable.h"
#include "../material/material.h"
class yzRect: public hitable{
public:
    float z0, z1, y0, y1, x;
    material* mtr;
    yzRect(float _y0, float _y1, float _z0, float _z1, float _x, material* m):z0(_z0), z1(_z1), y0(_y0), y1(_y1), x(_x), mtr(m){};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        box = aabb(vec3(x - 0.001, y0, z0), vec3(x + 0.001, y1, z1));
        return true;
    };
};

bool yzRect::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const{
    float t = (x-r.ori().x()) / r.dir().x();
    if (t < t_min || t > t_max)
        return false;
    float z = r.ori().z() + t*r.dir().z();
    float y = r.ori().y() + t*r.dir().y();
    if (z < z0 || z > z1 || y < y0 || y > y1) 
        return false;
    h_info.u = (z-z0)/(z1-z0);
    h_info.v = (y-y0)/(y1-y0); 
    h_info.t = t;
    h_info.mat_ptr = mtr;
    h_info.p = r.point_at_parameter(t);
    h_info.n = vec3(1, 0, 0);
    return true;
}

#endif