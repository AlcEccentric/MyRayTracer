#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
class camera{
public:
    vec3 origin;
    vec3 leftLowerCorner;
    vec3 horizontalRange;
    vec3 verticalRange;
    camera(const vec3& viewfrom, const vec3& viewto, const vec3& vup,float fov_degree, float w_h_ratio){
        float fov = fov_degree * M_PI / 180;
        float half_height = tan(fov/2);
        float half_width = half_height * w_h_ratio;
        vec3 z_basis = normalize(viewfrom - viewto);
        vec3 x_basis = cross(vup, z_basis);
        vec3 y_basis = cross(z_basis, x_basis); 
        origin = viewfrom;
        leftLowerCorner = origin + (-half_width) * x_basis + (-half_height) * y_basis + (-1.0) * z_basis;
        horizontalRange = 2 * half_width * x_basis;
        verticalRange = 2 * half_height * y_basis;
    };
    ray get_ray(float x, float y){ return ray(origin, leftLowerCorner + x * horizontalRange + y * verticalRange - origin);};
};
#endif