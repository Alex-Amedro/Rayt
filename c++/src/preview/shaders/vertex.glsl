#version 460 core

// ============================================================================
// LEÇON 4 : Vertex Shader avec Caméra
// ============================================================================

// ========================================================================
// INPUT : Ce qu'on reçoit du CPU
// ========================================================================

// La position du vertex (x, y, z)
layout(location = 0) in vec3 position;

// La couleur du vertex (r, g, b)
layout(location = 1) in vec3 color;

// ========================================================================
// UNIFORMS : Variables globales pour tous les vertices
// ========================================================================

// La matrice de projection (comment ça s'écrase sur l'écran)
uniform mat4 projection;

// La matrice de vue (où on regarde et d'où)
uniform mat4 view;

// La matrice du modèle (position/rotation/échelle de l'objet)
uniform mat4 model;

// Couleur de l'objet (nouveau pour l'éditeur !)
uniform vec3 objectColor;

// ========================================================================
// OUTPUT : Ce qu'on envoie au fragment shader
// ========================================================================

// La couleur qu'on veut au fragment shader
out vec3 vertex_color;

// ========================================================================
// MAIN : Le programme du shader
// ========================================================================

void main() {
    // ====================================================================
    // Transformer la position 3D en position écran
    // ====================================================================
    // On applique les trois transformations dans l'ordre :
    // 1. model = où est l'objet dans le monde
    // 2. view = où regarde la caméra
    // 3. projection = comment ça s'écrase sur l'écran (perspective)
    //
    // L'ordre est : projection * view * model * position
    //
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    // ====================================================================
    // Passer la couleur au fragment shader
    // ====================================================================
    // Utiliser objectColor au lieu de color (attribut)
    vertex_color = objectColor;
}
