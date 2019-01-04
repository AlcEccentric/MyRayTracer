#ifndef PERLIN_H
#define PERLIN_H
#include "vec3.h"
inline float perlin_trilinear_inter(float u, float v, float w, vec3 col[2][2][2]){
    float accum = 0;
    float uu = u*u*(3-2*u);
    float vv = v*v*(3-2*v);
    float ww = w*w*(3-2*w);
    for(int a = 0; a < 2; a++)
        for(int b = 0; b < 2; b++)
            for(int c = 0; c < 2; c++) {
                vec3 weight_vec(u-a, v-b, w-c);
                accum += (a*uu + (1-a)*(1-uu))*
                         (b*vv + (1-b)*(1-vv))*
                         (c*ww + (1-c)*(1-ww))* dot(col[a][b][c], weight_vec);
            }
    return fabs(accum);
}
class perlin{
public:
    float mosaic_noise(const vec3& p) const{
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
        int i = int(4*p.x()) & 255;
        int j = int(4*p.y()) & 255;
        int k = int(4*p.z()) & 255;
        return randNum[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }

    float noise(const vec3& p) const{
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());

        vec3 col[2][2][2];
        
        for(int a = 0; a < 2; a++)
             for(int b = 0; b < 2; b++)
                 for(int c = 0; c < 2; c++)
                    col[a][b][c] = randVec3[ perm_x[(i+a) & 255] ^ perm_y[(j+b) & 255] ^ perm_z[(k+c) & 255]];

        return perlin_trilinear_inter(u, v, w, col);
    };

    float turbo(const vec3& p, int depth = 7) const {
        float accum = 0, weight = 1.0;
        vec3 temp = p;
        for(int i = 0; i < depth; i++) {
            accum += weight * noise(temp);
            weight *= 0.5;
            temp *= 2;
        }
        return fabs(accum);
    }
    static int* perm_x;
    static int* perm_y;
    static int* perm_z;
    static vec3 * randVec3;
    static float * randNum;

};

static vec3* gen_rand_vec3_array(){
    vec3* p = new vec3[256];
    for(int i = 0; i < 256; i++)
        p[i] = normalize(vec3(-1+2*drand48(), -1+2*drand48(), -1+2*drand48()));
    
    return p;
}

static float* gen_rand_float_array(){
    float* p = new float[256];
    for(int i = 0; i < 256; i++)
        p[i] = drand48();
    
    return p;
}

static int* gen_permute_array(){
    int* p = new int[256];
    for(int i = 0; i < 256; i++)
        p[i] = i;
    
    for(int i = 255; i > 0; i--){
        int ind = (i+1) * drand48();
        int tmp = p[ind];
        p[ind] = p[i];
        p[i] = tmp;
    }
    return p;
}
int * perlin::perm_x = gen_permute_array();
int * perlin::perm_y = gen_permute_array();
int * perlin::perm_z = gen_permute_array();
vec3 * perlin::randVec3 = gen_rand_vec3_array();
float * perlin::randNum = gen_rand_float_array();
#endif