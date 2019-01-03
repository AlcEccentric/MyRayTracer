#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
vec3 rand_in_unit_dist(){
    vec3 p;
    do{
        p = 2.0*vec3(drand48(), drand48(), 0.0) - vec3(1.0, 1.0, 0.0);  
    }while(dot(p,p) >= 1.0);
    return p;
}
class camera{
public:
    vec3 origin;
    vec3 leftLowerCorner;
    vec3 horizontalRange;
    vec3 verticalRange;
    vec3 x_basis;
    vec3 y_basis;
    vec3 z_basis;
    float lens_radius;
    float openAperTime;
    float closeAperTime;

    camera(const vec3& viewfrom, const vec3& viewto, const vec3& vup,float v_fov_degree, float w_h_ratio, float aperture, float dist_to_focus_screen, float t0, float t1){
        float fov = v_fov_degree * M_PI / 180;
        float half_height = tan(fov/2);
        float half_width = half_height * w_h_ratio;
        z_basis = normalize(viewfrom - viewto);
        x_basis = normalize(cross(vup, z_basis));
        y_basis = cross(z_basis, x_basis); 
        openAperTime = t0;
        closeAperTime = t1;
        origin = viewfrom;
        lens_radius = aperture / 2;
        // the original screen is 1 unit away from the view plane
        // now we change it to dist_to_focus_screen
        // use the theory of similar triangle
        // we know the vertical and horizontal range will be enlarged by dist_to_focus_screen/1
        leftLowerCorner = origin + (-half_width)* dist_to_focus_screen * x_basis + (-half_height)* dist_to_focus_screen * y_basis + (-dist_to_focus_screen) * z_basis;
        horizontalRange = 2 * half_width * dist_to_focus_screen * x_basis;
        verticalRange = 2 * half_height * dist_to_focus_screen * y_basis;
    };
    ray get_ray(float x, float y){ 
        vec3 rd = lens_radius * rand_in_unit_dist();
        float randTime = openAperTime + drand48()*(closeAperTime - openAperTime);
        vec3 offset = rd.x() * x_basis + rd.y() * y_basis + rd.z() * z_basis;
        return ray(origin + offset, leftLowerCorner + x * horizontalRange + y * verticalRange - origin - offset, randTime);
    };
};
#endif