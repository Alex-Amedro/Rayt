// dielectric.hpp
#pragma once

#include "materials/material.hpp"
#include "core/vec3.hpp"
#include <cmath>     
#include <random>   

// Forward declarations - these are implemented in vec3.cpp and ray_color.cpp
bool refract(const vec3& v_in_normalized, const vec3& n, double ior_ratio, vec3& refracted_direction);
double reflectance(double cosine, double ref_idx_ratio);
vec3 reflect(const vec3& v_in, const vec3& normal);

extern std::mt19937 generator; 
extern std::uniform_real_distribution<double> distribution;

class dielectric : public material {
public:
    double ior; 
    vec3 teint;

    dielectric(double index_of_refraction, vec3 teint) : ior(index_of_refraction), teint(teint) {}

    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation,         
        vec3& scattered_direction  
    ) const override {

        attenuation = this->teint;

        vec3 unit_direction = ray_in.normalize();
        
        // Check if ray is entering or exiting
        bool front_face = unit_direction.dot(hit_normal) < 0;
        vec3 outward_normal = front_face ? hit_normal : (vec3(0,0,0) - hit_normal);
        double etai_over_etat = front_face ? (1.0 / ior) : ior;

        // Calculate cos_theta
        double cos_theta = std::min((vec3(0,0,0) - unit_direction).dot(outward_normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        // Check for total internal reflection
        bool cannot_refract = etai_over_etat * sin_theta > 1.0;

        // Use Schlick's approximation for reflectance
        double reflect_prob = reflectance(cos_theta, etai_over_etat);

        if (cannot_refract || reflect_prob > distribution(generator)) {
            // Reflect
            scattered_direction = reflect(unit_direction, outward_normal);
        } else {
            // Refract
            vec3 refracted;
            refract(unit_direction, outward_normal, etai_over_etat, refracted);
            scattered_direction = refracted;
        }

        return true;
    }
};
