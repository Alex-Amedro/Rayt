#pragma once
#include "core/vec3.hpp"
#include "geometry/hittable.hpp"
#include "materials/material.hpp"
#include <memory>

// Sphere primitive
class sphere : public hittable {
public:
    vec3 origin;
    double radius;

    sphere(vec3 origin, double radius, std::shared_ptr<material> material);

    double hit(const vec3& ray_origin, const vec3& ray_direction) override;
    vec3 get_normal(const vec3& hit_point) override;
};


