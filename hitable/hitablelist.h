#ifndef HITABLELIST_H
#define HITABLELIST_H
#include "hitable.h"
#include "../tools/vec3.h"
class hitable_list: public hitable{
public:
    hitable ** list;
    int list_size;
    hitable_list() {};
    hitable_list(hitable **l, int size) : list(l), list_size(size) {};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_info& info) const{
    hit_info hit_temp;
    float closest_point = t_max;
    bool hitany = false;
    for(int i = 0; i < list_size; i++)
    {
        if(list[i]->hit(r, t_min, closest_point, hit_temp)){
            closest_point = hit_temp.t;
            hitany = true;
            info = hit_temp;
        }
    }
    return hitany;
    
}

bool hitable_list::bounding_box(float time0, float time1, aabb& box) const{
    if(!list[0]->bounding_box(time0, time1, box))
        return false;
    
    for(int i = 1; i < list_size; i++){
        aabb tempBox;
        if(!list[i]->bounding_box(time0, time1, tempBox))
            return false;
        surrounding_box(box, tempBox);
    }
    return true;
}
#endif