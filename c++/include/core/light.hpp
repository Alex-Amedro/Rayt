#pragma once
#include "vec3.hpp"
#include <cmath>

class PointLight {
public:
    vec3 position;
    vec3 color;
    float intensity;

    PointLight(const vec3& pos, const vec3& col, float intens)
        : position(pos), color(col), intensity(intens) {}

    // Get light direction from a point (normalized)
    vec3 direction_from(const vec3& point) const {
        return (position - point).normalize();
    }

    // Get distance from point to light
    double distance_from(const vec3& point) const {
        return std::sqrt((position - point).length_squared());
    }

    // Get illumination at a point (with inverse square falloff)
    vec3 get_illumination(const vec3& point) const {
        double distance = distance_from(point);
        double distance_sq = distance * distance;
        double falloff = intensity / (distance_sq + 0.0001);
        return color * falloff;
    }
};

// ============================================================================
// DIRECTIONAL LIGHT (for sun / uniform lighting)
// ============================================================================
class DirectionalLight {
public:
    vec3 direction;  // Direction FROM which light comes (normalized)
    vec3 color;
    float intensity;

    DirectionalLight(const vec3& dir, const vec3& col, float intens)
        : direction(dir.normalize()), color(col), intensity(intens) {}

    // Get light direction from ANY point (always the same for directional light)
    vec3 direction_from(const vec3& /*point*/) const {
        return direction * -1.0;  // Reverse direction (light comes FROM direction)
    }

    // For directional light, there's no distance (infinite distance)
    // Return very large value to avoid shadowing issues
    double distance_from(const vec3& /*point*/) const {
        return 1e10;  // Very large distance (simulating infinite distance)
    }

    // Get illumination at a point (no falloff for directional light)
    vec3 get_illumination(const vec3& /*point*/) const {
        return color * intensity;
    }
};
