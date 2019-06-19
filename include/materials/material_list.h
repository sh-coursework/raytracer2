//
// Created by svhwan on 6/9/2019.
//

#ifndef RAYTRACER2_INCLUDE_MATERIALS_MATERIAL_LIST_H_
#define RAYTRACER2_INCLUDE_MATERIALS_MATERIAL_LIST_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

//#include "materials/material.h"
class Material;

class MaterialList {
 public:
  explicit MaterialList(std::string scope);
  MaterialList(MaterialList&& other) noexcept;
  MaterialList& operator=(MaterialList&& other) noexcept;
  void push_back(const std::shared_ptr<Material> &new_material_ptr)
    {vector_list_.push_back(new_material_ptr); }
 private:
  std::string scope_;
  std::vector<std::shared_ptr<Material>> vector_list_;
};

#endif //RAYTRACER2_INCLUDE_MATERIALS_MATERIAL_LIST_H_
