#ifndef SPHERE_H
#define SPHERE_H
#include "vec3.h"
#include "hitable.h"

vec3 randend_in_unit_sphere(){
    vec3 endpoint;
    do
    {
        endpoint = vec3(drand48(), drand48(), drand48()) * float(2.0) - vec3(1.0, 1.0, 1.0);
    } while (endpoint.squared_length() >= 1.0);
    return endpoint;

}

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
            // NOTICE 
            // we dont use normalize() here, because we will take advanage of the sign of radius
            // when radius is negative
            // the normal will points to the center
            // this kind of normal will be useful when rendering a bubble.
            h_info.n = (h_info.p - center) / radius;
            h_info.t = small_root;
            h_info.mat_ptr = mat_ptr;
            return true;
        }
        // if closer point does not hit, check further point
        float large_root = (-b - sqrt(delta))/(2.0*a);
        if(large_root > t_min && large_root < t_max){
            h_info.p = r.point_at_parameter(large_root);
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