// ============================================================================
// LEÇON 2 : SHADER MANAGER
// ============================================================================
//
// Qu'est-ce qu'on fait ici ?
//
// Un shader manager est une classe qui gère les shaders :
// 1. Lire les fichiers .glsl du disque
// 2. Compiler les shaders (vertex + fragment)
// 3. Linker les shaders ensemble en un "program"
// 4. Checker les erreurs de compilation
//
// C'est important parce que :
// - Les shaders doivent être compilés à runtime (pas à compile time)
// - Si y'a une erreur de shader, faut la voir tout de suite
// - C'est cool de pouvoir éditer les shaders sans recompiler le C++
//
// ============================================================================

#pragma once

#include <string>
#include <GL/glew.h>

class ShaderManager {
private:
    // Le "program" = vertex shader + fragment shader linkés ensemble
    GLuint program_id;
    
    // ====================================================================
    // FONCTIONS PRIVÉES (utilisées en interne)
    // ====================================================================
    
    // Lire un fichier de shader du disque
    std::string read_shader_file(const std::string& filepath);
    
    // Compiler un shader
    GLuint compile_shader(const std::string& source, GLenum shader_type);
    
    // Linker deux shaders ensemble
    bool link_program(GLuint vertex, GLuint fragment);
    
    // Afficher les erreurs de compilation
    void print_shader_error(GLuint shader);
    void print_program_error(GLuint program);

public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    // Crée un shader manager vide (program_id = 0)
    ShaderManager();
    
    // ====================================================================
    // DESTRUCTEUR
    // ====================================================================
    // Nettoie le shader program
    ~ShaderManager();
    
    // ====================================================================
    // CHARGER ET COMPILER LES SHADERS
    // ====================================================================
    // Charge un vertex shader et un fragment shader et les compile
    // Retourne true si succès, false si erreur
    //
    // Exemple d'utilisation :
    //   ShaderManager mgr;
    //   if (mgr.load_shaders("shaders/vertex.glsl", "shaders/fragment.glsl")) {
    //       mgr.use();  // Utiliser le shader
    //   }
    //
    bool load_shaders(const std::string& vertex_path, 
                      const std::string& fragment_path);
    
    // ====================================================================
    // UTILISER LE SHADER PROGRAM
    // ====================================================================
    // Dit à OpenGL : "utilise ce shader pour dessiner"
    void use() const;
    
    // ====================================================================
    // UNIFORMS : Passer des variables au shader
    // ====================================================================
    // Les uniforms sont des variables globales pour les shaders
    // (ex : matrices de caméra, lumière, etc)
    
    // Passer une matrice 4x4
    void set_uniform_matrix4fv(const std::string& name, const float* value) const;
    
    // Passer un vecteur 3D
    void set_uniform_3f(const std::string& name, float x, float y, float z) const;
    
    // Passer un float
    void set_uniform_1f(const std::string& name, float value) const;
    
    // ====================================================================
    // GETTERS
    // ====================================================================
    GLuint get_program_id() const { return program_id; }
    bool is_loaded() const { return program_id != 0; }
};
