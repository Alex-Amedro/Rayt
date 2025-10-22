#pragma once
#include "core/vec3.hpp"
#include <memory>

class material;

// Abstract base class for ray-intersectable objects
class hittable {
public:
    std::shared_ptr<material> mat;
    static constexpr double epsilon = 0.001;

    virtual ~hittable() = default;

    virtual double hit(const vec3& ray_origin, const vec3& ray_direction) = 0;
    virtual vec3 get_normal(const vec3& hit_point) = 0;
};
