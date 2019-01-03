#ifndef BVH_NODE_H
#define BVH_NODE_H
#include "hitable.h"
#include <iostream>

int compare_x_coor(const void * a, const void * b){
    aabb xaabb, yaabb;
    hitable* x = *(hitable**)a;
    hitable* y = *(hitable**)a;
    if(!x->bounding_box(0, 0, xaabb)||!x->bounding_box(0, 0, xaabb)){
        std::cout<< "no bounding box in bvh node constructor!"<< std::endl;
        exit(1);
    }
    if(xaabb.getMin()[0] < yaabb.getMin()[0])
        return -1;
    else 
        return 1;
}

int compare_y_coor(const void * a, const void * b){
    aabb xaabb, yaabb;
    hitable* x = *(hitable**)a;
    hitable* y = *(hitable**)a;
    if(!x->bounding_box(0, 0, xaabb)||!x->bounding_box(0, 0, xaabb)){
        std::cout<< "no bounding box in bvh node constructor!"<< std::endl;
        exit(1);
    }
    if(xaabb.getMin()[1] < yaabb.getMin()[1])
        return -1;
    else 
        return 1;
}

int compare_z_coor(const void * a, const void * b){
    aabb xaabb, yaabb;
    hitable* x = *(hitable**)a;
    hitable* y = *(hitable**)a;
    if(!x->bounding_box(0, 0, xaabb)||!x->bounding_box(0, 0, xaabb)){
        std::cout<< "no bounding box in bvh node constructor!"<< std::endl;
        exit(1);
    }
    if(xaabb.getMin()[2] < yaabb.getMin()[2])
        return -1;
    else 
        return 1;
}

class bvhNode : public hitable{
public:
    hitable * left;
    hitable * right;
    aabb _box;

    bvhNode(){};
    bvhNode(hitable ** l, int n, float t0, float t1);
    virtual bool hit(const ray& r, float t_min, float t_max, hit_info& h_info) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;

};

bool bvhNode::bounding_box(float time0, float time1, aabb& box) const{
    box = _box;
    return true;
}

bool bvhNode::hit(const ray& r, float t_min, float t_max, hit_info& h_info) const {
    if(_box.hit(r, t_min, t_max)){
        hit_info h0, h1;
        bool hit0 = left->hit(r, t_min, t_max, h0);
        bool hit1 = right->hit(r, t_min, t_max, h1);
        if(hit0 && hit1){
            if(h0.t < h1.t){
                h_info = h0;
            }else{
                h_info = h1;
            }
            return true;
        }else if(hit0){
            h_info = h0;
            return true;
        }else if(hit1){
            h_info = h1;
            return true;
        }else{
            return false;
        }
    }else   
        return false;
}

bvhNode::bvhNode(hitable ** l, int n, float time0, float time1){
    int axis = drand48() * 3;
    if(axis == 0)
        qsort(l, n, sizeof(hitable *), compare_x_coor);
    else if(axis == 1)
        qsort(l, n, sizeof(hitable *), compare_y_coor);
    else if(axis == 2)
        qsort(l, n, sizeof(hitable *), compare_z_coor);

    if(n == 1){
        left = l[0];
        right = l[0];
    }else if(n == 2){
        left = l[0];
        right = l[1];
    }else{
        left = new bvhNode(l, n/2, time0, time1);
        right = new bvhNode(l + n/2, n - n/2, time0, time1);
    }
    aabb raabb, laabb;
    if(!left->bounding_box(time0, time1, laabb) || !right->bounding_box(time0, time1, raabb)){
        std::cout<< "no bounding box in bvh node constructor!"<< std::endl;
        exit(1);
    }
    _box = surrounding_box(laabb, raabb);

}

#endif