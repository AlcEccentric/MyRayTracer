#include <fstream>
#include <iostream>
#include <cmath>
#include "camera/ray.h"
#include "camera/camera.h"
#include "hitable/hitablelist.h"
#include "hitable/sphere.h"
#include "hitable/mov_sphere.h"
#include "hitable/xy_rect.h"
#include "hitable/xz_rect.h"
#include "hitable/yz_rect.h"
#include "hitable/bvh_node.h"
#include "hitable/rev_normal_obj.h"
#include "hitable/box.h"
#include "hitable/rotated_x.h"
#include "hitable/rotated_y.h"
#include "hitable/rotated_z.h"
#include "hitable/translated.h"
#include "hitable/box.h"
#include "material/material.h"
#include "material/lambertian.h"
#include "material/metal.h"
#include "material/dielect.h"
#include "material/phong.h"
#include "material/diffuse_light.h"
#include "texture/constant_texture.h"
#include "texture/checker_texture.h"
#include "texture/noise_texture.h"
#include "texture/mosaic_texture.h"
#include "texture/marble1_texture.h"
#include "texture/marble2_texture.h"
#include "texture/image_texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"
#include "float.h"
#include "tools/vec3.h"

struct phongInfo{
    float specExpo;
    float Kd;
    vec3 viewDir;
};

struct boxInfo{
    float front; 
    float back;
    float top;
    float bottom; 
    float left;
    float right;
};
vec3 backgroundColor = vec3(0.5, 0.5, 0.5) ;
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
        if(r_hit_info.mat_ptr->scatter(r, r_hit_info, attenu, scattered) && depth < 40){
            if(r_hit_info.mat_ptr->isPhong()){
                phong* ph = (phong*)r_hit_info.mat_ptr;
                vec3 diffuse = vec3(0);
                vec3 specular = vec3(0);
                for(int i = 0; i < ph->lightsNum; i++){
                    vec3 lightDir = normalize(ph->lightDirs[i]);
                    float lightDist = ph->lightDists[i];
                    float intensity = ph->intensities[i];
                    ray shadDetect;
                    ph->phongScatter(r, r_hit_info, lightDir, attenu, shadDetect);
                    hit_info shadInfo;
                    int vis;
                    if(world->hit(shadDetect, 0.001, 5, shadInfo))
                        vis = 0;
                    else
                        vis = 1;
                    
                    diffuse += vis * attenu * intensity * std::max(0.0f, dot(r_hit_info.n, -lightDir));
                    vec3 reflected = reflect(lightDir, r_hit_info.n);
                    specular += vis * intensity * std::pow(std::max(0.0f, dot(reflected, -ph->viewDir)), ph->n);
                }
                
                return diffuse * ph->Kd + specular * (1 - ph->Kd);
                
            }
            vec3 rgb = color(scattered, world, depth + 1);
            // the result = scattered light + emitted light
            // return attenu * rgb + r_hit_info.mat_ptr->emitted(r_hit_info.u, r_hit_info.v, r_hit_info.p);
            return attenu * rgb ;
        }else{
            // ray does not get scattered means no color will be reflected from the surface
            // only return emitted light
            return r_hit_info.mat_ptr->emitted(r_hit_info.u, r_hit_info.v, r_hit_info.p);
            // return vec3(0, 0, 0);
        }
        
        
    }else{
        // if the ray hits places in the background we generate background color
        // the normalized coordinate is in [-1.0, 1.0]
        // +1 and *0.5 make t fall in [0.0, 1.0]

        float k = 0.5 * (normalize(r.dir()).y() + 1.0);
        return vec3(1, 1, 1) * (1.0-k) + backgroundColor * k;   
    
        // return vec3(0.0, 0.0, 0.0);
    }

}

