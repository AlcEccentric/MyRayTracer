#include "vec3.h"
#include <stdlib.h>
#include <iostream>

inline std::istream& operator >>(std::istream &is, vec3& v){
    is >> v.e[0] >> v.e[1] >>v.e[2];
    return is;
} 

inline std::ostream& operator <<(std::ostream &os, vec3& v){
    os<< v.e[0] <<  " " << v.e[1] << " " << v.e[2] <<std::endl;
    return os;
}

inline void vec3::normalize(){
    float oneOverLength = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); 
    e[0] *= oneOverLength;
    e[1] *= oneOverLength;
    e[2] *= oneOverLength;
}
inline vec3& vec3::operator+=(const vec3& v){
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}
inline vec3& vec3::operator-=(const vec3& v){
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}
inline vec3& vec3::operator*=(const vec3& v){
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3& v){
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3& vec3::operator*= (const float t){
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}
inline vec3& vec3::operator/= (const float t){
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

//reload + - between vec
inline vec3 vec3::operator+ (const vec3& v) const{
    return vec3(e[0]+v.e[0], e[1]+v.e[1],e[2]+v.e[2]);
}
inline vec3 vec3::operator- (const vec3& v)const{
    return vec3(e[0]-v.e[0], e[1]-v.e[1], e[2]-v.e[2]);
}

//reload * / for vec3 and float
inline vec3 vec3::operator* (const float f) const{
    return vec3(e[0]*f, e[1]*f, e[2]*f);
}
inline vec3 vec3::operator/ (const float f) const{
    return vec3(e[0]/f, e[1]/f, e[2]/f);
}
// reload == != = between vecs
inline bool vec3::operator== (const vec3& v) const{
    return e[0]==v.e[0] && e[1]==v.e[1] && e[2]==v.e[2]; 
}
inline bool vec3::operator!= (const vec3& v) const{
    return e[0]!=v.e[0] || e[1]!=v.e[1] || e[2]==v.e[2]; 
}
inline vec3& vec3::operator= (const vec3& v){
    e[0] = v.e[0];
    e[1] = v.e[1];
    e[2] = v.e[2];
    return *this;
}
inline float dot(const vec3& v1, const vec3& v2){
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2]; 
}
inline vec3 cross(const vec3& v1, const vec3& v2){
    return vec3(
        (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
        (v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2]),
        (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0])
    );
}