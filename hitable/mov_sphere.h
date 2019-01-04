#ifndef MOV_SPHERE_H
#define MOV_SPHERE_H
#include "hitable.h"
#include "../material/material.h"
#include "../tools/vec3.h"
extern void get_sphere_uv(const vec3& h_point, float &u, float &v);
class movSphere : public hitable {
public:
    vec3 beginCenter;
    vec3 endCenter;
    float beginTime, endTime;
    material* mat_ptr;
    float radius;
    movSphere() {};
    movSphere(const vec3& c0, const vec3& c1, float time0, float time1, float r, material * m): beginCenter(c0), endCenter(c1), beginTime(time0), endTime(time1), radius(r), mat_ptr(m) {};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    vec3 center(float t) const;
};

vec3 movSphere::center(float t) const{
    return beginCenter + (t - beginTime)/(endTime - beginTime) * (endCenter - beginCenter);
}

bool movSphere::bounding_box(float time0, float time1, aabb& box) const{
    vec3 center0 = this->center(time0);
    vec3 center1 = this->center(time1); 
    aabb aabb0(center0 - vec3(radius), center0 + vec3(radius));
    aabb aabb1(center1 - vec3(radius), center1 + vec3(radius));
    box = surrounding_box(aabb0, aabb1);
    return true;
}

bool movSphere::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const{
    vec3 center = this->center(r.time());
    float a = r.dir().squared_length();
    float b = dot(r.ori() - center, r.dir()) * 2;
    float c = (r.ori() - center).squared_length() - radius * radius;
    float delta = b*b - 4*a*c;

    if(delta < 0){
        // no root means cannot hit
        return false;
    }else{
        float small_root = (-b - sqrt(delta))/(2.0*a);
        // if closer point is in the range, count this closer point as hit point
        if(small_root > t_min && small_root < t_max){
            h_info.p = r.point_at_parameter(small_root);
            // NOTICE 
            // we dont use normalize() here, because we will take advanage of the sign of radius
            // when radius is negative
            // the normal will points to the center
            // this kind of normal will be useful when rendering a bubble.
            get_sphere_uv(h_info.p, h_info.u, h_info.v);
            h_info.n = (h_info.p - center) / radius;
            h_info.t = small_root;
            h_info.mat_ptr = mat_ptr;
            return true;
        }
        // if closer point does not hit, check further point
        float large_root = (-b - sqrt(delta))/(2.0*a);
        if(large_root > t_min && large_root < t_max){
            h_info.p = r.point_at_parameter(large_root);
            get_sphere_uv(h_info.p, h_info.u, h_info.v);
            h_info.n = (h_info.p - center) / radius;
            h_info.t = large_root;
            h_info.mat_ptr = mat_ptr;
            return true;
        }
        // if two points both fall outside hit range, return false
        return false;
    }

}
#endif