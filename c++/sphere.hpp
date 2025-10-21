#pragma once
#include "vec3.hpp"
#include "hittable.hpp"
#include <algorithm>
#include <memory>
#include "material.hpp"

class sphere : public hittable {
public:
    vec3 origin;
    double radius;

    sphere(vec3 origin, double radius, std::shared_ptr<material> material);

    double hit(const vec3& ray_origin, const vec3& ray_direction) override;
    vec3 get_normal(const vec3& hit_point) override;
};


