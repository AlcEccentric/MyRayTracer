#ifndef VEC3_H
#define VEC3_H
#include <math.h>
class vec3 {
public:
    float e[3];
    vec3() {};
    vec3(float e0, float e1, float e2){ e[0] = e0; e[1] = e1; e[2] = e2;}
    inline float x() {return e[0];}
    inline float y() {return e[1];}
    inline float z() {return e[2];}
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
#endif