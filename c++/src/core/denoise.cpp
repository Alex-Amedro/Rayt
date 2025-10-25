#include "core/denoise.hpp"
#include <algorithm>
#include <cmath>

namespace denoise {

// Helper: Get pixel safely (clamp to borders)
static vec3 get_pixel_safe(const std::vector<vec3>& pixels, int width, int height, int x, int y) {
    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);
    return pixels[y * width + x];
}

// Box blur: Simple average of neighbors
std::vector<vec3> box_blur(const std::vector<vec3>& pixels, int width, int height, int radius) {
    std::vector<vec3> result(pixels.size());
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            vec3 sum(0, 0, 0);
            int count = 0;
            
            // Average all pixels in radius
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    vec3 pixel = get_pixel_safe(pixels, width, height, x + dx, y + dy);
                    sum = sum + pixel;
                    count++;
                }
            }
            
            result[y * width + x] = sum / static_cast<double>(count);
        }
    }
    
    return result;
}

// Gaussian blur: Weighted average (better quality)
std::vector<vec3> gaussian_blur(const std::vector<vec3>& pixels, int width, int height, float sigma) {
    std::vector<vec3> result(pixels.size());
    int radius = static_cast<int>(std::ceil(3.0f * sigma));
    
    // Precompute Gaussian kernel
    std::vector<float> kernel;
    float sum = 0.0f;
    for (int i = -radius; i <= radius; ++i) {
        float value = std::exp(-(i * i) / (2.0f * sigma * sigma));
        kernel.push_back(value);
        sum += value;
    }
    // Normalize kernel
    for (float& v : kernel) v /= sum;
    
    // Apply horizontal pass
    std::vector<vec3> temp(pixels.size());
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            vec3 color(0, 0, 0);
            for (int i = -radius; i <= radius; ++i) {
                vec3 pixel = get_pixel_safe(pixels, width, height, x + i, y);
                float weight = kernel[i + radius];
                color = color + pixel * weight;
            }
            temp[y * width + x] = color;
        }
    }
    
    // Apply vertical pass
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            vec3 color(0, 0, 0);
            for (int i = -radius; i <= radius; ++i) {
                vec3 pixel = get_pixel_safe(temp, width, height, x, y + i);
                float weight = kernel[i + radius];
                color = color + pixel * weight;
            }
            result[y * width + x] = color;
        }
    }
    
    return result;
}

// Bilateral filter: Edge-preserving blur (best quality)
std::vector<vec3> bilateral_filter(const std::vector<vec3>& pixels, int width, int height, float sigma_space, float sigma_color) {
    std::vector<vec3> result(pixels.size());
    int radius = static_cast<int>(std::ceil(3.0f * sigma_space));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            vec3 center_pixel = pixels[y * width + x];
            vec3 sum(0, 0, 0);
            float weight_sum = 0.0f;
            
            // Weighted average based on spatial AND color distance
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    vec3 neighbor = get_pixel_safe(pixels, width, height, x + dx, y + dy);
                    
                    // Spatial weight (Gaussian based on distance)
                    float spatial_dist = std::sqrt(dx * dx + dy * dy);
                    float spatial_weight = std::exp(-(spatial_dist * spatial_dist) / (2.0f * sigma_space * sigma_space));
                    
                    // Color weight (Gaussian based on color difference)
                    vec3 color_diff = neighbor - center_pixel;
                    float color_dist = std::sqrt(color_diff.length_squared());
                    float color_weight = std::exp(-(color_dist * color_dist) / (2.0f * sigma_color * sigma_color));
                    
                    // Combined weight
                    float total_weight = spatial_weight * color_weight;
                    sum = sum + neighbor * total_weight;
                    weight_sum += total_weight;
                }
            }
            
            result[y * width + x] = sum / weight_sum;
        }
    }
    
    return result;
}

} // namespace denoise
