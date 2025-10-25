#pragma once

// 3D vector class for ray tracing
class vec3 {
public: 
    double x, y, z;

    vec3();
    vec3(double x, double y, double z);

    double length_squared() const;
    vec3 normalize() const;
    double dot(const vec3& other) const;
    vec3 cross(const vec3& other) const;
    
    vec3 operator+(const vec3& other) const;
    vec3 operator-(const vec3& other) const;
    vec3 operator/(double div) const;
    vec3 operator*(double mul) const;
    vec3 operator*(vec3 mul) const;
    vec3 operator+(double constant) const;
};

