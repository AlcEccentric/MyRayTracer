#include <fstream>
#include <iostream>
#include "vec3.h"
#include "ray.h"

bool hit_sphere(const vec3& center, float radius, const ray& r){
    float a = r.dir().squared_length();
    float b = dot(r.ori() - center, r.dir()) * 2;
    float c = (r.ori() - center).squared_length() - radius * radius;
    return (b*b - 4*a*c > 0);
}
vec3 color(const ray& r){
    // generating a color according the ray
    // if the ray hits the sphere generate color in a different way
    if(hit_sphere(vec3(0.0, 0.0, -1.0), 0.5, r))
        return vec3(1.0, 0.0, 0.0);
    // if the ray hits places in the background we generate background color
    float t = 0.58 * (normalize(r.dir()).y() + 1.0);
    t = 1;
    return vec3(0.5, 1.0, 0.7) * (1-t) + vec3(1.0, 1.0, 1.0) * t;
}

int main(){
    
    std::fstream File;
    File.open("sphere.ppm",std::ios::out);
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