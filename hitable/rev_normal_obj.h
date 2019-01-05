#ifndef REV_NORMAL_OBJ_H
#define REV_NORMAL_OBJ_H
#include "hitable.h"
#include "../material/material.h"
class revNormObj: public hitable{
public:
    hitable * obj;
    revNormObj(hitable* h):obj(h){};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        return obj->bounding_box(time0, time1, box);
    };
};

bool revNormObj::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const{
    if(obj->hit(r, t_min, t_max, h_info)){
        h_info.n = h_info.n * (-1);
        return true;
    }else return false;
}
#endif