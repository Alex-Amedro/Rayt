#include "vec3.hpp"
#include <cmath>
#include <random>

// Constructors
vec3::vec3() : x(0.0), y(0.0), z(0.0) {}

vec3::vec3(double x, double y, double z) : x(x), y(y), z(z) {}

// Vector operations
double vec3::dot(const vec3& other) const {
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

double vec3::length_squared() const {
    return this->dot(*this);
}

vec3 vec3::normalize() const {
    double length = std::sqrt(this->length_squared());
    if (length == 0.0) return vec3(0.0, 0.0, 0.0);
    return vec3(this->x / length, this->y / length, this->z / length);
}

// Arithmetic operators
vec3 vec3::operator+(const vec3& other) const {
    return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

vec3 vec3::operator-(const vec3& other) const {
    return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

vec3 vec3::operator/(double div) const {
    return vec3(this->x / div, this->y / div, this->z / div);
}

vec3 vec3::operator*(double mul) const {
    return vec3(this->x * mul, this->y * mul, this->z * mul);
}

vec3 vec3::operator*(vec3 mul) const {
    return vec3(this->x * mul.x, this->y * mul.y, this->z * mul.z);
}

// Random vector utilities
vec3 random_in_unit_sphere() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    while (true) {
        vec3 p = vec3(distribution(generator), distribution(generator), distribution(generator));
        if (p.length_squared() < 1.0)
            return p;
    }
}

vec3 random_unit_vector() {
    return random_in_unit_sphere().normalize();
}

// Reflection
vec3 reflect(const vec3& v_in, const vec3& normal) {
    return v_in - (normal * (2.0 * v_in.dot(normal)));
}
