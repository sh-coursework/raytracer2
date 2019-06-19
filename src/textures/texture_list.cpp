//
// Created by svhwan on 6/9/2019.
//

#include "textures/texture_list.h"

TextureList::TextureList(std::string scope) : scope_(std::move(scope)),
                                              vector_list_()
{}

TextureList::TextureList(TextureList&& other) noexcept : scope_(),
                                                            vector_list_()
{
  scope_ = std::move(other.scope_);
  vector_list_ = std::move(other.vector_list_);
}

TextureList& TextureList::operator=(TextureList&& other) noexcept {
  if (this != &other) {
    scope_ = std::move(other.scope_);
    vector_list_ = std::move(other.vector_list_);
  }
}


