#pragma once
#include "material.hpp"

vec3 random_in_unit_sphere();
vec3 reflect(const vec3& v_in, const vec3& normal);

// Metal material - reflects light with optional roughness
class metal : public material {
public:
    vec3 albedo;
    double roughness;

    metal(const vec3& a, double r) : albedo(a), roughness((r < 1.0) ? r : 1.0) {}

    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation,
        vec3& scattered_direction
    ) const override {
        vec3 reflected_direction = reflect(ray_in.normalize(), hit_normal);
        scattered_direction = (reflected_direction + (random_in_unit_sphere() * roughness)).normalize();

        attenuation = albedo;
        return (scattered_direction.dot(hit_normal) > 0.0);
    }
};
