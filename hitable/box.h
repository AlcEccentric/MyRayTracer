#ifndef BOX_H
#define BOX_H
#include "hitable.h"
#include "hitablelist.h"
#include "xy_rect.h"
#include "xz_rect.h"
#include "yz_rect.h"
#include "rev_normal_obj.h"

class box: public hitable{
public:
    box(){};
    box(const vec3& v0, const vec3& v1, material* mtr);
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const{
        return rectList->hit(r, t_min, t_max, h_info);
    };
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        box = aabb(vmin, vmax);
        return true;
    };
    vec3 vmin, vmax;
    hitable * rectList;
};

box::box(const vec3& v0, const vec3& v1, material* mtr){
    vmin = v0;
    vmax = v1;
    hitable **list = new hitable*[6];
    int i = 0;
    // back
    list[i++] = new revNormObj(new xyRect(v0.x(), v1.x(), v0.y(), v1.y(), v0.z(), mtr));
    // front
    list[i++] = new xyRect(v0.x(), v1.x(), v0.y(), v1.y(), v1.z(), mtr);
    // top   
    list[i++] = new revNormObj(new xzRect(v0.x(), v1.x(), v0.z(), v1.z(), v0.y(), mtr));
    // bottom
    list[i++] = new xzRect(v0.x(), v1.x(), v0.z(), v1.z(), v1.y(), mtr);
    // left
    list[i++] = new revNormObj(new yzRect(v0.y(), v1.y(), v0.z(), v1.z(), v0.x(), mtr));
    //right
    list[i++] = new yzRect(v0.y(), v1.y(), v0.z(), v1.z(), v1.x(), mtr);
    rectList = new hitable_list(list, i);

}
#endif