//
// Created by svhwan on 6/9/2019.
//

#ifndef RAYTRACER2_INCLUDE_MATERIALS_TEXTURE_LIST_H_
#define RAYTRACER2_INCLUDE_MATERIALS_TEXTURE_LIST_H_

#include <memory>
#include <string>
#include <vector>

//#include "textures/texture.handle"
class Texture;

class TextureList {
 public:
  explicit TextureList(std::string scope);
  TextureList(TextureList&& other) noexcept;
  TextureList& operator=(TextureList&& other) noexcept;
  void push_back(const std::shared_ptr<Texture> &new_texture_ptr)
    {vector_list_.push_back(new_texture_ptr); }
 private:
  std::string scope_;
  std::vector<std::shared_ptr<Texture>> vector_list_;
};

#endif //RAYTRACER2_INCLUDE_MATERIALS_TEXTURE_LIST_H_
