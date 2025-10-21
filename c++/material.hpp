#pragma once
#include "vec3.hpp"
#include "hittable.hpp" 
#include <algorithm> 

class material {
public:
    virtual ~material() = default;
    
    virtual bool scatter(
        const vec3& ray_in,
        const vec3& hit_point,
        const vec3& hit_normal,
        vec3& attenuation, 
        vec3& scattered_direction 
    ) const = 0; 
};
