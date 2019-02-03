//
// Created by svhwan on 2/2/2019.
//

#include <cmath>

#include "ortho_normal_basis.h"

void OrthoNormalBasis::BuildFromW(const Vec3 &n) {
  axis_[2] = unit_vector(n);
  Vec3 a;
  if (fabs(w().x()) > 0.9)
    a = Vec3(0, 1, 0);
  else
    a = Vec3(1, 0, 0);
  axis_[1] = unit_vector(cross(w(), a));
  axis_[0] = cross(w(), v());
}