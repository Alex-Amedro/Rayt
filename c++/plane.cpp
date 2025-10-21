#include "plane.hpp"
#include <cmath>
#include "vec3.hpp"
#include "hittable.hpp"
#include <memory>


plane::plane(const vec3& anchor, const vec3& normal, std::shared_ptr<material> mat) {
    this->anchor = anchor;
    this->normal = normal.normalize();
    this->mat = mat;
}

double plane::hit(const vec3& ray_origin, const vec3& ray_direction) {
    double denom = this->normal.dot(ray_direction);
    if (std::abs(denom) > epsilon) {
        vec3 num = this->anchor - ray_origin;
        double t = num.dot(this->normal) / denom;
        return (t >= epsilon) ? t : -1.0;
    }
    return -1.0;
}

vec3 plane::get_normal(const vec3& hit_point) {
    return this->normal;
}
