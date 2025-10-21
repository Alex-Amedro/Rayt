#pragma once
#include "hittable.hpp"
#include "vec3.hpp"
#include <memory>
#include "material.hpp"

class plane : public hittable {
public:
    vec3 anchor;
    vec3 normal;

    plane(const vec3& anchor, const vec3& normal, std::shared_ptr<material> mat);

    double hit(const vec3& ray_origin, const vec3& ray_direction) override;
    vec3 get_normal(const vec3& hit_point) override;
};
