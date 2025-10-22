#include "sphere.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include <cmath>
#include <algorithm>
#include <memory>

sphere::sphere(vec3 origin, double radius, std::shared_ptr<material> material) 
    : origin(origin), radius(radius) {
    this->mat = material;
}

vec3 sphere::get_normal(const vec3& hit_point) {
    return (hit_point - this->origin).normalize();
}

double sphere::hit(const vec3& ray_origin, const vec3& ray_direction) {
    vec3 oc = ray_origin - this->origin;
    double a = ray_direction.dot(ray_direction);
    double b = 2.0 * oc.dot(ray_direction);
    double c = oc.dot(oc) - this->radius * this->radius;
    double delta = b * b - 4.0 * a * c;
    
    if (delta < 0.0) return -1.0;
    
    double t1 = (-b - std::sqrt(delta)) / (2.0 * a);
    double t2 = (-b + std::sqrt(delta)) / (2.0 * a);
    
    if (t1 > epsilon) {
        return (t2 > epsilon) ? std::min(t1, t2) : t1;
    } else if (t2 > epsilon) {
        return t2;
    }
    return -1.0;
}
