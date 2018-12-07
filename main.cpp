#include <fstream>
#include <iostream>
#include "ray.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielect.h"
#include "float.h"

vec3 backgroundColor = vec3(78,179,211) / float(255.0);
vec3 randend_in_unit_sphere(){
    vec3 endpoint;
    do
    {
        endpoint = vec3(drand48(), drand48(), drand48()) * float(2.0) - vec3(1.0, 1.0, 1.0);
    } while (endpoint.squared_length() >= 1.0);
    return endpoint;

}
bool refract(const vec3& in, const vec3& n, float ni_over_nt, vec3& refracted){
    vec3 u_in = normalize(in);
    float cosi = -dot(u_in, n);
    float sint2 =  (ni_over_nt * ni_over_nt) * (1 - cosi*cosi);
    if(sint2 >= 1){
        return false;
    }else{
        refracted = -n * sqrt(1 - sint2) + (cosi * n + u_in) * ni_over_nt;
        return true;
    }
}
vec3 reflect(const vec3& in, const vec3& n){
    // n can points inside or outside, there is no influence. 
    // Because in and its projection on n always points to the same orientation no matter what orientation n is.
    // dot(in, info.n)*info.n = the projection of in on info.n
    // because in and its projection on n points to the same orientation, we need reverse the projection
    // that is why add a minus sign
    return in - 2*dot(in, n)*n;
}
// the meaning of this function
// get color of the light sent by "world" from "ray"'s direction 
// depth count the number of reflecions have been considered
// we dont consider too many times of reflection
vec3 color(const ray& r, hitable * world, int depth){
    // generating a color according the ray
    // if the ray hits the sphere generate color in a different way
    hit_info r_hit_info;
    // set 0.0 to 0.001 to get rid of shadow acne
    // this is caused by the reflected ray from very near surface which may have t = 0.000001
    // in real world, these very near surfaces are tiny areas where two objects touch each other
    // There should not be ray reflection in these areas.
    // The way is to set 0.0 to 0.001 here.
    // neglect color of reflecting rays from too near suface
    if(world->hit(r, 0.001, MAXFLOAT, r_hit_info)){
        // vec3 randDir = info.p + info.n + randend_in_unit_sphere(info) - info.p;
        vec3 attenu;
        ray scattered;
        // depth count the number of reflecions have been considered
        // consider less than 50 times of reflection
        // if the no. of reflection is greater than 50,
        // we treat this case as no reflection (i.e., not scattered)
        if(r_hit_info.mat_ptr->scatter(r, r_hit_info, attenu, scattered) && depth < 50){
            vec3 rgb = color(scattered, world, depth + 1);
            return vec3(
                attenu.x() * rgb.x(),
                attenu.y() * rgb.y(),
                attenu.z() * rgb.z()
            );
        }else{
            // ray does not get scattered means no color will be reflected from the surface
            // return black
            return vec3(0.0, 0.0, 0.0);
        }
        
        
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
    hitable * list[5];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.2));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielect(1.7));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielect(1.7));
    hitable* world = new hitable_list(list, 5);
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
                c += color(r, world, 0);
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