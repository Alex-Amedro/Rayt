#pragma once
#include "materials/material.hpp"
#include "core/vec3.hpp"

// Mirror material - perfect reflection (chrome-like)
class mirror : public material {
public:
    vec3 tint;  // Optional color tint (1,1,1 = pure mirror)

    mirror(const vec3& t = vec3(1.0, 1.0, 1.0)) : tint(t) {}

    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation,
        vec3& scattered_direction
    ) const override {
        // Perfect reflection: reflect = in - 2 * dot(in, normal) * normal
        vec3 ray_direction = ray_in.normalize();
        double dot_product = ray_direction.dot(hit_normal);
        scattered_direction = ray_direction - hit_normal * (2.0 * dot_product);
        
        attenuation = tint;
        return true;
    }
};
