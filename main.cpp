#include <fstream>
#include <iostream>
#include "ray.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "float.h"

vec3 backgroundColor = vec3(0.5, 0.7, 1.0);
vec3 randend_in_unit_sphere(hit_info &info){
    vec3 endpoint;
    do
    {
        endpoint = vec3(drand48(), drand48(), drand48()) * float(2.0) - vec3(1.0, 1.0, 1.0);
    } while (endpoint.squared_length() >= 1.0);
    return endpoint;

}
// the meaning of this function
// get color of the light sent by "world" from "ray"'s direction 
vec3 color(const ray& r, hitable * world){
    // generating a color according the ray
    // if the ray hits the sphere generate color in a different way
    hit_info info;
    if(world->hit(r, 0.0, MAXFLOAT, info)){
        // vec3 randDir = info.p + info.n + randend_in_unit_sphere(info) - info.p;
        vec3 randDir = info.n + randend_in_unit_sphere(info);
        return float(0.5) * color(ray(info.p , randDir), world);
    }else{
        // if the ray hits places in the background we generate background color
        // the normalized coordinate is in [-1.0, 1.0]
        // +1 and *0.5 make t fall in [0.0, 1.0]
        float k = 0.5 * (normalize(r.dir()).y() + 1.0);
        return vec3(1.0, 1.0, 1.0) * (1.0-k) + backgroundColor * k;   
    }

}

int main(){
    
    std::fstream File;
    File.open("shadingWorld.ppm",std::ios::out);
    if(!File){
        File.close();
        std::cout<< "fail to open file\n";
    } 

    
    int nx = 400, ny = 200, ns = 200;
    File<< "P3\n" << nx << " " << ny << "\n" << "255\n";
    camera cam;
    hitable * list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0,-100.5,-1), 100);
    hitable* world = new hitable_list(list, 2);
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            // Antialiasing
            // for each hit rigeon, emit 200 random thiner rays
            // get color of each ray
            // average the rgbs of all 200 ray to get antialiasing color of the origin hit point
            vec3 c = vec3(0.0, 0.0, 0.0);
            for(int s = 0; s < ns; s++){
                float tx = float(i + drand48())/float(nx);
                float ty = float(j + drand48())/float(ny);
                ray r = cam.get_ray(tx, ty);
                c += color(r, world);
            }
            c /= ns;
            // why need gamma correction:
            // 人眼对光强度的感知和光强的变化不成线性关系
            
            // the image viewer assumes that image is gamma corrected
            // so we should do gamma correction here
            // use gamma 2
            // we should power the rgb value from 0 to 1 with 1/2 (i.e. power(r, 0.5))
            c = vec3(sqrt(c.x()), sqrt(c.y()), sqrt(c.z()));
            int ir = int(255.99 * c.r());
            int ig = int(255.99 * c.g());
            int ib = int(255.99 * c.b());
            File<< ir << " " << ig << " " << ib << "\n";
        }
    }
    

    File.close();
    return 0;
}    