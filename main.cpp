#include <fstream>
#include <iostream>
#include "vec3.h"
#include "ray.h"

float hit_sphere(const vec3& center, float radius, const ray& r){
    float a = r.dir().squared_length();
    float b = dot(r.ori() - center, r.dir()) * 2;
    float c = (r.ori() - center).squared_length() - radius * radius;
    // if the sphere is behind the camera (at origin point), two roots are all negtive
    // or the camera is in the sphere, one of two roots is negtive
    // or no ray from camera has root
    // in these cases, we should not see the sphere

    // if the forward ray can hit the sphere, the returned value must be non-negtive
    float delta = b*b - 4*a*c;
    if(delta < 0){
        // no root means cannot hit
        return -1.0;
    }else{
        // have root(s) return the smaller one
        // if smaller one is negtive, it means the camera is in the sphere or in front of the sphere
        return (-b - sqrt(delta))/(2.0*a);
    }
}
vec3 color(const ray& r){
    // generating a color according the ray
    // if the ray hits the sphere generate color in a different way
    vec3 c = vec3(0.0, 0.0, -1.0);
    float t = hit_sphere(c, 0.5, r);
    if( t >= 0.0){
        // non negative means the forward ray can hit the sphere
        // ray.ori() + t * ray.dir() = the closer one of the points on the sphere hit by the forward ray
        vec3 normal = normalize(r.point_at_parameter(t) - c);
        // visualize normal by convert it xyz to rgb
        return float(0.5) * vec3(normal.x()+1, normal.y()+1, normal.z()+1);
    }

    // if the ray hits places in the background we generate background color
    // the normalized coordinate is in [-1.0, 1.0]
    // +1 and *0.5 make t fall in [0.0, 1.0]
    float k = 0.5 * (normalize(r.dir()).y() + 1.0);
    return vec3(0.5, 1.0, 0.7) * (1-k) + vec3(1.0, 1.0, 1.0) * k;
}

int main(){
    
    std::fstream File;
    File.open("shadingSphere.ppm",std::ios::out);
    if(!File){
        File.close();
        std::cout<< "fail to open file\n";
    } 

    
    int nx = 2000, ny = 1000;
    File<< "P3\n" << nx << " " << ny << "\n" << "255\n";
    vec3 origin(0.0, 0.0, 0.0);
    vec3 leftLowerCorner(-2.0, -1.0, -1.0);
    vec3 horizontalRange(4.0, 0.0, 0.0);
    vec3 verticalRange(0.0, 2.0, 0.0);
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            float tx = float(i)/float(nx);
            float ty = float(j)/float(ny);
            ray r(origin, leftLowerCorner + horizontalRange * tx + verticalRange * ty);
            vec3 c = color(r);
            int ir = int(255.99 * c.r());
            int ig = int(255.99 * c.g());
            int ib = int(255.99 * c.b());
            File<< ir << " " << ig << " " << ib << "\n";
        }
    }
    

    File.close();
    return 0;
}    