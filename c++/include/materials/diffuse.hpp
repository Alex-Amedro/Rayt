#pragma once
#include "materials/material.hpp"
#include "core/vec3.hpp"

vec3 random_unit_vector();

// Diffuse (Lambertian) material - scatters light randomly
class diffuse : public material {
public:
    vec3 albedo;

    diffuse(const vec3& a) : albedo(a) {}

    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation,
        vec3& scattered_direction
    ) const override {
        scattered_direction = hit_normal + random_unit_vector();
        
        // Catch degenerate scatter direction
        if (scattered_direction.length_squared() < hittable::epsilon * hittable::epsilon)
            scattered_direction = hit_normal;
        else
            scattered_direction = scattered_direction.normalize();

        attenuation = albedo;
        return true;
    }
};
