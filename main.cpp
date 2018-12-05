#include <fstream>
#include <iostream>
#include "vec3.h"
#include "ray.h"

vec3 color(const ray& r){
    // generating a color according the ray
    float t = 0.5 * (normalize(r.dir()).y() + 1.0);
    return vec3(0.5, 1.0, 0.7) * (1-t) + vec3(1.0, 1.0, 1.0) * t;
}

int main(){
    
    std::fstream File;
    File.open("blender.ppm",std::ios::out);
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