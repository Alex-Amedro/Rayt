#pragma once
#include "materials/material.hpp"
#include "core/vec3.hpp"

// Emissive material - pure light source (neon, LED, screen)
// Does NOT scatter light - only emits
// This is the standard behavior for emissive materials in path tracing
class emissive : public material {
public:
    vec3 emission_color;      // Color of emitted light
    double emission_strength; // Intensity multiplier (can be > 1.0 for HDR)

    emissive(const vec3& color, double strength = 1.0) 
        : emission_color(color), emission_strength(strength) {}

    // Emissive materials absorb all incoming rays (act as pure light sources)
    // They only contribute light through direct visibility, not bounces
    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation,
        vec3& scattered_direction
    ) const override {
        // Return false = ray stops here (no scatter)
        // This is correct for light sources - they emit, don't reflect
        return false;
    }

    // Get the emitted light (this is added on every ray hit)
    vec3 emitted() const {
        return emission_color * emission_strength;
    }
};
