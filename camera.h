#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
class camera{
public:
    vec3 origin;
    vec3 leftLowerCorner;
    vec3 horizontalRange;
    vec3 verticalRange;
    camera(){
        origin = vec3(0.0, 0.0, 0.0);
        leftLowerCorner = vec3(-2.0, -1.0, -1.0);
        horizontalRange = vec3(4.0, 0.0, 0.0);
        verticalRange = vec3(0.0, 2.0, 0.0);
    };
    ray get_ray(float x, float y){ return ray(origin, leftLowerCorner + x * horizontalRange + y * verticalRange - origin);};
};
#endif