#ifndef SPHERE_H
#define SPHERE_H
#include "vec3.h"
#include "hitable.h"
class sphere: public hitable{
public: 
    vec3 center;
    material* mat_ptr;
    float radius;
    sphere() {};
    sphere(const vec3& c, float r) : center(c), radius(r) {};
    sphere(const vec3& c, float r, material * m) : center(c), radius(r), mat_ptr(m) {};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    
};
bool sphere::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const{
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
            h_info.n = normalize(h_info.p - center);
            h_info.t = small_root;
            h_info.mat_ptr = mat_ptr;
            return true;
        }
        // if closer point does not hit, check further point
        float large_root = (-b - sqrt(delta))/(2.0*a);
        if(large_root > t_min && large_root < t_max){
            h_info.p = r.point_at_parameter(large_root);
            h_info.n = normalize(h_info.p - center);
            h_info.t = large_root;
            h_info.mat_ptr = mat_ptr;
            return true;
        }
        // if two points both fall outside hit range, return false
        return false;
    }
}
#endif