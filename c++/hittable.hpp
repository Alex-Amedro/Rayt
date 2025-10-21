#pragma once
#include "vec3.hpp"
#include <memory>

static const double epsilon = 0.0001;

class material;  // Forward declaration

class hittable {
public:
    std::shared_ptr<material> mat;

    virtual ~hittable() = default;

    virtual double hit(const vec3& ray_origin, const vec3& ray_direction) = 0;
    virtual vec3 get_normal(const vec3& hit_point) = 0;
};
