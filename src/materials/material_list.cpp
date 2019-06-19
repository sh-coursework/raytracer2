//
// Created by svhwan on 6/9/2019.
//

#include "materials/material_list.h"

MaterialList::MaterialList(std::string scope) : scope_(std::move(scope)),
                                                vector_list_()
{}

MaterialList::MaterialList(MaterialList&& other) noexcept : scope_(),
                                                            vector_list_()
{
  scope_ = std::move(other.scope_);
  vector_list_ = std::move(other.vector_list_);
}

MaterialList& MaterialList::operator=(MaterialList&& other) noexcept {
  if (this != &other) {
    scope_ = std::move(other.scope_);
    vector_list_ = std::move(other.vector_list_);
  }
}

