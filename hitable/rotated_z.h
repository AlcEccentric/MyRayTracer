#ifndef ROTATED_Z_H
#define ROTATED_Z_H
#include "hitable.h"
class rotatedZ: public hitable{
public:
    hitable* obj;
    aabb b;
    bool hasbox;
    float sin_theta, cos_theta;
    rotatedZ(hitable* h, float angle);
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        box = b;
        return hasbox;
    };
};
rotatedZ::rotatedZ(hitable* h, float angle){
    // ratate obj's aabb and set that aabb as the rotated aabb
    float rad = angle / 180.0 * M_PI;
    obj = h;
    sin_theta = sin(rad);
    cos_theta = cos(rad);
    hasbox = obj->bounding_box(0, 1, b); 
    vec3 vmin(FLT_MAX);
    vec3 vmax(-FLT_MAX);
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            for(int k = 0; k < 2; k++)
            {
                // rotate all the 8 vertex
                float x = i*b.getMax().x() + (1-i)*b.getMin().x();
                float y = j*b.getMax().y() + (1-j)*b.getMin().y();
                float z = k*b.getMax().z() + (1-k)*b.getMin().z();
                float newx = cos_theta*x - sin_theta*y;
                float newy = sin_theta*x + cos_theta*y;
                vec3 tmp(newx, newy, z);
                // get its min bounds and max bounds
                for ( int c = 0; c < 3; c++ )
                {
                    if ( tmp[c] > vmax[c] )
                        vmax[c] = tmp[c];
                    if ( tmp[c] < vmin[c] )
                        vmin[c] = tmp[c];
                }

            }
    b = aabb(vmin, vmax);
}
bool rotatedZ::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const {
    vec3 origin = r.ori();
    vec3 direction = r.dir();
    // rotate the r in the reversed direction
    origin[0] = cos_theta*r.ori()[0] + sin_theta*r.ori()[1];
    origin[1] =  -sin_theta*r.ori()[0] + cos_theta*r.ori()[1];
    direction[0] = cos_theta*r.dir()[0] + sin_theta*r.dir()[1];
    direction[1] = -sin_theta*r.dir()[0] + cos_theta*r.dir()[1];
    ray rev_rotated_r(origin, direction, r.time());
    if (obj->hit(rev_rotated_r, t_min, t_max, h_info)) {
        vec3 p = h_info.p;
        vec3 n = h_info.n;
        // rotate the hit point in the real direction
        p[0] = cos_theta*h_info.p[0] - sin_theta*h_info.p[1];
        p[1] = sin_theta*h_info.p[0] + cos_theta*h_info.p[1];
        n[0] = cos_theta*h_info.n[0] - sin_theta*h_info.n[1];
        n[1] = sin_theta*h_info.n[0] + cos_theta*h_info.n[1];
        h_info.p = p;
        h_info.n = n;
        return true;
    }
    else 
        return false;
}
#endif