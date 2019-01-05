#ifndef ROTATED_X_H
#define ROTATED_X_H
#include "hitable.h"
class rotatedX: public hitable{
public:
    hitable* obj;
    aabb b;
    bool hasbox;
    float sin_theta, cos_theta;
    rotatedX(hitable* h, float angle);
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float time0, float time1, aabb& box) const{
        box = b;
        return hasbox;
    };
};
rotatedX::rotatedX(hitable* h, float angle){
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
                float newy = cos_theta*y - sin_theta*z;
                float newz = sin_theta*y + cos_theta*z;
                vec3 tmp(x, newy, newz);
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
bool rotatedX::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const {
    vec3 origin = r.ori();
    vec3 direction = r.dir();
    // rotate the r in the reversed direction
    origin[1] = cos_theta*r.ori()[1] + sin_theta*r.ori()[2];
    origin[2] =  -sin_theta*r.ori()[1] + cos_theta*r.ori()[2];
    direction[1] = cos_theta*r.dir()[1] + sin_theta*r.dir()[2];
    direction[2] = -sin_theta*r.dir()[1] + cos_theta*r.dir()[2];
    ray rev_rotated_r(origin, direction, r.time());
    if (obj->hit(rev_rotated_r, t_min, t_max, h_info)) {
        vec3 p = h_info.p;
        vec3 n = h_info.n;
        // rotate the hit point in the real direction
        p[1] = cos_theta*h_info.p[1] - sin_theta*h_info.p[2];
        p[2] = sin_theta*h_info.p[1] + cos_theta*h_info.p[2];
        n[1] = cos_theta*h_info.n[1] - sin_theta*h_info.n[2];
        n[2] = sin_theta*h_info.n[1] + cos_theta*h_info.n[2];
        h_info.p = p;
        h_info.n = n;
        return true;
    }
    else 
        return false;
}
#endif