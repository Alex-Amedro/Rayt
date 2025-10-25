// ============================================================================
// RENDER SETTINGS : Paramètres globaux pour le raytracing
// ============================================================================

#pragma once

#include <glm/glm.hpp>

struct RenderSettings {
    // ====================================================================
    // IMAGE RESOLUTION
    // ====================================================================
    int image_width = 1920;      // 1080p par défaut
    int image_height = 1080;
    
    // ====================================================================
    // RAYTRACING QUALITY
    // ====================================================================
    int samples_per_pixel = 100;      // Anti-aliasing samples
    int max_depth = 50;               // Maximum ray bounce depth
    float gamma = 2.2f;               // Gamma correction
    
    // ====================================================================
    // CAMERA PARAMETERS
    // ====================================================================
    float aperture = 0.0f;            // 0 = no defocus blur (pinhole), >0 = defocus
    float focus_distance = 10.0f;      // Distance to focal plane
    float field_of_view = 45.0f;       // FOV in degrees
    
    // ====================================================================
    // ENVIRONMENT & LIGHTING
    // ====================================================================
    glm::vec3 background_color = glm::vec3(0.1f, 0.1f, 0.2f);  // Sky color
    float sun_intensity = 1.0f;        // Direct sunlight strength
    glm::vec3 sun_direction = glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f));  // Sun direction
    
    // ====================================================================
    // PERFORMANCE
    // ====================================================================
    int num_threads = 8;              // CPU threads for rendering
    bool use_gpu = false;             // GPU rendering (future)
    
    // ====================================================================
    // POST-PROCESSING
    // ====================================================================
    bool denoise = false;             // Apply denoising
    float exposure = 1.0f;            // Exposure adjustment
    float saturation = 1.0f;          // Color saturation
    
    // ====================================================================
    // PRESETS
    // ====================================================================
    void set_quality_low() {
        image_width = 1280;
        image_height = 720;
        samples_per_pixel = 32;
        max_depth = 20;
    }
    
    void set_quality_medium() {
        image_width = 1920;
        image_height = 1080;
        samples_per_pixel = 100;
        max_depth = 50;
    }
    
    void set_quality_high() {
        image_width = 3840;
        image_height = 2160;
        samples_per_pixel = 256;
        max_depth = 100;
    }
    
    void set_quality_ultra() {
        image_width = 3840;
        image_height = 2160;
        samples_per_pixel = 512;
        max_depth = 150;
    }
};
