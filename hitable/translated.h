#ifndef TRANSLATED_H
#define TRANSLATED_H
#include "hitable.h"
class translated: public hitable{
public:
    hitable* obj;
    vec3 offset;
    translated(hitable* h, const vec3& off): offset(off), obj(h){};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        if(obj->bounding_box(time0, time1, box)) {
            box = aabb(box.getMin() + offset, box.getMax() + offset);
            return true;
        }else return false;
    };
};
bool translated::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const {
    ray rev_translated_r(r.ori() - offset, r.dir(), r.time());
    if (obj->hit(rev_translated_r, t_min, t_max, h_info)) {
        h_info.p += offset;
        return true;
    }
    else 
        return false;
}
#endif