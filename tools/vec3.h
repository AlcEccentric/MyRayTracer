#ifndef VEC3_H
#define VEC3_H
#include <math.h>
class vec3 {
public:
    float e[3];
    vec3() {};
    vec3(float e0, float e1, float e2){ e[0] = e0; e[1] = e1; e[2] = e2;}
    vec3(const vec3& v){ e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2];}
    vec3(float n){e[0] = e[1] = e[2] = n;}
    inline float x() const {return e[0];}
    inline float y() const {return e[1];}
    inline float z() const {return e[2];}
    inline float r() {return e[0];}
    inline float g() {return e[1];}
    inline float b() {return e[2];}
    // reload positive and negtive
    inline const vec3& operator+() const {return *this;}
    inline vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
    // reload indexing
    // I cannot understand why two versions for indexing
    inline float operator[](int i) const {return e[i];}
    inline float& operator[](int i) {return e[i];}
    // reload +=/-=/*=//= between vec3s
    inline vec3& operator+=(const vec3& v);
    inline vec3& operator-=(const vec3& v);
    inline vec3& operator*=(const vec3& v);
    inline vec3& operator/=(const vec3& v);
    // reload *=//= for vec3 and float
    inline vec3& operator*= (const float t);
    inline vec3& operator/= (const float t);

    //reload + - between vec
    inline vec3 operator+ (const vec3& v) const;
    inline vec3 operator- (const vec3& v) const;

    //reload * / for vec3 and float
    inline vec3 operator* (const float f) const;
    inline vec3 operator/ (const float f) const;

    // reload == != = between vecs
    inline bool operator== (const vec3& v) const;
    inline bool operator!= (const vec3& v) const;
    inline vec3& operator= (const vec3& v);

    inline float length() const{
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    inline float squared_length() const{
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    inline void normalize();
};

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

inline vec3 normalize(const vec3& v){
    float oneOverLength = 1.0 / sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]); 
    return vec3(
        v.e[0] * oneOverLength,
        v.e[1] * oneOverLength,
        v.e[2] * oneOverLength
    );
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
inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}
inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator* (const float f, const vec3& v) {
    return vec3(v.e[0]*f, v.e[1]*f, v.e[2]*f);
}
inline vec3 operator/ (const float f, const vec3& v) {
    return vec3(v.e[0]/f, v.e[1]/f, v.e[2]/f);
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

#endif