//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_VEC3_H
#define RAYTRACER1_VEC3_H

#include <cmath>
#include <cstdlib>
#include <iostream>


class Vec3 {
public:
    Vec3() = default;
    Vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }

    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    // Do I have to const the return value?  Is this now a right hand ref?
    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline Vec3& operator+=(const Vec3 &v2);
    inline Vec3& operator-=(const Vec3 &v2);
    inline Vec3& operator*=(const Vec3 &v2);
    inline Vec3& operator/=(const Vec3 &v2);
    inline Vec3& operator*=(float t);
    inline Vec3& operator/=(float t);

    inline float length() const {
        return sqrtf(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }
    inline float squared_length() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    inline void make_unit_vector();

    // This e is the ONLY member of the Vec3 class.
    float e[3];
};

inline std::istream& operator>>(std::istream &is, Vec3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline void Vec3::make_unit_vector() {
    float k = 1.0f / sqrtf(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return {v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]};
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return {v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]};
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return {v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]};
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
    return {v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]};
}

inline Vec3 operator*(float t, const Vec3 &v) {
    return {t*v.e[0], t*v.e[1], t*v.e[2]};
}

inline Vec3 operator/(const Vec3 &v, float t) {
    return {v.e[0]/t, v.e[1]/t, v.e[2]/t};
}

inline Vec3 operator*(const Vec3 &v, float t) {
    return {t*v.e[0], t*v.e[1], t*v.e[2]};
}

inline float dot(const Vec3 &v1, const Vec3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
    return {(v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
            (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
            (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0])};
}

inline Vec3& Vec3::operator+=(const Vec3 &v2) {
    e[0] += v2.e[0];
    e[1] += v2.e[1];
    e[2] += v2.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v2) {
    e[0] *= v2.e[0];
    e[1] *= v2.e[1];
    e[2] *= v2.e[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v2) {
    e[0] /= v2.e[0];
    e[1] /= v2.e[1];
    e[2] /= v2.e[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v2) {
    e[0] -= v2.e[0];
    e[1] -= v2.e[1];
    e[2] -= v2.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(float t) {
    auto k = 1.0f / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline Vec3 unit_vector(const Vec3 &v) {
    return v / v.length();
}

Vec3 reflect(const Vec3& v, const Vec3& n);

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted);

float schlick(float cosine, float ref_idx);

Vec3 random_in_unit_disk();

Vec3 random_in_unit_sphere();

Vec3 random_cosine_direction();

#endif //RAYTRACER1_VEC3_H
