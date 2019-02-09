//
// Created by svhwan on 2/7/2019.
//

#include "materials/random_sphere_pdf.h"

#include "ray_engine/ray.h"
#include "vec3.h"

// TODO: Placeholder - haven't implemented isotropic pdf
float RandomSpherePDF::Value(const Vec3 &direction) const {
  return 1;
}

Vec3 RandomSpherePDF::Generate() const {
  return random_in_unit_sphere();
}

