//
// Created by Steve Hwan on 10/30/18.
//

#include "scene_geometry/box.h"

#include "scene_geometry/transforms/flip_normals.h"
#include "scene_geometry/rect.h"


Box::Box(const Vec3 &p_min, const Vec3 &p_max, Material *material_ptr) {
    p_min_ = p_min;
    p_max_ = p_max;

    std::vector<std::shared_ptr<Hitable>> scene_list;
    scene_list.push_back( std::shared_ptr<Hitable>(
            new XYRect(p_min.x(), p_max.x(),
                    p_min.y(), p_max.y(), p_max.z(), material_ptr)
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new FlipNormals(new XYRect(p_min.x(), p_max.x(),
                                       p_min.y(), p_max.y(), p_min.z(),
                                       material_ptr))
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new XZRect(p_min.x(), p_max.x(),
                       p_min.z(), p_max.z(), p_max.y(), material_ptr)
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new FlipNormals(new XZRect(p_min.x(), p_max.x(),
                                       p_min.z(), p_max.z(), p_min.y(),
                                       material_ptr))
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new YZRect(p_min.y(), p_max.y(),
                       p_min.z(), p_max.z(), p_max.x(), material_ptr)
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new FlipNormals(new YZRect(p_min.y(), p_max.y(),
                                       p_min.z(), p_max.z(), p_min.x(),
                                       material_ptr))
    ));

    hitables_ = new HitableList(scene_list);
}

bool Box::Hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    return hitables_->Hit(r, t_min, t_max, rec);
}


bool Box::BoundingBox(float t_min, float t_max, AABB &box) const {
    box = AABB(p_min_, p_max_);
    return true;
}
