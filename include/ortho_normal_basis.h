//
// Created by svhwan on 2/2/2019.
//

#ifndef RAYTRACER2_ORTHONORMALBASIS_H
#define RAYTRACER2_ORTHONORMALBASIS_H

#include "vec3.h"

class OrthoNormalBasis {
 public:
  OrthoNormalBasis() {}
  inline Vec3 operator[](int i) const {return axis_[i];}
  Vec3 u() const {return axis_[0];}
  Vec3 v() const {return axis_[1];}
  Vec3 w() const {return axis_[2];}
  Vec3 Local(float a, float b, float c) const {return a*u() + b*v() + c*w();}
  Vec3 Local(const Vec3 &a) const {return a.x()*u() + a.y()*v() + a.z()*w();}
  void BuildFromW(const Vec3 &n);

  Vec3 axis_[3];
};

#endif //RAYTRACER2_ORTHONORMALBASIS_H