hitable_list *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    texture * checker0 = new constantTexture(vec3(0.2, 0.3, 0.1));
    texture * checker1 = new constantTexture(vec3(0.9, 0.9, 0.9));
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(new checkerTexture(checker0, checker1)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48()); 
            if ((center-vec3(3,0.2,0)).length() > 0.9) { 
                if (choose_mat < 0.3) {
                    list[i++] = new sphere(center, 0.2, new lambertian(
                        new constantTexture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())))
                    );
                }
                else if (choose_mat < 0.45) {  // diffuse
                    list[i++] = new movSphere(center, center + vec3(0.0, drand48()*0.5, 0.0), 0.0, 1.0, 0.2, new lambertian(
                        new constantTexture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())))
                    );
                }
                else if (choose_mat < 0.75) { // metal
                    list[i++] = new sphere(center, 0.2,
                            new metal(new constantTexture(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48()))),  0.4*drand48()));
                }
                else if(choose_mat < 0.9){  // glass
                    list[i++] = new sphere(center, 0.2, new dielect(1.7));
                }
                else{// bubble
                    list[i++] = new sphere(center, 0.2, new dielect(1.7));
                    list[i++] = new sphere(center, -0.2, new dielect(1.7));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(-3, 1, 0), 1.0, new dielect(1.7));
    list[i++] = new sphere(vec3(-1, 1, 0), 1.0, new lambertian(new constantTexture(vec3(0.8, 0.2, 0.4))));
    list[i++] = new sphere(vec3(1, 1, 0), 1.0, new dielect(1.7));
    list[i++] = new sphere(vec3(1, 1, 0), -1.0, new dielect(1.7));
    list[i++] = new sphere(vec3(3, 1, 0), 1.0, new metal(new constantTexture(vec3(0.7, 0.6, 0.5)), 0.0));

    return new hitable_list(list,i);
}
hitable_list *two_spheres() {
    hitable **list = new hitable*[3];

    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("earthmap.jpg", &width, &height, &nrChannels, 0);
    
    texture* marble1Tex = new marble1Texture(1.0, vec3(1.0), 0.5);
    // texture* imgTex = new imageTexture(data, width, height);
    texture* marble2Tex = new marble2Texture(1.0, vec3(1.0), 0.5);
    texture* mosaicTex = new mosaicTexture(1.0, vec3(0.2, 0.8, 0.3));

    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(mosaicTex));
    list[1] = new sphere(vec3(1, 2, 0), 2, new lambertian(marble1Tex));
    // list[1] = new sphere(vec3(1, 2, 0), 2, new lambertian(imgTex));
    list[2] = new sphere(vec3(-1, 2, 0), 2, new lambertian(marble2Tex));

    return new hitable_list(list, 3);

}
hitable_list *img_sphere() {
    hitable **list = new hitable*[1];

    int width, height, nrChannels;
    unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
    
    // texture* marble1Tex = new marble1Texture(1.0, vec3(1.0), 0.5);
    texture* imgTex = new imageTexture(data, width, height);
    list[0] = new sphere(vec3(0, 0, 0), 2, new lambertian(imgTex));

    return new hitable_list(list, 1);
}
hitable_list *dark_room() {
    hitable **list = new hitable*[6];

    texture* whiteLight = new constantTexture(vec3(4));
    texture* marble1Tex = new marble1Texture(1.0, vec3(1.0), 0.5);
    texture* marble2Tex = new marble2Texture(1.0, vec3(1.0), 0.5);
    texture* mosaicTex = new mosaicTexture(1.0, vec3(0.2, 0.8, 0.3));

    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(mosaicTex));
    list[1] = new sphere(vec3(1, 2, 0), 2, new lambertian(marble1Tex));
    list[2] = new sphere(vec3(-1, 2, 0), 2, new lambertian(marble2Tex));
    list[3] = new xyRect(-1, 1, 1, 3, 3, new diffuseLight(whiteLight));
    list[4] = new xzRect(-1, 1, -1, 1, 5, new diffuseLight(whiteLight));
    list[5] = new yzRect(1, 3, -1, 1, 4, new diffuseLight(whiteLight));
    return new hitable_list(list, 6);
}
hitable_list *phong_sphere(vec3& viewDir) {
    hitable **list = new hitable*[2];
    texture* blue = new constantTexture(vec3(0.1, 0.2, 0.5));
    texture* mosaicTex = new mosaicTexture(1.0, vec3(0.2, 0.8, 0.3));

    int lightsNum = 2;
    vec3 *lightDirs = new vec3[lightsNum];
    float *lightDists = new float[lightsNum];
    float *lightInts = new float[lightsNum];
    lightDirs[0] = vec3(-1);
    lightDists[0] = 10;
    lightDirs[1] = vec3(-1, 0, -1);
    lightDists[1] = 10;
    lightInts[0] = 1;
    lightInts[1] = 0.45;
    list[1] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(mosaicTex));
    list[0] = new sphere(vec3(2, 2, 0), 2, new phong(blue, lightDirs, lightsNum, lightDists, viewDir, lightInts, 0.6, 15));

    return new hitable_list(list, 2);
}
hitable_list *cornell_box(const boxInfo& bi, const phongInfo& pi) {

    vec3 viewDir = pi.viewDir;
    float specExpo = pi.specExpo;
    float Kd = pi.Kd;

    float front = bi.front; 
    float back = bi.back; 
    float top = bi.top; 
    float bottom = bi.bottom; 
    float left = bi.left; 
    float right = bi.right;

    hitable **list = new hitable*[14];
    vec3 span(left - right, top - bottom, back - front);
    
    texture* light = new constantTexture(vec3(1));

    texture * checker0 = new constantTexture(vec3(0.2, 0.3, 0.1));
    texture * checker1 = new constantTexture(vec3(0.9, 0.9, 0.9));
    texture* marble1Tex = new marble1Texture(0.2, vec3(1.0), 0.8);
    texture* mosaicTex = new mosaicTexture(0.1, vec3(0.2, 0.8, 0.3));
    texture* red = new constantTexture(vec3(0.65, 0.05, 0.05));
    texture* orange = new constantTexture(vec3(190/255.0, 138/255.0, 89/255.0));
    texture* green = new constantTexture(vec3(0.12, 0.45, 0.15));
    texture* yellow = new constantTexture(vec3(0.8, 0.6, 0.2));
    texture* blue = new constantTexture(vec3(0.1, 0.2, 0.5));
    texture* white = new constantTexture(vec3(0.73));

    // back
    list[0] = new revNormObj(new xyRect(right, left, bottom, top, back, new lambertian(new checkerTexture(checker0, checker1, 0.5))));
    // top   
    list[1] = new revNormObj(new xzRect(right, left, front, back, top, new lambertian(marble1Tex)));
    // bottom
    list[2] = new xzRect(right, left, front, back, bottom, new lambertian(mosaicTex));
    // left
    list[3] = new revNormObj(new yzRect(bottom, top, front, back, left, new lambertian(red)));
    //right
    list[4] = new yzRect(bottom, top, front, back, right, new lambertian(green));
    // up light
    list[5] = new xzRect(right + 0.35*(left-right), right + 0.65*(left-right), front + 0.25*(back-front), front + 0.75*(back-front), top - 0.1, new diffuseLight(light));
    // front box
    list[6] = new translated(
                    new rotatedY(
                        new box(vec3(right, bottom, front), 
                                vec3(0.16) * span + vec3(right, bottom, front), 
                                new lambertian(white)), 
                        -20), // rotate angle
                    vec3(0.2, 0, 0.2) * span); // translate offset

    // front left dielect sphere
    float radius = 0.12 * fabs(span.x());
    list[7] = new translated(
                    new sphere(vec3(right, bottom, front) + vec3(radius), radius, new dielect(1.7)), 
                    vec3(0.65, 0, 0.2) * span); // translate offset

     // front right bubble over the box    
    radius = 0.11 * fabs(span.x());
    list[8] = new translated(
                    new sphere(vec3(right, bottom, front) + vec3(radius), radius, new dielect(1.4)),
                    vec3(0.2, 0.4, 0.2) * span); // translate offset 
    radius = 0.1 * fabs(span.x());
    list[9] = new translated(
                    new sphere(vec3(right, bottom, front) + vec3(radius), -radius, new dielect(1.4)),
                    vec3(0.2, 0.4, 0.2) * span); // translate offset              
    // back box
    list[10] = new translated(
                    new rotatedY(
                        new box(vec3(right, bottom, front), 
                                vec3(0.15, 0.3, 0.15) * span + vec3(right, bottom, front), 
                                new lambertian(white)), 
                        30), // rotate angle
                    vec3(0.6, 0, 0.6) * span); // translate offset
 
    // back metal sphere on the back box
    radius = 0.12 * fabs(span.y());
    list[11] = new translated(
                    new sphere(vec3(right, bottom, front) + vec3(radius), radius, new metal(yellow, 0.5)), 
                    vec3(0.6, 0.3, 0.6) * span); // translate offset
    
    // front middle floating moving box
    list[12] = new translated(
                    new movSphere(vec3(right, bottom, front) + vec3(radius), vec3(right, bottom, front) + vec3(radius) + vec3(0, radius, 0), 0.0, 1.0, radius, new lambertian(orange)), 
                    vec3(0.6, 0.5, 0.25) * span); // translate offset

    // back middle phong sphere
    radius = 0.12 * fabs(span.y());
    int lightsNum = 1;
    vec3 *lightDirs = new vec3[lightsNum];
    float *lightDists = new float[lightsNum];
    float *lightInts = new float[lightsNum];
    vec3 phongCenter = vec3(right, bottom, front) + vec3(radius) + vec3(0.3, 0, 0.6) * span;
    vec3 lightCenter = vec3((right+left)/2, top - 0.1, (back+front)/2);
    lightDists[0] = (phongCenter - lightCenter).length() - 1;
    lightDirs[0] = normalize(phongCenter - lightCenter);
    lightCenter = phongCenter + vec3(0, (top-bottom)/3, -2 * (phongCenter.z() - front));
    lightDists[1] = (phongCenter - lightCenter).length() - 1;
    lightDirs[1] = normalize(phongCenter - lightCenter);
    lightInts[0] = 1;
    lightInts[1] = 0.2;
    list[13] = new sphere(vec3(right, bottom, front) + vec3(radius) + vec3(0.3, 0, 0.6) * span, radius, 
                    new phong(blue, lightDirs, lightsNum, lightDists, viewDir, lightInts, Kd, specExpo)); // translate offset
        
    
    return new hitable_list(list, 14);

}
int main(){
    
    std::fstream File;
    File.open("shadingWorld.ppm",std::ios::out);
    if(!File){
        File.close();
        std::cout<< "fail to open file\n";
    } 

    
    int nx = 500, ny = 500, ns = 30;
    File<< "P3\n" << nx << " " << ny << "\n" << "255\n";
    boxInfo bi;
    bi.back = 80;
    bi.front = 0;
    bi.left = 50;
    bi.right = 0;
    bi.top = 50;
    bi.bottom = 0;
    vec3 lookfrom((bi.left+bi.right)/2, (bi.top+bi.bottom)/2, bi.front - 1 * (bi.back - bi.front));
    vec3 lookat((bi.left+bi.right)/2, (bi.top+bi.bottom)/2, bi.front);
    // vec3 lookfrom(13,2,8);
    // vec3 lookat(0,2,0);
    vec3 vup(0,1,0);
    float v_fov = 40;
    float dist_to_focus_screen = (lookfrom - lookat).length();
    float aperture = 0.0;
    camera cam(lookfrom, lookat, vup, v_fov, float(nx)/float(ny), aperture, dist_to_focus_screen, 0.0, 1.0);
    // hitable * list[5];
    // list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    // list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    // list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.2));
    // list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielect(1.7));
    // list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielect(1.7));
    // hitable* world = new hitable_list(list, 5);
    // hitable_list* worldlist = random_scene();
    // hitable* world = new bvhNode(worldlist->list, worldlist->list_size, 0.0, 1.0);
    // hitable_list* worldlist = two_spheres();
    // hitable*  world = new bvhNode(worldlist->list, worldlist->list_size, 0, 0);
    // hitable_list* worldlist = img_sphere();
    // hitable*  world = new bvhNode(worldlist->list, worldlist->list_size, 0, 0);
    // hitable_list* worldlist = dark_room();
    // hitable*  world = new bvhNode(worldlist->list, worldlist->list_size, 0, 0);
    // hitable_list* worldlist = phong_sphere(viewDir);
    // hitable*  world = new bvhNode(worldlist->list, worldlist->list_size, 0, 0);
    phongInfo pi;
    pi.viewDir = normalize(lookat - lookfrom);
    pi.specExpo = 10;
    pi.Kd = 0.6;
    hitable_list* worldlist = cornell_box(bi, pi);
    hitable*  world = new bvhNode(worldlist->list, worldlist->list_size, 0, 0);
    int count = 0;
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            // No AntiAliasing
            // float tx = float(i)/float(nx);
            // float ty = float(j)/float(ny);
            // vec3 c(0.0, 0.0, 0.0);
            // int point_num_on_lens = 10;
            // for(int i = 0; i < point_num_on_lens; i++)
            // {
            //     /* code */
            //     ray r = cam.get_ray(tx, ty);
            //     c += color(r, world, 0);
            // }
            // c /= point_num_on_lens;
            
            
            
            // Antialiasing
            // for each hit rigeon, emit 200 random thiner rays
            // get color of each ray
            // average the rgbs of all 200 ray to get antialiasing color of the origin hit point
           
            vec3 c = vec3(0.0, 0.0, 0.0);
            int point_num_on_lens = 2;
            for(int k = 0; k < point_num_on_lens; k++)
            {
                for(int s = 0; s < ns; s++){
                    float tx = float(i + drand48())/float(nx);
                    float ty = float(j + drand48())/float(ny);
                    ray r = cam.get_ray(tx, ty);
                    c += color(r, world, 0);
                }
            }
            c /= (ns*point_num_on_lens);
           
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
            count++;
            fprintf(stderr, "\r%3d%c", uint32_t(count / (float)(nx*ny) * 100), '%');
        }
    }
    

    File.close();
    return 0;
}    