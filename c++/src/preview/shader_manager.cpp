// ============================================================================
// LEÇON 2 : SHADER MANAGER - IMPLÉMENTATION
// ============================================================================

#include "preview/shader_manager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================
ShaderManager::ShaderManager() : program_id(0) {}

// ============================================================================
// DESTRUCTEUR
// ============================================================================
ShaderManager::~ShaderManager() {
    if (program_id != 0) {
        glDeleteProgram(program_id);
    }
}

// ============================================================================
// LIRE UN FICHIER DE SHADER
// ============================================================================
std::string ShaderManager::read_shader_file(const std::string& filepath) {
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "❌ ERREUR : Impossible de lire le shader : " 
                  << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

// ============================================================================
// COMPILER UN SHADER
// ============================================================================
GLuint ShaderManager::compile_shader(const std::string& source, 
                                     GLenum shader_type) {
    // Créer un objet shader vide
    GLuint shader = glCreateShader(shader_type);
    
    // Attacher le source code au shader
    const char* source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, nullptr);
    
    // Compiler
    glCompileShader(shader);
    
    // ====================================================================
    // VÉRIFIER S'IL Y A DES ERREURS
    // ====================================================================
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        // Afficher l'erreur
        print_shader_error(shader);
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

// ============================================================================
// LINKER DEUX SHADERS
// ============================================================================
bool ShaderManager::link_program(GLuint vertex, GLuint fragment) {
    // Créer un program vide
    program_id = glCreateProgram();
    
    // Attacher les shaders au program
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    
    // Linker
    glLinkProgram(program_id);
    
    // ====================================================================
    // VÉRIFIER S'IL Y A DES ERREURS
    // ====================================================================
    int success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    
    if (!success) {
        print_program_error(program_id);
        return false;
    }
    
    // On peut maintenant nettoyer les shaders (ils sont dans le program)
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return true;
}

// ============================================================================
// AFFICHER LES ERREURS DE COMPILATION
// ============================================================================
void ShaderManager::print_shader_error(GLuint shader) {
    // Récupérer la longueur du message d'erreur
    int log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    
    if (log_length <= 0) return;
    
    // Récupérer le message
    char* error_log = new char[log_length];
    glGetShaderInfoLog(shader, log_length, nullptr, error_log);
    
    std::cerr << "❌ ERREUR DE COMPILATION SHADER :\n" 
              << error_log << std::endl;
    
    delete[] error_log;
}

void ShaderManager::print_program_error(GLuint program) {
    // Récupérer la longueur du message d'erreur
    int log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    
    if (log_length <= 0) return;
    
    // Récupérer le message
    char* error_log = new char[log_length];
    glGetProgramInfoLog(program, log_length, nullptr, error_log);
    
    std::cerr << "❌ ERREUR DE LINKING PROGRAM :\n" 
              << error_log << std::endl;
    
    delete[] error_log;
}

// ============================================================================
// CHARGER ET COMPILER LES SHADERS
// ============================================================================
bool ShaderManager::load_shaders(const std::string& vertex_path,
                                 const std::string& fragment_path) {
    std::cout << "\n[LEÇON 2] Chargement des shaders..." << std::endl;
    
    // ====================================================================
    // ÉTAPE 1 : Lire les fichiers
    // ====================================================================
    std::string vertex_source = read_shader_file(vertex_path);
    if (vertex_source.empty()) return false;
    std::cout << "✓ Vertex shader lu" << std::endl;
    
    std::string fragment_source = read_shader_file(fragment_path);
    if (fragment_source.empty()) return false;
    std::cout << "✓ Fragment shader lu" << std::endl;
    
    // ====================================================================
    // ÉTAPE 2 : Compiler les shaders
    // ====================================================================
    GLuint vertex_id = compile_shader(vertex_source, GL_VERTEX_SHADER);
    if (vertex_id == 0) return false;
    std::cout << "✓ Vertex shader compilé" << std::endl;
    
    GLuint fragment_id = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
    if (fragment_id == 0) {
        glDeleteShader(vertex_id);
        return false;
    }
    std::cout << "✓ Fragment shader compilé" << std::endl;
    
    // ====================================================================
    // ÉTAPE 3 : Linker les shaders
    // ====================================================================
    if (!link_program(vertex_id, fragment_id)) return false;
    std::cout << "✓ Shaders linkés en program" << std::endl;
    
    std::cout << "✓ Shaders chargés avec succès !" << std::endl << std::endl;
    
    return true;
}

// ============================================================================
// UTILISER LE SHADER PROGRAM
// ============================================================================
void ShaderManager::use() const {
    if (program_id != 0) {
        glUseProgram(program_id);
    }
}

// ============================================================================
// UNIFORMS : Passer des variables aux shaders
// ============================================================================
void ShaderManager::set_uniform_matrix4fv(const std::string& name, const float* value) const {
    // Récupérer la location de l'uniform (où il est dans le shader)
    GLint loc = glGetUniformLocation(program_id, name.c_str());
    
    if (loc == -1) {
        std::cerr << "⚠️  Uniform '" << name << "' non trouvé dans le shader" << std::endl;
        return;
    }
    
    // Passer la matrice au shader
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

void ShaderManager::set_uniform_3f(const std::string& name, float x, float y, float z) const {
    GLint loc = glGetUniformLocation(program_id, name.c_str());
    
    if (loc == -1) {
        std::cerr << "⚠️  Uniform '" << name << "' non trouvé dans le shader" << std::endl;
        return;
    }
    
    glUniform3f(loc, x, y, z);
}

void ShaderManager::set_uniform_1f(const std::string& name, float value) const {
    GLint loc = glGetUniformLocation(program_id, name.c_str());
    
    if (loc == -1) {
        std::cerr << "⚠️  Uniform '" << name << "' non trouvé dans le shader" << std::endl;
        return;
    }
    
    glUniform1f(loc, value);
}
