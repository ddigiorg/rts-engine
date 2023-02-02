#ifndef SHADER_H
#define SHADER_H

// thrid party includes
#include <GL/glew.h>
#include <SDL.h>

// STL includes
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// =============================================================================
// Shader Class
// =============================================================================
class Shader {
    public:
        Shader() {};

        Shader(
            std::string vertFilepath,
            std::string fragFilepath);

        Shader(
            std::string vertFilepath,
            std::string geomFilepath,
            std::string fragFilepath);

        GLuint getProgId() { return progId; }; // TODO: should be reference?

    private:
        void loadGLSLFromFile(std::string filepath, GLuint& shader, GLuint type);

        GLuint progId = 0;
        GLuint vertShader = NULL;
        GLuint geomShader = NULL;
        GLuint fragShader = NULL;
};

// =============================================================================
// Construct Shader
// =============================================================================
Shader::Shader(
        std::string vertFilepath,
        std::string fragFilepath) {

    progId = glCreateProgram();
    loadGLSLFromFile(vertFilepath, vertShader, GL_VERTEX_SHADER);
    loadGLSLFromFile(fragFilepath, fragShader, GL_FRAGMENT_SHADER);
    glAttachShader(progId, vertShader);
    glAttachShader(progId, fragShader);
    glLinkProgram(progId);
    glValidateProgram(progId);
}

// =============================================================================
// Construct Shader
// =============================================================================
Shader::Shader(
        std::string vertFilepath,
        std::string geomFilepath,
        std::string fragFilepath) {

    progId = glCreateProgram();
    loadGLSLFromFile(vertFilepath, vertShader, GL_VERTEX_SHADER);
    loadGLSLFromFile(geomFilepath, geomShader, GL_GEOMETRY_SHADER);
    loadGLSLFromFile(fragFilepath, fragShader, GL_FRAGMENT_SHADER);
    glAttachShader(progId, vertShader);
    glAttachShader(progId, geomShader);
    glAttachShader(progId, fragShader);
    glLinkProgram(progId);
    glValidateProgram(progId);
}

// =============================================================================
// Load GLSL From File
// =============================================================================
void Shader::loadGLSLFromFile(std::string filepath, GLuint& shader, GLuint type) {

    // read shader source from file
    std::string line = "";
    std::string source = "";
    std::ifstream file(filepath.c_str());

    if(file.is_open()) {
        while(std::getline(file, line)) {
            source += line + "\n";
        }
        file.close();
    }

    // load and compile shader
    shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // error check shader
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(shader, length, &length, errorMessages);

        std::cout << "ERROR: shader compilation failed." << std::endl
                  << filepath << std::endl
                  << errorMessages << std::endl;

        delete[] errorMessages;
        glDeleteShader(shader);
        exit(1);
    }
}

#endif // SHADER_H