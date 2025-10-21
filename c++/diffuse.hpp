#pragma once
#include "material.hpp"
#include "vec3.hpp"
#include <algorithm>

vec3 random_unit_vector();

class diffuse : public material {
public:
    vec3 albedo; 

    diffuse(const vec3& a) { this->albedo = a; }

    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation,         
        vec3& scattered_direction  
    ) const override {
        
        scattered_direction = hit_normal + random_unit_vector();
        
        if (scattered_direction.length_squared() < epsilon * epsilon)
            scattered_direction = hit_normal;
        else
            scattered_direction = scattered_direction.normalize();

        attenuation = albedo;

        return true;
    }
};
