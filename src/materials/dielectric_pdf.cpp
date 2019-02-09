//
// Created by svhwan on 2/7/2019.
//

#include "materials/dielectric_pdf.h"
#include "vec3.h"

DielectricPDF::DielectricPDF(float ref_idx,
                             Vec3 r_in_direction,
                             Vec3 hit_record_normal)
  : r_in_direction_(r_in_direction),
  hit_record_normal_(hit_record_normal),
  ref_idx_(ref_idx) {}

// TODO: Placeholder - haven't implemented a true pdf for dielectric.
float DielectricPDF::Value(const Vec3 &direction) const {
  return 1;
}

Vec3 DielectricPDF::Generate() const {
  Vec3 outward_normal;
  auto reflected = reflect(r_in_direction_, hit_record_normal_);
  float ni_over_nt;
  Vec3 refracted;
  float reflect_prob;
  float cosine;
  if (dot(r_in_direction_, hit_record_normal_) > 0.0f) {
    outward_normal = -hit_record_normal_;
    ni_over_nt = ref_idx_;
    cosine = ref_idx_ * dot(r_in_direction_, hit_record_normal_)
        / r_in_direction_.length();
  } else {
    outward_normal = hit_record_normal_;
    ni_over_nt = 1.0f / ref_idx_;
    cosine = -dot(r_in_direction_, hit_record_normal_) / r_in_direction_.length();
  }
  if (refract(r_in_direction_, outward_normal, ni_over_nt, refracted)) {
    reflect_prob = schlick(cosine, ref_idx_);
  } else {
    reflect_prob = 1.0;
  }
  if (drand48() < reflect_prob) {
    return reflected;
  } else {
    return refracted;
  }

}
