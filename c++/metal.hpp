#pragma once
#include "material.hpp"

vec3 random_in_unit_sphere();
vec3 reflect(const vec3& v_in, const vec3& normal);

class metal : public material {
public:
    vec3 albedo;    // La "teinte" du miroir (ex: or, argent)
    double roughness; // Le "flou" (0.0 à 1.0)

    // Constructeur
    metal(const vec3& a, double r) { 
        this->albedo = a;
        this->roughness = (r < 1.0) ? r : 1.0; 
    }

    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation,         
        vec3& scattered_direction  
    ) const override {
        
        vec3 reflected_direction = reflect(ray_in.normalize(), hit_normal);

        scattered_direction = reflected_direction + (random_in_unit_sphere() * roughness);
        scattered_direction = scattered_direction.normalize(); // N'oublie pas de normaliser

        attenuation = albedo;

        return (scattered_direction.dot(hit_normal) > 0.0);
    }
};
