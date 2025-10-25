#pragma once
#include "core/vec3.hpp"
#include <vector>
#include <cmath>

// Simple denoising filters for reducing noise in rendered images
namespace denoise {

// Apply a simple box blur (fast, ok quality)
std::vector<vec3> box_blur(const std::vector<vec3>& pixels, int width, int height, int radius = 1);

// Apply gaussian blur (better quality than box, still fast)
std::vector<vec3> gaussian_blur(const std::vector<vec3>& pixels, int width, int height, float sigma = 1.0f);

// Apply bilateral filter (preserves edges, best quality but slower)
std::vector<vec3> bilateral_filter(const std::vector<vec3>& pixels, int width, int height, float sigma_space = 2.0f, float sigma_color = 0.1f);

} // namespace denoise
